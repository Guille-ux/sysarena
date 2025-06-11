/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 *
 * Copyright (c) 2025 Guillermo Leira Temes
 */

#include "../include/types.h"
#include "../include/sysarena.h"

// Initializes an arena as empty or unused.
void poor_arena_init(Arena *arena) {
    if (arena==NULL) return;
    arena->size = 0;
    arena->used = 0;
    arena->base = NULL;
    arena->in_use = false;
}

// Initializes an arena with a specific size and memory base.
bool arena_init(Arena *arena, size_t size, ptr_t base) {
    if (arena==NULL || base==NULL) return false;
    arena->size = size;
    arena->base = base;
    arena->used = 0;
    arena->in_use = true;
    return true;
}

// Allocates a block of 'size' bytes from an arena.
// Returns NULL if there's not enough space.
void* arena_alloc(Arena *arena, size_t size) {
    if (arena==NULL || size==0 || !arena->in_use) return NULL;
    // If the allocation exceeds available size, there's no memory.
    if (arena->used + size > arena->size) {
        return NULL;
    }
    // Calculates the address of the new block.
    ptr_t ptr = (ptr_t)((char*)arena->base + arena->used);
    // Updates the arena's usage.
    arena->used += size;
    return ptr;
}

// Frees an entire arena, resetting its usage counter.
void arena_free(Arena *arena) {
    arena->used = 0;
    arena->in_use = false; // Marks the arena as unused.
}

// Initializes the arena manager.
bool sysarena_init(ArenaManager *manager, uint8_t *memory, Arena *arenas, size_t total_size, size_t num_arenas) {
    if (!manager || !memory || !arenas || num_arenas < 1) return false;

    manager->arenas = arenas;
    manager->max_arenas = num_arenas;
    manager->initial_memory = memory;
    manager->initial_size = total_size;
    manager->current_arena_idx = 0;

    // Inicializar primera arena con todo el bloque
    Arena *first = &manager->arenas[0];
    first->size = total_size;
    first->base = memory;
    first->used = 0;
    first->in_use = true;
    first->is_contiguous = true;

    // Inicializar resto de arenas como vacías
    for (size_t i = 1; i < num_arenas; i++) {
        poor_arena_init(&manager->arenas[i]);
    }
    
    return true;
}

// Checks if two arenas can be merged.
bool arena_can_merge(const Arena *a, const Arena *b) {
    // They can only be merged if both are not in use.
    if (a->in_use == false && b->in_use == false) {
        return true;
    }
    return false;
}

// Checks if an arena is completely empty.
bool arena_is_void(const Arena *a) {
    // It's empty if it has no bytes used.
    if (a->used == 0) {
        return true;
    }
    return false;
}

// Merges the 'src' arena into the 'dest' arena.
bool arena_merge(Arena *dest, Arena *src) {
    if (!arena_can_merge(dest, src)) return false;

    //checks for overflow
    if (dest->size > SIZE_MAX - src->size) {
      return false;
    }

    dest->size += src->size; // Increases the destination's size.
    poor_arena_init(src); // Resets the source arena.
    return true;
}

// Defragments arenas by attempting to merge empty and adjacent ones.
void sysarena_defragment(ArenaManager *manager) {
    if (!manager) return;

    for (size_t i = 0; i < manager->max_arenas; i++) {
        Arena *current = &manager->arenas[i];
        
        if (!current->in_use && current->is_contiguous) {
            // Buscar siguiente arena contigua vacía
            for (size_t j = i + 1; j < manager->max_arenas; j++) {
                Arena *next = &manager->arenas[j];
                
                if (!next->in_use && next->is_contiguous &&
                    (uint8_t*)current->base + current->size == (uint8_t*)next->base) {
                    
                    // Fusionar arenas
                    current->size += next->size;
                    poor_arena_init(next);
                    next->is_contiguous = false;
                    
                    // Reorganizar lista de arenas
                    for (size_t k = j; k < manager->max_arenas - 1; k++) {
                        manager->arenas[k] = manager->arenas[k + 1];
                    }
                    poor_arena_init(&manager->arenas[manager->max_arenas - 1]);
                    
                    // Reiniciar el proceso
                    i--;
                    break;
                }
            }
        }
    }
}

// Allocates memory in the arena system.
void* sysarena_alloc(ArenaManager *manager, size_t size) {
    if (!manager || size == 0) return NULL;

    // Primero buscar en arenas existentes
    for (size_t i = 0; i < manager->max_arenas; i++) {
        Arena *arena = &manager->arenas[i];
        
        if (arena->in_use && arena->size - arena->used >= size) {
            void *ptr = arena_alloc(arena, size);
            if (ptr) {
                manager->current_arena_idx = i;
                return ptr;
            }
        }
    }

    // Si no hay espacio, intentar dividir una arena grande
    for (size_t i = 0; i < manager->max_arenas; i++) {
        Arena *arena = &manager->arenas[i];
        
        if (arena->in_use && !arena->is_contiguous && arena->size >= size * 2) {
            if (sysarena_split(manager, i, size)) {
                return sysarena_alloc(manager, size);  // Intentar de nuevo
            }
        }
    }

    return NULL;
}

// Splits an arena into two.
bool sysarena_split(ArenaManager *manager, size_t index, size_t size) {
    if (!manager || index >= manager->max_arenas || size == 0) return false;

    Arena *source = &manager->arenas[index];
    if (!source->in_use || source->size < size) return false;

    // Buscar espacio para nueva arena
    for (size_t i = 0; i < manager->max_arenas; i++) {
        Arena *dest = &manager->arenas[i];
        
        if (!dest->in_use && dest->used == 0) {
            // Configurar nueva arena
            dest->base = (uint8_t*)source->base + (source->size - size);
            dest->size = size;
            dest->used = 0;
            dest->in_use = true;
            dest->is_contiguous = false;
            
            // Reducir arena original
            source->size -= size;
            source->is_contiguous = false;
            
            return true;
        }
    }
    
    return false;
}

// Frees memory associated with a pointer.
// This means resetting the entire arena the pointer belongs to.
// It does NOT free individual blocks.
bool sysarena_free(ArenaManager *manager, void *ptr) {
    if (!manager || !ptr) return false;

    for (size_t i = 0; i < manager->max_arenas; i++) {
        Arena *arena = &manager->arenas[i];
        
        if (arena->in_use && arena->base && 
            (uint8_t*)ptr >= (uint8_t*)arena->base && 
            (uint8_t*)ptr < (uint8_t*)arena->base + arena->size) {
            
            arena_free(arena);
            arena->is_contiguous = true;  // Marcar como fusionable
            
            // Intentar fusionar con bloques adyacentes
            sysarena_defragment(manager);
            
            return true;
        }
    }
    return false;
}
// Copies data from one arena struct to another.
void copy_arena(Arena *dest, const Arena *src) {
    if (dest==NULL || src==NULL) return;
    dest->in_use = src->in_use;
    dest->base = src->base;
    dest->size = src->size;
    dest->used = src->used;
}

// Displaces arenas to reorder them.
void sysarena_displacement(ArenaManager *manager, size_t where) {
    if (manager==NULL || where >= manager->max_arenas) {
        return;
    }
    // Move arenas from right to left from the end towards 'where'.
    size_t empty_slot=manager->max_arenas;
    for (size_t i = manager->max_arenas; i-- > 0;) {
        if (!manager->arenas[i].in_use && manager->arenas[i].used==0) {
            empty_slot=i;
            break;
        }
    }

    if (empty_slot==manager->max_arenas) return;

    for (size_t i=empty_slot;i>where;i--) {
        copy_arena(&manager->arenas[i], &manager->arenas[i-1]);
    }

    poor_arena_init(&manager->arenas[where]);
}

bool sysarena_is_fully_merged(ArenaManager *manager) {
    if (!manager) return false;
    
    size_t active_arenas = 0;
    for (size_t i = 0; i < manager->max_arenas; i++) {
        if (manager->arenas[i].in_use) active_arenas++;
    }
    
    return active_arenas == 1 && 
           manager->arenas[0].size == manager->initial_size &&
           manager->arenas[0].base == manager->initial_memory;
}

