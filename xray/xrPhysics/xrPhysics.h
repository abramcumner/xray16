#pragma once

#if defined XRPHYSICS_STATIC
#  define XRPHYSICS_API
#elif defined XRPHYSICS_EXPORTS
#define XRPHYSICS_API __declspec(dllexport)
#else
#define XRPHYSICS_API __declspec(dllimport)
#endif

