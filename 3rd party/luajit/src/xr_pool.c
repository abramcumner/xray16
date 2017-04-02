#include "xr_pool.h"
#include "lj_def.h"
#include "lj_arch.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#define MAX_SIZE_T		(~(size_t)0)
#define MFAIL			((void *)(MAX_SIZE_T))

static int inited = 0;

#define DEBUG_MEM
#ifdef DEBUG_MEM
static char buf[100];
#endif

HANDLE heap;

void XR_INIT()
{
	if (inited)
		return;
	inited = 1;
	
	heap = HeapCreate(0, 256 * 1024 * 1024, 0);
#ifdef DEBUG_MEM
	sprintf(buf, "XR_INIT HeapCreate = %p\r\n", heap);
	OutputDebugString(buf);
#endif	
}

void* XR_MMAP(size_t size)
{
	void* ptr = HeapAlloc(heap, 0, size);
#ifdef DEBUG_MEM
	sprintf(buf, "XR_MMAP(%Iu) = %p\r\n", size, ptr);
	OutputDebugString(buf);
#endif
	if (ptr == NULL)
		ptr = MFAIL;
	if (ptr >= 2ull * 1024 * 1024 * 1024)
		ptr = MFAIL;
	return ptr;
}

// Судя по комментарию к CALL_MUNMAP, луаджит может объединять выделенные ему чанки
// и освобождать их как один. Надеюсь он не слепит вместе чанки из разных пулов
void XR_DESTROY(void* ptr, size_t size)
{
#ifdef DEBUG_MEM
	sprintf(buf, "XR_DESTROY(ptr=%p, size=%Iu) ", ptr, size);
	OutputDebugString(buf);
#endif
	while (1) {
		size_t blockSize = HeapSize(heap, 0, ptr);
		HeapFree(heap, 0, ptr);
#ifdef DEBUG_MEM			
		sprintf(buf, "XR_DESTROY: destroy ptr=%p, size=%Iu\r\n", ptr, blockSize);
		OutputDebugString(buf);					
#endif
		if (blockSize == size)
			break;
		size -= blockSize;
		ptr = (char*)ptr + blockSize;
	}
}