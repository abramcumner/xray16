#include "stdafx.h"
#include "cpuid.h"
#include <intrin.h>

int _cpuid(_processor_info* pinfo)
{
    _processor_info& P = *pinfo;
    ZeroMemory(&P, sizeof(_processor_info));

    int cpinfo[4];
    // detect cpu vendor
    __cpuid(cpinfo, 0);

    memcpy(P.v_name, &(cpinfo[1]), sizeof(int));
    memcpy(P.v_name + sizeof(int), &(cpinfo[3]), sizeof(int));
    memcpy(P.v_name + 2 * sizeof(int), &(cpinfo[2]), sizeof(int));

    // detect cpu model
    __cpuid(cpinfo, 0x80000002);
    memcpy(P.model_name, cpinfo, sizeof(cpinfo));
    __cpuid(cpinfo, 0x80000003);
    memcpy(P.model_name + 16, cpinfo, sizeof(cpinfo));
    __cpuid(cpinfo, 0x80000004);
    memcpy(P.model_name + 32, cpinfo, sizeof(cpinfo));

    // detect cpu main features
    __cpuid(cpinfo, 1);

    P.stepping = cpinfo[0] & 0xf;
    P.model = (u8)((cpinfo[0] >> 4) & 0xf) | ((u8)((cpinfo[0] >> 16) & 0xf) << 4);
    P.family = (u8)((cpinfo[0] >> 8) & 0xf) | ((u8)((cpinfo[0] >> 20) & 0xff) << 4);

    if (cpinfo[3] & (1 << 23))
        P.feature |= _CPU_FEATURE_MMX;
    if (cpinfo[3] & (1 << 25))
        P.feature |= _CPU_FEATURE_SSE;
    if (cpinfo[3] & (1 << 26))
        P.feature |= _CPU_FEATURE_SSE2;
    if (cpinfo[2] & 0x1)
        P.feature |= _CPU_FEATURE_SSE3;
    if (cpinfo[2] & (1 << 19))
        P.feature |= _CPU_FEATURE_SSE41;
    if (cpinfo[2] & (1 << 20))
        P.feature |= _CPU_FEATURE_SSE42;
    if (cpinfo[2] & (1 << 9))
        P.feature |= _CPU_FEATURE_SSSE3;
    if (cpinfo[2] & (1 << 3))
        P.feature |= _CPU_FEATURE_MWAIT;

    // and check 3DNow! support
    __cpuid(cpinfo, 0x80000001);
    if (cpinfo[3] & (1 << 31))
        P.feature |= _CPU_FEATURE_3DNOW;

    P.os_support = P.feature;

    // get version of OS
    DWORD dwMajorVersion = 0;
    DWORD dwVersion = 0;
    dwVersion = GetVersion();

    dwMajorVersion = (DWORD)(LOBYTE(LOWORD(dwVersion)));

    if (dwMajorVersion <= 5) // XP don't support SSE3+ instruction sets
    {
        P.os_support &= ~_CPU_FEATURE_SSE3;
        P.os_support &= ~_CPU_FEATURE_SSE41;
        P.os_support &= ~_CPU_FEATURE_SSE42;
    }

    return P.feature;
}