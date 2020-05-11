#ifndef __P2DEFS_H__
#define __P2DEFS_H__

#include <stdio.h>

//  NULL just in case ----------------------

#ifdef NULL
#undef NULL
#endif
#define NULL  0

// Deletes a buffer
#define RELEASE( x ) \
    {                        \
    if( x != NULL )        \
	    {                      \
      delete x;            \
	  x = NULL;              \
	    }                      \
    }

// Deletes an array of buffers
#define RELEASE_ARRAY( x ) \
    {                              \
    if( x != NULL )              \
	    {                            \
      delete[] x;                \
	  x = NULL;                    \
	    }                            \
                              \
    }

#define IN_RANGE( value, min, max ) ( ((value) >= (min) && (value) <= (max)) ? 1 : 0 )
#define MIN( a, b ) ( ((a) < (b)) ? (a) : (b) )
#define MAX( a, b ) ( ((a) > (b)) ? (a) : (b) )
#define TO_BOOL( a )  ( (a != 0) ? true : false )

typedef signed char schar;
typedef unsigned char uchar;
typedef signed int sint;
typedef unsigned int uint;
typedef unsigned __int32 uint32;
typedef unsigned __int64 uint64;

template <class VALUE_TYPE> void SWAP(VALUE_TYPE& a, VALUE_TYPE& b)
{
	VALUE_TYPE tmp = a;
	a = b;
	b = tmp;
}

/*template <class VALUE_TYPE> void FLIP(VALUE_TYPE& a)					//Alternative method to flip an array/vector. WIP
{
	VALUE_TYPE* start = &a[0];
	VALUE_TYPE* end = &a[num_elements - 1];

	while (start < end)
		SWAP(*start++, *end--);
}*/

// Standard string size
#define SHORT_STR	32
#define MID_STR		255
#define HUGE_STR	8192

// Joins a path and file
inline const char* const PATH(const char* folder, const char* file)
{
	static char path[MID_STR];
	sprintf_s(path, MID_STR, "%s/%s", folder, file);
	return path;
}

// Performance macros
#define PERF_START(timer) timer.Start();
#define PERF_PEEK(timer) LOG("%s took %f ms", __FUNCTION__, timer.ReadMs())

// Own Definitions
#define CAP_AT_30 30
#define CAP_AT_60 60
#define CAP_AT_120 120

#define ALPHA 80

#define MAX_UNIT_HEALTHBAR_WIDTH 66
#define MAX_BUILDING_HEALTHBAR_WIDTH 115
#define MAX_RESOURCE_HEALTHBAR_WIDTH 115

#define MAX_CREATION_BAR_WIDTH 115

#endif