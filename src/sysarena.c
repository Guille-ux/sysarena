/* 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 any later version.
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU General Public License for more details.
 You should have received a copy of the GNU General Public License
 along with this program. If not, see <https://www.gnu.org/licenses/>.
 Copyright (c) 2025 Guillermo Leira Temes
*/

#include "../include/sysarena.h" // incluir la arena
#include "../include/types.h" // incluir types.h, es rebundante pero por si acaso

void poor_arena_init(Arena *arena) {
    arena->size=0;
    arena->used=0;
    arena->base=null;
    arena->in_use=false;
}

void arena_init(Arena *arena, size_t size, ptr_t base) {
    arena->size=size;
    arena->base=base;
    arena->used=0;
    arena->in_use=false;
}

void* arena_alloc(Arena *arena, size_t size) {
    if (arena->used + size > arena->size) {
        return null; // devolver null si no hay suficiente memoria
    }
    ptr_t ptr=(void *)(arena->used + arena->base);
    arena->used += size;
    return ptr;
}

void arena_free(Arena *arena) {
    arena->used=0;
    arena->in_use=false;
}

void sysarena_init(ArenaManager *manager, uint8_t *memory, Arena *arenas, size_t size) {
    manager->arenas = arenas;
    manager->max_arenas=size;
}

bool arena_can_merge(Arena *a, Arena *b) {
    if (a->in_use==false && b->in_use==false) {
        return true;
    }
    return false;
}