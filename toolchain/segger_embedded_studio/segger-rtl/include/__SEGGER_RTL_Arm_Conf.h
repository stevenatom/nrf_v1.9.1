/*********************************************************************
*                   (c) SEGGER Microcontroller GmbH                  *
*                        The Embedded Experts                        *
*                           www.segger.com                           *
**********************************************************************

-------------------------- END-OF-HEADER -----------------------------
*/

#ifndef __SEGGER_RTL_ARM_CONF_H
#define __SEGGER_RTL_ARM_CONF_H

/*********************************************************************
*
*       Applicability checks
*
**********************************************************************
*/

// Not all compilers define __ARM_ACLE so this is disabled for now
#if 0 && !defined(__ARM_ACLE)
  #error This configuration file expects and ACLE-conforming compiler for configuration!
#endif
#if !defined(__ARM_ARCH_ISA_ARM) && !defined(__ARM_ARCH_ISA_THUMB)
  #error This configuration file expects __ARM_ARCH_ISA_ARM or __ARM_ARCH_ISA_THUMB to be defined!
#endif

/*********************************************************************
*
*       Defines, fixed
*
**********************************************************************
*/

#define __SEGGER_RTL_ISA_T16                    0
#define __SEGGER_RTL_ISA_T32                    1
#define __SEGGER_RTL_ISA_ARM                    2

/*********************************************************************
*
*       Defines, configurable
*
**********************************************************************
*/

#if !defined(__SEGGER_RTL_NO_BUILTIN)
  #if defined(__clang__)
    #define __SEGGER_RTL_NO_BUILTIN
  #elif defined(__GNUC__)
    #define __SEGGER_RTL_NO_BUILTIN             __attribute__((optimize("-fno-tree-loop-distribute-patterns")))
  #else
  #endif
#endif
 
#if defined(__thumb__) && !defined(__thumb2__)
  #define __SEGGER_RTL_TARGET_ISA               __SEGGER_RTL_ISA_T16
#elif defined(__thumb2__)
  #define __SEGGER_RTL_TARGET_ISA               __SEGGER_RTL_ISA_T32
#else
  #define __SEGGER_RTL_TARGET_ISA               __SEGGER_RTL_ISA_ARM
#endif

//
// GCC and clang on ARM default to include the Arm AEABI
// with assembly speedups (2).  Define this to 1 to use the
// C implementation.
//
#if defined(__GNUC__) || defined(__clang__)
  #define __SEGGER_RTL_INCLUDE_AEABI_API        2
#endif

//
// GCC on Arm doesn't use the AEABI naming conventio for half-float.
// Define this to 1 to force inclusion for this retrograde compiler.
//
#if defined(__GNUC__) || defined(__clang__)
  #define __SEGGER_RTL_INCLUDE_GNU_FP16_API     1
#endif

//
// Configuration of byte order.
//
#if defined(__ARM_BIG_ENDIAN) && (__ARM_BIG_ENDIAN == 1)
  #define __SEGGER_RTL_BYTE_ORDER               (+1)
#else
  #define __SEGGER_RTL_BYTE_ORDER               (-1)
#endif

//
// Configuration of typeset.
//
#define __SEGGER_RTL_TYPESET                    32

//
// Configuration of 16-bit floating type.
//
#define __SEGGER_RTL_FLOAT16                    _Float16

//
// Configuration of maximal type alignment
//
#define __SEGGER_RTL_MAX_ALIGN                  8

//
// Configuration of floating-point ABI.
//
#if defined(__ARM_PCS_VFP) && (__ARM_PCS_VFP == 1)
  //
  // PCS uses hardware registers for passing parameters.  For VFP
  // with only single-precision operations, parameters are still
  // passed in floating registers.
  //
  #define __SEGGER_RTL_FP_ABI                   2
  //
#elif defined(__ARM_PCS) && (__ARM_PCS == 1)
  //
  // PCS is standard integer PCS.
  //
  #define __SEGGER_RTL_FP_ABI                   0
  //
#else
  #error Unable to determine floating-point ABI used
#endif

//
// Configuration of floating-point hardware.
//
#if defined(__ARM_FP) && (__ARM_FP & 0x08)
  #define __SEGGER_RTL_FP_HW                    2
#elif defined(__ARM_FP) && (__ARM_FP & 0x04)
  #define __SEGGER_RTL_FP_HW                    1
#else
  #define __SEGGER_RTL_FP_HW                    0
#endif

// Clang gets __ARM_FP wrong for the T16 target ISA indicating
// that floating-point instructions exist in this ISA -- which
// they don't.  Patch that definition up here.
#if __ARM_ARCH_ISA_THUMB == 1
  #undef  __SEGGER_RTL_FP_HW
  #define __SEGGER_RTL_FP_HW                    0
  #undef  __SEGGER_RTL_FP_ABI
  #define __SEGGER_RTL_FP_ABI                   0
#endif

//
// Configuration of full or Arm AEABI NaNs.
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
// Configuration of multiply-add capability.
//
#if __SEGGER_RTL_TARGET_ISA != __SEGGER_RTL_ISA_T16
  #define __SEGGER_RTL_CORE_HAS_MLA             1
#else
  #define __SEGGER_RTL_CORE_HAS_MLA             0
#endif

//
// Configuration of multiply-subtract capability.
//
#if (__ARM_ARCH >= 7) && (__SEGGER_RTL_TARGET_ISA != __SEGGER_RTL_ISA_T16)
  #define __SEGGER_RTL_CORE_HAS_MLS             1
#else
  #define __SEGGER_RTL_CORE_HAS_MLS             0
#endif

//
// Configuration of multiplication capability.
//
// In the ARM Architecture Reference Manual, DDI 01001, Arm states
// the following for SMULL and UMULL:
//
//   "Specifying the same register for either <RdHi> and <Rm>,
//   or <RdLo> and <Rm>, was previously described as producing
//   UNPREDICTABLE results. There is no restriction in ARMv6, and
//   it is believed all relevant ARMv4 and ARMv5 implementations
//   do not require this restriction either, because high
//   performance multipliers read all their operands prior to
//   writing back any results."
//
// Unfortunately, the GNU assembler enforces this restriction which
// means that assembly-level inserts will not work for ARMv4 and
// ARMv5 even though there is no indication that they fail in
// practice.
//
#if __SEGGER_RTL_TARGET_ISA == __SEGGER_RTL_ISA_T16
  //
  // T16 ISA has no extended multiplication at all.
  //
  #define __SEGGER_RTL_CORE_HAS_EXT_MUL         0
  //
#elif __ARM_ARCH >= 6
  //
  // ARMv6 and above have no restrictions on their input
  // and output registers, so assembly-level inserts with
  // constraints to guide the compiler are acceptable.
  //
  #define __SEGGER_RTL_CORE_HAS_EXT_MUL         1
  //
#elif (__ARM_ARCH == 5) && defined(__clang__)
  //
  // Take Arm at its word and disable restrictions on input
  // and output registers.
  //
  #define __SEGGER_RTL_CORE_HAS_EXT_MUL         1
  //
#else 
  //
  // ARMv5TE and lower have restrictions on their input
  // and output registers, therefore do not enable extended
  // multiply inserts.
  //
  #define __SEGGER_RTL_CORE_HAS_EXT_MUL         0
  //
#endif

#if __SEGGER_RTL_CORE_HAS_EXT_MUL && (defined(__GNUC__) || defined(__clang__))
  //
  // v6+DSP and v7E-M has SMMUL instruction, others do not.
  //
  // Benchmarking using GCC and Clang/LLVM shows that using SMULL results in faster
  // code than SMMUL.  Using SMMUL results in marginally smaller code because there
  // is less register pressure (no requirement to allocate a bit-bucket register).
  //
  // Given this, we disable the detection of SMMUL and use SMULL always.
  //
  #if 0 && (__ARM_ARCH >= 6) && defined(__ARM_FEATURE_DSP) && (__ARM_FEATURE_DSP == 1)
    #define __SEGGER_RTL_SMULL_HI(x0, x1)                      \
      ({ long __hi;                                            \
         __asm__(  "smmul %0, %1, %2"                          \
                 : "=r"(__hi)                                  \
                 : "r"((unsigned)(x0)), "r"((unsigned)(x1)) ); \
         __hi;                                                 \
      })
  #else
    #define __SEGGER_RTL_SMULL_HI(x0, x1)                      \
      ({ long __trash, __hi;                                   \
         __asm__(  "smull %0, %1, %2, %3"                      \
                 : "=r"(__trash), "=r"(__hi)                   \
                 : "r"((unsigned)(x0)), "r"((unsigned)(x1)) ); \
         __hi;                                                 \
      })
  #endif

  #define __SEGGER_RTL_SMULL(lo, hi, x0, x1)                 \
    do {                                                     \
      __asm__(  "smull %0, %1, %2, %3"                       \
              : "=r"(lo), "=r"(hi)                           \
              : "r"((unsigned)(x0)), "r"((unsigned)(x1)) );  \
    } while (0)

  #define __SEGGER_RTL_SMLAL(lo, hi, x0, x1)                 \
    do {                                                     \
      __asm__(  "smlal %0, %1, %2, %3"                       \
              : "+r"(lo), "+r"(hi)                           \
              : "r"((unsigned)(x0)), "r"((unsigned)(x1)) );  \
    } while (0)

  #define __SEGGER_RTL_UMULL_HI(x0, x1)                      \
    ({ unsigned long __trash, __hi;                          \
       __asm__(  "umull %0, %1, %2, %3"                      \
               : "=r"(__trash), "=r"(__hi)                   \
               : "r"((unsigned)(x0)), "r"((unsigned)(x1)) ); \
       __hi;                                                 \
    })

  #define __SEGGER_RTL_UMULL(lo, hi, x0, x1)                 \
    do {                                                     \
      __asm__(  "umull %0, %1, %2, %3"                       \
              : "=r"(lo), "=r"(hi)                           \
              : "r"((unsigned)(x0)), "r"((unsigned)(x1)) );  \
    } while (0)

  #define __SEGGER_RTL_UMULL_X(x, y)                         \
    ((__SEGGER_RTL_U64)(__SEGGER_RTL_U32)(x) *               \
                       (__SEGGER_RTL_U32)(y))

  #define __SEGGER_RTL_UMLAL(lo, hi, x0, x1)                 \
    do {                                                     \
      __asm__("umlal %0, %1, %2, %3"                         \
              : "+r"(lo), "+r"(hi)                           \
              : "r"((unsigned)(x0)), "r"((unsigned)(x1)) );  \
    } while (0)

#endif

//
// Configuration of static branch probability.
//
#if defined(__GNUC__) || defined(__clang__)
  #define __SEGGER_RTL_UNLIKELY(X)              __builtin_expect((X), 0)
#endif

//
// Configuration of thread-local storage
//
#if defined(__GNUC__) || defined(__clang__)
  #define __SEGGER_RTL_THREAD                   __thread
#endif

//
// Configuration of inlining.
//
#if (defined(__GNUC__) || defined(__clang__)) && (__SEGGER_RTL_CONFIG_CODE_COVERAGE == 0)
  #ifndef   __SEGGER_RTL_NEVER_INLINE
    //
    // Clang doesn't know noclone...
    //
    #if defined(__clang__)
      #define __SEGGER_RTL_NEVER_INLINE         __attribute__((__noinline__))
    #else
      #define __SEGGER_RTL_NEVER_INLINE         __attribute__((__noinline__, __noclone__))
    #endif
  #endif
  //
  #ifndef   __SEGGER_RTL_ALWAYS_INLINE
    #define __SEGGER_RTL_ALWAYS_INLINE          __inline__ __attribute__((__always_inline__))
  #endif
  //
  #ifndef   __SEGGER_RTL_REQUEST_INLINE
    #define __SEGGER_RTL_REQUEST_INLINE         __inline__
  #endif
  //
#endif

//
// Configuration of public APIs.
//
#if defined(__GNUC__) || defined(__clang__)
  #define __SEGGER_RTL_PUBLIC_API               __attribute__((__weak__)) 
#endif

//
// Using these builtins requires that the library is compiled
// with -fno-math-errno.
//
#if (__SEGGER_RTL_FP_HW >= 1) && (defined(__GNUC__) || defined(__clang__))
  #define __SEGGER_RTL_FLOAT32_ABS(__m)\
  ({\
    float __d;\
    __asm__ __volatile__("vabs.f32 %[d], %[m]"\
                          : [d] "=t"(__d)\
                          : [m] "t"(__m));\
    __d;\
  })

#endif
#if (__SEGGER_RTL_FP_HW >= 2) && (defined(__GNUC__) || defined(__clang__))
  #define __SEGGER_RTL_FLOAT64_ABS(__m)\
  ({\
    double __d;\
    __asm__ __volatile__("vabs.f64 %P[d], %P[m]"\
                          : [d] "=w"(__d)\
                          : [m] "w"(__m));\
    __d;\
  })
#endif

#if (__SEGGER_RTL_FP_HW >= 1) && (defined(__GNUC__) || defined(__clang__))
  #define __SEGGER_RTL_FLOAT32_SQRT(__m)\
  ({\
    float __d;\
    __asm__ __volatile__("vsqrt.f32 %[d], %[m]"\
                         : [d] "=t"(__d)\
                         : [m] "t"(__m));\
    __d;\
  })
#endif
#if (__SEGGER_RTL_FP_HW >= 2) && (defined(__GNUC__) || defined(__clang__))
  #define __SEGGER_RTL_FLOAT64_SQRT(__m)\
  ({\
    double __d;\
    __asm__ __volatile__("vsqrt.f64 %P[d], %P[m]"\
                         : [d] "=w"(__d)\
                         : [m] "w"(__m));\
    __d;\
  })
#endif

//
// Configuration of CLZ support.
//
#if defined(__ARM_FEATURE_CLZ) && (__ARM_FEATURE_CLZ == 1)
  #define __SEGGER_RTL_CORE_HAS_CLZ             1
#else
  #define __SEGGER_RTL_CORE_HAS_CLZ             0
#endif

// Clang gets __ARM_FEATURE_CLZ wrong for v8M.Baseline, indicating
// that CLZ is available in this ISA  -- which it isn't.  Patch that
// definition up here.
#if (__ARM_ARCH == 8) && (__SEGGER_RTL_TARGET_ISA == __SEGGER_RTL_ISA_T16)
  #undef  __SEGGER_RTL_CORE_HAS_CLZ
  #define __SEGGER_RTL_CORE_HAS_CLZ             0
#endif

// GCC gets __ARM_FEATURE_CLZ wrong for v5TE compiling for Thumb,
// indicating that CLZ is available in this ISA -- which it isn't.
// Patch that definition up here.
#if (__ARM_ARCH == 5) && (__SEGGER_RTL_TARGET_ISA == __SEGGER_RTL_ISA_T16)
  #undef  __SEGGER_RTL_CORE_HAS_CLZ
  #define __SEGGER_RTL_CORE_HAS_CLZ             0
#endif

#if __SEGGER_RTL_CORE_HAS_CLZ
  //
  // For ACLE-conforming C compilers that declare the architecture or
  // profile has a CLZ instruction, use that CLZ instruction.
  //
  #define __SEGGER_RTL_CLZ_U32(X)               __builtin_clz(X)
#endif

//
// For Arm architectures using GNU C or clang, the SEGGER RTL offers
// optimized versions written in GNU-compatible assembly language.
// Selection of them is made here.
//
#if defined(__SEGGER_RTL_COMPILING_LIBRARY) && __SEGGER_RTL_COMPILING_LIBRARY
  #if defined(__GNUC__) || defined(__clang__)
    #define strcpy(x, y)                        __SEGGER_RTL_HIDE(strcpy)(x, y)
    #define strcmp(x, y)                        __SEGGER_RTL_HIDE(strcmp)(x, y)
    #define strchr(x, y)                        __SEGGER_RTL_HIDE(strchr)(x, y)
    #define strlen(x)                           __SEGGER_RTL_HIDE(strlen)(x)
    #define memcpy(x, y, z)                     __SEGGER_RTL_HIDE(memcpy)(x, y, z)
    #define memset(x, y, z)                     __SEGGER_RTL_HIDE(memset)(x, y, z)
  #endif
#endif

/*********************************************************************
*
*       Configuration of core features.
*
**********************************************************************
*/

#if (__SEGGER_RTL_TARGET_ISA != __SEGGER_RTL_ISA_T16) && defined(__ARM_FEATURE_SIMD32) && __ARM_FEATURE_SIMD32
  #define __SEGGER_RTL_CORE_HAS_UQADD_UQSUB     1
#else
  #define __SEGGER_RTL_CORE_HAS_UQADD_UQSUB     0
#endif

#if defined(__ARM_ARCH) && (__ARM_ARCH >= 7)
  #define __SEGGER_RTL_CORE_HAS_REV             1
#else
  #define __SEGGER_RTL_CORE_HAS_REV             0
#endif

#if defined(__ARM_ARCH) && (__ARM_ARCH >= 6) && (__SEGGER_RTL_TARGET_ISA != __SEGGER_RTL_ISA_T16) && defined(__ARM_FEATURE_DSP) && (__ARM_FEATURE_DSP == 1)
  #define __SEGGER_RTL_CORE_HAS_PKHTB_PKHBT     1
#else
  #define __SEGGER_RTL_CORE_HAS_PKHTB_PKHBT     0
#endif

#if (__ARM_ARCH >= 7) && (__SEGGER_RTL_TARGET_ISA == __SEGGER_RTL_ISA_T32)
  #define __SEGGER_RTL_CORE_HAS_ADDW_SUBW       1   // ARMv8A/R only has ADDW in Thumb mode
#else
  #define __SEGGER_RTL_CORE_HAS_ADDW_SUBW       0
#endif

#if __ARM_ARCH >= 7
  #define __SEGGER_RTL_CORE_HAS_MOVW_MOVT       1
#else
  #define __SEGGER_RTL_CORE_HAS_MOVW_MOVT       0
#endif

#if defined(__ARM_FEATURE_IDIV) && __ARM_FEATURE_IDIV
  #define __SEGGER_RTL_CORE_HAS_IDIV            1
#else
  #define __SEGGER_RTL_CORE_HAS_IDIV            0
#endif

// Unfortunately the ACLE specifies "__ARM_FEATURE_IDIV is defined to 1 if the target
// has hardware support for 32-bit integer division in all available instruction sets."
// For v7R, there is typically no divide in the Arm instruction set but there is
// support for divide in the Thumb instruction set, so provide an exception here
// when targeting v7R in Thumb mode.
#if (__ARM_ARCH_PROFILE == 'R') && (__SEGGER_RTL_TARGET_ISA == __SEGGER_RTL_ISA_T32)
  #undef  __SEGGER_RTL_CORE_HAS_IDIV
  #define __SEGGER_RTL_CORE_HAS_IDIV            1
#endif

#if (__ARM_ARCH >= 7) && (__SEGGER_RTL_TARGET_ISA != __SEGGER_RTL_ISA_ARM)
  #define __SEGGER_RTL_CORE_HAS_CBZ_CBNZ        1
#else
  #define __SEGGER_RTL_CORE_HAS_CBZ_CBNZ        0
#endif

#if (__ARM_ARCH >= 7) && (__SEGGER_RTL_TARGET_ISA == __SEGGER_RTL_ISA_T32)
  #define __SEGGER_RTL_CORE_HAS_TBB_TBH         1
#else
  #define __SEGGER_RTL_CORE_HAS_TBB_TBH         0
#endif

#if __ARM_ARCH >= 6
  #define __SEGGER_RTL_CORE_HAS_UXT_SXT         1
#else
  #define __SEGGER_RTL_CORE_HAS_UXT_SXT         0
#endif

#if (__SEGGER_RTL_TARGET_ISA == __SEGGER_RTL_ISA_T32) || (__ARM_ARCH >= 7)
  #define __SEGGER_RTL_CORE_HAS_BFC_BFI_BFX     1
#else
  #define __SEGGER_RTL_CORE_HAS_BFC_BFI_BFX     0
#endif

#if __ARM_ARCH >= 5
  #define __SEGGER_RTL_CORE_HAS_BLX_REG         1
#else
  #define __SEGGER_RTL_CORE_HAS_BLX_REG         0
#endif

#if (__ARM_ARCH >= 6) && (__SEGGER_RTL_TARGET_ISA == __SEGGER_RTL_ISA_T32)
  #define __SEGGER_RTL_CORE_HAS_LONG_SHIFT      1
#else
  #define __SEGGER_RTL_CORE_HAS_LONG_SHIFT      0
#endif

#ifndef __SEGGER_RTL_FAST_CODE_SECTION
  #if defined(__GNUC__) || defined(__clang__)
    #define __SEGGER_RTL_FAST_CODE_SECTION(X)   __attribute__((__section__(".fast." X)))
  #endif
#endif

#ifndef   __SEGGER_RTL_USE_FPU_FOR_IDIV
  #define __SEGGER_RTL_USE_FPU_FOR_IDIV         0
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

#ifndef   __SEGGER_RTL_PREFER_BRANCH_FREE_CODE
  #define __SEGGER_RTL_PREFER_BRANCH_FREE_CODE  1
#endif

//
// GCC and clang provide a built-in va_list.
//
#if defined(__GNUC__) || defined(__clang__)
  #define __SEGGER_RTL_VA_LIST        __builtin_va_list
#endif

//
// ARM C library ABI requires low-level assert function to be __aeabi_assert
//
#define __SEGGER_RTL_X_assert                   __aeabi_assert

//
// ARM C library ABI defines how to interrogate errno
//
#define __SEGGER_RTL_X_errno_addr               __aeabi_errno_addr

#endif

/*************************** End of file ****************************/
