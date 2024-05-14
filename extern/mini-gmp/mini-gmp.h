/* mini-gmp, a minimalistic implementation of a GNU GMP subset.

Copyright 2011-2015, 2017, 2019-2021 Free Software Foundation, Inc.

This file is part of the GNU MP Library.

The GNU MP Library is free software; you can redistribute it and/or modify
it under the terms of either:

  * the GNU Lesser General Public License as published by the Free
    Software Foundation; either version 3 of the License, or (at your
    option) any later version.

or

  * the GNU General Public License as published by the Free Software
    Foundation; either version 2 of the License, or (at your option) any
    later version.

or both in parallel, as here.

The GNU MP Library is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received copies of the GNU General Public License and the
GNU Lesser General Public License along with the GNU MP Library.  If not,
see https://www.gnu.org/licenses/.  */

/* About mini-gmp: This is a minimal implementation of a subset of the
   GMP interface. It is intended for inclusion into applications which
   have modest bignums needs, as a fallback when the real GMP library
   is not installed.

   This file defines the public interface. */

#ifndef __MINI_GMP_H__
#define __MINI_GMP_H__

/* For size_t */
#include <stddef.h>

/* For realloc */
#include <stdlib.h>

/* For uint64_t and int64_t */
#include <stdint.h>

#if defined(__cplusplus)
extern "C" {
#endif

void mp_set_memory_functions(
    void* (*)(size_t), void* (*)(void*, size_t, size_t), void (*)(void*, size_t)
);

void mp_get_memory_functions(
    void* (**)(size_t), void* (**)(void*, size_t, size_t), void (**)(void*, size_t)
);

/* ********************************************************************************** *
 * *                               GMP limb types                                   * *
 * ********************************************************************************** */

#if !defined(COMPILER_LIMB_SIZE)
#error "C Macro `COMPILER_LIMB_SIZE` not specified. Must be set during compilation."
#else /* defined(COMPILER_LIMB_SIZE) */
#if COMPILER_LIMB_SIZE == 32
typedef uint32_t mp_limb_t;
typedef int32_t mp_limb_signed_t;
#elif COMPILER_LIMB_SIZE == 64
typedef uint64_t mp_limb_t;
typedef int64_t mp_limb_signed_t;
#elif COMPILER_LIMB_SIZE == NATIVE
#if SIZE_MAX == 4294967295ull             /* 32-bit system detected */
typedef uint32_t mp_limb_t;
typedef int32_t mp_limb_signed_t;
#elif SIZE_MAX == 18446744073709551615ull /* 64-bit system detected */
typedef uint64_t mp_limb_t;
typedef int64_t mp_limb_signed_t;
#else
#error "Could not detect native target architecture word size."
#endif
#else
#error "C Macro `COMPILER_LIMB_SIZE` must be one of `32`, `64` or `NATIVE`"
#endif

#endif

/* ********************************************************************************** *
 * *                               Other GMP types                                  * *
 * ********************************************************************************** */

typedef long mp_size_t;
typedef unsigned long mp_bitcnt_t;

typedef mp_limb_t* mp_ptr;
typedef const mp_limb_t* mp_srcptr;

typedef struct {
    int _mp_alloc;    /* Number of *limbs* allocated and pointed
                         to by the _mp_d field.  */
    int _mp_size;     /* abs(_mp_size) is the number of limbs the
                         last field points to.  If _mp_size is
                         negative this is a negative number.  */
    mp_limb_t* _mp_d; /* Pointer to the limbs.  */
} __mpz_struct;

typedef __mpz_struct mpz_t[1];

typedef __mpz_struct* mpz_ptr;
typedef const __mpz_struct* mpz_srcptr;

extern const int mp_bits_per_limb;

/*
 * APyTypes custom changes to get mini-gmp working with APyTypes. These additions are
 * needed to provide functionallity for:
 *
 *   * Nail support for `mpz_import` and `mpz_export`
 *   * Division support using `mpn_div_qr`
 *
 * Mikael Henriksson (2024)
 */
#define ABS(x) ((x) >= 0 ? (x) : -(x))
#define ABSIZ(x) ABS(SIZ(x))
#define ALLOC(x) ((x)->_mp_alloc)
#define MPZ_NEWALLOC(z, n) (((n) > ALLOC(z)) ? (mp_ptr)mpz_realloc(z, n) : PTR(z))
#define PTR(x) ((x)->_mp_d)
#define SIZ(x) ((x)->_mp_size)

#define BITS_TO_LIMBS(n) (((n) + (GMP_NUMB_BITS - 1)) / GMP_NUMB_BITS)
#define GMP_LIMB_BITS (8 * sizeof(mp_limb_t))
#define GMP_NAIL_BITS 0
#define GMP_NUMB_BITS (GMP_LIMB_BITS - GMP_NAIL_BITS)
#define GMP_NUMB_MASK ((~((mp_limb_t)0)) >> GMP_NAIL_BITS)

static const mp_limb_t endian_test = (((mp_limb_t)1) << (GMP_LIMB_BITS - 7)) - 1;
#define HOST_ENDIAN (*(signed char*)&endian_test)

static inline size_t mini_gmp_bit_width(mp_limb_t x)
{
    // Optimized on x86-64 using single `bsr` instruction since GCC-13.1
    size_t result = 0;
    while (x) {
        x >>= 1;
        result++;
    }
    return result;
}

#define COUNT_LEADING_ZEROS(count, limb)                                               \
    do {                                                                               \
        count = 8 * sizeof(limb) - mini_gmp_bit_width(limb);                           \
    } while (0)

#define MPN_NORMALIZE(DST, NLIMBS)                                                     \
    do {                                                                               \
        while ((NLIMBS) > 0) {                                                         \
            if ((DST)[(NLIMBS) - 1] != 0)                                              \
                break;                                                                 \
            (NLIMBS)--;                                                                \
        }                                                                              \
    } while (0)

#define MPN_SIZEINBASE_2EXP(result, ptr, size, base2exp)                               \
    do {                                                                               \
        int __cnt;                                                                     \
        mp_bitcnt_t __totbits;                                                         \
        assert((size) > 0);                                                            \
        assert((ptr)[(size) - 1] != 0);                                                \
        COUNT_LEADING_ZEROS(__cnt, (ptr)[(size) - 1]);                                 \
        __totbits = (mp_bitcnt_t)(size) * GMP_NUMB_BITS - (__cnt - GMP_NAIL_BITS);     \
        (result) = (__totbits + (base2exp) - 1) / (base2exp);                          \
    } while (0)

#define MPN_COPY_INCR(dst, src, n)                                                     \
    do {                                                                               \
        assert((n) >= 0);                                                              \
        if ((n) != 0) {                                                                \
            mp_size_t __n = (n) - 1;                                                   \
            mp_ptr __dst = (dst);                                                      \
            mp_srcptr __src = (src);                                                   \
            mp_limb_t __x;                                                             \
            __x = *__src++;                                                            \
            if (__n != 0) {                                                            \
                do {                                                                   \
                    *__dst++ = __x;                                                    \
                    __x = *__src++;                                                    \
                } while (--__n);                                                       \
            }                                                                          \
            *__dst++ = __x;                                                            \
        }                                                                              \
    } while (0)

#define MPN_COPY(d, s, n)                                                              \
    do {                                                                               \
        MPN_COPY_INCR(d, s, n);                                                        \
    } while (0)

/*
 * End of APyTypes custom changes to mini-gmp.h
 */

void mpn_copyi(mp_ptr, mp_srcptr, mp_size_t);
void mpn_copyd(mp_ptr, mp_srcptr, mp_size_t);
void mpn_zero(mp_ptr, mp_size_t);

int mpn_cmp(mp_srcptr, mp_srcptr, mp_size_t);
int mpn_zero_p(mp_srcptr, mp_size_t);

mp_limb_t mpn_add_1(mp_ptr, mp_srcptr, mp_size_t, mp_limb_t);
mp_limb_t mpn_add_n(mp_ptr, mp_srcptr, mp_srcptr, mp_size_t);
mp_limb_t mpn_add(mp_ptr, mp_srcptr, mp_size_t, mp_srcptr, mp_size_t);

mp_limb_t mpn_sub_1(mp_ptr, mp_srcptr, mp_size_t, mp_limb_t);
mp_limb_t mpn_sub_n(mp_ptr, mp_srcptr, mp_srcptr, mp_size_t);
mp_limb_t mpn_sub(mp_ptr, mp_srcptr, mp_size_t, mp_srcptr, mp_size_t);

mp_limb_t mpn_mul_1(mp_ptr, mp_srcptr, mp_size_t, mp_limb_t);
mp_limb_t mpn_addmul_1(mp_ptr, mp_srcptr, mp_size_t, mp_limb_t);
mp_limb_t mpn_submul_1(mp_ptr, mp_srcptr, mp_size_t, mp_limb_t);

mp_limb_t mpn_mul(mp_ptr, mp_srcptr, mp_size_t, mp_srcptr, mp_size_t);
void mpn_mul_n(mp_ptr, mp_srcptr, mp_srcptr, mp_size_t);
void mpn_sqr(mp_ptr, mp_srcptr, mp_size_t);
int mpn_perfect_square_p(mp_srcptr, mp_size_t);
mp_size_t mpn_sqrtrem(mp_ptr, mp_ptr, mp_srcptr, mp_size_t);

mp_limb_t mpn_lshift(mp_ptr, mp_srcptr, mp_size_t, unsigned int);
mp_limb_t mpn_rshift(mp_ptr, mp_srcptr, mp_size_t, unsigned int);

mp_bitcnt_t mpn_scan0(mp_srcptr, mp_bitcnt_t);
mp_bitcnt_t mpn_scan1(mp_srcptr, mp_bitcnt_t);

void mpn_com(mp_ptr, mp_srcptr, mp_size_t);
mp_limb_t mpn_neg(mp_ptr, mp_srcptr, mp_size_t);

mp_bitcnt_t mpn_popcount(mp_srcptr, mp_size_t);

mp_limb_t mpn_invert_3by2(mp_limb_t, mp_limb_t);
#define mpn_invert_limb(x) mpn_invert_3by2((x), 0)

size_t mpn_get_str(unsigned char*, int, mp_ptr, mp_size_t);
mp_size_t mpn_set_str(mp_ptr, const unsigned char*, size_t, int);

/* APyTypes custom add: `mpn_div_qr` */
void mpn_div_qr(mp_ptr qp, mp_ptr np, mp_size_t nn, mp_srcptr dp, mp_size_t dn);

void mpz_init(mpz_t);
void mpz_init2(mpz_t, mp_bitcnt_t);
void mpz_clear(mpz_t);

#define mpz_odd_p(z) (((z)->_mp_size != 0) & (int)(z)->_mp_d[0])
#define mpz_even_p(z) (!mpz_odd_p(z))

int mpz_sgn(const mpz_t);
int mpz_cmp_si(const mpz_t, long);
int mpz_cmp_ui(const mpz_t, unsigned long);
int mpz_cmp(const mpz_t, const mpz_t);
int mpz_cmpabs_ui(const mpz_t, unsigned long);
int mpz_cmpabs(const mpz_t, const mpz_t);
int mpz_cmp_d(const mpz_t, double);
int mpz_cmpabs_d(const mpz_t, double);

void mpz_abs(mpz_t, const mpz_t);
void mpz_neg(mpz_t, const mpz_t);
void mpz_swap(mpz_t, mpz_t);

void mpz_add_ui(mpz_t, const mpz_t, unsigned long);
void mpz_add(mpz_t, const mpz_t, const mpz_t);
void mpz_sub_ui(mpz_t, const mpz_t, unsigned long);
void mpz_ui_sub(mpz_t, unsigned long, const mpz_t);
void mpz_sub(mpz_t, const mpz_t, const mpz_t);

void mpz_mul_si(mpz_t, const mpz_t, long int);
void mpz_mul_ui(mpz_t, const mpz_t, unsigned long int);
void mpz_mul(mpz_t, const mpz_t, const mpz_t);
void mpz_mul_2exp(mpz_t, const mpz_t, mp_bitcnt_t);
void mpz_addmul_ui(mpz_t, const mpz_t, unsigned long int);
void mpz_addmul(mpz_t, const mpz_t, const mpz_t);
void mpz_submul_ui(mpz_t, const mpz_t, unsigned long int);
void mpz_submul(mpz_t, const mpz_t, const mpz_t);

void mpz_cdiv_qr(mpz_t, mpz_t, const mpz_t, const mpz_t);
void mpz_fdiv_qr(mpz_t, mpz_t, const mpz_t, const mpz_t);
void mpz_tdiv_qr(mpz_t, mpz_t, const mpz_t, const mpz_t);
void mpz_cdiv_q(mpz_t, const mpz_t, const mpz_t);
void mpz_fdiv_q(mpz_t, const mpz_t, const mpz_t);
void mpz_tdiv_q(mpz_t, const mpz_t, const mpz_t);
void mpz_cdiv_r(mpz_t, const mpz_t, const mpz_t);
void mpz_fdiv_r(mpz_t, const mpz_t, const mpz_t);
void mpz_tdiv_r(mpz_t, const mpz_t, const mpz_t);

void mpz_cdiv_q_2exp(mpz_t, const mpz_t, mp_bitcnt_t);
void mpz_fdiv_q_2exp(mpz_t, const mpz_t, mp_bitcnt_t);
void mpz_tdiv_q_2exp(mpz_t, const mpz_t, mp_bitcnt_t);
void mpz_cdiv_r_2exp(mpz_t, const mpz_t, mp_bitcnt_t);
void mpz_fdiv_r_2exp(mpz_t, const mpz_t, mp_bitcnt_t);
void mpz_tdiv_r_2exp(mpz_t, const mpz_t, mp_bitcnt_t);

void mpz_mod(mpz_t, const mpz_t, const mpz_t);

void mpz_divexact(mpz_t, const mpz_t, const mpz_t);

int mpz_divisible_p(const mpz_t, const mpz_t);
int mpz_congruent_p(const mpz_t, const mpz_t, const mpz_t);

unsigned long mpz_cdiv_qr_ui(mpz_t, mpz_t, const mpz_t, unsigned long);
unsigned long mpz_fdiv_qr_ui(mpz_t, mpz_t, const mpz_t, unsigned long);
unsigned long mpz_tdiv_qr_ui(mpz_t, mpz_t, const mpz_t, unsigned long);
unsigned long mpz_cdiv_q_ui(mpz_t, const mpz_t, unsigned long);
unsigned long mpz_fdiv_q_ui(mpz_t, const mpz_t, unsigned long);
unsigned long mpz_tdiv_q_ui(mpz_t, const mpz_t, unsigned long);
unsigned long mpz_cdiv_r_ui(mpz_t, const mpz_t, unsigned long);
unsigned long mpz_fdiv_r_ui(mpz_t, const mpz_t, unsigned long);
unsigned long mpz_tdiv_r_ui(mpz_t, const mpz_t, unsigned long);
unsigned long mpz_cdiv_ui(const mpz_t, unsigned long);
unsigned long mpz_fdiv_ui(const mpz_t, unsigned long);
unsigned long mpz_tdiv_ui(const mpz_t, unsigned long);

unsigned long mpz_mod_ui(mpz_t, const mpz_t, unsigned long);

void mpz_divexact_ui(mpz_t, const mpz_t, unsigned long);

int mpz_divisible_ui_p(const mpz_t, unsigned long);

unsigned long mpz_gcd_ui(mpz_t, const mpz_t, unsigned long);
void mpz_gcd(mpz_t, const mpz_t, const mpz_t);
void mpz_gcdext(mpz_t, mpz_t, mpz_t, const mpz_t, const mpz_t);
void mpz_lcm_ui(mpz_t, const mpz_t, unsigned long);
void mpz_lcm(mpz_t, const mpz_t, const mpz_t);
int mpz_invert(mpz_t, const mpz_t, const mpz_t);

void mpz_sqrtrem(mpz_t, mpz_t, const mpz_t);
void mpz_sqrt(mpz_t, const mpz_t);
int mpz_perfect_square_p(const mpz_t);

void mpz_pow_ui(mpz_t, const mpz_t, unsigned long);
void mpz_ui_pow_ui(mpz_t, unsigned long, unsigned long);
void mpz_powm(mpz_t, const mpz_t, const mpz_t, const mpz_t);
void mpz_powm_ui(mpz_t, const mpz_t, unsigned long, const mpz_t);

void mpz_rootrem(mpz_t, mpz_t, const mpz_t, unsigned long);
int mpz_root(mpz_t, const mpz_t, unsigned long);

void mpz_fac_ui(mpz_t, unsigned long);
void mpz_2fac_ui(mpz_t, unsigned long);
void mpz_mfac_uiui(mpz_t, unsigned long, unsigned long);
void mpz_bin_uiui(mpz_t, unsigned long, unsigned long);

int mpz_probab_prime_p(const mpz_t, int);

int mpz_tstbit(const mpz_t, mp_bitcnt_t);
void mpz_setbit(mpz_t, mp_bitcnt_t);
void mpz_clrbit(mpz_t, mp_bitcnt_t);
void mpz_combit(mpz_t, mp_bitcnt_t);

void mpz_com(mpz_t, const mpz_t);
void mpz_and(mpz_t, const mpz_t, const mpz_t);
void mpz_ior(mpz_t, const mpz_t, const mpz_t);
void mpz_xor(mpz_t, const mpz_t, const mpz_t);

mp_bitcnt_t mpz_popcount(const mpz_t);
mp_bitcnt_t mpz_hamdist(const mpz_t, const mpz_t);
mp_bitcnt_t mpz_scan0(const mpz_t, mp_bitcnt_t);
mp_bitcnt_t mpz_scan1(const mpz_t, mp_bitcnt_t);

int mpz_fits_slong_p(const mpz_t);
int mpz_fits_ulong_p(const mpz_t);
int mpz_fits_sint_p(const mpz_t);
int mpz_fits_uint_p(const mpz_t);
int mpz_fits_sshort_p(const mpz_t);
int mpz_fits_ushort_p(const mpz_t);
long int mpz_get_si(const mpz_t);
unsigned long int mpz_get_ui(const mpz_t);
double mpz_get_d(const mpz_t);
size_t mpz_size(const mpz_t);
mp_limb_t mpz_getlimbn(const mpz_t, mp_size_t);

void mpz_realloc2(mpz_t, mp_bitcnt_t);
mp_srcptr mpz_limbs_read(mpz_srcptr);
mp_ptr mpz_limbs_modify(mpz_t, mp_size_t);
mp_ptr mpz_limbs_write(mpz_t, mp_size_t);
void mpz_limbs_finish(mpz_t, mp_size_t);
mpz_srcptr mpz_roinit_n(mpz_t, mp_srcptr, mp_size_t);

#define MPZ_ROINIT_N(xp, xs)                                                           \
    {                                                                                  \
        {                                                                              \
            0, (xs), (xp)                                                              \
        }                                                                              \
    }

void mpz_set_si(mpz_t, signed long int);
void mpz_set_ui(mpz_t, unsigned long int);
void mpz_set(mpz_t, const mpz_t);
void mpz_set_d(mpz_t, double);

void mpz_init_set_si(mpz_t, signed long int);
void mpz_init_set_ui(mpz_t, unsigned long int);
void mpz_init_set(mpz_t, const mpz_t);
void mpz_init_set_d(mpz_t, double);

size_t mpz_sizeinbase(const mpz_t, int);
char* mpz_get_str(char*, int, const mpz_t);
int mpz_set_str(mpz_t, const char*, int);
int mpz_init_set_str(mpz_t, const char*, int);

/* This long list taken from gmp.h. */
/* For reference, "defined(EOF)" cannot be used here.  In g++ 2.95.4,
   <iostream> defines EOF but not FILE.  */
#if defined(FILE) || defined(H_STDIO) || defined(_H_STDIO) /* AIX */                   \
    || defined(_STDIO_H)                                   /* glibc, Sun, SCO */       \
    || defined(_STDIO_H_)                                  /* BSD, OSF */              \
    || defined(__STDIO_H)                                  /* Borland */               \
    || defined(__STDIO_H__)                                /* IRIX */                  \
    || defined(_STDIO_INCLUDED)                            /* HPUX */                  \
    || defined(__dj_include_stdio_h_)                      /* DJGPP */                 \
    || defined(_FILE_DEFINED)                              /* Microsoft */             \
    || defined(__STDIO__)                                  /* Apple MPW MrC */         \
    || defined(_MSL_STDIO_H)                               /* Metrowerks */            \
    || defined(_STDIO_H_INCLUDED)                          /* QNX4 */                  \
    || defined(_ISO_STDIO_ISO_H)                           /* Sun C++ */               \
    || defined(__STDIO_LOADED)                             /* VMS */                   \
    || defined(_STDIO)                                     /* HPE NonStop */           \
    || defined(__DEFINED_FILE)                             /* musl */
size_t mpz_out_str(FILE*, int, const mpz_t);
#endif

void mpz_import(mpz_t, size_t, int, size_t, int, size_t, const void*);
void* mpz_export(void*, size_t*, int, size_t, int, size_t, const mpz_t);

#if defined(__cplusplus)
}
#endif
#endif /* __MINI_GMP_H__ */
