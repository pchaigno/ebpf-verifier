#pragma once

/**
 *  Safe signed integers.
 **/

#include <cstdint>
#include <limits>

#include "crab/bignums.hpp"
#include "crab/safeint.hpp"

namespace crab {

class safe_i64 {

    // Current implementation is based on
    // https://blog.regehr.org/archives/1139 using wider integers.

    // TODO/FIXME: the current code compiles assuming the type __int128
    // exists. Both clang and gcc supports __int128 if the targeted
    // architecture is x86/64, but it wont' work with 32 bits.
    using wideint_t = __int128;

    inline int64_t get_max() const{ return std::numeric_limits<int64_t>::max(); }
    inline int64_t get_min() const { return std::numeric_limits<int64_t>::min(); }

    int checked_add(int64_t a, int64_t b, int64_t* rp) const {
        wideint_t lr = (wideint_t)a + (wideint_t)b;
        *rp = lr;
        return lr > get_max() || lr < get_min();
    }

    int checked_sub(int64_t a, int64_t b, int64_t* rp) const{
        wideint_t lr = (wideint_t)a - (wideint_t)b;
        *rp = lr;
        return lr > get_max() || lr < get_min();
    }
    int checked_mul(int64_t a, int64_t b, int64_t* rp) const{
        wideint_t lr = (wideint_t)a * (wideint_t)b;
        *rp = lr;
        return lr > get_max() || lr < get_min();
    }
    int checked_div(int64_t a, int64_t b, int64_t* rp) const{
        wideint_t lr = (wideint_t)a / (wideint_t)b;
        *rp = lr;
        return lr > get_max() || lr < get_min();
    }

  public:
    safe_i64() : m_num(0) {}

    safe_i64(int64_t num) : m_num(num) {}

    safe_i64(const z_number& n) : m_num((long)n) {}

    operator long() const{ return (long)m_num; }

    // TODO: output parameters whether operation overflows
    safe_i64 operator+(safe_i64 x) const{
        int64_t z;
        int err = checked_add(m_num, x.m_num, &z);
        if (err) {
            CRAB_ERROR("Integer overflow during addition");
        }
        return safe_i64(z);
    }

    // TODO: output parameters whether operation overflows
    safe_i64 operator-(safe_i64 x) const{
        int64_t z;
        int err = checked_sub(m_num, x.m_num, &z);
        if (err) {
            CRAB_ERROR("Integer overflow during subtraction");
        }
        return safe_i64(z);
    }

    // TODO: output parameters whether operation overflows
    safe_i64 operator*(safe_i64 x) const{
        int64_t z;
        int err = checked_mul(m_num, x.m_num, &z);
        if (err) {
            CRAB_ERROR("Integer overflow during multiplication");
        }
        return safe_i64(z);
    }

    // TODO: output parameters whether operation overflows
    safe_i64 operator/(safe_i64 x) const{
        int64_t z;
        int err = checked_div(m_num, x.m_num, &z);
        if (err) {
            CRAB_ERROR("Integer overflow during multiplication");
        }
        return safe_i64(z);
    }

    // TODO: output parameters whether operation overflows
    safe_i64 operator-() const { return safe_i64(0) - *this; }


    // TODO: output parameters whether operation overflows
    safe_i64& operator+=(safe_i64 x){
        int err = checked_add(m_num, x.m_num, &m_num);
        if (err) {
            CRAB_ERROR("Integer overflow during addition");
        }
        return *this;
    }

    // TODO: output parameters whether operation overflows
    safe_i64& operator-=(safe_i64 x) {
        int err = checked_sub(m_num, x.m_num, &m_num);
        if (err) {
            CRAB_ERROR("Integer overflow during subtraction");
        }
        return *this;
    }

    bool operator==(safe_i64 x) const { return m_num == x.m_num; }

    bool operator!=(safe_i64 x) const { return m_num != x.m_num; }

    bool operator<(safe_i64 x) const { return m_num < x.m_num; }

    bool operator<=(safe_i64 x) const { return m_num <= x.m_num; }

    bool operator>(safe_i64 x) const { return m_num > x.m_num; }

    bool operator>=(safe_i64 x) const{ return m_num >= x.m_num; }

    void write(std::ostream& os) const { os << m_num; }

    friend std::ostream& operator<<(std::ostream& o, const safe_i64& n) {
        n.write(o);
        return o;
    }

  private:
    int64_t m_num;
};

} // end namespace crab
