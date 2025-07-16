# `sysarena`: An Arena-Based Memory Manager (Experimental)

**Copyright (c) 2025 Guillermo Leira Temes**

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see [https://www.gnu.org/licenses/](https://www.gnu.org/licenses/).

---

`sysarena` is a **highly experimental** project implementing a memory manager based on the "arena" or "memory pool" concept. This project was developed primarily to **explore and demonstrate the fundamental principles of dynamic memory management in low-level environments**, particularly within a basic kernel or embedded system context.

---

## Warning: Not for Production Use

**`sysarena` is NOT designed for production environments.** You should never use it in systems where stability, security, or memory efficiency are critical.

Here's why:

* **Severe External Fragmentation:** This manager is particularly prone to **memory fragmentation**. While it includes a basic defragmentation phase (`ksysarena_defragment`), the linear nature of allocations and inefficient consolidation of intermediate free spaces can quickly lead to "out of memory" situations. In these cases, despite having enough total free memory, there might not be contiguous blocks large enough to satisfy new requests.
* **Limited Capabilities:** It's designed as a conceptual example, not a general-purpose manager. It lacks many optimizations, robustness, and security features found in modern operating system memory allocators (like Buddy System, Slab Allocators, or more complex managers).
* **Educational Focus:** Its value lies in its simplicity for illustrating how arenas work and the associated challenges, not in its performance or reliability in a complex operating environment.

---

## What is `sysarena` For?

`sysarena` is an educational tool and an excellent starting point for:

* **Understanding Basic Memory Management:** It's ideal for learning how memory blocks are simply allocated and freed.
* **Visualizing Fragmentation:** It lets you see firsthand how external fragmentation occurs and impacts a memory manager.
* **Exploring the Arena Concept:** It serves as a basic reference implementation for understanding the arena-based memory management model.
* **Foundation for Learning:** It offers a solid base for understanding why more sophisticated memory management algorithms are needed and how they address the problems that `sysarena` reveals.

---

## How It Works (Basic Implementation Concepts)

`sysarena` operates on a predefined set of `Arena` structures managed by an `ArenaManager`. Initially, all available memory is assigned to a single arena.

* **Allocation (`sysarena_alloc`):** This function searches for the first free arena large enough to satisfy the request. If an arena is larger than needed, it's split into two: one for the allocation and another for the remaining free space. This can lead to the creation of many small arenas.
* **Deallocation (`sysarena_free`):** This simply marks an arena as not in use (`in_use = false`) and resets its `used` byte count to zero.
* **Defragmentation (`sysarena_defragment`):** This function attempts to mitigate external fragmentation in two ways:
    1.  **Merging:** It iterates through free arenas and tries to merge adjacent ones (`is_contiguous = true`) to form larger free blocks.
    2.  **Compaction:** It shifts "empty" free arenas (those with no base address or size) to the end of the `manager->arenas` array, compacting the arena array so useful arenas are at the beginning.

While simplistic, this approach allows you to observe how fragmentation can reduce usable memory and how a basic defragmentation mechanism attempts to reclaim it.

---

## Contributions and Learning

If you're interested in memory management or operating system development, I encourage you to review the `sysarena` code. It's an excellent case study for understanding the challenges and limitations of simple memory management approaches, and why more complex algorithms (like the Buddy System) are practically necessary for robust and efficient memory management.
