#ifndef XRMEMORY_PURE_H
#define XRMEMORY_PURE_H

#ifdef XRCORE_STATIC_OLD
#	define PURE_ALLOC
#else // XRCORE_STATIC_OLD
#	ifdef DEBUG
#		define PURE_ALLOC
#	endif // DEBUG
#endif // XRCORE_STATIC_OLD

#endif // XRMEMORY_PURE_H