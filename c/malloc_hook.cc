#include "malloc_hook.h"
//#include <malloc.h>  //Just for reference
#ifdef _DEBUG
#include <stdio.h>
#define DBGPRINTF(format, ...) printf(format, ##__VA_ARGS__)
#else
#define DBGPRINTF(format, ...)
#endif
// Source:
// https://www.gnu.org/software/libc/manual/html_node/Hooks-for-Malloc.html
#include "malloc_array.cc"
#include "malloc_array.h"
GLOBAL global;

//#define _STATIC_MALLOC

/*Pretty prints */
void pretty_header() {
  DBGPRINTF("%-7s%-10s%-35s%-20s\n", "Order", "Operation", "Pointer", "Size");
}
inline void pretty_malloc_print(void *ptr, size_t size) {
  DBGPRINTF("%-7i%-10s%-35p%-20i\n", global.counter, "malloc", ptr, size);
}
inline void pretty_free_print(void *ptr, bool recorded) {
  if (recorded) {
    DBGPRINTF("%-7i%-10s%-35p\n", global.counter, "free", ptr);
  } else {
    DBGPRINTF("%-7i%-10s%-35p%-3s\n", global.counter, "free", ptr, "Not");
  }
}

/* Call after 'pretty_map_print(void)' for best results */
inline void CopyMePrint(void) {
  DBGPRINTF("IDs (%i):\n", _call_order_counter - 1);
  for (int i = 0; i < _call_order_counter; i++) {
    DBGPRINTF("%i,", global.id_malloc_call_order[i]);
  }
  DBGPRINTF("\n \n");  // IDs call order.

  DBGPRINTF("Size (%i):\n", _malloc_array_size_counter - 1);
  for (int j = 0; j < _malloc_array_size_counter - 1; j++) {
    DBGPRINTF("%i,", _malloc_array_size_to_copy[j]);
  }
  DBGPRINTF("\n");  // Size call order.
}

/* Print the saved map and the rest of the data for the next run.*/
inline void pretty_map_print(void) {
  int total_size = 0;
  DBGPRINTF("-----------------------------------------------\n");
  DBGPRINTF("%-35s%-10s%-10s%-10s%-10s%-5s\n", "Pointer", "Size", "Allocated",
            "Lifetime", "Reuse", "ID");
  // A very bad sort but memory-effcient.
  for (int counter = 0; counter < global.StatBufMapper.size(); counter++) {
    for (auto elem : global.StatBufMapper) {
      if (counter == elem.second.id) {
        total_size += elem.second.size;
        DBGPRINTF("%-35p%-10i%-10i%-10i%-10i%-5i\n", elem.first,
                  elem.second.size, elem.second.firstUsed, elem.second.lastUsed,
                  elem.second.use, elem.second.id);
        continue;
      }
    }
  }
  DBGPRINTF("Total memory allocated: %i\n", total_size);
  DBGPRINTF("-----------------------------------------------\n");
  DBGPRINTF("%-15s%-10s%-15s%-15s%-15s\n", "ID 2.0", "Size", "firstAlloc",
            "lastFree", "Pointer");
  for (auto elem_x : global.StatIDMapper) {
    DBGPRINTF("%-15i%-10i%-15i%-15i%-15p\n", elem_x.first, elem_x.second.size,
              elem_x.second.firstAllocated, elem_x.second.lastFreed,
              elem_x.second.ptr);
  }
  DBGPRINTF("-----------------------------------------------\n");
  CopyMePrint();
}

/*prototypes*/
static void cus_hook_init(void);
static void *cus_malloc_hook(size_t, const void *);
static void cus_free_hook(void *, const void *);

/*original hooks*/
extern void *(*__malloc_hook)(size_t, const void *);
extern void (*__free_hook)(void *, const void *);
static void *(*old_malloc_hook)(size_t, const void *);
static void (*old_free_hook)(void *, const void *);

// Use this to replace hooks and initiate the hooking process. MANDATORY!
static void cus_hook_init(void) {
  old_malloc_hook = __malloc_hook;
  old_free_hook = __free_hook;
  __malloc_hook = cus_malloc_hook;
  __free_hook = cus_free_hook;
}
// Reset hooking to original malloc.
static void done_hook(void) {
  __malloc_hook = old_malloc_hook;
  __free_hook = old_free_hook;
}

// A custom hook for malloc
static void *cus_malloc_hook(size_t size, const void *caller) {
  void *result;
  /* Restore all old hooks */
  __malloc_hook = old_malloc_hook;
  __free_hook = old_free_hook;
/* Call recursively */
#ifdef _STATIC_MALLOC  // Use pointers from BigMalloc
  result = malloc_pointers_data[id_array_data[_malloc_counter]];
  _malloc_counter++;
  DBGPRINTF("array[%i]=%i\n", _malloc_counter, id_array_data[_malloc_counter]);
#else
  result = malloc(size);
#endif
  /* Save underlying hooks */
  old_malloc_hook = __malloc_hook;
  old_free_hook = __free_hook;
  /* printf might call malloc, so protect it too. */
  pretty_malloc_print(result, size);

  auto it = global.StatBufMapper.find(result);
  if (it == global.StatBufMapper.end()) {
    global.StatBufMapper.insert({result, ptr_id(size, global.counter)});
    global.id_malloc_call_order[_call_order_counter] = id_counter - 1;
    _malloc_array_size_to_copy[_malloc_array_size_counter] = size;
    _malloc_array_size_counter++;
  } else {
    if (size > it->second.size) it->second.size = size;
    it->second.use++;
    global.id_malloc_call_order[_call_order_counter] = it->second.id;
    _malloc_array_size_to_copy[it->second.id] = it->second.size;
  }
  global.StatIDMapper.insert(
      {map_id_counter++, id_lifetime(size, global.counter, result)});
  /* Restore our own hooks */
  __malloc_hook = cus_malloc_hook;
  __free_hook = cus_free_hook;
  _call_order_counter++;
  global.counter++;

  return result;
}
// A custom hook for free
static void cus_free_hook(void *ptr, const void *caller) {
  /* Restore all old hooks */
  __malloc_hook = old_malloc_hook;
  __free_hook = old_free_hook;
/* Call recursively */
#ifndef _STATIC_MALLOC
  free(ptr);
#endif  // Else no call for free is needed (NOP)
  /* Save underlying hooks */
  old_malloc_hook = __malloc_hook;
  old_free_hook = __free_hook;
  /* printf might call free, so protect it too. */
  auto it = global.StatBufMapper.find(ptr);
  if (it != global.StatBufMapper.end()) {
    it->second.lastUsed = global.counter;
    pretty_free_print(ptr, true);
  } else {
    pretty_free_print(ptr, false);
  }

  /*update liftime of current pointer_ID*/
  for (auto elem = global.StatIDMapper.begin(); elem != global.StatIDMapper.end();elem++) {
    if ((elem->second.ptr == ptr) && (elem->second.lastFreed == -1)) {
      elem->second.lastFreed = global.counter;
      break;
    }
  }
  /* Restore our own hooks */
  __malloc_hook = cus_malloc_hook;
  __free_hook = cus_free_hook;
  global.counter++;
}
// Allocates all sizes given in malloc_array.cc file.
// Stores return result into global array.
static void BigMalloc() {
#ifdef _STATIC_MALLOC
  // void *result;
  for (int i = 0; i < malloc_array_size_length; i++) {
    malloc_pointers_data[i] = malloc(malloc_array_size_data[i]);
  }
#else
  DBGPRINTF("Nothing to be done. _STATIC_MALLOC OFF\n");
  return;
#endif
}