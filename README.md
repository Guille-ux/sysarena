# SysArena

This project implements a **dynamic memory manager** based on **arenas**, written entirely in C, without relying on the standard library. It is designed for **low-level environments** such as operating systems or embedded systems.

## 📚 What is an Arena?

An **arena** is a pre-allocated block of memory where users can quickly allocate space for their data in an efficient and linear fashion.  
In this system:

- Each `Arena` represents a **memory block** containing:
  - `base`: start pointer,
  - `size`: total size,
  - `used`: used bytes,
  - `in_use`: whether the arena is currently active.

Multiple arenas can be managed at once through an `ArenaManager`, allowing complex memory management strategies like splitting and merging memory regions.

---

## 🛠️ Main Structures

- **Arena**: Represents a single block of memory, with tracking for size, usage, and status.
- **ArenaManager**: Handles an array of arenas, enabling memory subdivision, dynamic allocation, deallocation, and defragmentation.

---

## 🔥 Features

| Function | Description |
|:---------|:------------|
| `poor_arena_init` | Initializes a single arena as free. |
| `arena_init` | Initializes an arena with a given base address and size. |
| `arena_alloc` | Allocates memory within a single arena. |
| `arena_free` | Frees all memory within a single arena. |
| `sysarena_init` | Initializes a system managing multiple arenas. |
| `arena_can_merge` | Checks if two arenas can be merged. |
| `arena_merge` | Merges two adjacent arenas. |
| `sysarena_defragment` | Defragments the system by merging free adjacent arenas. |
| `sysarena_alloc` | Allocates memory within the system of multiple arenas. |
| `sysarena_free` | Frees memory pointed to by a given pointer. |
| `sysarena_split` | Splits an Arena. |
| `sysarena_displacement` | Move's arenas to make easier merge them. |
| `copy_arena` | Function to copy an Arena to other Arena (used at `sysarena_displacement`). |
|`sysarena_is_fully_merged(ArenaManager *manager)`|Funtion to see if the system has merged|

---

## Documentation

**I'm working on this**

## ⚙️ Requirements

- **C99 compiler or newer**.
- **No standard library dependencies** (e.g., no `stdlib.h`, no `string.h`).
- Designed for **bare-metal** or **custom operating systems**.

---

## 📜 License

This project is distributed under the **GNU General Public License v3.0** or later.  
You are free to use, modify, and share it, as long as you respect the terms of the license.

[See GPLv3 License](https://www.gnu.org/licenses/gpl-3.0.html)

---

## 🧠 Author

- Guillermo Leira Temes, 2025.

---
