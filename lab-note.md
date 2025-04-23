# Inspcect a user-process page table

```
$ pgtbltest
print_pgtbl starting
va 0x0 pte 0x21FC885B pa 0x87F22000 perm 0x5B
va 0x1000 pte 0x21FC7C1B pa 0x87F1F000 perm 0x1B
va 0x2000 pte 0x21FC7817 pa 0x87F1E000 perm 0x17
va 0x3000 pte 0x21FC7407 pa 0x87F1D000 perm 0x7
va 0x4000 pte 0x21FC70D7 pa 0x87F1C000 perm 0xD7
va 0x5000 pte 0x0 pa 0x0 perm 0x0
va 0x6000 pte 0x0 pa 0x0 perm 0x0
va 0x7000 pte 0x0 pa 0x0 perm 0x0
va 0x8000 pte 0x0 pa 0x0 perm 0x0
va 0x9000 pte 0x0 pa 0x0 perm 0x0
va 0xFFFF6000 pte 0x0 pa 0x0 perm 0x0
va 0xFFFF7000 pte 0x0 pa 0x0 perm 0x0
va 0xFFFF8000 pte 0x0 pa 0x0 perm 0x0
va 0xFFFF9000 pte 0x0 pa 0x0 perm 0x0
va 0xFFFFA000 pte 0x0 pa 0x0 perm 0x0
va 0xFFFFB000 pte 0x0 pa 0x0 perm 0x0
va 0xFFFFC000 pte 0x0 pa 0x0 perm 0x0
va 0xFFFFD000 pte 0x0 pa 0x0 perm 0x0
va 0xFFFFE000 pte 0x21FD08C7 pa 0x87F42000 perm 0xC7
va 0xFFFFF000 pte 0x2000184B pa 0x80006000 perm 0x4B
print_pgtbl: OK
ugetpid_test starting
usertrap(): unexpected scause 0xd pid=4
            sepc=0x57a stval=0x3fffffd000
```

1. va 0x0, pte 0x21FC885B.
    - PPN: (pte >> 10) << 12. 0x87F22
    - Offset: 000
    - Flags: 00,0101,1011 -> RXUAV (text area RXU)
2. va pte PPN offset ...
    - Flags: 00,0001,1011 -> RXUV. (text that not accessed.)
3. ...
    - Flags: 00,0001,0111 -> VRWU (date area, write ok)
4. ...
    - Flags: VRW -> (not User, IDK)
5. ...
    - Flags: 00,1101,0111 -> DAUWRV (dirty data)
6. va 0xFFFFE000
    - Flags: 00,1100,0111 -> VRWDA (trapframe)
7. ...
    - Flags: 00,0100,1011 -> VRXA (trampoline)

# Use superpages

Some keypoints:

1. `uvmcopy` copies pagetable in size continuously, which means that in a process's vm space, if you alloc less than 2MB and you want to alloc superpages, you should first fill the space between oldsz, after which the memory is superpage aligned and continuous. If you don't fill up the fragments, `uvmcopy`would work wrong.
2. you should define a way to get the level-1 PTE instead of level-0 PTE, I defined a `superwalk`. Except this, `walk` can be used generally to get the right PTE since it use `PTE_LEAF` to dynamically detect if one PTE points to a pyhsical mem rather than a pagetable whatever the level of the PTE is.
3. `mappages` is used so commonly that it's really hard to modify it. Our superpage only works during process `sbrk` and under the condition, mappages are used to map only one page at a time. But in other cases like OS's initial work, it's used to map a range of pages. So I defined a `mapsuperpage`, which is only used in `sbrk` situation. I think it better to separate the general-use `mappages` from the situation specified function `mapsuperpage`.
