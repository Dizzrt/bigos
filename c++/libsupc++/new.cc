//
// File: new.cc
// Created by Dizzrt on 2023/04/08.
//
// Copyright (C) 2023 The BigOS Authors.
// Licensed under the GNU General Public License v3.0 only.
//

#include <bigos/types.h>

// TODO operator new
void *operator new(size_t size) {
    return nullptr;
}

void *operator new[](size_t size) {
    return nullptr;
}

// TODO operator delete
void operator delete(void *p) {}

void operator delete[](void *p) {}

void operator delete(void *p, size_t size) {}

void operator delete[](void *p, size_t size) {}