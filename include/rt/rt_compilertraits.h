/*===============================================================*
 *       Copyright (c) Fux Games,Inc. All Rights Reserved.       *
 *===============================================================*/

/**
* Currently supported defines include:
*    FUX_JOIN
*
*    FUX_COMPILER_IS_ANSIC
*    FUX_COMPILER_IS_C99
*    FUX_COMPILER_IS_C11
*    FUX_COMPILER_HAS_C99_TYPES
*    FUX_COMPILER_IS_CPLUSPLUS
*    FUX_COMPILER_MANAGED_CPP
*    FUX_COMPILER_INTMAX_SIZE
*    FUX_OFFSETOF
*    FUX_SIZEOF_MEMBER
*
*    FUX_ALIGN_OF()
*    FUX_ALIGN_MAX_STATIC / FUX_ALIGN_MAX_AUTOMATIC
*    FUX_ALIGN() / FUX_PREFIX_ALIGN() / FUX_POSTFIX_ALIGN()
*    FUX_ALIGNED()
*    FUX_PACKED()
*
*    FUX_LIKELY()
*    FUX_UNLIKELY()
*    FUX_INIT_PRIORITY()
*    FUX_MAY_ALIAS()
*    FUX_ASSUME()
*    FUX_ANALYSIS_ASSUME()
*    FUX_PURE
*    FUX_WEAK
*    FUX_UNUSED()
*    FUX_EMPTY()
*
*    FUX_WCHAR_T_NON_NATIVE
*    FUX_WCHAR_SIZE = <n bytes>
*
*    FUX_RESTRICT
*    FUX_DEPRECATED   / FUX_PREFIX_DEPRECATED   / FUX_POSTFIX_DEPRECATED
*    FUX_INL / FUX_PREFIX_FORCE_INLINE / FUX_POSTFIX_FORCE_INLINE
*	  FUX_EX_INL
*    FUX_NO_INLINE    / FUX_PREFIX_NO_INLINE    / FUX_POSTFIX_NO_INLINE
*    FUX_NO_VTABLE    / FUX_CLASS_NO_VTABLE     / FUX_STRUCT_NO_VTABLE
*    FUX_PASCAL
*    FUX_PASCAL_FUNC()
*    FUX_SSE = [0 | 1]
*    FUX_IMPORT
*    FUX_EXPORT
*    FUX_PRAGMA_ONCE_SUPPORTED
*    FUX_ONCE
*    FUX_OVERRIDE
*    FUX_INHERITANCE_FINAL
*    FUX_SEALED
*    FUX_ABSTRACT
*    FUX_CONSTEXPR / FUX_CONSTEXPR_OR_CONST
*    FUX_EXTERN_TEMPLATE
*    FUX_NOEXCEPT
*    FUX_NORETURN
*    FUX_CARRIES_DEPENDENCY
*    FUX_NON_COPYABLE / struct nocopy_t
*    FUX_OPTIMIZE_OFF / FUX_OPTIMIZE_ON
*    FUX_SIGNED_RIGHT_SHIFT_IS_UNSIGNED
*
*    FUX_DISABLE_VC_WARNING    / FUX_RESTORE_VC_WARNING / FUX_DISABLE_ALL_VC_WARNINGS / FUX_RESTORE_ALL_VC_WARNINGS
*    FUX_DISABLE_GCC_WARNING   / FUX_RESTORE_GCC_WARNING
*    FUX_DISABLE_CLANG_WARNING / FUX_RESTORE_CLANG_WARNING
*    FUX_DISABLE_SN_WARNING    / FUX_RESTORE_SN_WARNING / FUX_DISABLE_ALL_SN_WARNINGS / FUX_RESTORE_ALL_SN_WARNINGS
*    FUX_DISABLE_GHS_WARNING   / FUX_RESTORE_GHS_WARNING
*    FUX_DISABLE_EDG_WARNING   / FUX_RESTORE_EDG_WARNING
*    FUX_DISABLE_CW_WARNING    / FUX_RESTORE_CW_WARNING
*
*    FUX_DISABLE_DEFAULT_CTOR
*    FUX_DISABLE_COPY_CTOR
*    FUX_DISABLE_MOVE_CTOR
*    FUX_DISABLE_ASSIGNMENT_OPERATOR
*    FUX_DISABLE_MOVE_OPERATOR
*
*  Todo:
*    Find a way to reliably detect wchar_t size at preprocessor time and
*    implement it below for FUX_WCHAR_SIZE.
*
*  Todo:
*    Find out how to support FUX_PASCAL and FUX_PASCAL_FUNC for systems in
*    which it hasn't yet been found out for.
*---------------------------------------------------------------------------*/


#ifndef FUX_RT_COMPILER_TRAITS_H
#define FUX_RT_COMPILER_TRAITS_H

#include "rt_platform.h"
#include "rt_compiler.h"


// Metrowerks uses #defines in its core C header files to define
// the kind of information we need below (e.g. C99 compatibility)



// Determine if this compiler is ANSI C compliant and if it is C99 compliant.
#if defined(__STDC__)
    #define FUX_COMPILER_IS_ANSIC 1    // The compiler claims to be ANSI C
    
    // Is the compiler a C99 compiler or equivalent?
    // From ISO/IEC 9899:1999:
    //    6.10.8 Predefined macro names
    //    __STDC_VERSION__ The integer constant 199901L. (150)
    //
    //    150) This macro was not specified in ISO/IEC 9899:1990 and was
    //    specified as 199409L in ISO/IEC 9899/AMD1:1995. The intention
    //    is that this will remain an integer constant of type long int
    //    that is increased with each revision of this International Standard.
    //
    #if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)
        #define FUX_COMPILER_IS_C99 1
    #endif
    
    // Is the compiler a C11 compiler?
    // From ISO/IEC 9899:2011:
    //   Page 176, 6.10.8.1 (Predefined macro names) :
    //   __STDC_VERSION__ The integer constant 201112L. (178)
    //
    #if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 201112L)
        #define FUX_COMPILER_IS_C11 1
    #endif
#endif

// Some compilers (e.g. GCC) define __USE_ISOC99 if they are not
// strictly C99 compilers (or are simply C++ compilers) but are set
// to use C99 functionality. Metrowerks defines _MSL_C99 as 1 in
// this case, but 0 otherwise.
#if (defined(__USE_ISOC99) || (defined(_MSL_C99) && (_MSL_C99 == 1))) && !defined(FUX_COMPILER_IS_C99)
    #define FUX_COMPILER_IS_C99 1
#endif

// Metrowerks defines C99 types (e.g. intptr_t) instrinsically when in C99 mode (-lang C99 on the command line).
#if (defined(_MSL_C99) && (_MSL_C99 == 1))
    #define FUX_COMPILER_HAS_C99_TYPES 1
#endif

#if defined(__GNUC__)
    #if (((__GNUC__ * 100) + __GNUC_MINOR__) >= 302) // Also, GCC defines _HAS_C9X.
        #define FUX_COMPILER_HAS_C99_TYPES 1 // The compiler is not necessarily a C99 compiler, but it defines C99 types.
        
        #ifndef __STDC_LIMIT_MACROS
            #define __STDC_LIMIT_MACROS 1
        #endif
        
        #ifndef __STDC_CONSTANT_MACROS
            #define __STDC_CONSTANT_MACROS 1    // This tells the GCC compiler that we want it to use its native C99 types.
        #endif
    #endif
#endif

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
    #define FUX_COMPILER_HAS_C99_TYPES 1
#endif

#ifdef  __cplusplus
    #define FUX_COMPILER_IS_CPLUSPLUS 1
#endif


// ------------------------------------------------------------------------
// FUX_JOIN
//
// This macro joins the two arguments together, even when one of
// the arguments is itself a macro (see 16.3.1 in C++98 standard).
// This is often used to create a unique name with __LINE__.
//
// For example, this declaration:
//    char FUX_JOIN(unique_, __LINE__);
// expands to this:
//    char unique_73;
//
// Note that all versions of MSVC++ up to at least version 7.1
// fail to properly compile macros that use __LINE__ in them
// when the "program database for edit and continue" option
// is enabled. The result is that __LINE__ gets converted to
// something like __LINE__(Var+37).
//
#ifndef FUX_JOIN
    #define FUX_JOIN(a, b)  FUX_JOIN1(a, b)
    #define FUX_JOIN1(a, b) FUX_JOIN2(a, b)
    #define FUX_JOIN2(a, b) a##b
#endif


// ------------------------------------------------------------------------
// FUX_STRINGIFY
//
// Example usage:
//     printf("Line: %s", FUX_STRINGIFY(__LINE__));
//
#ifndef FUX_STRINGIFY
    #define FUX_STRINGIFY(x)     FUX_STRINGIFYIMPL(x)
    #define FUX_STRINGIFYIMPL(x) #x
#endif


// ------------------------------------------------------------------------
// FUX_IDENTITY
//
#ifndef FUX_IDENTITY
    #define FUX_IDENTITY(x) x
#endif


// ------------------------------------------------------------------------
// FUX_COMPILER_MANAGED_CPP
// Defined if this is being compiled with Managed C++ extensions
#ifdef FUX_COMPILER_MSVC
    #if FUX_COMPILER_VERSION >= 1300
        #ifdef _MANAGED
            #define FUX_COMPILER_MANAGED_CPP 1
        #endif
    #endif
#endif


// ------------------------------------------------------------------------
// FUX_COMPILER_INTMAX_SIZE
//
// This is related to the concept of intmax_t uintmax_t, but is available
// in preprocessor form as opposed to compile-time form. At compile-time
// you can use intmax_t and uintmax_t to use the actual types.
//
#if defined(__GNUC__) && defined(__x86_64__)
    #define FUX_COMPILER_INTMAX_SIZE 16  // intmax_t is __int128_t (GCC extension) and is 16 bytes.
#else
    #define FUX_COMPILER_INTMAX_SIZE 8   // intmax_t is int64_t and is 8 bytes.
#endif



// ------------------------------------------------------------------------
// FUX_LPAREN / FUX_RPAREN / FUX_COMMA / FUX_SEMI
//
// These are used for using special characters in macro-using expressions.
// Note that this macro intentionally uses (), as in some cases it can't
// work unless it does.
//
// Example usage:
//     int x = SOME_MACRO(SomeTemplate<int FUX_COMMA() int EACOMMA() char>);
//
#ifndef FUX_LPAREN
    #define FUX_LPAREN() (
#endif
#ifndef FUX_RPAREN
    #define FUX_RPAREN() )
#endif
#ifndef FUX_COMMA
    #define FUX_COMMA()  ,
#endif
#ifndef FUX_SEMI
    #define FUX_SEMI()   ;
#endif




// ------------------------------------------------------------------------
// FUX_OFFSETOF
// Implements a portable version of the non-standard offsetof macro.
//
// The offsetof macro is guaranteed to only work with POD types. However, we wish to use
// it for non-POD types but where we know that offsetof will still work for the cases
// in which we use it. GCC unilaterally gives a warning when using offsetof with a non-POD,
// even if the given usage happens to work. So we make a workaround version of offsetof
// here for GCC which has the same effect but tricks the compiler into not issuing the warning.
// The 65536 does the compiler fooling; the reinterpret_cast prevents the possibility of
// an overloaded operator& for the class getting in the way.
//
// Example usage:
//     struct A{ int x; int y; };
//     size_t n = FUX_OFFSETOF(A, y);
//
#if defined(__GNUC__)                       // We can't use GCC 4's __builtin_offsetof because it mistakenly complains about non-PODs that are really PODs.
    #define FUX_OFFSETOF(struct_, member_)  ((size_t)(((uintptr_t)&reinterpret_cast<const volatile char&>((((struct_*)65536)->member_))) - 65536))
#else
    #define FUX_OFFSETOF(struct_, member_)  offsetof(struct_, member_)
#endif

// ------------------------------------------------------------------------
// FUX_SIZEOF_MEMBER
// Implements a portable way to determine the size of a member.
//
// The FUX_SIZEOF_MEMBER simply returns the size of a member within a class or struct; member
// access rules still apply. We offer two approaches depending on the compiler's support for non-static member
// initializers although most C++11 compilers support this.
//
// Example usage:
//     struct A{ int x; int y; };
//     size_t n = FUX_SIZEOF_MEMBER(A, y);
//
#ifndef FUX_COMPILER_NO_EXTENDED_SIZEOF
    #define FUX_SIZEOF_MEMBER(struct_, member_) (sizeof(struct_::member_))
#else
    #define FUX_SIZEOF_MEMBER(struct_, member_) (sizeof(((struct_*)0)->member_))
#endif

// ------------------------------------------------------------------------
// alignment expressions
//
// Here we define
//    FUX_ALIGN_OF(type)         // Returns size_t.
//    FUX_ALIGN_MAX_STATIC       // The max align value that the compiler will respect for FUX_ALIGN for static data (global and static variables). Some compilers allow high values, some allow no more than 8. FUX_ALIGN_MIN is assumed to be 1.
//    FUX_ALIGN_MAX_AUTOMATIC    // The max align value for automatic variables (variables declared as local to a function).
//    FUX_ALIGN(n)               // Used as a prefix. n is byte alignment, with being a power of two. Most of the time you can use this and avoid using FUX_PREFIX_ALIGN/FUX_POSTFIX_ALIGN.
//    FUX_ALIGNED(t, v, n)       // Type, variable, alignment. Used to align an instance. You should need this only for unusual compilers.
//    FUX_PACKED                 // Specifies that the given structure be packed (and not have its members aligned).
//
// Also we define the following for rare cases that it's needed.
//    FUX_PREFIX_ALIGN(n)        // n is byte alignment, with being a power of two. You should need this only for unusual compilers.
//    FUX_POSTFIX_ALIGN(n)       // Valid values for n are 1, 2, 4, 8, etc. You should need this only for unusual compilers.
//
// Example usage:
//    size_t x = FUX_ALIGN_OF(int);                                  Non-aligned equivalents.        Meaning
//    FUX_PREFIX_ALIGN(8) int x = 5;                                 int x = 5;                      Align x on 8 for compilers that require prefix attributes. Can just use FUX_ALIGN instead.
//    FUX_ALIGN(8) int x;                                            int x;                          Align x on 8 for compilers that allow prefix attributes.
//    int x FUX_POSTFIX_ALIGN(8);                                    int x;                          Align x on 8 for compilers that require postfix attributes.
//    int x FUX_POSTFIX_ALIGN(8) = 5;                                int x = 5;                      Align x on 8 for compilers that require postfix attributes.
//    int x FUX_POSTFIX_ALIGN(8)(5);                                 int x(5);                       Align x on 8 for compilers that require postfix attributes.
//    struct FUX_PREFIX_ALIGN(8) X { int x; } FUX_POSTFIX_ALIGN(8);   struct X { int x; };            Define X as a struct which is aligned on 8 when used.
//    FUX_ALIGNED(int, x, 8) = 5;                                    int x = 5;                      Align x on 8.
//    FUX_ALIGNED(int, x, 16)(5);                                    int x(5);                       Align x on 16.
//    FUX_ALIGNED(int, x[3], 16);                                    int x[3];                       Align x array on 16.
//    FUX_ALIGNED(int, x[3], 16) = { 1, 2, 3 };                      int x[3] = { 1, 2, 3 };         Align x array on 16.
//    int x[3] FUX_PACKED;                                           int x[3];                       Pack the 3 ints of the x array. GCC doesn't seem to support packing of int arrays.
//    struct FUX_ALIGN(32) X { int x; int y; };                      struct X { int x; };            Define A as a struct which is aligned on 32 when used.
//    FUX_ALIGN(32) struct X { int x; int y; } Z;                    struct X { int x; } Z;          Define A as a struct, and align the instance Z on 32.
//    struct X { int x FUX_PACKED; int y FUX_PACKED; };               struct X { int x; int y; };     Pack the x and y members of struct X.
//    struct X { int x; int y; } FUX_PACKED;                         struct X { int x; int y; };     Pack the members of struct X.
//    typedef FUX_ALIGNED(int, int16, 16); int16 n16;                typedef int int16; int16 n16;   Define int16 as an int which is aligned on 16.
//    typedef FUX_ALIGNED(X, X16, 16); X16 x16;                      typedef X X16; X16 x16;         Define X16 as an X which is aligned on 16.

#if !defined(FUX_ALIGN_MAX)          // If the user hasn't globally set an alternative value...
    #if defined(FUX_PROCESSOR_ARM)                       // ARM compilers in general tend to limit automatic variables to 8 or less.
        #define FUX_ALIGN_MAX_STATIC    1048576
        #define FUX_ALIGN_MAX_AUTOMATIC       1          // Typically they support only built-in natural aligment types (both arm-eabi and apple-abi).
    #elif defined(FUX_PLATFORM_APPLE)
        #define FUX_ALIGN_MAX_STATIC    1048576
        #define FUX_ALIGN_MAX_AUTOMATIC      16
    #else
        #define FUX_ALIGN_MAX_STATIC    1048576          // Arbitrarily high value. What is the actual max?
        #define FUX_ALIGN_MAX_AUTOMATIC 1048576
    #endif
#endif

// EDG intends to be compatible with GCC but has a bug whereby it
// fails to support calling a constructor in an aligned declaration when
// using postfix alignment attributes. Prefix works for alignment, but does not align
// the size like postfix does.  Prefix also fails on templates.  So gcc style post fix
// is still used, but the user will need to use FUX_POSTFIX_ALIGN before the constructor parameters.
#if   defined(__GNUC__) && (__GNUC__ < 3)
#define FUX_ALIGN_OF(type) ((size_t)__alignof__(type))
#define FUX_ALIGN(n)
#define FUX_PREFIX_ALIGN(n)
#define FUX_POSTFIX_ALIGN(n) __attribute__((aligned(n)))
#define FUX_ALIGNED(variable_type, variable, n) variable_type variable __attribute__((aligned(n)))
#define FUX_PACKED __attribute__((packed))

// GCC 3.x+, IBM, and clang support prefix attributes.
#elif (defined(__GNUC__) && (__GNUC__ >= 3)) || defined(__xlC__) || defined(__clang__)
#define FUX_ALIGN_OF(type) ((size_t)__alignof__(type))
#define FUX_ALIGN(n) __attribute__((aligned(n)))
#define FUX_PREFIX_ALIGN(n)
#define FUX_POSTFIX_ALIGN(n) __attribute__((aligned(n)))
#define FUX_ALIGNED(variable_type, variable, n) variable_type variable __attribute__((aligned(n)))
#define FUX_PACKED __attribute__((packed))

// Metrowerks supports prefix attributes.
// Metrowerks does not support packed alignment attributes.
#elif defined(FUX_COMPILER_INTEL) || defined(CS_UNDEFINED_STRING) || (defined(FUX_COMPILER_MSVC) && (FUX_COMPILER_VERSION >= 1300))
#define FUX_ALIGN_OF(type) ((size_t)__alignof(type))
#define FUX_ALIGN(n) __declspec(align(n))
#define FUX_PREFIX_ALIGN(n) FUX_ALIGN(n)
#define FUX_POSTFIX_ALIGN(n)
#define FUX_ALIGNED(variable_type, variable, n) FUX_ALIGN(n) variable_type variable
#define FUX_PACKED // See FUX_PRAGMA_PACK_VC for an alternative.

// Arm brand compiler
#elif defined(FUX_COMPILER_ARM)
#define FUX_ALIGN_OF(type) ((size_t)__ALIGNOF__(type))
#define FUX_ALIGN(n) __align(n)
#define FUX_PREFIX_ALIGN(n) __align(n)
#define FUX_POSTFIX_ALIGN(n)
#define FUX_ALIGNED(variable_type, variable, n) __align(n) variable_type variable
#define FUX_PACKED __packed

#else // Unusual compilers
// There is nothing we can do about some of these. This is not as bad a problem as it seems.
// If the given platform/compiler doesn't support alignment specifications, then it's somewhat
// likely that alignment doesn't matter for that platform. Otherwise they would have defined
// functionality to manipulate alignment.
#define FUX_ALIGN(n)
#define FUX_PREFIX_ALIGN(n)
#define FUX_POSTFIX_ALIGN(n)
#define FUX_ALIGNED(variable_type, variable, n) variable_type variable
#define FUX_PACKED

#ifdef __cplusplus
template <typename T> struct alignof_t {
    enum { s = sizeof( T ), value = s ^ ( s & ( s - 1 ) ) };
};
template <typename T> struct alignof_t2;
template <int size_diff> struct helper {
    template <typename T> struct Val {
        enum { value = size_diff };
    };
};
template <> struct helper<0> {
    template <typename T> struct Val {
        enum { value = alignof_t2<T>::value };
    };
};
template <typename T> struct alignof_t2 {
    struct Big {
        T x;
        char c;
    };
    enum { diff = sizeof( Big ) - sizeof( T ), value = helper<diff>::template Val<Big>::value };
};
template <typename T> struct alignof_t3 {
    enum { x = alignof_t2<T>::value, y = alignof_t<T>::value, value = x < y ? x : y };
};
#define FUX_ALIGN_OF(type) ((size_t)alignof_t3<type>::value)

#else
// C implementation of FUX_ALIGN_OF
// This implementation works for most cases, but doesn't directly work
// for types such as function pointer declarations. To work with those
// types you need to typedef the type and then use the typedef in FUX_ALIGN_OF.
#define FUX_ALIGN_OF(type) ((size_t)offsetof(struct { char c; type m; }, m))
#endif
#endif

// FUX_PRAGMA_PACK_VC
//
// Wraps #pragma pack in a way that allows for cleaner code.
//
// Example usage:
//    FUX_PRAGMA_PACK_VC(push, 1)
//    struct X{ char c; int i; };
//    FUX_PRAGMA_PACK_VC(pop)
//
#if !defined(FUX_PRAGMA_PACK_VC)
    #if defined(FUX_COMPILER_MSVC)
        #define FUX_PRAGMA_PACK_VC(...) __pragma(pack(__VA_ARGS__))
    #elif !defined(FUX_COMPILER_NO_VARIADIC_MACROS)
        #define FUX_PRAGMA_PACK_VC(...)
    #else
        // No support. However, all compilers of significance to us support variadic macros.
    #endif
#endif


// ------------------------------------------------------------------------
// FUX_LIKELY / FUX_UNLIKELY
//
// Defined as a macro which gives a hint to the compiler for branch
// prediction. GCC gives you the ability to manually give a hint to
// the compiler about the result of a comparison, though it's often
// best to compile shipping code with profiling feedback under both
// GCC (-fprofile-arcs) and VC++ (/LTCG:PGO, etc.). However, there
// are times when you feel very sure that a boolean expression will
// usually evaluate to either true or false and can help the compiler
// by using an explicity directive...
//
// Example usage:
//    if(FUX_LIKELY(a == 0)) // Tell the compiler that a will usually equal 0.
//       { ... }
//
// Example usage:
//    if(FUX_UNLIKELY(a == 0)) // Tell the compiler that a will usually not equal 0.
//       { ... }
//
#ifndef FUX_LIKELY
    #if (defined(__GNUC__) && (__GNUC__ >= 3)) || defined(__clang__)
        #if defined(__cplusplus)
            #define FUX_LIKELY(x)   __builtin_expect(!!(x), true)
            #define FUX_UNLIKELY(x) __builtin_expect(!!(x), false)
        #else
            #define FUX_LIKELY(x)   __builtin_expect(!!(x), 1)
            #define FUX_UNLIKELY(x) __builtin_expect(!!(x), 0)
        #endif
    #else
        #define FUX_LIKELY(x)   (x)
        #define FUX_UNLIKELY(x) (x)
    #endif
#endif


// ------------------------------------------------------------------------
// FUX_INIT_PRIORITY_AVAILABLE
//
// This value is either not defined, or defined to 1.
// Defines if the GCC attribute init_priority is supported by the compiler.
//
#if !defined(FUX_INIT_PRIORITY_AVAILABLE)
    #if   defined(__GNUC__) && !defined(__EDG__) // EDG typically #defines __GNUC__ but doesn't implement init_priority.
        #define FUX_INIT_PRIORITY_AVAILABLE 1
    #endif
#endif


// ------------------------------------------------------------------------
// FUX_INIT_PRIORITY
//
// This is simply a wrapper for the GCC init_priority attribute that allows
// multiplatform code to be easier to read. This attribute doesn't apply
// to VC++ because VC++ uses file-level pragmas to control init ordering.
//
// Example usage:
//     SomeClass gSomeClass FUX_INIT_PRIORITY(2000);
//
#if !defined(FUX_INIT_PRIORITY)
    #if defined(FUX_INIT_PRIORITY_AVAILABLE)
        #define FUX_INIT_PRIORITY(x)  __attribute__ ((init_priority (x)))
    #else
        #define FUX_INIT_PRIORITY(x)
    #endif
#endif


// ------------------------------------------------------------------------
// FUX_MAY_ALIAS_AVAILABLE
//
// Defined as 0, 1, or 2.
// Defines if the GCC attribute may_alias is supported by the compiler.
// Consists of a value 0 (unsupported, shouldn't be used), 1 (some support),
// or 2 (full proper support).
//
#ifndef FUX_MAY_ALIAS_AVAILABLE
    #if defined(__GNUC__) && (((__GNUC__ * 100) + __GNUC_MINOR__) >= 303)
        #if   !defined(__EDG__)                 // define it as 1 while defining GCC's support as 2.
            #define FUX_MAY_ALIAS_AVAILABLE 2
        #else
            #define FUX_MAY_ALIAS_AVAILABLE 0
        #endif
    #else
        #define FUX_MAY_ALIAS_AVAILABLE 0
    #endif
#endif


// FUX_MAY_ALIAS
//
// Defined as a macro that wraps the GCC may_alias attribute. This attribute
// has no significance for VC++ because VC++ doesn't support the concept of
// strict aliasing. Users should avoid writing code that breaks strict
// aliasing rules; FUX_MAY_ALIAS is for cases with no alternative.
//
// Example usage:
//    void* FUX_MAY_ALIAS gPtr = NULL;
//
// Example usage:
//    typedef void* FUX_MAY_ALIAS pvoid_may_alias;
//    pvoid_may_alias gPtr = NULL;
//
#if FUX_MAY_ALIAS_AVAILABLE
    #define FUX_MAY_ALIAS __attribute__((__may_alias__))
#else
    #define FUX_MAY_ALIAS
#endif


// ------------------------------------------------------------------------
// FUX_ASSUME
//
// This acts the same as the VC++ __assume directive and is implemented
// simply as a wrapper around it to allow portable usage of it and to take
// advantage of it if and when it appears in other compilers.
//
// Example usage:
//    void Function(int a) {
//       switch(a) {
//          case 1:
//             DoSomething(1);
//             break;
//          case 2:
//             DoSomething(-1);
//             break;
//          default:
//             FUX_ASSUME(0); // This tells the optimizer that the default cannot be reached.
//       }
//    }
//
#ifndef FUX_ASSUME
    #if defined(_MSC_VER) && (_MSC_VER >= 1300) // If VC7.0 and later
        #define FUX_ASSUME(x) __assume(x)
    #else
        #define FUX_ASSUME(x)
    #endif
#endif



// ------------------------------------------------------------------------
// FUX_ANALYSIS_ASSUME
//
// This acts the same as the VC++ __analysis_assume directive and is implemented
// simply as a wrapper around it to allow portable usage of it and to take
// advantage of it if and when it appears in other compilers.
//
// Example usage:
//    char Function(char* p) {
//       FUX_ANALYSIS_ASSUME(p != NULL);
//       return *p;
//    }
//
#ifndef FUX_ANALYSIS_ASSUME
    #if defined(_MSC_VER) && (_MSC_VER >= 1300) // If VC7.0 and later
        #define FUX_ANALYSIS_ASSUME(x) __analysis_assume(!!(x)) // !! because that allows for convertible-to-bool in addition to bool.
    #else
        #define FUX_ANALYSIS_ASSUME(x)
    #endif
#endif



// ------------------------------------------------------------------------
// FUX_DISABLE_VC_WARNING / FUX_RESTORE_VC_WARNING
//
// Disable and re-enable warning(s) within code.
// This is simply a wrapper for VC++ #pragma warning(disable: nnnn) for the
// purpose of making code easier to read due to avoiding nested compiler ifdefs
// directly in code.
//
// Example usage:
//     FUX_DISABLE_VC_WARNING(4127 3244)
//     <code>
//     FUX_RESTORE_VC_WARNING()
//
#ifndef FUX_DISABLE_VC_WARNING
#if defined(_MSC_VER)
#define FUX_DISABLE_VC_WARNING(w)  \
    __pragma(warning(push))       \
    __pragma(warning(disable:w))
#else
#define FUX_DISABLE_VC_WARNING(w)
#endif
#endif

#ifndef FUX_RESTORE_VC_WARNING
#if defined(_MSC_VER)
#define FUX_RESTORE_VC_WARNING()   \
    __pragma(warning(pop))
#else
#define FUX_RESTORE_VC_WARNING()
#endif
#endif


// ------------------------------------------------------------------------
// FUX_ENABLE_VC_WARNING_AS_ERROR / FUX_DISABLE_VC_WARNING_AS_ERROR
//
// Disable and re-enable treating a warning as error within code.
// This is simply a wrapper for VC++ #pragma warning(error: nnnn) for the
// purpose of making code easier to read due to avoiding nested compiler ifdefs
// directly in code.
//
// Example usage:
//     FUX_ENABLE_VC_WARNING_AS_ERROR(4996)
//     <code>
//     FUX_DISABLE_VC_WARNING_AS_ERROR()
//
#ifndef FUX_ENABLE_VC_WARNING_AS_ERROR
#if defined(_MSC_VER)
#define FUX_ENABLE_VC_WARNING_AS_ERROR(w) \
    __pragma(warning(push)) \
    __pragma(warning(error:w))
#else
#define FUX_ENABLE_VC_WARNING_AS_ERROR(w)
#endif
#endif

#ifndef FUX_DISABLE_VC_WARNING_AS_ERROR
#if defined(_MSC_VER)
#define FUX_DISABLE_VC_WARNING_AS_ERROR() \
    __pragma(warning(pop))
#else
#define FUX_DISABLE_VC_WARNING_AS_ERROR()
#endif
#endif


// ------------------------------------------------------------------------
// FUX_DISABLE_GCC_WARNING / FUX_RESTORE_GCC_WARNING
//
// Example usage:
//     // Only one warning can be ignored per statement, due to how GCC works.
//     FUX_DISABLE_GCC_WARNING(-Wuninitialized)
//     FUX_DISABLE_GCC_WARNING(-Wunused)
//     <code>
//     FUX_RESTORE_GCC_WARNING()
//     FUX_RESTORE_GCC_WARNING()
//
#ifndef FUX_DISABLE_GCC_WARNING
#if defined(FUX_COMPILER_GNUC)
    #define EAGCCWHELP0(x) #x
    #define EAGCCWHELP1(x) EAGCCWHELP0(GCC diagnostic ignored x)
    #define EAGCCWHELP2(x) EAGCCWHELP1(#x)
#endif

#if defined(FUX_COMPILER_GNUC) && (FUX_COMPILER_VERSION >= 4006) // Can't test directly for __GNUC__ because some compilers lie.
#define FUX_DISABLE_GCC_WARNING(w)   \
    _Pragma("GCC diagnostic push")  \
    _Pragma(EAGCCWHELP2(w))
#elif defined(FUX_COMPILER_GNUC) && (FUX_COMPILER_VERSION >= 4004)
#define FUX_DISABLE_GCC_WARNING(w)   \
    _Pragma(EAGCCWHELP2(w))
#else
#define FUX_DISABLE_GCC_WARNING(w)
#endif
#endif

#ifndef FUX_RESTORE_GCC_WARNING
#if defined(FUX_COMPILER_GNUC) && (FUX_COMPILER_VERSION >= 4006)
#define FUX_RESTORE_GCC_WARNING()    \
    _Pragma("GCC diagnostic pop")
#else
#define FUX_RESTORE_GCC_WARNING()
#endif
#endif


// ------------------------------------------------------------------------
// FUX_DISABLE_ALL_GCC_WARNINGS / FUX_RESTORE_ALL_GCC_WARNINGS
//
// This isn't possible except via using _Pragma("GCC system_header"), though
// that has some limitations in how it works. Another means is to manually
// disable individual warnings within a GCC diagnostic push statement.
// GCC doesn't have as many warnings as VC++ and EDG and so this may be feasible.
// ------------------------------------------------------------------------


// ------------------------------------------------------------------------
// FUX_ENABLE_GCC_WARNING_AS_ERROR / FUX_DISABLE_GCC_WARNING_AS_ERROR
//
// Example usage:
//     // Only one warning can be treated as an error per statement, due to how GCC works.
//     FUX_ENABLE_GCC_WARNING_AS_ERROR(-Wuninitialized)
//     FUX_ENABLE_GCC_WARNING_AS_ERROR(-Wunused)
//     <code>
//     FUX_DISABLE_GCC_WARNING_AS_ERROR()
//     FUX_DISABLE_GCC_WARNING_AS_ERROR()
//
#ifndef FUX_ENABLE_GCC_WARNING_AS_ERROR
#if defined(FUX_COMPILER_GNUC)
    #define EAGCCWERRORHELP0(x) #x
    #define EAGCCWERRORHELP1(x) EAGCCWERRORHELP0(GCC diagnostic error x)
    #define EAGCCWERRORHELP2(x) EAGCCWERRORHELP1(#x)
#endif

#if defined(FUX_COMPILER_GNUC) && (FUX_COMPILER_VERSION >= 4006) // Can't test directly for __GNUC__ because some compilers lie.
#define FUX_ENABLE_GCC_WARNING_AS_ERROR(w)   \
    _Pragma("GCC diagnostic push")  \
    _Pragma(EAGCCWERRORHELP2(w))
#elif defined(FUX_COMPILER_GNUC) && (FUX_COMPILER_VERSION >= 4004)
#define FUX_DISABLE_GCC_WARNING(w)   \
    _Pragma(EAGCCWERRORHELP2(w))
#else
#define FUX_DISABLE_GCC_WARNING(w)
#endif
#endif

#ifndef FUX_DISABLE_GCC_WARNING_AS_ERROR
#if defined(FUX_COMPILER_GNUC) && (FUX_COMPILER_VERSION >= 4006)
#define FUX_DISABLE_GCC_WARNING_AS_ERROR()    \
    _Pragma("GCC diagnostic pop")
#else
#define FUX_DISABLE_GCC_WARNING_AS_ERROR()
#endif
#endif


// ------------------------------------------------------------------------
// FUX_DISABLE_CLANG_WARNING / FUX_RESTORE_CLANG_WARNING
//
// Example usage:
//     // Only one warning can be ignored per statement, due to how clang works.
//     FUX_DISABLE_CLANG_WARNING(-Wuninitialized)
//     FUX_DISABLE_CLANG_WARNING(-Wunused)
//     <code>
//     FUX_RESTORE_CLANG_WARNING()
//     FUX_RESTORE_CLANG_WARNING()
//
#ifndef FUX_DISABLE_CLANG_WARNING
#if defined(FUX_COMPILER_CLANG)
#define EACLANGWHELP0(x) #x
#define EACLANGWHELP1(x) EACLANGWHELP0(clang diagnostic ignored x)
#define EACLANGWHELP2(x) EACLANGWHELP1(#x)

#define FUX_DISABLE_CLANG_WARNING(w)   \
    _Pragma("clang diagnostic push")  \
    _Pragma(EACLANGWHELP2(w))
#else
#define FUX_DISABLE_CLANG_WARNING(w)
#endif
#endif

#ifndef FUX_RESTORE_CLANG_WARNING
#if defined(FUX_COMPILER_CLANG)
#define FUX_RESTORE_CLANG_WARNING()    \
    _Pragma("clang diagnostic pop")
#else
#define FUX_RESTORE_CLANG_WARNING()
#endif
#endif


// ------------------------------------------------------------------------
// FUX_DISABLE_ALL_CLANG_WARNINGS / FUX_RESTORE_ALL_CLANG_WARNINGS
//
// The situation for clang is the same as for GCC. See above.
// ------------------------------------------------------------------------


// ------------------------------------------------------------------------
// FUX_ENABLE_CLANG_WARNING_AS_ERROR / FUX_DISABLE_CLANG_WARNING_AS_ERROR
//
// Example usage:
//     // Only one warning can be treated as an error per statement, due to how clang works.
//     FUX_ENABLE_CLANG_WARNING_AS_ERROR(-Wuninitialized)
//     FUX_ENABLE_CLANG_WARNING_AS_ERROR(-Wunused)
//     <code>
//     FUX_DISABLE_CLANG_WARNING_AS_ERROR()
//     FUX_DISABLE_CLANG_WARNING_AS_ERROR()
//
#ifndef FUX_ENABLE_CLANG_WARNING_AS_ERROR
#if defined(FUX_COMPILER_CLANG)
#define EACLANGWERRORHELP0(x) #x
#define EACLANGWERRORHELP1(x) EACLANGWERRORHELP0(clang diagnostic error x)
#define EACLANGWERRORHELP2(x) EACLANGWERRORHELP1(#x)

#define FUX_ENABLE_CLANG_WARNING_AS_ERROR(w)   \
    _Pragma("clang diagnostic push")  \
    _Pragma(EACLANGWERRORHELP2(w))
#else
#define FUX_DISABLE_CLANG_WARNING(w)
#endif
#endif

#ifndef FUX_DISABLE_CLANG_WARNING_AS_ERROR
#if defined(FUX_COMPILER_CLANG)
#define FUX_DISABLE_CLANG_WARNING_AS_ERROR()    \
    _Pragma("clang diagnostic pop")
#else
#define FUX_DISABLE_CLANG_WARNING_AS_ERROR()
#endif
#endif


// ------------------------------------------------------------------------
// FUX_DISABLE_SN_WARNING / FUX_RESTORE_SN_WARNING
//
// Note that we define this macro specifically for the SN compiler instead of
// having a generic one for EDG-based compilers. The reason for this is that
// while SN is indeed based on EDG, SN has different warning value mappings
// and thus warning 1234 for SN is not the same as 1234 for all other EDG compilers.
//
// Example usage:
//     // Currently we are limited to one warning per line.
//     FUX_DISABLE_SN_WARNING(1787)
//     FUX_DISABLE_SN_WARNING(552)
//     <code>
//     FUX_RESTORE_SN_WARNING()
//     FUX_RESTORE_SN_WARNING()
//
#ifndef FUX_DISABLE_SN_WARNING
    #define FUX_DISABLE_SN_WARNING(w)
#endif

#ifndef FUX_RESTORE_SN_WARNING
    #define FUX_RESTORE_SN_WARNING()
#endif


// ------------------------------------------------------------------------
// FUX_DISABLE_ALL_SN_WARNINGS / FUX_RESTORE_ALL_SN_WARNINGS
//
// Example usage:
//     FUX_DISABLE_ALL_SN_WARNINGS()
//     <code>
//     FUX_RESTORE_ALL_SN_WARNINGS()
//
#ifndef FUX_DISABLE_ALL_SN_WARNINGS
    #define FUX_DISABLE_ALL_SN_WARNINGS()
#endif

#ifndef FUX_RESTORE_ALL_SN_WARNINGS
    #define FUX_RESTORE_ALL_SN_WARNINGS()
#endif



// ------------------------------------------------------------------------
// FUX_DISABLE_GHS_WARNING / FUX_RESTORE_GHS_WARNING
//
// Disable warnings from the Green Hills compiler.
//
// Example usage:
//     FUX_DISABLE_GHS_WARNING(193)
//     FUX_DISABLE_GHS_WARNING(236, 5323)
//     <code>
//     FUX_RESTORE_GHS_WARNING()
//     FUX_RESTORE_GHS_WARNING()
//
#ifndef FUX_DISABLE_GHS_WARNING
    #define FUX_DISABLE_GHS_WARNING(w)
#endif

#ifndef FUX_RESTORE_GHS_WARNING
    #define FUX_RESTORE_GHS_WARNING()
#endif


// ------------------------------------------------------------------------
// FUX_DISABLE_ALL_GHS_WARNINGS / FUX_RESTORE_ALL_GHS_WARNINGS
//
// #ifndef FUX_DISABLE_ALL_GHS_WARNINGS
//     #if defined(FUX_COMPILER_GREEN_HILLS)
//         #define FUX_DISABLE_ALL_GHS_WARNINGS(w)  \_
//             _Pragma("_________")
//     #else
//         #define FUX_DISABLE_ALL_GHS_WARNINGS(w)
//     #endif
// #endif
//
// #ifndef FUX_RESTORE_ALL_GHS_WARNINGS
//     #if defined(FUX_COMPILER_GREEN_HILLS)
//         #define FUX_RESTORE_ALL_GHS_WARNINGS()   \_
//             _Pragma("_________")
//     #else
//         #define FUX_RESTORE_ALL_GHS_WARNINGS()
//     #endif
// #endif



// ------------------------------------------------------------------------
// FUX_DISABLE_EDG_WARNING / FUX_RESTORE_EDG_WARNING
//
// Example usage:
//     // Currently we are limited to one warning per line.
//     FUX_DISABLE_EDG_WARNING(193)
//     FUX_DISABLE_EDG_WARNING(236)
//     <code>
//     FUX_RESTORE_EDG_WARNING()
//     FUX_RESTORE_EDG_WARNING()
//
#ifndef FUX_DISABLE_EDG_WARNING
// EDG-based compilers are inconsistent in how the implement warning pragmas.
#if defined(FUX_COMPILER_EDG) && !defined(FUX_COMPILER_INTEL) && !defined(FUX_COMPILER_RVCT)
#define EAEDGWHELP0(x) #x
#define EAEDGWHELP1(x) EAEDGWHELP0(diag_suppress x)

#define FUX_DISABLE_EDG_WARNING(w)   \
    _Pragma("control %push diag")   \
    _Pragma(EAEDGWHELP1(w))
#else
#define FUX_DISABLE_EDG_WARNING(w)
#endif
#endif

#ifndef FUX_RESTORE_EDG_WARNING
#if defined(FUX_COMPILER_EDG) && !defined(FUX_COMPILER_INTEL) && !defined(FUX_COMPILER_RVCT)
#define FUX_RESTORE_EDG_WARNING()   \
    _Pragma("control %pop diag")
#else
#define FUX_RESTORE_EDG_WARNING()
#endif
#endif


// ------------------------------------------------------------------------
// FUX_DISABLE_ALL_EDG_WARNINGS / FUX_RESTORE_ALL_EDG_WARNINGS
//
//#ifndef FUX_DISABLE_ALL_EDG_WARNINGS
//    #if defined(FUX_COMPILER_EDG) && !defined(FUX_COMPILER_SN)
//        #define FUX_DISABLE_ALL_EDG_WARNINGS(w)  \_
//            _Pragma("_________")
//    #else
//        #define FUX_DISABLE_ALL_EDG_WARNINGS(w)
//    #endif
//#endif
//
//#ifndef FUX_RESTORE_ALL_EDG_WARNINGS
//    #if defined(FUX_COMPILER_EDG) && !defined(FUX_COMPILER_SN)
//        #define FUX_RESTORE_ALL_EDG_WARNINGS()   \_
//            _Pragma("_________")
//    #else
//        #define FUX_RESTORE_ALL_EDG_WARNINGS()
//    #endif
//#endif



// ------------------------------------------------------------------------
// FUX_DISABLE_CW_WARNING / FUX_RESTORE_CW_WARNING
//
// Note that this macro can only control warnings via numbers and not by
// names. The reason for this is that the compiler's syntax for such
// warnings is not the same as for numbers.
//
// Example usage:
//     // Currently we are limited to one warning per line and must also specify the warning in the restore macro.
//     FUX_DISABLE_CW_WARNING(10317)
//     FUX_DISABLE_CW_WARNING(10324)
//     <code>
//     FUX_RESTORE_CW_WARNING(10317)
//     FUX_RESTORE_CW_WARNING(10324)
//
#ifndef FUX_DISABLE_CW_WARNING
    #define FUX_DISABLE_CW_WARNING(w)
#endif

#ifndef FUX_RESTORE_CW_WARNING
    
    #define FUX_RESTORE_CW_WARNING(w)
    
#endif


// ------------------------------------------------------------------------
// FUX_DISABLE_ALL_CW_WARNINGS / FUX_RESTORE_ALL_CW_WARNINGS
//
#ifndef FUX_DISABLE_ALL_CW_WARNINGS
    #define FUX_DISABLE_ALL_CW_WARNINGS()
    
#endif

#ifndef FUX_RESTORE_ALL_CW_WARNINGS
    #define FUX_RESTORE_ALL_CW_WARNINGS()
#endif



// ------------------------------------------------------------------------
// FUX_PURE
//
// This acts the same as the GCC __attribute__ ((pure)) directive and is
// implemented simply as a wrapper around it to allow portable usage of
// it and to take advantage of it if and when it appears in other compilers.
//
// A "pure" function is one that has no effects except its return value and
// its return value is a function of only the function's parameters or
// non-volatile global variables. Any parameter or global variable access
// must be read-only. Loop optimization and subexpression elimination can be
// applied to such functions. A common example is strlen(): Given identical
// inputs, the function's return value (its only effect) is invariant across
// multiple invocations and thus can be pulled out of a loop and called but once.
//
// Example usage:
//    FUX_PURE void Function();
//
#ifndef FUX_PURE
    #if defined(FUX_COMPILER_GNUC)
        #define FUX_PURE __attribute__((pure))
    #elif defined(FUX_COMPILER_ARM)  // Arm brand compiler for ARM CPU
        #define FUX_PURE __pure
    #else
        #define FUX_PURE
    #endif
#endif



// ------------------------------------------------------------------------
// FUX_WEAK
// FUX_WEAK_SUPPORTED -- defined as 0 or 1.
//
// GCC
// The weak attribute causes the declaration to be emitted as a weak
// symbol rather than a global. This is primarily useful in defining
// library functions which can be overridden in user code, though it
// can also be used with non-function declarations.
//
// VC++
// At link time, if multiple definitions of a COMDAT are seen, the linker
// picks one and discards the rest. If the linker option /OPT:REF
// is selected, then COMDAT elimination will occur to remove all the
// unreferenced data items in the linker output.
//
// Example usage:
//    FUX_WEAK void Function();
//
#ifndef FUX_WEAK
    #if defined(_MSC_VER) && (_MSC_VER >= 1300) // If VC7.0 and later
        #define FUX_WEAK __declspec(selectany)
        #define FUX_WEAK_SUPPORTED 1
    #elif defined(_MSC_VER) || (defined(__GNUC__) && defined(__CYGWIN__))
        #define FUX_WEAK
        #define FUX_WEAK_SUPPORTED 0
    #elif defined(FUX_COMPILER_ARM)  // Arm brand compiler for ARM CPU
        #define FUX_WEAK __weak
        #define FUX_WEAK_SUPPORTED 1
    #else                           // GCC and IBM compilers, others.
        #define FUX_WEAK __attribute__((weak))
        #define FUX_WEAK_SUPPORTED 1
    #endif
#endif



// ------------------------------------------------------------------------
// FUX_UNUSED
//
// Makes compiler warnings about unused variables go away.
//
// Example usage:
//    void Function(int x)
//    {
//        int y;
//        FUX_UNUSED(x);
//        FUX_UNUSED(y);
//    }
//
#ifndef FUX_UNUSED
// The EDG solution below is pretty weak and needs to be augmented or replaced.
// It can't handle the C language, is limited to places where template declarations
// can be used, and requires the type x to be usable as a functions reference argument.
#if defined(__cplusplus) && defined(__EDG__)
template <typename T>
inline void fux_unused( T const volatile& x )
{
    ( void )x;
}
#define FUX_UNUSED(x) fux_unused(x)
#else
#define FUX_UNUSED(x) (void)x
#endif
#endif



// ------------------------------------------------------------------------
// FUX_EMPTY
//
// Allows for a null statement, usually for the purpose of avoiding compiler warnings.
//
// Example usage:
//    #ifdef FUX_DEBUG
//        #define MyDebugPrintf(x, y) printf(x, y)
//    #else
//        #define MyDebugPrintf(x, y)  FUX_EMPTY
//    #endif
//
#ifndef FUX_EMPTY
    #define FUX_EMPTY (void)0
#endif



// ------------------------------------------------------------------------
// wchar_t
// Here we define:
//    FUX_WCHAR_T_NON_NATIVE
//    FUX_WCHAR_SIZE = <sizeof(wchar_t)>
//
#ifndef FUX_WCHAR_T_NON_NATIVE
    // Compilers that always implement wchar_t as native include:
    //     COMEAU, new SN, and other EDG-based compilers.
    //     GCC
    //     Borland
    //     SunPro
    //     IBM Visual Age
    #if defined(FUX_COMPILER_INTEL)
        #if (FUX_COMPILER_VERSION < 700)
            #define FUX_WCHAR_T_NON_NATIVE 1
        #else
            #if (!defined(_WCHAR_T_DEFINED) && !defined(_WCHAR_T))
                #define FUX_WCHAR_T_NON_NATIVE 1
            #endif
        #endif
    #elif defined(FUX_COMPILER_MSVC) || defined(FUX_COMPILER_BORLAND) || (defined(FUX_COMPILER_CLANG) && defined(FUX_PLATFORM_WINDOWS))
        #ifndef _NATIVE_WCHAR_T_DEFINED
            #define FUX_WCHAR_T_NON_NATIVE 1
        #endif
    #elif defined(__EDG_VERSION__) && (!defined(_WCHAR_T) && (__EDG_VERSION__ < 400)) // EDG prior to v4 uses _WCHAR_T to indicate if wchar_t is native. v4+ may define something else, but we're not currently aware of it.
        #define FUX_WCHAR_T_NON_NATIVE 1
    #endif
#endif

#ifndef FUX_WCHAR_SIZE // If the user hasn't specified that it is a given size...
    #if defined(__WCHAR_MAX__) // GCC defines this for most platforms.
        #if (__WCHAR_MAX__ == 2147483647) || (__WCHAR_MAX__ == 4294967295)
            #define FUX_WCHAR_SIZE 4
        #elif (__WCHAR_MAX__ == 32767) || (__WCHAR_MAX__ == 65535)
            #define FUX_WCHAR_SIZE 2
        #elif (__WCHAR_MAX__ == 127) || (__WCHAR_MAX__ == 255)
            #define FUX_WCHAR_SIZE 1
        #else
            #define FUX_WCHAR_SIZE 4
        #endif
    #elif defined(WCHAR_MAX) // The SN and Arm compilers define this.
        #if (WCHAR_MAX == 2147483647) || (WCHAR_MAX == 4294967295)
            #define FUX_WCHAR_SIZE 4
        #elif (WCHAR_MAX == 32767) || (WCHAR_MAX == 65535)
            #define FUX_WCHAR_SIZE 2
        #elif (WCHAR_MAX == 127) || (WCHAR_MAX == 255)
            #define FUX_WCHAR_SIZE 1
        #else
            #define FUX_WCHAR_SIZE 4
        #endif
    #elif defined(__WCHAR_BIT) // Green Hills (and other versions of EDG?) uses this.
        #if (__WCHAR_BIT == 16)
            #define FUX_WCHAR_SIZE 2
        #elif (__WCHAR_BIT == 32)
            #define FUX_WCHAR_SIZE 4
        #elif (__WCHAR_BIT == 8)
            #define FUX_WCHAR_SIZE 1
        #else
            #define FUX_WCHAR_SIZE 4
        #endif
    #elif defined(_WCMAX) // The SN and Arm compilers define this.
        #if (_WCMAX == 2147483647) || (_WCMAX == 4294967295)
            #define FUX_WCHAR_SIZE 4
        #elif (_WCMAX == 32767) || (_WCMAX == 65535)
            #define FUX_WCHAR_SIZE 2
        #elif (_WCMAX == 127) || (_WCMAX == 255)
            #define FUX_WCHAR_SIZE 1
        #else
            #define FUX_WCHAR_SIZE 4
        #endif
    #elif defined(FUX_PLATFORM_UNIX)
        // It is standard on Unix to have wchar_t be int32_t or uint32_t.
        // All versions of GNUC default to a 32 bit wchar_t, but EA has used
        // the -fshort-wchar GCC command line option to force it to 16 bit.
        // If you know that the compiler is set to use a wchar_t of other than
        // the default, you need to manually define FUX_WCHAR_SIZE for the build.
        #define FUX_WCHAR_SIZE 4
    #else
        // It is standard on Windows to have wchar_t be uint16_t.  GCC
        // defines wchar_t as int by default.  FuxGames has
        // standardized on wchar_t being an unsigned 16 bit value on all
        // console platforms. Given that there is currently no known way to
        // tell at preprocessor time what the size of wchar_t is, we declare
        // it to be 2, as this is the FuxGames standard. If you have
        // FUX_WCHAR_SIZE != sizeof(wchar_t), then your code might not be
        // broken, but it also won't work with wchar libraries and data from
        // other parts of EA. Under GCC, you can force wchar_t to two bytes
        // with the -fshort-wchar compiler argument.
        #define FUX_WCHAR_SIZE 2
    #endif
#endif


// ------------------------------------------------------------------------
// FUX_RESTRICT
//
// The C99 standard defines a new keyword, restrict, which allows for the
// improvement of code generation regarding memory usage. Compilers can
// generate significantly faster code when you are able to use restrict.
//
// Example usage:
//    void DoSomething(char* FUX_RESTRICT p1, char* FUX_RESTRICT p2);
//
#ifndef FUX_RESTRICT
    #if defined(FUX_COMPILER_MSVC) && (FUX_COMPILER_VERSION >= 1400) // If VC8 (VS2005) or later...
        #define FUX_RESTRICT __restrict
    #elif defined(FUX_COMPILER_CLANG)
        #define FUX_RESTRICT __restrict
    #elif defined(FUX_COMPILER_GNUC)     // Includes GCC and other compilers emulating GCC.
        #define FUX_RESTRICT __restrict  // GCC defines 'restrict' (as opposed to __restrict) in C99 mode only.
    #elif defined(FUX_COMPILER_ARM)
        #define FUX_RESTRICT __restrict
    #elif defined(FUX_COMPILER_IS_C99)
        #define FUX_RESTRICT restrict
    #else
        // If the compiler didn't support restricted pointers, defining FUX_RESTRICT
        // away would result in compiling and running fine but you just wouldn't
        // the same level of optimization. On the other hand, all the major compilers
        // support restricted pointers.
        #define FUX_RESTRICT
    #endif
#endif


// ------------------------------------------------------------------------
// FUX_DEPRECATED            // Used as a prefix.
// FUX_PREFIX_DEPRECATED     // You should need this only for unusual compilers.
// FUX_POSTFIX_DEPRECATED    // You should need this only for unusual compilers.
// FUX_DEPRECATED_MESSAGE    // Used as a prefix and provides a deprecation message.
//
// Example usage:
//    FUX_DEPRECATED void Function();
//    FUX_DEPRECATED_MESSAGE("Use 1.0v API instead") void Function();
//
// or for maximum portability:
//    FUX_PREFIX_DEPRECATED void Function() FUX_POSTFIX_DEPRECATED;
//

#ifndef FUX_DEPRECATED
    #if defined(FUX_COMPILER_CPP14_ENABLED)
        #define FUX_DEPRECATED [[deprecated]]
    #elif defined(FUX_COMPILER_MSVC) && (FUX_COMPILER_VERSION > 1300) // If VC7 (VS2003) or later...
        #define FUX_DEPRECATED __declspec(deprecated)
    #elif defined(FUX_COMPILER_MSVC)
        #define FUX_DEPRECATED
    #else
        #define FUX_DEPRECATED __attribute__((deprecated))
    #endif
#endif

#ifndef FUX_PREFIX_DEPRECATED
    #if defined(FUX_COMPILER_CPP14_ENABLED)
        #define FUX_PREFIX_DEPRECATED [[deprecated]]
        #define FUX_POSTFIX_DEPRECATED
    #elif defined(FUX_COMPILER_MSVC) && (FUX_COMPILER_VERSION > 1300) // If VC7 (VS2003) or later...
        #define FUX_PREFIX_DEPRECATED __declspec(deprecated)
        #define FUX_POSTFIX_DEPRECATED
    #elif defined(FUX_COMPILER_MSVC)
        #define FUX_PREFIX_DEPRECATED
        #define FUX_POSTFIX_DEPRECATED
    #else
        #define FUX_PREFIX_DEPRECATED
        #define FUX_POSTFIX_DEPRECATED __attribute__((deprecated))
    #endif
#endif

#ifndef FUX_DEPRECATED_MESSAGE
    #if defined(FUX_COMPILER_CPP14_ENABLED)
        #define FUX_DEPRECATED_MESSAGE(msg) [[deprecated(#msg)]]
    #else
        // Compiler does not support depreaction messages, explicitly drop the msg but still mark the function as deprecated
        #define FUX_DEPRECATED_MESSAGE(msg) FUX_DEPRECATED
    #endif
#endif


// ------------------------------------------------------------------------
// FUX_INL              // Used as a prefix.
// FUX_PREFIX_FORCE_INLINE       // You should need this only for unusual compilers.
// FUX_POSTFIX_FORCE_INLINE      // You should need this only for unusual compilers.
//
// Example usage:
//     FUX_INL void Foo();                                // Implementation elsewhere.
//     FUX_PREFIX_FORCE_INLINE void Foo() FUX_POSTFIX_FORCE_INLINE; // Implementation elsewhere.
//
// Note that when the prefix version of this function is used, it replaces
// the regular C++ 'inline' statement. Thus you should not use both the
// C++ inline statement and this macro with the same function declaration.
//
// To force inline usage under GCC 3.1+, you use this:
//    inline void Foo() __attribute__((always_inline));
//       or
//    inline __attribute__((always_inline)) void Foo();
//
// The CodeWarrior compiler doesn't have the concept of forcing inlining per function.
//
#ifndef FUX_INL
    #if defined(FUX_COMPILER_MSVC)
        #define FUX_INL __forceinline
        
    #elif defined(FUX_COMPILER_GNUC) && (((__GNUC__ * 100) + __GNUC_MINOR__) >= 301) || defined(FUX_COMPILER_CLANG)
        #if defined(__cplusplus)
            #define FUX_INL inline __attribute__((always_inline))
        #else
            #define FUX_INL __inline__ __attribute__((always_inline))
        #endif
    #else
        #if defined(__cplusplus)
            #define FUX_INL inline
        #else
            #define FUX_INL __inline
        #endif
    #endif
#endif

#define FUX_EX_INL extern FUX_INL

#if   defined(FUX_COMPILER_GNUC) && (((__GNUC__ * 100) + __GNUC_MINOR__) >= 301) || defined(FUX_COMPILER_CLANG)
    #define FUX_PREFIX_FORCE_INLINE  inline
    #define FUX_POSTFIX_FORCE_INLINE __attribute__((always_inline))
#else
    #define FUX_PREFIX_FORCE_INLINE  inline
    #define FUX_POSTFIX_FORCE_INLINE
#endif


// ------------------------------------------------------------------------
// FUX_NO_INLINE             // Used as a prefix.
// FUX_PREFIX_NO_INLINE      // You should need this only for unusual compilers.
// FUX_POSTFIX_NO_INLINE     // You should need this only for unusual compilers.
//
// Example usage:
//     FUX_NO_INLINE        void Foo();                       // Implementation elsewhere.
//     FUX_PREFIX_NO_INLINE void Foo() FUX_POSTFIX_NO_INLINE;  // Implementation elsewhere.
//
// That this declaration is incompatbile with C++ 'inline' and any
// variant of FUX_INL.
//
// To disable inline usage under VC++ priof to VS2005, you need to use this:
//    #pragma inline_depth(0) // Disable inlining.
//    void Foo() { ... }
//    #pragma inline_depth()  // Restore to default.
//
// Since there is no easy way to disable inlining on a function-by-function
// basis in VC++ prior to VS2005, the best strategy is to write platform-specific
// #ifdefs in the code or to disable inlining for a given module and enable
// functions individually with FUX_INL.
//
#ifndef FUX_NO_INLINE
    #if defined(FUX_COMPILER_MSVC) && (FUX_COMPILER_VERSION >= 1400) // If VC8 (VS2005) or later...
        #define FUX_NO_INLINE __declspec(noinline)
    #elif defined(FUX_COMPILER_MSVC)
        #define FUX_NO_INLINE
    #else
        #define FUX_NO_INLINE __attribute__((noinline))
    #endif
#endif

#if defined(FUX_COMPILER_MSVC) && (FUX_COMPILER_VERSION >= 1400) // If VC8 (VS2005) or later...
    #define FUX_PREFIX_NO_INLINE  __declspec(noinline)
    #define FUX_POSTFIX_NO_INLINE
#elif defined(FUX_COMPILER_MSVC)
    #define FUX_PREFIX_NO_INLINE
    #define FUX_POSTFIX_NO_INLINE
#else
    #define FUX_PREFIX_NO_INLINE
    #define FUX_POSTFIX_NO_INLINE __attribute__((noinline))
#endif


// ------------------------------------------------------------------------
// FUX_NO_VTABLE
//
// Example usage:
//     class FUX_NO_VTABLE X {
//        virtual void InterfaceFunction();
//     };
//
//     FUX_CLASS_NO_VTABLE(X) {
//        virtual void InterfaceFunction();
//     };
//
#ifdef FUX_COMPILER_MSVC
    #define FUX_NO_VTABLE           __declspec(novtable)
    #define FUX_CLASS_NO_VTABLE(x)  class __declspec(novtable) x
    #define FUX_STRUCT_NO_VTABLE(x) struct __declspec(novtable) x
#else
    #define FUX_NO_VTABLE
    #define FUX_CLASS_NO_VTABLE(x)  class x
    #define FUX_STRUCT_NO_VTABLE(x) struct x
#endif


// ------------------------------------------------------------------------
// FUX_PASCAL
//
// Also known on PC platforms as stdcall.
// This convention causes the compiler to assume that the called function
// will pop off the stack space used to pass arguments, unless it takes a
// variable number of arguments.
//
// Example usage:
//    this:
//       void DoNothing(int x);
//       void DoNothing(int x){}
//    would be written as this:
//       void FUX_PASCAL_FUNC(DoNothing(int x));
//       void FUX_PASCAL_FUNC(DoNothing(int x)){}
//
#ifndef FUX_PASCAL
    #if defined(FUX_COMPILER_MSVC)
        #define FUX_PASCAL __stdcall
    #elif defined(FUX_COMPILER_GNUC) && defined(FUX_PROCESSOR_X86)
        #define FUX_PASCAL __attribute__((stdcall))
    #else
        // Some compilers simply don't support pascal calling convention.
        // As a result, there isn't an issue here, since the specification of
        // pascal calling convention is for the purpose of disambiguating the
        // calling convention that is applied.
        #define FUX_PASCAL
    #endif
#endif

#ifndef FUX_PASCAL_FUNC
    #if defined(FUX_COMPILER_MSVC)
        #define FUX_PASCAL_FUNC(funcname_and_paramlist)    __stdcall funcname_and_paramlist
    #elif defined(FUX_COMPILER_GNUC) && defined(FUX_PROCESSOR_X86)
        #define FUX_PASCAL_FUNC(funcname_and_paramlist)    __attribute__((stdcall)) funcname_and_paramlist
    #else
        #define FUX_PASCAL_FUNC(funcname_and_paramlist)    funcname_and_paramlist
    #endif
#endif


// ------------------------------------------------------------------------
// FUX_SSE
// Visual C Processor Packs define _MSC_FULL_VER and are needed for SSE
// Intel C also has SSE support.
// FUX_SSE is used to select FPU or SSE versions in hw_select.inl
//
// FUX_SSE defines the level of SSE support:
//  0 indicates no SSE support
//  1 indicates SSE1 is supported
//  2 indicates SSE2 is supported
//  3 indicates SSE3 (or greater) is supported
//
// Note: SSE support beyond SSE3 can't be properly represented as a single
// version number.  Instead users should use specific SSE defines (e.g.
// FUX_SSE4_2) to detect what specific support is available.  FUX_SSE being
// equal to 3 really only indicates that SSE3 or greater is supported.
#ifndef FUX_SSE
    #if defined(FUX_COMPILER_GNUC) || defined(FUX_COMPILER_CLANG)
        #if defined(__SSE3__)
            #define FUX_SSE 3
        #elif defined(__SSE2__)
            #define FUX_SSE 2
        #elif defined(__SSE__) && __SSE__
            #define FUX_SSE 1
        #else
            #define FUX_SSE 0
        #endif
    #elif (defined(FUX_SSE3) && FUX_SSE3) || defined FUX_PLATFORM_CAPILANO
        #define FUX_SSE 3
    #elif defined(FUX_SSE2) && FUX_SSE2
        #define FUX_SSE 2
    #elif defined(FUX_PROCESSOR_X86) && defined(_MSC_FULL_VER) && !defined(__NOSSE__) && defined(_M_IX86_FP)
        #define FUX_SSE _M_IX86_FP
    #elif defined(FUX_PROCESSOR_X86) && defined(FUX_COMPILER_INTEL) && !defined(__NOSSE__)
        #define FUX_SSE 1
    #elif defined(FUX_PROCESSOR_X86_64)
        // All x64 processors support SSE2 or higher
        #define FUX_SSE 2
    #else
        #define FUX_SSE 0
    #endif
#endif

// ------------------------------------------------------------------------
// We define separate defines for SSE support beyond SSE1.  These defines
// are particularly useful for detecting SSE4.x features since there isn't
// a single concept of SSE4.
//
// The following SSE defines are always defined.  0 indicates the
// feature/level of SSE is not supported, and 1 indicates support is
// available.
#ifndef FUX_SSE2
    #if FUX_SSE >= 2
        #define FUX_SSE2 1
    #else
        #define FUX_SSE2 0
    #endif
#endif
#ifndef FUX_SSE3
    #if FUX_SSE >= 3
        #define FUX_SSE3 1
    #else
        #define FUX_SSE3 0
    #endif
#endif
#ifndef FUX_SSSE3
    #if defined __SSSE3__ || defined FUX_PLATFORM_CAPILANO
        #define FUX_SSSE3 1
    #else
        #define FUX_SSSE3 0
    #endif
#endif
#ifndef FUX_SSE4_1
    #if defined __SSE4_1__ || defined FUX_PLATFORM_CAPILANO
        #define FUX_SSE4_1 1
    #else
        #define FUX_SSE4_1 0
    #endif
#endif
#ifndef FUX_SSE4_2
    #if defined __SSE4_2__ || defined FUX_PLATFORM_CAPILANO
        #define FUX_SSE4_2 1
    #else
        #define FUX_SSE4_2 0
    #endif
#endif
#ifndef FUX_SSE4A
    #if defined __SSE4A__ || defined FUX_PLATFORM_CAPILANO
        #define FUX_SSE4A 1
    #else
        #define FUX_SSE4A 0
    #endif
#endif

// ------------------------------------------------------------------------
// FUX_AVX
// FUX_AVX may be used to determine if Advanced Vector Extensions are available for the target architecture
//
// FUX_AVX defines the level of AVX support:
//  0 indicates no AVX support
//  1 indicates AVX1 is supported
//  2 indicates AVX2 is supported
#ifndef FUX_AVX
    #if defined __AVX2__
        #define FUX_AVX 2
    #elif defined __AVX__ || defined FUX_PLATFORM_CAPILANO
        #define FUX_AVX 1
    #else
        #define FUX_AVX 0
    #endif
#endif
#ifndef FUX_AVX2
    #if FUX_AVX >= 2
        #define FUX_AVX2 1
    #else
        #define FUX_AVX2 0
    #endif
#endif

// FUX_FP16C may be used to determine the existence of float <-> half conversion operations on an x86 CPU.
// (For example to determine if _mm_cvtph_ps or _mm_cvtps_ph could be used.)
#ifndef FUX_FP16C
    #if defined __F16C__ || defined FUX_PLATFORM_CAPILANO
        #define FUX_FP16C 1
    #else
        #define FUX_FP16C 0
    #endif
#endif

// FUX_FP128 may be used to determine if __float128 is a supported type for use. This type is enabled by a GCC extension (_GLIBCXX_USE_FLOAT128)
// but has support by some implementations of clang (__FLOAT128__)
// PS4 does not support __float128 as of SDK 5.500 https://ps4.siedev.net/resources/documents/SDK/5.500/CPU_Compiler_ABI-Overview/0003.html
#ifndef FUX_FP128
    #if (defined __FLOAT128__ || defined _GLIBCXX_USE_FLOAT128) && !defined(FUX_PLATFORM_KETTLE)
        #define FUX_FP128 1
    #else
        #define FUX_FP128 0
    #endif
#endif

// ------------------------------------------------------------------------
// FUX_ABM
// FUX_ABM may be used to determine if Advanced Bit Manipulation sets are available for the target architecture (POPCNT, LZCNT)
#ifndef FUX_ABM
    #if defined(__ABM__) || defined(FUX_PLATFORM_CAPILANO)
        #define FUX_ABM 1
    #else
        #define FUX_ABM 0
    #endif
#endif

// ------------------------------------------------------------------------
// FUX_NEON
// FUX_NEON may be used to determine if NEON is supported.
#ifndef FUX_NEON
    #if defined(__ARM_NEON__) || defined(__ARM_NEON)
        #define FUX_NEON 1
    #else
        #define FUX_NEON 0
    #endif
#endif

// ------------------------------------------------------------------------
// FUX_BMI
// FUX_BMI may be used to determine if Bit Manipulation Instruction sets are available for the target architecture
//
// FUX_BMI defines the level of BMI support:
//  0 indicates no BMI support
//  1 indicates BMI1 is supported
//  2 indicates BMI2 is supported
#ifndef FUX_BMI
    #if defined(__BMI2__)
        #define FUX_BMI 2
    #elif defined(__BMI__) || defined(FUX_PLATFORM_CAPILANO)
        #define FUX_BMI 1
    #else
        #define FUX_BMI 0
    #endif
#endif
#ifndef FUX_BMI2
    #if FUX_BMI >= 2
        #define FUX_BMI2 1
    #else
        #define FUX_BMI2 0
    #endif
#endif

// ------------------------------------------------------------------------
// FUX_FMA3
// FUX_FMA3 may be used to determine if Fused Multiply Add operations are available for the target architecture
// __FMA__ is defined only by GCC, Clang, and ICC; MSVC only defines __AVX__ and __AVX2__
// FMA3 was introduced alongside AVX2 on Intel Haswell
// All AMD processors support FMA3 if AVX2 is also supported
//
// FUX_FMA3 defines the level of FMA3 support:
//  0 indicates no FMA3 support
//  1 indicates FMA3 is supported
#ifndef FUX_FMA3
    #if defined(__FMA__) || FUX_AVX2 >= 1
        #define FUX_FMA3 1
    #else
        #define FUX_FMA3 0
    #endif
#endif

// ------------------------------------------------------------------------
// FUX_TBM
// FUX_TBM may be used to determine if Trailing Bit Manipulation instructions are available for the target architecture
#ifndef FUX_TBM
    #if defined(__TBM__)
        #define FUX_TBM 1
    #else
        #define FUX_TBM 0
    #endif
#endif


// ------------------------------------------------------------------------
// FUX_IMPORT
// import declaration specification
// specifies that the declared symbol is imported from another dynamic library.
#ifndef FUX_IMPORT
    #if defined(FUX_COMPILER_MSVC)
        #define FUX_IMPORT __declspec(dllimport)
    #else
        #define FUX_IMPORT
    #endif
#endif


// ------------------------------------------------------------------------
// FUX_EXPORT
// export declaration specification
// specifies that the declared symbol is exported from the current dynamic library.
// this is not the same as the C++ export keyword. The C++ export keyword has been
// removed from the language as of C++11.
#ifndef FUX_EXPORT
    #if defined(FUX_COMPILER_MSVC)
        #define FUX_EXPORT __declspec(dllexport)
    #else
        #define FUX_EXPORT
    #endif
#endif


// ------------------------------------------------------------------------
// FUX_PRAGMA_ONCE_SUPPORTED
//
// This is a wrapper for the #pragma once preprocessor directive.
// It allows for some compilers (in particular VC++) to implement signifcantly
// faster include file preprocessing. #pragma once can be used to replace
// header include guards or to augment them. However, #pragma once isn't
// necessarily supported by all compilers and isn't guaranteed to be so in
// the future, so using #pragma once to replace traditional include guards
// is not strictly portable. Note that a direct #define for #pragma once is
// impossible with VC++, due to limitations, but can be done with other
// compilers/preprocessors via _Pragma("once").
//
// Example usage (which includes traditional header guards for portability):
//    #ifndef SOMEPACKAGE_SOMEHEADER_H
//    #define SOMEPACKAGE_SOMEHEADER_H
//
//    #if defined(FUX_PRAGMA_ONCE_SUPPORTED)
//        #pragma once
//    #endif
//
//    <user code>
//
//    #endif
//
#if defined(_MSC_VER) || defined(__GNUC__) || defined(__EDG__) || defined(__APPLE__)
    #define FUX_PRAGMA_ONCE_SUPPORTED 1
#endif



// ------------------------------------------------------------------------
// FUX_ONCE
//
// Example usage (which includes traditional header guards for portability):
//    #ifndef SOMEPACKAGE_SOMEHEADER_H
//    #define SOMEPACKAGE_SOMEHEADER_H
//
//    FUX_ONCE()
//
//    <user code>
//
//    #endif
//
#if defined(FUX_PRAGMA_ONCE_SUPPORTED)
    #if defined(_MSC_VER)
        #define FUX_ONCE() __pragma(once)
    #else
        #define FUX_ONCE() // _Pragma("once")   It turns out that _Pragma("once") isn't supported by many compilers.
    #endif
#endif



// ------------------------------------------------------------------------
// FUX_OVERRIDE
//
// C++11 override
// See http://msdn.microsoft.com/en-us/library/jj678987.aspx for more information.
// You can use FUX_FINAL_OVERRIDE to combine usage of FUX_OVERRIDE and FUX_INHERITANCE_FINAL in a single statement.
//
// Example usage:
//        struct B     { virtual void f(int); };
//        struct D : B { void f(int) FUX_OVERRIDE; };
//
#ifndef FUX_OVERRIDE
    #if defined(FUX_COMPILER_NO_OVERRIDE)
        #define FUX_OVERRIDE
    #else
        #define FUX_OVERRIDE override
    #endif
#endif


// ------------------------------------------------------------------------
// FUX_INHERITANCE_FINAL
//
// Portably wraps the C++11 final specifier.
// See http://msdn.microsoft.com/en-us/library/jj678985.aspx for more information.
// You can use FUX_FINAL_OVERRIDE to combine usage of FUX_OVERRIDE and FUX_INHERITANCE_FINAL in a single statement.
// This is not called FUX_FINAL because that term is used within EA to denote debug/release/final builds.
//
// Example usage:
//     struct B { virtual void f() FUX_INHERITANCE_FINAL; };
//
#ifndef FUX_INHERITANCE_FINAL
    #if defined(FUX_COMPILER_NO_INHERITANCE_FINAL)
        #define FUX_INHERITANCE_FINAL
    #elif (defined(_MSC_VER) && (FUX_COMPILER_VERSION < 1700))  // Pre-VS2012
        #define FUX_INHERITANCE_FINAL sealed
    #else
        #define FUX_INHERITANCE_FINAL final
    #endif
#endif


// ------------------------------------------------------------------------
// FUX_FINAL_OVERRIDE
//
// Portably wraps the C++11 override final specifiers combined.
//
// Example usage:
//     struct A            { virtual void f(); };
//     struct B : public A { virtual void f() FUX_FINAL_OVERRIDE; };
//
#ifndef FUX_FINAL_OVERRIDE
    #define FUX_FINAL_OVERRIDE FUX_OVERRIDE FUX_INHERITANCE_FINAL
#endif


// ------------------------------------------------------------------------
// FUX_SEALED
//
// This is deprecated, as the C++11 Standard has final (FUX_INHERITANCE_FINAL) instead.
// See http://msdn.microsoft.com/en-us/library/0w2w91tf.aspx for more information.
// Example usage:
//     struct B { virtual void f() FUX_SEALED; };
//
#ifndef FUX_SEALED
    #if defined(FUX_COMPILER_MSVC) && (FUX_COMPILER_VERSION >= 1400) // VS2005 (VC8) and later
        #define FUX_SEALED sealed
    #else
        #define FUX_SEALED
    #endif
#endif


// ------------------------------------------------------------------------
// FUX_ABSTRACT
//
// This is a Microsoft language extension.
// See http://msdn.microsoft.com/en-us/library/b0z6b513.aspx for more information.
// Example usage:
//     struct X FUX_ABSTRACT { virtual void f(){} };
//
#ifndef FUX_ABSTRACT
    #if defined(FUX_COMPILER_MSVC) && (FUX_COMPILER_VERSION >= 1400) // VS2005 (VC8) and later
        #define FUX_ABSTRACT abstract
    #else
        #define FUX_ABSTRACT
    #endif
#endif


// ------------------------------------------------------------------------
// FUX_CONSTEXPR
// FUX_CONSTEXPR_OR_CONST
//
// Portable wrapper for C++11's 'constexpr' support.
//
// See http://www.cprogramming.com/c++11/c++11-compile-time-processing-with-constexpr.html for more information.
// Example usage:
//     FUX_CONSTEXPR int GetValue() { return 37; }
//     FUX_CONSTEXPR_OR_CONST double gValue = std::sin(kTwoPi);
//
#if !defined(FUX_CONSTEXPR)
    #if defined(FUX_COMPILER_NO_CONSTEXPR)
        #define FUX_CONSTEXPR
    #else
        #define FUX_CONSTEXPR constexpr
    #endif
#endif

#if !defined(FUX_CONSTEXPR_OR_CONST)
    #if defined(FUX_COMPILER_NO_CONSTEXPR)
        #define FUX_CONSTEXPR_OR_CONST const
    #else
        #define FUX_CONSTEXPR_OR_CONST constexpr
    #endif
#endif


// ------------------------------------------------------------------------
// FUX_EXTERN_TEMPLATE
//
// Portable wrapper for C++11's 'extern template' support.
//
// Example usage:
//     FUX_EXTERN_TEMPLATE(class basic_string<char>);
//
#if !defined(FUX_EXTERN_TEMPLATE)
    #if defined(FUX_COMPILER_NO_EXTERN_TEMPLATE)
        #define FUX_EXTERN_TEMPLATE(declaration)
    #else
        #define FUX_EXTERN_TEMPLATE(declaration) extern template declaration
    #endif
#endif


// ------------------------------------------------------------------------
// FUX_NOEXCEPT
// FUX_NOEXCEPT_IF(predicate)
// FUX_NOEXCEPT_EXPR(expression)
//
// Portable wrapper for C++11 noexcept
// http://en.cppreference.com/w/cpp/language/noexcept
// http://en.cppreference.com/w/cpp/language/noexcept_spec
//
// Example usage:
//     FUX_NOEXCEPT
//     FUX_NOEXCEPT_IF(predicate)
//     FUX_NOEXCEPT_EXPR(expression)
//
//     This function never throws an exception.
//     void DoNothing() FUX_NOEXCEPT
//         { }
//
//     This function throws an exception of T::T() throws an exception.
//     template <class T>
//     void DoNothing() FUX_NOEXCEPT_IF(FUX_NOEXCEPT_EXPR(T()))
//         { T t; }
//
#if !defined(FUX_NOEXCEPT)
    #if defined(FUX_COMPILER_NO_NOEXCEPT)
        #define FUX_NOEXCEPT
        #define FUX_NOEXCEPT_IF(predicate)
        #define FUX_NOEXCEPT_EXPR(expression) false
    #else
        #define FUX_NOEXCEPT noexcept
        #define FUX_NOEXCEPT_IF(predicate) noexcept((predicate))
        #define FUX_NOEXCEPT_EXPR(expression) noexcept((expression))
    #endif
#endif


// ------------------------------------------------------------------------
// FUX_NORETURN
//
// Wraps the C++11 noreturn attribute. See FUX_COMPILER_NO_NORETURN
// http://en.cppreference.com/w/cpp/language/attributes
// http://msdn.microsoft.com/en-us/library/k6ktzx3s%28v=vs.80%29.aspx
// http://blog.aaronballman.com/2011/09/understanding-attributes/
//
// Example usage:
//     FUX_NORETURN void SomeFunction()
//         { throw "error"; }
//
#if !defined(FUX_NORETURN)
    #if defined(FUX_COMPILER_MSVC) && (FUX_COMPILER_VERSION >= 1300) // VS2003 (VC7) and later
        #define FUX_NORETURN __declspec(noreturn)
    #elif defined(FUX_COMPILER_NO_NORETURN)
        #define FUX_NORETURN
    #else
        #define FUX_NORETURN [[noreturn]]
    #endif
#endif


// ------------------------------------------------------------------------
// FUX_CARRIES_DEPENDENCY
//
// Wraps the C++11 carries_dependency attribute
// http://en.cppreference.com/w/cpp/language/attributes
// http://blog.aaronballman.com/2011/09/understanding-attributes/
//
// Example usage:
//     FUX_CARRIES_DEPENDENCY int* SomeFunction()
//         { return &mX; }
//
//
#if !defined(FUX_CARRIES_DEPENDENCY)
    #if defined(FUX_COMPILER_NO_CARRIES_DEPENDENCY)
        #define FUX_CARRIES_DEPENDENCY
    #else
        #define FUX_CARRIES_DEPENDENCY [[carries_dependency]]
    #endif
#endif


// ------------------------------------------------------------------------
// FUX_FALLTHROUGH
//
// [[fallthrough] is a C++17 standard attribute that appears in switch
// statements to indicate that the fallthrough from the previous case in the
// switch statement is intentially and not a bug.
//
// http://en.cppreference.com/w/cpp/language/attributes
//
// Example usage:
// 		void f(int n)
// 		{
// 			switch(n)
// 			{
// 				case 1:
// 				DoCase1();
// 				// Compiler may generate a warning for fallthrough behaviour
//
// 				case 2:
// 				DoCase2();
//
// 				FUX_FALLTHROUGH;
// 				case 3:
// 				DoCase3();
// 			}
// 		}
//
#if !defined(FUX_FALLTHROUGH)
    #if defined(FUX_COMPILER_NO_FALLTHROUGH)
        #define FUX_FALLTHROUGH
    #else
        #define FUX_FALLTHROUGH [[fallthrough]]
    #endif
#endif



// ------------------------------------------------------------------------
// FUX_NODISCARD
//
// [[nodiscard]] is a C++17 standard attribute that can be applied to a
// function declaration, enum, or class declaration.  If a any of the list
// previously are returned from a function (without the user explicitly
// casting to void) the addition of the [[nodiscard]] attribute encourages
// the compiler to generate a warning about the user discarding the return
// value. This is a useful practice to encourage client code to check API
// error codes.
//
// http://en.cppreference.com/w/cpp/language/attributes
//
// Example usage:
//
//     FUX_NODISCARD int baz() { return 42; }
//
//     void foo()
//     {
//         baz(); // warning: ignoring return value of function declared with 'nodiscard' attribute
//     }
//
#if !defined(FUX_NODISCARD)
    #if defined(FUX_COMPILER_NO_NODISCARD)
        #define FUX_NODISCARD
    #else
        #define FUX_NODISCARD [[nodiscard]]
    #endif
#endif


// ------------------------------------------------------------------------
// FUX_MAYBE_UNUSED
//
// [[maybe_unused]] is a C++17 standard attribute that suppresses warnings
// on unused entities that are declared as maybe_unused.
//
// http://en.cppreference.com/w/cpp/language/attributes
//
// Example usage:
//    void foo(FUX_MAYBE_UNUSED int i)
//    {
//        assert(i == 42);  // warning suppressed when asserts disabled.
//    }
//
#if !defined(FUX_MAYBE_UNUSED)
    #if defined(FUX_COMPILER_NO_MAYBE_UNUSED)
        #define FUX_MAYBE_UNUSED
    #else
        #define FUX_MAYBE_UNUSED [[maybe_unused]]
    #endif
#endif


// ------------------------------------------------------------------------
// FUX_NO_UBSAN
//
// The LLVM/Clang undefined behaviour sanitizer will not analyse a function tagged with the following attribute.
//
// https://clang.llvm.org/docs/UndefinedBehaviorSanitizer.html#disabling-instrumentation-with-attribute-no-sanitize-undefined
//
// Example usage:
//     FUX_NO_UBSAN int SomeFunction() { ... }
//
#ifndef FUX_NO_UBSAN
    #if defined(FUX_COMPILER_CLANG)
        #define FUX_NO_UBSAN __attribute__((no_sanitize("undefined")))
    #else
        #define FUX_NO_UBSAN
    #endif
#endif


// ------------------------------------------------------------------------
// FUX_NO_ASAN
//
// The LLVM/Clang address sanitizer will not analyse a function tagged with the following attribute.
//
// https://clang.llvm.org/docs/AddressSanitizer.html#disabling-instrumentation-with-attribute-no-sanitize-address
//
// Example usage:
//     FUX_NO_ASAN int SomeFunction() { ... }
//
#ifndef FUX_NO_ASAN
    #if defined(FUX_COMPILER_CLANG)
        #define FUX_NO_ASAN __attribute__((no_sanitize("address")))
    #else
        #define FUX_NO_ASAN
    #endif
#endif


// ------------------------------------------------------------------------
// FUX_ASAN_ENABLED
//
// Defined as 0 or 1. It's value depends on the compile environment.
// Specifies whether the code is being built with Clang's Address Sanitizer.
//
#if defined(__has_feature)
    #if __has_feature(address_sanitizer)
        #define FUX_ASAN_ENABLED 1
    #else
        #define FUX_ASAN_ENABLED 0
    #endif
#else
    #define FUX_ASAN_ENABLED 0
#endif


// ------------------------------------------------------------------------
// FUX_NON_COPYABLE
//
// This macro defines as a class as not being copy-constructable
// or assignable. This is useful for preventing class instances
// from being passed to functions by value, is useful for preventing
// compiler warnings by some compilers about the inability to
// auto-generate a copy constructor and assignment, and is useful
// for simply declaring in the interface that copy semantics are
// not supported by the class. Your class needs to have at least a
// default constructor when using this macro.
//
// Beware that this class works by declaring a private: section of
// the class in the case of compilers that don't support C++11 deleted
// functions.
//
// Note: With some pre-C++11 compilers (e.g. Green Hills), you may need
//       to manually define an instances of the hidden functions, even
//       though they are not used.
//
// Example usage:
//    class Widget {
//       Widget();
//       . . .
//       FUX_NON_COPYABLE(Widget)
//    };
//
#if !defined(FUX_NON_COPYABLE)
#if defined(FUX_COMPILER_NO_DELETED_FUNCTIONS)
#define FUX_NON_COPYABLE(klass)               \
    private:                                      \
    FUX_DISABLE_VC_WARNING(4822);	/* local class member function does not have a body	*/		\
    klass(const klass&);                  \
    void operator=(const klass&);			\
    FUX_RESTORE_VC_WARNING();
#else
#define FUX_NON_COPYABLE(klass)               \
    FUX_DISABLE_VC_WARNING(4822);	/* local class member function does not have a body	*/		\
    klass(const klass&) = delete;         \
    void operator=(const klass&) = delete;	\
    FUX_RESTORE_VC_WARNING();
#endif
#endif


// ------------------------------------------------------------------------
// FUX_FN_DELETE
//
// Semi-portable way of specifying a deleted function which allows for
// cleaner code in class declarations.
//
// Example usage:
//
//  class Example
//  {
//  private: // For portability with pre-C++11 compilers, make the function private.
//      void foo() FUX_FN_DELETE;
//  };
//
// Note: FUX_FN_DELETE'd functions should be private to prevent the
// functions from being called even when the compiler does not support
// deleted functions. Some compilers (e.g. Green Hills) that don't support
// C++11 deleted functions can require that you define the function,
// which you can do in the associated source file for the class.
//
#if defined(FUX_COMPILER_NO_DELETED_FUNCTIONS)
    #define FUX_FN_DELETE
#else
    #define FUX_FN_DELETE = delete
#endif

// ------------------------------------------------------------------------
// FUX_DISABLE_DEFAULT_CTOR
//
// Disables the compiler generated default constructor. This macro is
// provided to improve portability and clarify intent of code.
//
// Example usage:
//
//  class Example
//  {
//  private:
//      FUX_DISABLE_DEFAULT_CTOR(Example);
//  };
//
#define FUX_DISABLE_DEFAULT_CTOR(ClassName) ClassName() FUX_FN_DELETE

// ------------------------------------------------------------------------
// FUX_DISABLE_COPY_CTOR
//
// Disables the compiler generated copy constructor. This macro is
// provided to improve portability and clarify intent of code.
//
// Example usage:
//
//  class Example
//  {
//  private:
//      FUX_DISABLE_COPY_CTOR(Example);
//  };
//
#define FUX_DISABLE_COPY_CTOR(ClassName) ClassName(const ClassName &) FUX_FN_DELETE

// ------------------------------------------------------------------------
// FUX_DISABLE_MOVE_CTOR
//
// Disables the compiler generated move constructor. This macro is
// provided to improve portability and clarify intent of code.
//
// Example usage:
//
//  class Example
//  {
//  private:
//      FUX_DISABLE_MOVE_CTOR(Example);
//  };
//
#define FUX_DISABLE_MOVE_CTOR(ClassName) ClassName(ClassName&&) FUX_FN_DELETE

// ------------------------------------------------------------------------
// FUX_DISABLE_ASSIGNMENT_OPERATOR
//
// Disables the compiler generated assignment operator. This macro is
// provided to improve portability and clarify intent of code.
//
// Example usage:
//
//  class Example
//  {
//  private:
//      FUX_DISABLE_ASSIGNMENT_OPERATOR(Example);
//  };
//
#define FUX_DISABLE_ASSIGNMENT_OPERATOR(ClassName) ClassName & operator=(const ClassName &) FUX_FN_DELETE

// ------------------------------------------------------------------------
// FUX_DISABLE_MOVE_OPERATOR
//
// Disables the compiler generated move operator. This macro is
// provided to improve portability and clarify intent of code.
//
// Example usage:
//
//  class Example
//  {
//  private:
//      FUX_DISABLE_MOVE_OPERATOR(Example);
//  };
//
#define FUX_DISABLE_MOVE_OPERATOR(ClassName) ClassName & operator=(ClassName&&) FUX_FN_DELETE

// ------------------------------------------------------------------------
// nocopy_t
//
// Declares a class as not supporting copy construction or assignment.
// May be more reliable with some situations that FUX_NON_COPYABLE alone,
// though it may result in more code generation.
//
// Note that VC++ will generate warning C4625 and C4626 if you use nocopy_t
// and you are compiling with /W4 and /Wall. There is no resolution but
// to redelare FUX_NON_COPYABLE in your subclass or disable the warnings with
// code like this:
//     FUX_DISABLE_VC_WARNING(4625 4626)
//     ...
//     FUX_RESTORE_VC_WARNING()
//
// Example usage:
//     struct Widget : nocopy_t {
//        . . .
//     };
//
#ifdef __cplusplus
struct nocopy_t {
    #if defined(FUX_COMPILER_NO_DEFAULTED_FUNCTIONS) || defined(__EDG__) // EDG doesn't appear to behave properly for the case of defaulted constructors; it generates a mistaken warning about missing default constructors.
    nocopy_t() {} // Putting {} here has the downside that it allows a class to create itself,
    ~nocopy_t() {} // but avoids linker errors that can occur with some compilers (e.g. Green Hills).
    #else
    nocopy_t() = default;
    ~nocopy_t() = default;
    #endif
    FUX_NON_COPYABLE( nocopy_t )
};
#endif


// ------------------------------------------------------------------------
// FUX_OPTIMIZE_OFF / FUX_OPTIMIZE_ON
//
// Implements portable inline optimization enabling/disabling.
// Usage of these macros must be in order OFF then ON. This is
// because the OFF macro pushes a set of settings and the ON
// macro pops them. The nesting of OFF/ON sets (e.g. OFF, OFF, ON, ON)
// is not guaranteed to work on all platforms.
//
// This is often used to allow debugging of some code that's
// otherwise compiled with undebuggable optimizations. It's also
// useful for working around compiler code generation problems
// that occur in optimized builds.
//
// Some compilers (e.g. VC++) don't allow doing this within a function and
// so the usage must be outside a function, as with the example below.
// GCC on x86 appears to have some problem with argument passing when
// using FUX_OPTIMIZE_OFF in optimized builds.
//
// Example usage:
//     // Disable optimizations for SomeFunction.
//     FUX_OPTIMIZE_OFF()
//     void SomeFunction()
//     {
//         ...
//     }
//     FUX_OPTIMIZE_ON()
//
#if !defined(FUX_OPTIMIZE_OFF)
#if   defined(FUX_COMPILER_MSVC)
#define FUX_OPTIMIZE_OFF() __pragma(optimize("", off))
#elif defined(FUX_COMPILER_GNUC) && (FUX_COMPILER_VERSION > 4004) && (defined(__i386__) || defined(__x86_64__)) // GCC 4.4+ - Seems to work only on x86/Linux so far. However, GCC 4.4 itself appears broken and screws up parameter passing conventions.
#define FUX_OPTIMIZE_OFF()            \
    _Pragma("GCC push_options")      \
    _Pragma("GCC optimize 0")
#elif defined(FUX_COMPILER_CLANG) && (!defined(FUX_PLATFORM_ANDROID) || (FUX_COMPILER_VERSION >= 380))
#define FUX_OPTIMIZE_OFF() \
    FUX_DISABLE_CLANG_WARNING(-Wunknown-pragmas) \
    _Pragma("clang optimize off") \
    FUX_RESTORE_CLANG_WARNING()
#else
#define FUX_OPTIMIZE_OFF()
#endif
#endif

#if !defined(FUX_OPTIMIZE_ON)
#if   defined(FUX_COMPILER_MSVC)
#define FUX_OPTIMIZE_ON() __pragma(optimize("", on))
#elif defined(FUX_COMPILER_GNUC) && (FUX_COMPILER_VERSION > 4004) && (defined(__i386__) || defined(__x86_64__)) // GCC 4.4+ - Seems to work only on x86/Linux so far. However, GCC 4.4 itself appears broken and screws up parameter passing conventions.
#define FUX_OPTIMIZE_ON() _Pragma("GCC pop_options")
#elif defined(FUX_COMPILER_CLANG) && (!defined(FUX_PLATFORM_ANDROID) || (FUX_COMPILER_VERSION >= 380))
#define FUX_OPTIMIZE_ON() \
    FUX_DISABLE_CLANG_WARNING(-Wunknown-pragmas) \
    _Pragma("clang optimize on") \
    FUX_RESTORE_CLANG_WARNING()
#else
#define FUX_OPTIMIZE_ON()
#endif
#endif



#endif // Header include guard











