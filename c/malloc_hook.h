#ifndef MALLOC_HOOK_H_
#define MALLOC_HOOK_H_
#include <map>
#define _STATIC_MALLOC  // Comment or uncomment this to change the mode.
#ifdef _DEBUG
#include <stdio.h>
#define DBGPRINTF(format, ...) printf(format, ##__VA_ARGS__)
#else
#define DBGPRINTF(format, ...)
#endif

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
        ptr(_ptr){};  // lastFreed == -1 : NOT FREED
};

// Contains necessary information about Buffer bindings.
struct GLOBAL {
  int counter = 0;  // A timer-like variable to store allocs and frees
  std::map<int, id_lifetime>
      StatIDMapper;         // Id Lifetime mapper for memory optimization-
                            // Global {ID, {size,firstAllocated,lastFree,ptr}}
  int _malloc_counter = 0;  // Counter for malloc calls to read databse
  void* _offset_base;       // BigMalloc return value

  int map_id_counter = 0; /* StatIDMapper counter*/
};

extern GLOBAL malloc_hook_global;
inline void pretty_header(){
    DBGPRINTF("%-7s%-10s%-35s%-20s\n", "Order", "Operation", "Pointer", "Size");
}

inline void pretty_map_print(void) {
  DBGPRINTF("%-15s%-10s%-15s%-15s%-15s\n", "ID 2.0", "Size", "firstAlloc",
            "lastFree", "Pointer");
  for (auto elem_x : malloc_hook_global.StatIDMapper) {
    DBGPRINTF("%-15i%-10i%-15i%-15i%-15p\n", elem_x.first, elem_x.second.size,
              elem_x.second.firstAllocated, elem_x.second.lastFreed,
              elem_x.second.ptr);
  }
  DBGPRINTF("-----------------------------------------------\n");
}
static void init_transform_allocs_static(void);

/* inline void pretty_header();
inline void pretty_map_print();
static void init_transform_allocs_static(void);
static void done_hook(void);
static void *cus_malloc_hook(size_t size, const void *caller);
static void cus_free_hook(void *ptr, const void *caller); */


/* void pretty_header();
inline void pretty_malloc_print(void *ptr, size_t size);
inline void pretty_free_print(void *ptr, bool recorded);
inline void pretty_map_print(void);

static void BigMalloc();
static void BigFree();

static void cus_hook_init(void);
*/

#endif