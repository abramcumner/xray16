#include "xr_pool.h"
#include "lj_def.h"
#include "lj_arch.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

typedef long (*PNTAVM)(HANDLE handle, void **addr, ULONG zbits,
		       size_t *size, ULONG alloctype, ULONG prot);
extern PNTAVM ntavm;
/* Number of top bits of the lower 32 bits of an address that must be zero.
** Apparently 0 gives us full 64 bit addresses and 1 gives us the lower 2GB.
*/
#define NTAVM_ZEROBITS		1

#define MAX_SIZE_T		(~(size_t)0)
#define MFAIL			((void *)(MAX_SIZE_T))

struct PoolSS
{
	void* list;
	int chunk_size;
	void* first;
	void* last;
	int count;
};
void* create(struct PoolSS* pool);
void destroy(struct PoolSS* pool, void* P);
void create_block(struct PoolSS* pool);

// Луаджит вроде как выделяет память кусками по 128К и 256К
// Поэтому сделаю два пула по эти размеры
#define CHUNK_SIZE (128 * 1024)
#define CHUNK_SIZE_2 (2 * CHUNK_SIZE)
static struct PoolSS pools[2] = {{0, CHUNK_SIZE, 0, 0, 0}, {0, CHUNK_SIZE_2, 0, 0, 0}};
static int inited = 0;

//#define DEBUG_MEM
#ifdef DEBUG_MEM
static char buf[100];
#endif

void XR_INIT()
{
	if (inited)
		return;
	create_block(pools);
	create_block(pools + 1);
	inited = 1;
}

void* XR_MMAP(size_t size)
{
#ifdef DEBUG_MEM
	sprintf(buf, "XR_MMAP(%Iu)", size);
	OutputDebugString(buf);
#endif
	int index = size / CHUNK_SIZE - 1;
	void* ptr = create(pools + index);
#ifdef DEBUG_MEM	
	sprintf(buf, " ptr=%p pool %d\r\n", ptr, index);
	OutputDebugString(buf);
#endif
	return ptr;
}

// Судя по комментарию к CALL_MUNMAP, луаджит может объединять выделенные ему чанки
// и освобождать их как один. Надеюсь он не слепит вместе чанки из разных пулов
void XR_DESTROY(void* ptr, size_t size)
{
#ifdef DEBUG_MEM
	sprintf(buf, "XR_DESTROY(ptr=%p, size=%Iu)", ptr, size);
	OutputDebugString(buf);
#endif
	for (int i = 0; i < 2; i++) {
		if (ptr < pools[i].first || pools[i].last <= ptr)
			continue;
#ifdef DEBUG_MEM			
		sprintf(buf, " pool %d\r\n", i);
		OutputDebugString(buf);
#endif
		while(size) {
			destroy(pools + i, ptr);
#ifdef DEBUG_MEM			
			sprintf(buf, "XR_DESTROY: destroy ptr=%p, size=%Iu\r\n", ptr, size);
			OutputDebugString(buf);					
#endif
			size -= pools[i].chunk_size;
			ptr = (char*)ptr + pools[i].chunk_size;
		}
		return;
	}
#ifdef DEBUG_MEM	
	sprintf(buf, "XR_DESTROY FATAL ERROR!\r\n");
	OutputDebugString(buf);	
#endif
}

void** access(void* p)
{
	return (void**)(void*)(p);
}

void create_block(struct PoolSS* pool)
{
	void *ptr = NULL;
	size_t size = 128 * 1024 * 1024;
	long st = ntavm(INVALID_HANDLE_VALUE, &ptr, NTAVM_ZEROBITS, &size,
	                MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);
	pool->list = ptr;
	pool->first = ptr;
	pool->last = (char*)ptr + size;
#ifdef DEBUG_MEM	
	sprintf(buf, "XR_INIT create_block %p-%p pool %d result=%X\r\n", pool->first, pool->last, pool->chunk_size / CHUNK_SIZE - 1, st);
	OutputDebugString(buf);		
#endif
	
	const int count = size / pool->chunk_size;
	for (int it = 0; it < count - 1; it++) {
		void* E	= (char*)pool->list + it * pool->chunk_size;
		*access(E) = (char*)E + pool->chunk_size;
	}
	*access((char*)pool->list + (count - 1) * pool->chunk_size) = NULL;
}

void* create(struct PoolSS* pool)
{
	if (NULL == pool->list)
		return MFAIL;

	void* E = pool->list;
	pool->list = *access(pool->list);
	pool->count++;
	return E;
}

void destroy(struct PoolSS* pool, void* P)
{
	*access(P) = pool->list;
	pool->list = P;
	pool->count--;
	
// ни в коем случае не освобождать, а то потом можно и не выделить уже
//	if (pool->count == 0) {
//		VirtualFree(pool->first, 0, MEM_RELEASE);
//		pool->list = 0;
//#ifdef DEBUG_MEM		
//		sprintf(buf, "XR_DONE free_block %p  pool %d\r\n", pool->first, pool->chunk_size / CHUNK_SIZE - 1);
//		OutputDebugString(buf);			
//#endif
//	}
}