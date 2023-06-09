#ifndef DO_H
#define DO_H

#include <cstdint>

inline __attribute__((always_inline)) int CMOV(bool P, int A, int B)
{
    return (-P & A) + ((P - 1) & B);
}

inline __attribute__((always_inline)) unsigned int CMOV(bool P, unsigned int A, unsigned int B)
{
    return (-P & A) + ((P - 1) & B);
}

inline __attribute__((always_inline)) unsigned int CMOV(bool P, uint64_t A, uint64_t B)
{
    return (-P & A) + ((P - 1) & B);
}

inline __attribute__((always_inline)) double CMOV(bool P, double A, double B)
{
    uint64_t T = *((uint64_t *)&A);
    uint64_t F = *((uint64_t *)&B);

    uint64_t value = (-P & T) + ((P - 1) & F);

    return *((double *)&value);
}

template <class T>
inline __attribute__((always_inline)) T CMOV_X86_template(bool cond, T v_true, T v_false)
{
    T result;
    __asm__ __volatile__(
        "test   %1, %1;"
        "cmovz  %3, %0;" // zero flag cleared?
        "cmovnz %2, %0;" // zero flag not cleared?
        : "=r"(result)
        : "r"(cond), "r"(v_true), "r"(v_false)
        : "cc");
    return result;
}

inline __attribute__((always_inline)) uint16_t CMOV_X86(bool cond, uint16_t v_true, uint16_t v_false)
{
    uint16_t result;
    __asm__ __volatile__(
        "test   %1, %1;"
        "cmovz  %3, %0;" // zero flag cleared?
        "cmovnz %2, %0;" // zero flag not cleared?
        : "=r"(result)
        : "r"(cond), "r"(v_true), "r"(v_false)
        : "cc");
    return char(result);
}

inline __attribute__((always_inline)) int CMOV_X86(bool cond, int v_true, int v_false)
{
    return CMOV_X86_template<int>(cond, v_true, v_false);
}

inline __attribute__((always_inline)) unsigned int CMOV_X86(bool cond, unsigned int v_true, unsigned int v_false)
{
    return CMOV_X86_template<unsigned int>(cond, v_true, v_false);
}

inline __attribute__((always_inline)) unsigned long CMOV_X86(bool cond, unsigned long v_true, unsigned long v_false)
{
    return CMOV_X86_template<unsigned long>(cond, v_true, v_false);
}

inline __attribute__((always_inline)) double CMOV_X86(bool cond, double v_true, double v_false)
{
    return CMOV_X86_template<double>(cond, v_true, v_false);
}

inline __attribute__((always_inline)) float CMOV_X86(bool cond, float v_true, float v_false)
{
    return CMOV_X86_template<float>(cond, v_true, v_false);
}

inline __attribute__((always_inline)) bool CMOV_X86(bool cond, bool v_true, bool v_false)
{
    uint16_t result;
    __asm__ __volatile__(
        "test   %1, %1;"
        "cmovz  %3, %0;" // zero flag cleared?
        "cmovnz %2, %0;" // zero flag not cleared?
        : "=r"(result)
        : "r"(cond), "r"((uint16_t)v_true), "r"((uint16_t)v_false)
        : "cc");
    return bool(result);
}

#endif