//
// File: memory.cc
// Created by Dizzrt on 2023/08/27.
//
// Copyright (C) 2023 The BigOS Authors.
// Licensed under the GNU General Public License v3.0 only.
//

#include <memory.h>

#include "kmem.h"
namespace bigos {
    void init_mem() {
        mm::init_mem();
    }
}   // namespace bigos
