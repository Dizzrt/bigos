//
// File: bitset.h
// Created by Dizzrt on 2023/04/02.
//
// Copyright (C) 2023 The BigOS Authors.
// Licensed under the GNU General Public License v3.0 only.
//

#ifndef _BIG_BITSET_H
#define _BIG_BITSET_H

#include <bigos/types.h>

namespace ktl {
    class bitset {
    protected:
        // bits heap pointer
        ptr8_t heap_ptr_;

        uint32_t nr_bits_;
        // how many bits are not set
        uint32_t nr_avl_bits_;

    public:
        // constructor
        bitset(uint32_t _nr_bits, ptr8_t _heap_ptr);
        bitset(uint32_t _nr_bits);
        bitset();

        virtual ~bitset() = default;

        inline uint32_t size() const noexcept { return nr_bits_; }
        inline uint32_t reset_size() const noexcept { return nr_avl_bits_; }
        inline uint32_t set_size() const noexcept { return nr_bits_ - nr_avl_bits_; }

        // ret => how many bits are set
        uint32_t set(uint32_t _pos, uint32_t _len) noexcept;
        uint32_t set(uint32_t _pos) noexcept;
        uint32_t set() noexcept;

        // ret => how many bits are reset
        uint32_t reset(uint32_t _pos, uint32_t _len);
        uint32_t reset(uint32_t _pos) noexcept;
        uint32_t reset() noexcept;

        uint32_t scan(uint32_t _len) noexcept;

        // ret => how many bits are fliped
        // uint32_t flip(uint32_t _pos, uint32_t _len) noexcept;
        // uint32_t flip(uint32_t _pos) noexcept;
        // uint32_t flip() noexcept;

        // return true if all target bits are set, else return false
        // bool test(uint32_t _pos, uint32_t _len) noexcept;
        bool test(uint32_t _pos) noexcept;
        // bool test() noexcept;
    };

}   // namespace ktl

#endif   // _BIG_BITSET_H