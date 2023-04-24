//
// File: new.cc
// Created by Dizzrt on 2023/04/08.
//
// Copyright (C) 2023 The BigOS Authors.
// Licensed under the GNU General Public License v3.0 only.
//

#include <mm/memory.h>
#include <bigos/types.h>

// operator new
void *operator new(size_t size) {
    return bigos::kmalloc(size);
}

void *operator new[](size_t size) {
    return bigos::kmalloc(size);
}

// operator delete
void operator delete(void *p) {
    bigos::free(p);
}

void operator delete[](void *p) {
    bigos::free(p);
}

void operator delete(void *p, size_t size) {
    bigos::free(p);
}

void operator delete[](void *p, size_t size) {
    bigos::free(p);
}
