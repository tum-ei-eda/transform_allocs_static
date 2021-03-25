#ifndef MALLOC_HOOK_H_
#define MALLOC_HOOK_H_
#include <map>
#define MAX_PTRS_SUPPORTED 200         // TO BE ASSIGNED.
#define MAX_ALLOCATION_SUPPORTED 1000  // TO BE ASSIGNED.

#define _STATIC_MALLOC  // Comment or uncomment this to change the mode.

// Used by GLOBAL

/*StatIDMapper struct to store .second data */
struct id_lifetime {
  size_t size;
  int firstAllocated;
  int lastFreed;
  void* ptr;
  id_lifetime(size_t _size, int _firstAllocated, void* _ptr)
      : size(_size),
        firstAllocated(_firstAllocated),
        lastFreed(-1),
        ptr(_ptr){};
};

// Contains necessary information about Buffer bindings.
struct GLOBAL {
  int counter = 0;  // A timer-like variable to store allocs and frees
  std::map<int, id_lifetime>
      StatIDMapper;  // Id Lifetime mapper for memory optimization-
};                   // Global {ID, {size,firstAllocated,lastFree,ptr}}

extern GLOBAL global;
// Counter for malloc and free .
int _malloc_counter = 0;  // Counter for malloc calls to read databse
void* _offset_base;       // BigMalloc return value

/* StatIDMapper counters*/
int map_id_counter = 0;
#endif