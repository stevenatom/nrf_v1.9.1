/*********************************************************************
*                   (c) SEGGER Microcontroller GmbH                  *
*                        The Embedded Experts                        *
*                           www.segger.com                           *
**********************************************************************

-------------------------- END-OF-HEADER -----------------------------
*/

#ifndef __SEGGER_RTL_RISCV_CONF_H
#define __SEGGER_RTL_RISCV_CONF_H

/*********************************************************************
*
*       Applicability checks
*
**********************************************************************
*/

#if !defined(__riscv)
  #error This configuration file expects __riscv to be defined!
#elif !defined(__riscv_xlen) || (__riscv_xlen != 32 && __riscv_xlen != 64)
  #error This configuration file expects __riscv_xlen to be defined to 32 or 64!
#elif defined(__riscv_flen) && (__riscv_flen != 32 && __riscv_flen != 64)
  #error This configuration file expects __riscv_flen to be undefined or defined to 32 or 64!
#endif

/*********************************************************************
*
*       Defines, fixed
*
**********************************************************************
*/

//
// Values returned when classifying floating values for RISC-V using fclass instruction
//
#define __SEGGER_RTL_RV_NEG_INF                 (1<<0)
#define __SEGGER_RTL_RV_NEG_NORMAL              (1<<1)
#define __SEGGER_RTL_RV_NEG_SUBNORMAL           (1<<2)
#define __SEGGER_RTL_RV_NEG_ZERO                (1<<3)
#define __SEGGER_RTL_RV_POS_ZERO                (1<<4)
#define __SEGGER_RTL_RV_POS_SUBNORMAL           (1<<5)
#define __SEGGER_RTL_RV_POS_NORMAL              (1<<6)
#define __SEGGER_RTL_RV_POS_INF                 (1<<7)
#define __SEGGER_RTL_RV_SNAN                    (1<<8)
#define __SEGGER_RTL_RV_QNAN                    (1<<9)

/*********************************************************************
*
*       Configuration of compiler features.
*
**********************************************************************
*/

#if !defined(__SEGGER_RTL_NO_BUILTIN)
  #if defined(__clang__)
    #define __SEGGER_RTL_NO_BUILTIN
  #elif defined(__GNUC__)
    #define __SEGGER_RTL_NO_BUILTIN             __attribute__((optimize("-fno-tree-loop-distribute-patterns")))
  #endif
#endif

/*********************************************************************
*
*       Configuration of core features.
*
**********************************************************************
*/

#if defined(__riscv_abi_rve)
  #define __SEGGER_RTL_CORE_HAS_ISA_RVE                1
#else
  #define __SEGGER_RTL_CORE_HAS_ISA_RVE                0
#endif

#if defined(__riscv_dsp)
  #define __SEGGER_RTL_CORE_HAS_ISA_SIMD               1
#else
  #define __SEGGER_RTL_CORE_HAS_ISA_SIMD               0
#endif

#if defined(__nds_v5)
  #define __SEGGER_RTL_CORE_HAS_ISA_ANDES_V5           1
#else
  #define __SEGGER_RTL_CORE_HAS_ISA_ANDES_V5           0
#endif

#if defined(__riscv_mul)
  #define __SEGGER_RTL_CORE_HAS_MUL_MULH               1
#else
  #define __SEGGER_RTL_CORE_HAS_MUL_MULH               0
#endif

#if defined(__riscv_div)
  #define __SEGGER_RTL_CORE_HAS_DIV                    1
#else
  #define __SEGGER_RTL_CORE_HAS_DIV                    0
#endif

#if defined(__riscv_dsp)
  #define __SEGGER_RTL_CORE_HAS_CLZ32                  1
#else
  #define __SEGGER_RTL_CORE_HAS_CLZ32                  0
#endif

#if  defined(__riscv_zbb)
  #define __SEGGER_RTL_CORE_HAS_CLZ                    1
#else
  #define __SEGGER_RTL_CORE_HAS_CLZ                    0
#endif

#if defined(__riscv_zbb)
  #define __SEGGER_RTL_CORE_HAS_ANDN_ORN_XORN          1
#else
  #define __SEGGER_RTL_CORE_HAS_ANDN_ORN_XORN          0
#endif

#if defined(__riscv_zbs)
  #define __SEGGER_RTL_CORE_HAS_BSET_BCLR_BINV_BEXT    1
#else
  #define __SEGGER_RTL_CORE_HAS_BSET_BCLR_BINV_BEXT    0
#endif

#if defined(__riscv_zba)
  #define __SEGGER_RTL_CORE_HAS_SHxADD                 1
#else
  #define __SEGGER_RTL_CORE_HAS_SHxADD                 0
#endif

#ifndef   __SEGGER_RTL_CORE_HAS_FUSED_DIVREM
  #define __SEGGER_RTL_CORE_HAS_FUSED_DIVREM           0
#endif

#ifndef   __SEGGER_RTL_PREFER_BRANCH_FREE_CODE
  #define __SEGGER_RTL_PREFER_BRANCH_FREE_CODE         0
#endif

//
// Configuration of CLZ support.
//
#if __SEGGER_RTL_CORE_HAS_CLZ || __SEGGER_RTL_CORE_HAS_CLZ32
  #define __SEGGER_RTL_CLZ_U32(X)               __builtin_clz(X)
#endif

//
// GCC and clang provide a built-in va_list.
//
#if defined(__GNUC__) || defined(__clang__)
  #define __SEGGER_RTL_VA_LIST                  __builtin_va_list
#endif

//
// GCC and clang on RISC-V default to include the GNU libgcc API with assembly speedups.
//
#if defined(__GNUC__) || defined(__clang__)
  #if __riscv_xlen == 32
    #define __SEGGER_RTL_INCLUDE_GNU_API  2
  #else
    #define __SEGGER_RTL_INCLUDE_GNU_API  1
  #endif
#endif


//
// Configuration of byte order.
//
#define __SEGGER_RTL_BYTE_ORDER                 (-1)

//
// Configuration of typeset.
//
#define __SEGGER_RTL_TYPESET                    __riscv_xlen

#if !defined(__nds_v5)

  //
  // Configuration of 16-bit floating type.
  //
  #define __SEGGER_RTL_FLOAT16                  _Float16

  //
  // Configuration of 16-bit floating type runtime support.
  //
  #define __SEGGER_RTL_INCLUDE_GNU_FP16_API     1

#endif

//
// Configuration of maximal type alignment
//
#define __SEGGER_RTL_MAX_ALIGN                  16

//
// Configuration of floating-point ABI.
//
#if defined(__riscv_float_abi_soft)
  #define __SEGGER_RTL_FP_ABI                   0
#elif defined(__riscv_float_abi_single)
  #define __SEGGER_RTL_FP_ABI                   1
#elif defined(__riscv_float_abi_double)
  #define __SEGGER_RTL_FP_ABI                   2
#else
  #error Cannot determine RISC-V floating-point ABI
#endif

//
// Configuration of floating-point hardware.
//
#if defined(__riscv_flen) && (__riscv_flen == 64)
  #define __SEGGER_RTL_FP_HW                    2
#elif defined(__riscv_flen) && (__riscv_flen == 32)
  #define __SEGGER_RTL_FP_HW                    1
#else
  #define __SEGGER_RTL_FP_HW                    0
#endif

//
// Configuration of long double size
//
#ifndef   __SEGGER_RTL_SIZEOF_LDOUBLE
  #define __SEGGER_RTL_SIZEOF_LDOUBLE           16
#endif

//
// Configuration of full or compact NaNs.
//
#if __SEGGER_RTL_FP_ABI == 0
  #define __SEGGER_RTL_NAN_FORMAT               __SEGGER_RTL_NAN_FORMAT_IEEE
#endif

//
// Configuration of floating constant selection.
//
#if defined(__GNUC__) || defined(__clang__)
  #define __SEGGER_RTL_FLT_SELECT(HEX, DEC)     HEX
#endif

//
// Configuration of stack alignment.
//
#ifndef   __SEGGER_RTL_STACK_ALIGN
  #if __SEGGER_RTL_CORE_HAS_ISA_RVE
    #define __SEGGER_RTL_STACK_ALIGN            4    // 4-byte alignment for RV32E
  #else
    #define __SEGGER_RTL_STACK_ALIGN            16   // 16-byte alignment for RV32E
  #endif
#endif

//
// Configuration of multiplication capability.
//
#if (__riscv_xlen == 32) && __SEGGER_RTL_CORE_HAS_MUL_MULH
  //
  // RV32M
  //
  #define __SEGGER_RTL_SMULL_HI(x0, x1)                                                          \
    ({ int __p;                                                                                  \
      __asm__("mulh %0, %1, %2" : "=r"(__p) : "r"((unsigned)(x0)), "r"((unsigned)(x1)));         \
      __p;                                                                                       \
    })
  #define __SEGGER_RTL_SMULL(lo, hi, x0, x1)                                                     \
    do { unsigned __tmphi, __tmplo;                                                              \
      __asm__("mulh %0, %1, %2" : "=r"(__tmphi) : "r"((unsigned)(x0)), "r"((unsigned)(x1)) );    \
      __asm__("mul  %0, %1, %2" : "=r"(__tmplo) : "r"((unsigned)(x0)), "r"((unsigned)(x1)) );    \
      hi = __tmphi; lo = __tmplo;                                                                \
    } while (0)
  #define __SEGGER_RTL_SMULL_X(x0, x1)                                                           \
    ({ unsigned __thi, __tlo;                                                                    \
      __SEGGER_RTL_SMULL(__tlo, __thi, x0, x1);                                                  \
      (__SEGGER_RTL_I64)(((__SEGGER_RTL_U64)__thi << 32) + __tlo);                               \
    })
  #define __SEGGER_RTL_SMLAL(lo, hi, x0, x1)                                                     \
    do {                                                                                         \
      unsigned __tmp;                                                                            \
      __asm__("mul  %0, %1, %2" : "=r"(__tmp) : "r"((unsigned)(x0)), "r"((unsigned)(x1))   );    \
      __asm__("add  %0, %0, %1" : "+r"(lo)    : "r"(__tmp)                                 );    \
      __asm__("sltu %0, %1, %2" : "=r"(__tmp) : "r"((unsigned)(lo)), "r"((unsigned)__tmp)  );    \
      __asm__("add  %0, %0, %1" : "+r"(hi)    : "r"(__tmp)                                 );    \
      __asm__("mulh %0, %1, %2" : "=r"(__tmp) : "r"((unsigned)(x0)), "r"((unsigned)(x1))   );    \
      __asm__("add  %0, %0, %1" : "+r"(hi)    : "r"(__tmp)                                 );    \
    } while (0)
  #define __SEGGER_RTL_UMULL_HI(x0, x1)                                                          \
    ({ unsigned __product;                                                                       \
      __asm__("mulhu %0, %1, %2" : "=r"(__product) : "r"((unsigned)(x0)), "r"((unsigned)(x1)));  \
      __product;                                                                                 \
    })
  #define __SEGGER_RTL_UMULL(lo, hi, x0, x1)                                                     \
    do {                                                                                         \
      __asm__("mulhu %0, %1, %2" : "=r"(hi) : "r"((unsigned)(x0)), "r"((unsigned)(x1)) );        \
      __asm__("mul   %0, %1, %2" : "=r"(lo) : "r"((unsigned)(x0)), "r"((unsigned)(x1)) );        \
    } while (0)
  #define __SEGGER_RTL_UMLAL(lo, hi, x0, x1)                                                     \
    do {                                                                                         \
      unsigned __tmp;                                                                            \
      __asm__("mul   %0, %1, %2" : "=r"(__tmp) : "r"((unsigned)(x0)), "r"((unsigned)(x1)) );     \
      __asm__("add   %0, %0, %1" : "+r"(lo)    : "r"(__tmp)                               );     \
      __asm__("sltu  %0, %1, %2" : "=r"(__tmp) : "r"((unsigned)lo),   "r"((unsigned)__tmp));     \
      __asm__("add   %0, %0, %1" : "+r"(hi)    : "r"(__tmp)                               );     \
      __asm__("mulhu %0, %1, %2" : "=r"(__tmp) : "r"((unsigned)(x0)), "r"((unsigned)(x1)) );     \
      __asm__("add   %0, %0, %1" : "+r"(hi)    : "r"((unsigned)__tmp)                     );     \
    } while (0)
  //
#endif

//
// Configuration of thread-local storage
//
#if defined(__GNUC__) || defined(__clang__)
  #define __SEGGER_RTL_THREAD     __thread
#endif

//
// Configuration of inlining.
//
#if (defined(__GNUC__) || defined(__clang__)) && (__SEGGER_RTL_CONFIG_CODE_COVERAGE == 0)
  #if defined(__clang__)
    #define __SEGGER_RTL_NEVER_INLINE           __attribute__((__noinline__))  // clang does not support noclone.
  #else
    #define __SEGGER_RTL_NEVER_INLINE           __attribute__((__noinline__, __noclone__))
  #endif
  //
  #define __SEGGER_RTL_ALWAYS_INLINE            __inline__ __attribute__((__always_inline__))
  #define __SEGGER_RTL_REQUEST_INLINE           __inline__
#endif

//
// Configuration of static branch probability.
//
#if defined(__GNUC__) || defined(__clang__)
  #define __SEGGER_RTL_UNLIKELY(X)       __builtin_expect((X), 0)
#endif

//
// Configuration of division-remainder.
//
#if defined(__GNUC__)
  #define __SEGGER_RTL_DIVMOD_U32(Q, R, N, D)                                   \
    do {                                                                        \
      Q = N / D;                                                                \
      __asm__("# %0" : "+r"(Q));   /* Poison remainder rewrite by leaving       \
                                      compiler clueless as to the value         \
                                      contained in Q. */                        \
      R = N - Q*D;                                                              \
    } while (0)

  #define __SEGGER_RTL_DIVMOD_U64(Q, R, N, D)                                   \
    do {                                                                        \
      Q = N / D;                                                                \
      __asm__("# %0" : "+r"(Q));   /* Poison remainder rewrite by leaving       \
                                      compiler clueless as to the value         \
                                      contained in Q. */                        \
      R = N - Q*D;                                                              \
    } while (0)
#endif

//
// Configuration of floating-point inquiry functions.
//
#if defined(__riscv_flen) && (__riscv_flen >= 32) && !__SEGGER_RTL_FORCE_SOFT_FLOAT
  #if defined(__GNUC__) || defined(__clang__)

    #define __SEGGER_RTL_RV_FLOAT32_CLASSIFY(X)                                 \
      ({ unsigned __cls;                                                        \
        __asm__("fclass.s %0,%1" : "=r"(__cls) : "f"((float)(X)));              \
        __cls;                                                                  \
      })

    #define __SEGGER_RTL_FLOAT32_ISSPECIAL(X)                                   \
      (__SEGGER_RTL_RV_FLOAT32_CLASSIFY(X) & (__SEGGER_RTL_RV_NEG_ZERO      |   \
                                              __SEGGER_RTL_RV_POS_ZERO      |   \
                                              __SEGGER_RTL_RV_NEG_SUBNORMAL |   \
                                              __SEGGER_RTL_RV_POS_SUBNORMAL |   \
                                              __SEGGER_RTL_RV_NEG_INF       |   \
                                              __SEGGER_RTL_RV_POS_INF       |   \
                                              __SEGGER_RTL_RV_QNAN          |   \
                                              __SEGGER_RTL_RV_SNAN))

    #define __SEGGER_RTL_FLOAT32_ISSPECIAL_OR_NEGATIVE(X)                       \
      (__SEGGER_RTL_RV_FLOAT32_CLASSIFY(X) & (__SEGGER_RTL_RV_NEG_ZERO      |   \
                                              __SEGGER_RTL_RV_POS_ZERO      |   \
                                              __SEGGER_RTL_RV_NEG_SUBNORMAL |   \
                                              __SEGGER_RTL_RV_POS_SUBNORMAL |   \
                                              __SEGGER_RTL_RV_NEG_NORMAL    |   \
                                              __SEGGER_RTL_RV_NEG_INF       |   \
                                              __SEGGER_RTL_RV_POS_INF       |   \
                                              __SEGGER_RTL_RV_QNAN          |   \
                                              __SEGGER_RTL_RV_SNAN))

    #define __SEGGER_RTL_FLOAT32_ISINF(X)                                       \
      (__SEGGER_RTL_RV_FLOAT32_CLASSIFY(X) & (__SEGGER_RTL_RV_NEG_INF |         \
                                              __SEGGER_RTL_RV_POS_INF))

    #define __SEGGER_RTL_FLOAT32_ISPOSINF(X)                                    \
      (__SEGGER_RTL_RV_FLOAT32_CLASSIFY(X) & __SEGGER_RTL_RV_POS_INF)

    #define __SEGGER_RTL_FLOAT32_ISNEGINF(X)                                    \
      (__SEGGER_RTL_RV_FLOAT32_CLASSIFY(X) & __SEGGER_RTL_RV_NEG_INF)

    #define __SEGGER_RTL_FLOAT32_ISNAN(X)                                       \
      (__SEGGER_RTL_RV_FLOAT32_CLASSIFY(X) & (__SEGGER_RTL_RV_QNAN |            \
                                              __SEGGER_RTL_RV_SNAN))

    #define __SEGGER_RTL_FLOAT32_ISFINITE(X)                                    \
      (__SEGGER_RTL_RV_FLOAT32_CLASSIFY(X) & (__SEGGER_RTL_RV_NEG_ZERO      |   \
                                              __SEGGER_RTL_RV_POS_ZERO      |   \
                                              __SEGGER_RTL_RV_NEG_SUBNORMAL |   \
                                              __SEGGER_RTL_RV_POS_SUBNORMAL |   \
                                              __SEGGER_RTL_RV_NEG_NORMAL    |   \
                                              __SEGGER_RTL_RV_POS_NORMAL))

    #define __SEGGER_RTL_FLOAT32_ISNORMAL(X)                                    \
      (__SEGGER_RTL_RV_FLOAT32_CLASSIFY(X) & (__SEGGER_RTL_RV_POS_NORMAL |      \
                                              __SEGGER_RTL_RV_NEG_NORMAL))

    #define __SEGGER_RTL_FLOAT32_SIGNBIT_COPY(X, Y)                                        \
      ({ float __out;                                                                      \
         __asm__ ("fsgnj.s %0, %1, %2" : "=f"(__out) : "f"((float)(X)), "f"((float)(Y)));  \
         __out;                                                                            \
      })

    #define __SEGGER_RTL_FLOAT32_SIGNBIT_XOR(X, Y)                                         \
      ({ float __out;                                                                      \
         __asm__ ("fsgnjx.s %0, %1, %2" : "=f"(__out) : "f"((float)(X)), "f"((float)(Y))); \
         __out;                                                                            \
      })

    #define __SEGGER_RTL_FLOAT32_ABS(X)                                                    \
      ({ float __out;                                                                      \
         __asm__ ("fabs.s %0, %1" : "=f"(__out) : "f"((float)(X)));                        \
         __out;                                                                            \
      })

    #define __SEGGER_RTL_FLOAT32_FMA(X, Y, Z)                                              \
      ({ float __out;                                                                      \
         __asm__ ("fmadd.s %0, %1, %2, %3" : "=f"(__out) : "f"((float)(X)),                \
                                                           "f"((float)(Y)),                \
                                                           "f"((float)(Z)));               \
         __out;                                                                            \
      })

    #define __SEGGER_RTL_FLOAT32_SQRT_FAST(X)                                              \
      ({ float __result;                                                                   \
        __asm__("fsqrt.s %0, %1" : "=f"(__result) : "f"((float)(X)));                      \
        __result;                                                                          \
      })
    #endif
#endif

#if defined(__riscv_flen) && (__riscv_flen >= 64) && !__SEGGER_RTL_FORCE_SOFT_FLOAT
  #if defined(__GNUC__) || defined(__clang__)

    #define __SEGGER_RTL_RV_FLOAT64_CLASSIFY(X)                                 \
      ({ unsigned __cls;                                                        \
        __asm__("fclass.d %0,%1" : "=r"(__cls) : "f"((double)(X)));             \
        __cls;                                                                  \
      })

    #define __SEGGER_RTL_FLOAT64_ISSPECIAL(X)                                   \
      __SEGGER_RTL_RV_FLOAT64_CLASSIFY(X) & (__SEGGER_RTL_RV_NEG_ZERO      |    \
                                             __SEGGER_RTL_RV_POS_ZERO      |    \
                                             __SEGGER_RTL_RV_NEG_SUBNORMAL |    \
                                             __SEGGER_RTL_RV_POS_SUBNORMAL |    \
                                             __SEGGER_RTL_RV_NEG_INF       |    \
                                             __SEGGER_RTL_RV_POS_INF       |    \
                                             __SEGGER_RTL_RV_QNAN          |    \
                                             __SEGGER_RTL_RV_SNAN)

    #define __SEGGER_RTL_FLOAT64_ISSPECIAL_OR_NEGATIVE(X)                       \
      __SEGGER_RTL_RV_FLOAT64_CLASSIFY(X) & (__SEGGER_RTL_RV_NEG_ZERO      |    \
                                             __SEGGER_RTL_RV_POS_ZERO      |    \
                                             __SEGGER_RTL_RV_NEG_SUBNORMAL |    \
                                             __SEGGER_RTL_RV_POS_SUBNORMAL |    \
                                             __SEGGER_RTL_RV_NEG_NORMAL    |    \
                                             __SEGGER_RTL_RV_NEG_INF       |    \
                                             __SEGGER_RTL_RV_POS_INF       |    \
                                             __SEGGER_RTL_RV_QNAN          |    \
                                             __SEGGER_RTL_RV_SNAN)

    #define __SEGGER_RTL_FLOAT64_ISINF(X)                                       \
      __SEGGER_RTL_RV_FLOAT64_CLASSIFY(X) & (__SEGGER_RTL_RV_NEG_INF |          \
                                             __SEGGER_RTL_RV_POS_INF)

    #define __SEGGER_RTL_FLOAT64_ISNAN(X)                                       \
      __SEGGER_RTL_RV_FLOAT64_CLASSIFY(X) & (__SEGGER_RTL_RV_QNAN |             \
                                             __SEGGER_RTL_RV_SNAN)

    #define __SEGGER_RTL_FLOAT64_ISFINITE(X)                                    \
      __SEGGER_RTL_RV_FLOAT64_CLASSIFY(X) & (__SEGGER_RTL_RV_NEG_ZERO      |    \
                                             __SEGGER_RTL_RV_POS_ZERO      |    \
                                             __SEGGER_RTL_RV_NEG_SUBNORMAL |    \
                                             __SEGGER_RTL_RV_POS_SUBNORMAL |    \
                                             __SEGGER_RTL_RV_NEG_NORMAL    |    \
                                             __SEGGER_RTL_RV_POS_NORMAL)

    #define __SEGGER_RTL_FLOAT64_ISNORMAL(X)                                    \
      __SEGGER_RTL_RV_FLOAT64_CLASSIFY(X) & (__SEGGER_RTL_RV_POS_NORMAL |       \
                                             __SEGGER_RTL_RV_NEG_NORMAL)

    #define __SEGGER_RTL_FLOAT64_SIGNBIT_COPY(X, Y)                                          \
      ({ double __out;                                                                       \
         __asm__ ("fsgnj.d %0, %1, %2" : "=f"(__out) : "f"((double)(X)), "f"((double)(Y)));  \
         __out;                                                                              \
      })

    #define __SEGGER_RTL_FLOAT64_SIGNBIT_XOR(X, Y)                                           \
      ({ double __out;                                                                       \
         __asm__ ("fsgnjx.d %0, %1, %2" : "=f"(__out) : "f"((double)(X)), "f"((double)(Y))); \
         __out;                                                                              \
      })

    #define __SEGGER_RTL_FLOAT64_ABS(X)                                                      \
      ({ double __out;                                                                       \
         __asm__ ("fabs.d %0, %1" : "=f"(__out) : "f"((double)(X)));                         \
         __out;                                                                              \
      })

    #define __SEGGER_RTL_FLOAT64_FMA(X, Y, Z)                                                \
      ({ double __out;                                                                       \
         __asm__ ("fmadd.d %0, %1, %2, %3" : "=f"(__out) : "f"((double)(X)),                 \
                                                           "f"((double)(Y)),                 \
                                                           "f"((double)(Z)));                \
         __out;                                                                              \
      })

    #define __SEGGER_RTL_FLOAT64_SQRT_FAST(X)                                                \
      ({ double __result;                                                                    \
        __asm__("fsqrt.d %0, %1" : "=f"(__result) : "f"((double)(X)));                       \
        __result;                                                                            \
      })
  #endif
#endif

//
// GCC and clang provide a built-in support for _Complex.
//
#if defined(__GNUC__) || defined(__clang__)
  #ifndef   __SEGGER_RTL_FLOAT32_C_COMPLEX
    #define __SEGGER_RTL_FLOAT32_C_COMPLEX      float _Complex
  #endif
  #ifndef   __SEGGER_RTL_FLOAT64_C_COMPLEX
    #define __SEGGER_RTL_FLOAT64_C_COMPLEX      double _Complex
  #endif
  #ifndef   __SEGGER_RTL_LDOUBLE_C_COMPLEX
    #define __SEGGER_RTL_LDOUBLE_C_COMPLEX      long double _Complex
  #endif
#endif

//
// Configuration of public APIs.
//
#if defined(__GNUC__) || defined(__clang__)
  #define __SEGGER_RTL_PUBLIC_API               __attribute__((__weak__))
#endif

//
// For RISC-V architectures using GNU C or clang, the SEGGER RTL offers
// optimized versions written in GNU-compatible assembly language.
// Selection of them is made here.
//
#if defined(__SEGGER_RTL_COMPILING_LIBRARY) && __SEGGER_RTL_COMPILING_LIBRARY && (__SEGGER_RTL_INCLUDE_GNU_API != 1)
  #if defined(__GNUC__) || defined(__clang__)
    #define strlen(x)                           __SEGGER_RTL_HIDE(strlen)(x)
    #define strcpy(x, y)                        __SEGGER_RTL_HIDE(strcpy)(x, y)
    #define strcmp(x, y)                        __SEGGER_RTL_HIDE(strcmp)(x, y)
    #define strchr(x, y)                        __SEGGER_RTL_HIDE(strchr)(x, y)
    #define __mulsi3(x, y)                      __SEGGER_RTL_HIDE(__mulsi3)(x, y)
    #define __divsi3(x, y)                      __SEGGER_RTL_HIDE(__divsi3)(x, y)
    #define __modsi3(x, y)                      __SEGGER_RTL_HIDE(__modsi3)(x, y)
    #define __udivsi3(x, y)                     __SEGGER_RTL_HIDE(__udivsi3)(x, y)
    #define __umodsi3(x, y)                     __SEGGER_RTL_HIDE(__umodsi3)(x, y)
    //
    // RV32{E,I} defaults to assembly language implementation.
    // RV64I defaults to C implementation.
    //
    #if __riscv_xlen == 32
      #define __eqsf2(x, y)                     __SEGGER_RTL_HIDE(__eqsf2)(x, y)
      #define __nesf2(x, y)                     __SEGGER_RTL_HIDE(__nesf2)(x, y)
      #define __ltsf2(x, y)                     __SEGGER_RTL_HIDE(__ltsf2)(x, y)
      #define __lesf2(x, y)                     __SEGGER_RTL_HIDE(__lesf2)(x, y)
      #define __gtsf2(x, y)                     __SEGGER_RTL_HIDE(__gtsf2)(x, y)
      #define __gesf2(x, y)                     __SEGGER_RTL_HIDE(__gesf2)(x, y)
      #define __unordsf2(x, y)                  __SEGGER_RTL_HIDE(__unordsf2)(x, y)
      //
      #define __eqdf2(x, y)                     __SEGGER_RTL_HIDE(__eqdf2)(x, y)
      #define __nedf2(x, y)                     __SEGGER_RTL_HIDE(__nedf2)(x, y)
      #define __ltdf2(x, y)                     __SEGGER_RTL_HIDE(__ltdf2)(x, y)
      #define __ledf2(x, y)                     __SEGGER_RTL_HIDE(__ledf2)(x, y)
      #define __gtdf2(x, y)                     __SEGGER_RTL_HIDE(__gtdf2)(x, y)
      #define __gedf2(x, y)                     __SEGGER_RTL_HIDE(__gedf2)(x, y)
      #define __unorddf2(x, y)                  __SEGGER_RTL_HIDE(__unorddf2)(x, y)
      //
      #define __addsf3(x, y)                    __SEGGER_RTL_HIDE(__addsf3)(x, y)
      #define __subsf3(x, y)                    __SEGGER_RTL_HIDE(__subsf3)(x, y)
      #define __mulsf3(x, y)                    __SEGGER_RTL_HIDE(__mulsf3)(x, y)
      #define __divsf3(x, y)                    __SEGGER_RTL_HIDE(__divsf3)(x, y)
      //
      #define __adddf3(x, y)                    __SEGGER_RTL_HIDE(__adddf3)(x, y)
      #define __subdf3(x, y)                    __SEGGER_RTL_HIDE(__subdf3)(x, y)
      #define __muldf3(x, y)                    __SEGGER_RTL_HIDE(__muldf3)(x, y)
      #define __divdf3(x, y)                    __SEGGER_RTL_HIDE(__divdf3)(x, y)
      //
      #define __fixsfsi(x)                      __SEGGER_RTL_HIDE(__fixsfsi)(x)
      #define __fixsfdi(x)                      __SEGGER_RTL_HIDE(__fixsfdi)(x)
      #define __fixdfsi(x)                      __SEGGER_RTL_HIDE(__fixdfsi)(x)
      #define __fixdfdi(x)                      __SEGGER_RTL_HIDE(__fixdfdi)(x)
      #define __fixunssfsi(x)                   __SEGGER_RTL_HIDE(__fixunssfsi)(x)
      #define __fixunssfdi(x)                   __SEGGER_RTL_HIDE(__fixunssfdi)(x)
      #define __fixunsdfsi(x)                   __SEGGER_RTL_HIDE(__fixunsdfsi)(x)
      #define __fixunsdfdi(x)                   __SEGGER_RTL_HIDE(__fixunsdfdi)(x)
      //
      #define __floatsisf(x)                    __SEGGER_RTL_HIDE(__floatsisf)(x)
      #define __floatsidf(x)                    __SEGGER_RTL_HIDE(__floatsidf)(x)
      #define __floatdisf(x)                    __SEGGER_RTL_HIDE(__floatdisf)(x)
      #define __floatdidf(x)                    __SEGGER_RTL_HIDE(__floatdidf)(x)
      #define __floatunsisf(x)                  __SEGGER_RTL_HIDE(__floatunsisf)(x)
      #define __floatunsidf(x)                  __SEGGER_RTL_HIDE(__floatunsidf)(x)
      #define __floatundisf(x)                  __SEGGER_RTL_HIDE(__floatundisf)(x)
      #define __floatundidf(x)                  __SEGGER_RTL_HIDE(__floatundidf)(x)
      //
      #define __extendsfdf2(x)                  __SEGGER_RTL_HIDE(__extendsfdf2)(x)
      #define __truncdfsf2(x)                   __SEGGER_RTL_HIDE(__truncdfsf2)(x)
    #endif
  #endif
#endif

#if (__SEGGER_RTL_STACK_ALIGN % 4) != 0
  #error Stack alignment must be a multiple of 4
#endif

#endif

/*************************** End of file ****************************/
