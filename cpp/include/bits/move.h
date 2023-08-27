//
// File: move.h
// Created by Dizzrt on 2023/03/28.
//
// Copyright (C) 2023 The BigOS Authors.
// Licensed under the GNU General Public License v3.0 only.
//

#ifndef _BIG_MOVE_H
#define _BIG_MOVE_H

namespace std {
    template <typename _Tp>
    inline void swap(_Tp& __a, _Tp& __b) {
        _Tp __tmp = __a;
        __a = __b;
        __b = __tmp;
    }
}   // namespace std

#endif   // _BIG_MOVE_H
