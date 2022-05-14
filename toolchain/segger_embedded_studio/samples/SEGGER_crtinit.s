/*********************************************************************
*                    SEGGER Microcontroller GmbH                     *
*                        The Embedded Experts                        *
**********************************************************************
*                                                                    *
*            (c) 2014 - 2021 SEGGER Microcontroller GmbH             *
*                                                                    *
*       www.segger.com     Support: support@segger.com               *
*                                                                    *
**********************************************************************
*                                                                    *
* All rights reserved.                                               *
*                                                                    *
* Redistribution and use in source and binary forms, with or         *
* without modification, are permitted provided that the following    *
* condition is met:                                                  *
*                                                                    *
* - Redistributions of source code must retain the above copyright   *
*   notice, this condition and the following disclaimer.             *
*                                                                    *
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND             *
* CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,        *
* INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF           *
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE           *
* DISCLAIMED. IN NO EVENT SHALL SEGGER Microcontroller BE LIABLE FOR *
* ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR           *
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT  *
* OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;    *
* OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF      *
* LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT          *
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE  *
* USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH   *
* DAMAGE.                                                            *
*                                                                    *
**********************************************************************

-------------------------- END-OF-HEADER -----------------------------

File      : SEGGER_crtinit.s
Purpose   : Generic runtime initialization code and library helper 
            functions
            for ARM CPUs running in THUMB2 mode.
            Designed to work with the SEGGER linker to produce 
            smallest possible executables.
            
            This file does not normally require any customization.
*/

        .syntax unified  

/*********************************************************************
*
*       Macros
*
**********************************************************************
*/
//
// Declare init function
// They are all called indirectly from the start function using the 
// init table
//

.macro INIT_FUNC Name
        .section .segger.init.\Name, "ax"
        .global \Name
        .weak \Name
#if defined(__thumb__) || defined(__thumb2__)
        .balign 2
        .thumb_func
        .code 16
#else
        .balign 4
        .type \Name, function
        .code 32
#endif
\Name:
.endm

//
// Declare a regular weak function, such as default C/C++ library helpers
//
.macro WEAK_FUNC Name
        .section .text.\Name, "ax", %progbits
        .global \Name
        .weak \Name
#if defined(__thumb__) || defined(__thumb2__)
        .balign 2
        .thumb_func
        .code 16
#else
        .balign 4
        .type \Name, function
        .code 32
#endif
\Name:
.endm

#if defined(__ARM_ARCH_4T__)
.macro blx reg
  mov lr, pc
  bx \reg
.endm
#endif

//
// Mark the end of a function and calculate its size
//
.macro END_FUNC name
        .size \name,.-\name
.endm


/*********************************************************************
*
*       SEGGER Init functions
*
*  All functions in this category are called indirectly from _start, 
*  which uses the linker-generated Init Table
*
*  Calling convention
*    All SEGGER Init functions follow the same calling convention:
*      - R4 holds the address of the first parameter of the function in the init table
*      - R4 += 4 * NumParameters before returning
*
**********************************************************************
*/

/*********************************************************************
*
*       __SEGGER_init_lzss
*
*  Function description
*    Fills sections which should be initialized with custom contents via unpacking using the "lzss" algorithm.
*    This function is only linked into the application image if one or more sections were compressed by the linker using lzss.
*    This can be configured in the linker file (.icf):
*
*    initialize by copy with packing=lzss        { section .<SectionName> [...]};
*
*  Calling convention
*    R4 must contain the address of a table with parameters used by this function:
*      [R4]      <pDest>
*      [R4 + 4]  <pSrc>
*    Additionally, the caller expects this function to adjust R4 so it points after the table:
*      R4 = R4 + 8
*/

#undef L
#define L(label) .L__SEGGER_init_lzss_##label

INIT_FUNC __SEGGER_init_lzss
        //
        // Load parameters, adjust pointer
        //
        ldr     R0, [R4]                // pDest
        ldr     R1, [R4, #4]            // pSrc
        adds    R4, R4, #8              // Skip table entries
L(NextByte):
        ldrb    R2, [R1]
        adds    R1, R1, #1
        tst     R2, R2
        beq     L(Done)                 // 0 -> end of table
        cmp     R2, #0x80
        bcc     L(LoopLiteral)          // +ve -> literal run
        //
        // -ve -> copy run
        //
        // R0 = pointer to output stream
        // R1 = pointer to input stream
        // R2 = run length
        // R3 = copy byte
        // R4 = pointer to initialization table
        //
        subs    R2, R2, #0x80           // Convert to run length
        beq     L(Error)
        ldrb    R3, [R1]                // R3 = first byte of distance
        adds    R1, R1, #1
        cmp     R3, #0x80
        bcc     L(ShortRun)             // R3 < 128, short run
        subs    R3, R3, #0x80           // Adjust to recover true run length high byte
        lsls    R3, R3, #8              // Prepare to fuse
        ldrb    R5, [R1]                // Extract run length low byte
        adds    R1, R1, #1
        adds    R3, R3, R5              // Construct run length
L(ShortRun):
        subs    R5, R0, R3              // Source of where to copy from
L(LoopShort):
        ldrb    R3, [R5]                // Source byte of run
        strb    R3, [R0]                // Store to destination
        adds    R5, R5, #1
        adds    R0, R0, #1
        subs    R2, R2, #1
        bne     L(LoopShort)
        b       L(NextByte)
        //
        // +ve -> literal run
        //
        // R0 = pointer to output stream
        // R1 = pointer to input stream
        // R2 = run length
        // R3 = copy byte
        // R4 = pointer to initialization table
        //
L(LoopLiteral):
        ldrb    R3, [R1]                // Source byte of run
        adds    R1, R1, #1
        strb    R3, [R0]                // Store to destination
        adds    R0, R0, #1
        subs    R2, R2, #1
        bne     L(LoopLiteral)
        b       L(NextByte)
L(Done):
        bx      LR
L(Error):
        b       L(Error)
END_FUNC __SEGGER_init_lzss

/*********************************************************************
*
*       __SEGGER_init_zero
*
*  Function description
*    Fills sections which should be initialized with zero.
*    This function is only linked into the application image if one or more sections exist which need zero init.
*    Almost all programs contain one or more zero init sections.
*
*  Calling convention
*    R4 must contain the address of a table with parameters used by this function:
*      [R4]      <pDest>
*      [R4 + 4]  <NumBytes>
*    Additionally, the caller expects this function to adjust R4 so it points after the table:
*      R4 = R4 + 8
*/

#undef L
#define L(label) .L__SEGGER_init_zero_##label

INIT_FUNC __SEGGER_init_zero
        //
        // Load parameters, adjust pointer
        //
        ldr     R0, [R4]                // pDest
        ldr     R1, [R4, #4]            // NumBytes
        adds    R4, R4, #8              // Skip table entries
        movs    R2, #0
        b       L(Tail)
L(Loop):
        strb    R2, [R0, R1]
L(Tail):
        subs    R1, R1, #1
        bpl     L(Loop)
        bx      LR
END_FUNC __SEGGER_init_zero

/*********************************************************************
*
*       __SEGGER_init_copy
*
*  Function description
*    Fills sections which should be initialized with custom contents via copy.
*    This function is only linked into the application image if one or more sections exist which are NOT compressed and need non-zero init.
*    This can be configured in the linker file (.icf):
*
*    initialize by copy   { section .<SectionName> [...]};
*
*  Calling convention
*    R4 must contain the address of a table with parameters used by this function:
*      [R4]      <pDest>
*      [R4 + 4]  <pSrc>
*      [R4 + 8]  <NumBytes>
*    Additionally, the caller expects this function to adjust R4 so it points after the table:
*      R4 = R4 + 12
*/

#undef L
#define L(label) .L__SEGGER_init_copy_##label

INIT_FUNC __SEGGER_init_copy
        //
        // Load parameters, adjust pointer
        //
        ldr     R0, [R4]                // pDest
        ldr     R1, [R4, #4]            // pSrc
        ldr     R2, [R4, #8]            // NumBytes
        adds    R4, R4, #12             // Skip table entries
        b       L(Tail)
L(Loop):
        ldrb    R3, [R1, R2]
        strb    R3, [R0, R2]
L(Tail):
        subs    R2, R2, #1
        bpl     L(Loop)
        bx      LR
END_FUNC __SEGGER_init_copy

/*********************************************************************
*
*       __SEGGER_init_pack
*
*  Function description
*    Fills sections which should be initialized with custom contents via unpacking using the "packbits" algorithm.
*    This function is only linked into the application image if one or more sections were compressed by the linker using packbits.
*    This can be configured in the linker file (.icf):
*
*    initialize by copy with packing=packbits        { section .<SectionName> [...]};
*
*  Calling convention
*    R4 must contain the address of a table with parameters used by this function:
*      [R4]      <pDest>
*      [R4 + 4]  <pSrc>
*    Additionally, the caller expects this function to adjust R4 so it points after the table:
*      R4 = R4 + 8
*/

#undef L
#define L(label) .L__SEGGER_init_pack_##label

INIT_FUNC __SEGGER_init_pack
        //
        // Load parameters, adjust pointer
        //
        ldr     R0, [R4]                // pDest
        ldr     R1, [R4, #4]            // pSrc
        adds    R4, R4, #8              // Skip table entries
L(NextByte):
        ldrb    R2, [R1]
        adds    R1, R1, #1              // pSrc += 1
        cmp     R2, #0x80     
        beq     L(Done)                  // Done
        bcc     L(CopyBytes)
        //
        // Fill bytes: Fills a number of bytes with the same value
        //
        ldrb    R3, [R1]                // Byte to replicate
        adds    R1, R1, #1              // pSrc += 1
        negs    R2, R2                  // <Byte> = 256 - <Byte>
        adds    R2, R2, #255
        adds    R2, R2, #1
L(FillLoop):                            // One byte repeated n times
        strb    R3, [R0]
        adds    R0, R0, #1              // pDest += 1
        subs    R2, R2, #1
        bpl     L(FillLoop)
        b       L(NextByte)
        //
        // Copy bytes
        // do {
        //   *R0++;
        //   *R1++;
        //   *R2--;
        // } while (R2 > 0);
        //
L(CopyBytes):                           // 1 + n literal bytes
        ldrb    R3, [R1]      
        strb    R3, [R0]      
        adds    R0, R0, #1              // pDest += 1
        adds    R1, R1, #1              // pSrc  += 1
        subs    R2, R2, #1
        bpl     L(CopyBytes)
        b       L(NextByte)
L(Done):
        bx      LR
END_FUNC __SEGGER_init_pack

/*********************************************************************
*
*       __SEGGER_init_zpak
*
*  Function description
*    Fills sections which should be initialized with custom contents via unpacking using the "ZPak" algorithm.
*    This function is only linked into the application image if one or more sections were compressed by the linker using ZPak.
*    This can be configured in the linker file (.icf):
*
*    initialize by copy with packing=zpak        { section .<SectionName> [...]};
*
*  Calling convention
*    R4 must contain the address of a table with parameters used by this function:
*      [R4]      <pDest>
*      [R4 + 4]  <pSrc>
*      [R4 + 8]  <NumBytes>
*    Additionally, the caller expects this function to adjust R4 so it points after the table:
*      R4 = R4 + 12
*/

#undef L
#define L(label) .L__SEGGER_init_zpak_##label

INIT_FUNC __SEGGER_init_zpak
        //
        // Load parameters, adjust pointer
        //
        ldr     R0, [R4]                // pDest
        ldr     R1, [R4, #4]            // pSrc
        ldr     R2, [R4, #8]            // NumBytes
        adds    R4, R4, #12             // Skip table entries
L(NextByte):
        ldrb    R3, [R1]                // Get control byte from source stream
        adds    R1, R1, #1    
        movs    R6, #8        
L(CheckCtrl):               
        movs    R5, #0                  // Prepare zero filler
        lsrs    R3, R3, #1              // Get byte control flag
        bcs     L(FillDest)             // Carry set -> zero filler
        ldrb    R5, [R1]                // Get literal byte from source stream
        adds    R1, R1, #1    
L(FillDest):
        strb    R5, [R0]                // Store initialization byte
        adds    R0, R0, #1              // pDest    += 1
        subs    R2, R2, #1              // NumBytes -= 1
        beq     L(Done)                 // Destination filled? => Done
        subs    R6, R6, #1              // Decrement bit count
        bne     L(CheckCtrl)            // Still within this control byte
        b       L(NextByte)             // Get next control byte
L(Done):
        bx      LR
END_FUNC __SEGGER_init_zpak

/*********************************************************************
*
*       __SEGGER_init_heap
*
*  Function description
*    Initialize the heap as "all-free" in order for the heap-related functions
*    malloc(), realloc(), calloc(), and free() to function correctly.
*    This function is only linked into the application image if one the heap size is greater than zero.
*    This can be configured in the linker file (.icf):
*
*    initialize by symbol __SEGGER_init_heap     { block heap  };
*/

#undef L
#define L(label) .L__SEGGER_init_heap_##label

INIT_FUNC __SEGGER_init_heap
        ldr     R0, = __heap_start__
        ldr     R1, = __heap_end__
#if defined(__thumb__) || defined(__thumb2__)
        subs    R1, R1, R0
        cmp     R1, #8 
        blt     L(Done)                 // Heap size is smaller than 8 bytes? => Done
        movs    R2, #0
        str     R2, [R0]                // Write 0 at <__heap_start__>
        adds    R0, R0, #4
        str     R1, [R0]                // Write heap size at (<__heap_start__> + 4)
L(Done):
#else
        sub     R1, R1, R0
        cmp     R1, #8                  // Check that heap is larger than 8 bytes
        movge   R2, #0
        strge   R2, [R0], #+4           // Write 0 at <__heap_start__>
        strge   R1, [R0]                // Write heap size at (<__heap_start__> + 4)
#endif
        bx      LR
END_FUNC __SEGGER_init_heap

/*********************************************************************
*
*       __SEGGER_init_ctors
*
*  Function description
*    Call static constructors from the init-array.
*    This is typically C++ constructors, but a pre-init function can be declared in C using GNU C attribute syntax, for instance:
*
*    void __attribute__((constructor)) ExecBeforeMain() { ... }
*
*    This function is only linked into the application image if there are any constructors to be called.
*    This can be configured in the linker file (.icf):
*
*    initialize by symbol __SEGGER_init_ctors    { block ctors };
*/

#undef L
#define L(label) .L__SEGGER_init_ctors_##label

INIT_FUNC __SEGGER_init_ctors
        mov     R5, LR                  // Save link register
        ldr     R0, =__ctors_start__
        ldr     R1, =__ctors_end__
L(Loop):
        cmp     R0, R1
        beq     L(Done)                 // Reached end of constructor list? => Done
#if defined(__thumb__) || defined(__thumb2__)
        ldr     R2, [R0]                // Load current constructor address into R2
        adds    R0, #4                  // Skip current constructor address
#else
        ldr     R2, [R0], #+4           // Load current constructor address into R2 and skip it
#endif
        push    {R0-R1}                 // Save R0 and R1
        blx     R2                      // Call constructor
        pop     {R0-R1}                 // Restore R0 and R1
        b       L(Loop)
L(Done):
        bx      R5                      // Branch to the LR address saved before
END_FUNC __SEGGER_init_ctors

/*********************************************************************
*
*       HELPER functions
*
**********************************************************************
*/

WEAK_FUNC __aeabi_read_tp
        ldr     R0, =__tbss_start__-8
        bx      LR
WEAK_FUNC abort
        b       .
WEAK_FUNC __assert
        b       .
WEAK_FUNC __assert_func
        b       .
WEAK_FUNC __aeabi_assert
        b       .
WEAK_FUNC __sync_synchronize
        bx      LR
WEAK_FUNC __getchar
        b       debug_getchar
WEAK_FUNC __putchar
        b       debug_putchar
WEAK_FUNC __open
        b       debug_fopen
WEAK_FUNC __close
        b       debug_fclose
WEAK_FUNC __write   
        movs    R3, R0
        movs    R0, R1
#if defined(__thumb__) || defined(__thumb2__)
        movs    R1, #1  
#else
        movs    R1, #1
#endif
        b       debug_fwrite
WEAK_FUNC __read  
        movs    R3, R0
        movs    R0, R1
#if defined(__thumb__) || defined(__thumb2__)
        movs    R1, #1 
#else
        movs    R1, #1
#endif
        b       debug_fread
WEAK_FUNC __seek
#if defined(__thumb__) || defined(__thumb2__)
        push    {R4, LR}
        movs    R4, R0
        bl      debug_fseek
        cmp     R0, #0
        bne     1f
        movs    R0, R4
        bl      debug_ftell
        pop     {R4, PC}
1:                              //seek done
        ldr     R0, =-1
        pop     {R4, PC}
#else
        push    {R4, LR}
        movs    R4, R0
        bl      debug_fseek
        cmp     R0, #0
        bne     1f
        movs    R0, R4
        bl      debug_ftell
        b       2f
1:                              //seek error
        ldr     R0, =-1
2:                              //seek error
        pop     {R4, LR}
        bx      LR
#endif

//
// char __user_locale_name_buffer[];
//
        .section .bss.__user_locale_name_buffer, "aw", %nobits
        .global __user_locale_name_buffer
        .weak __user_locale_name_buffer
__user_locale_name_buffer:
        .space 0x4
        .size __user_locale_name_buffer, .-__user_locale_name_buffer
        .type __user_locale_name_buffer, %object

/*************************** End of file ****************************/
