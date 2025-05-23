#ifndef DEBUG_ARENA_H
#define DEBUG_ARENA_H

#include "ultra64/ultratypes.h"

void* DebugArena_Malloc(u32 size);
void* DebugArena_MallocR(u32 size);
void* DebugArena_Realloc(void* ptr, u32 newSize);
void DebugArena_Free(void* ptr);
void* DebugArena_Calloc(u32 num, u32 size);
void DebugArena_GetSizes(u32* outMaxFree, u32* outFree, u32* outAlloc);
void DebugArena_Check(void);
void DebugArena_Init(void* start, u32 size);
void DebugArena_Cleanup(void);
s32 DebugArena_IsInitialized(void);

#if IS_DEBUG_HEAP_ENABLED

#define DEBUG_ARENA_MALLOC(size, ...) DebugArena_MallocDebug(size,  __FILE__, __LINE__)
#define DEBUG_ARENA_MALLOC_R(size, ...) DebugArena_MallocRDebug(size,  __FILE__, __LINE__)
#define DEBUG_ARENA_FREE(size, ...) DebugArena_FreeDebug(size,  __FILE__, __LINE__)

void DebugArena_CheckPointer(void* ptr, u32 size, const char* name, const char* action);
void* DebugArena_MallocDebug(u32 size, const char* file, int line);
void* DebugArena_MallocRDebug(u32 size, const char* file, int line);
void* DebugArena_ReallocDebug(void* ptr, u32 newSize, const char* file, int line);
void DebugArena_FreeDebug(void* ptr, const char* file, int line);
void DebugArena_Display(void);

#else

#define DEBUG_ARENA_MALLOC(size, ...) DebugArena_Malloc(size)
#define DEBUG_ARENA_MALLOC_R(size, ...) DebugArena_MallocR(size)
#define DEBUG_ARENA_FREE(size, ...) DebugArena_Free(size)

#endif

#endif
