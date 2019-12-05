/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALMATH_H
#define HWVC_ANDROID_ALMATH_H

#include <cmath>
#include "Object.h"

al_class(AlMath) {
public:
    static constexpr double PI = M_PI;

    static uint16_t swap16(uint16_t val);

    static uint32_t swap32(uint32_t val);

    /// 求解最大公约数
    /// \param a
    /// \param b
    /// \return
    static int32_t gcd(int32_t a, int32_t b);

    /// 求解最小公倍数
    /// \param a
    /// \param b
    /// \return
    static int32_t lcm(int32_t a, int32_t b);

private:
    AlMath() : Object() {};

    AlMath(const AlMath &o) : Object() {};

    ~AlMath() {};
};


#endif //HWVC_ANDROID_ALMATH_H
