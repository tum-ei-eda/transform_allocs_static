/* Made by Oussama Sayari and Rafael Stahl.
                  TUM-EDA 2021
=========================================================================== */
#include "malloc_hook.h"
#include "malloc_array.h"
//#include <malloc.h>  //Just for reference

// Source:
// https://www.gnu.org/software/libc/manual/html_node/Hooks-for-Malloc.html
// Global var containing for function intercommunication.
// - counter/tick (init at 0)
// - StatIDMapper
// - ...
GLOBAL malloc_hook_global;

// Print one malloc op
inline void pretty_malloc_print(void *ptr, size_t size) {
  DBGPRINTF("%-7i%-10s%-35p%-20i\n", malloc_hook_global.counter, "malloc", ptr,
            size);
}

// Print one free op
inline void pretty_free_print(void *ptr, bool recorded) {
  if (recorded) {
    DBGPRINTF("%-7i%-10s%-35p\n", malloc_hook_global.counter, "free", ptr);
  } else {
    DBGPRINTF("%-7i%-10s%-35p%-3s\n", malloc_hook_global.counter, "free", ptr,
              "Not");
  }
}

/*original hooks*/
extern void *(*__malloc_hook)(size_t, const void *);
extern void (*__free_hook)(void *, const void *);
static void *(*old_malloc_hook)(size_t, const void *);
static void (*old_free_hook)(void *, const void *);

// A custom hook for malloc
static void *cus_malloc_hook(size_t size, const void *caller) {
  void *result;
  /* Restore all old hooks */
  __malloc_hook = old_malloc_hook;
  __free_hook = old_free_hook;
/* Call recursively */
#ifdef _STATIC_MALLOC  // Use offset from BigMalloc
  result = (void *)((uintptr_t)malloc_hook_global._offset_base +
                    offset_data[malloc_hook_global._malloc_counter]);
  malloc_hook_global._malloc_counter++;  // update data counter
#else                                    // Call for normal malloc
  result = malloc(size);
#endif
  /* Save underlying hooks */
  old_malloc_hook = __malloc_hook;
  old_free_hook = __free_hook;

  pretty_malloc_print(result, size);

  /* Save in StatIDMapper */
  malloc_hook_global.StatIDMapper.insert(
      {malloc_hook_global.map_id_counter++,
       id_lifetime(size, malloc_hook_global.counter, result)});
  /* Restore our own hooks */
  __malloc_hook = cus_malloc_hook;
  __free_hook = cus_free_hook;

  /* update counters*/
  malloc_hook_global.counter++;
  return result;
}
// A custom hook for free
static void cus_free_hook(void *ptr, const void *caller) {
  /* Restore all old hooks */
  __malloc_hook = old_malloc_hook;
  __free_hook = old_free_hook;
/* Call recursively */
#ifndef _STATIC_MALLOC  // Use normal freeS
  free(ptr);
#endif  // Else no call for free is needed (NOP)

  /* Save underlying hooks */
  old_malloc_hook = __malloc_hook;
  old_free_hook = __free_hook;
  pretty_free_print(ptr, false);
  /*update liftime of current pointer_ID*/
  for (auto elem = malloc_hook_global.StatIDMapper.begin();
       elem != malloc_hook_global.StatIDMapper.end(); elem++) {
    if ((elem->second.ptr == ptr) && (elem->second.lastFreed == -1)) {
      elem->second.lastFreed = malloc_hook_global.counter;
      break;
    }
  }
  /* Restore our own hooks */
  __malloc_hook = cus_malloc_hook;
  __free_hook = cus_free_hook;
  /*update counter*/
  malloc_hook_global.counter++;
}

// Allocates all sizes given in malloc_array.cc file.
// Stores return result into malloc_hook_global array.
static void BigMalloc() {
#ifdef _STATIC_MALLOC
  DBGPRINTF("[BigMalloc]: Allocating %i. _STATIC_MALLOC ON\n",
            big_size + MEM_OVERHEAD);
  malloc_hook_global._offset_base =
      malloc(big_size + 100);  // Allocate for all with a small headroom
#else
  DBGPRINTF("[BigMalloc]: Nothing to be done. _STATIC_MALLOC OFF\n");
  return;
#endif
}

// Call if necessary,
// Safe to call at the end.
static void BigFree() {
#ifdef _STATIC_MALLOC
  DBGPRINTF("[BigFree]: Freeing at %p. _STATIC_MALLOC ON\n",
            malloc_hook_global._offset_base);
  free(malloc_hook_global._offset_base);
#else
  DBGPRINTF("[BigFree]: Nothing to be done. _STATIC_MALLOC OFF\n");
  return;
#endif
}

// Use this to replace hooks and initiate the hooking process. MANDATORY!
static void init_hook(void) {
#ifdef _STATIC_MALLOC
  BigMalloc();
#endif
  old_malloc_hook = __malloc_hook;
  old_free_hook = __free_hook;
  __malloc_hook = cus_malloc_hook;
  __free_hook = cus_free_hook;
}
// Reset hooking to original.
static void done_hook(void) {
#ifdef _STATIC_MALLOC
  BigFree();
#endif
  __malloc_hook = old_malloc_hook;
  __free_hook = old_free_hook;
}

void init_transform_allocs_static(void) { init_hook(); }
