//
// File: attributes.h
// Created by Dizzrt on 2023/03/30.
//
// Copyright (C) 2023 The BigOS Authors.
// Licensed under the GNU General Public License v3.0 only.
//

#ifndef _BIG_ATTRIBUTES_H
#define _BIG_ATTRIBUTES_H

#define _attr_nodiscard_ [[nodiscard]]

#define _attr_pure_   __attribute__((__pure__))
#define _attr_malloc_ __attribute__((__malloc__))

#define _section_4k_ __attribute__((section(".4k_area,\"aw\",@nobits#")))

#endif