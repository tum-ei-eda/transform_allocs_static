#ifndef MALLOC_HOOK_H_
#define MALLOC_HOOK_H_
#include <map>
#include "malloc_array.h"
#define MAX_PTRS_SUPPORTED 200         // TO BE ASSIGNED.
#define MAX_ALLOCATION_SUPPORTED 1000  // TO BE ASSIGNED.

//#define _STATIC

int id_counter = 0;

// Used by GLOBAL

/*StatBufMapper struct to store .second data */
struct ptr_id {
  size_t size;
  int firstUsed;
  int lastUsed;
  int use;
  int id;
  ptr_id(size_t _size, int _firstUsed)
      : size(_size),
        firstUsed(_firstUsed),
        lastUsed(-1),
        use(0),
        id(id_counter++){};
};

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
  std::map<void*, ptr_id> StatBufMapper;  // {ADDRESS,{size,firstUsed,lastUsed}}
  int counter = 0;  // A timer-like variable to store allocs and frees
  int id_malloc_call_order[MAX_ALLOCATION_SUPPORTED];
  std::map<int, id_lifetime>
      StatIDMapper;  // Id Lifetime mapper for memory optimization-
};                   // Global

extern GLOBAL global;
// Counter for malloc and free .
void* malloc_pointers_data[MAX_PTRS_SUPPORTED];      // Pointers on current run,
                                                     // stored.
int _malloc_counter = 0;                             // Counter for malloc calls
int _call_order_counter = 0;                         // IDs order counter
int _malloc_array_size_to_copy[MAX_PTRS_SUPPORTED];  // Size order stored
int _malloc_array_size_counter = 0;

/* StatIDMapper counters*/
int map_id_counter = 0;
#endif