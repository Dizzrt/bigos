//
// File: types.h
// Created by Dizzrt on 2023/03/20.
//
// Copyright (C) 2023 The BigOS Authors.
// Licensed under the GNU General Public License v3.0 only.
//

#ifndef _BIG_TYPES_H
#define _BIG_TYPES_H

#include <stdint.h>
#include <stddef.h>

// get free memory
typedef unsigned int gfm_t;

typedef void* ptr_t;
typedef uint8_t* ptr8_t;
typedef uint16_t* ptr16_t;
typedef uint32_t* ptr32_t;
typedef uint64_t* ptr64_t;

#define NAMESPACE_BIGOS_BEG namespace bigos {
#define NAMESPACE_BIGOS_END }

#define NAMESPACE_DRIVER_BEG namespace driver {
#define NAMESPACE_DRIVER_END }

#endif   // _BIG_TYPES_H
