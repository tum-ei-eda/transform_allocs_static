# Description
The files introduce malloc and free custom hooks.

- Original allocation:
size ---> malloc() ---> ptr
ptr ---> free()

- New hooks:
size ---> malloc_hook() ---->malloc()---> ptr 
ptr ---> free_hook()--->free()

# #define _STATIC_MALLOC 
As the hooks are designed in the purpose of avoiding dynamic memory allocations inside TensorFlow Lite Micro interpreter and optimizing the planned static buffer, there is two methods to use these hooks.

1. Normal (default):
This will call for malloc()/free() provided by the toolchain (The GNU C Library/Newlib ..), simply output a print after each call during runtime, and prvoide a mapping of every call to the correspondant pointer, size, and lifetime:

ID 2.0         Size      firstAlloc     lastFree       Pointer        
0              20        0              -1             0x1307c0       
1              112       1              -1             0x1307d4       
2              32        2              15             0x130844       
3              512       3              53             0x130864  

2. #define _STATIC_MALLOC: 
This will replace the malloc()/free() calls with a lookup function in a given malloc_array[]([Can be created here](python/offseter.py)).
In static mode, a call to BigMalloc() is necessary as the goal is to offer TFLite enough pre-allocatoed memory from the heap to ensure that it has enough room to execute the kernel codes in the Prepare()/Eval() stages. ([Look here for TFLite documentation](https://www.tensorflow.org/lite/guide/ops_custom)). 
However BigFree() is optional to call, as there is no expected shutdown for micro implenetations running specific, non-changing pre-implemented ML models.
Additionaly, all the mapping and print options from normal mode are still viable.

# How to use 
0. make/compile using _modified_ newlib.
1. Include malloc_hook.cc & malloc_array.cc in your main.cpp in TFLite's "app".
2. Use **_STATIC_MALLOC** to enable 2nd method of alloction (Either through CMAKE ARGs or #define _STATIC_MALLOC in main).
3. Call cus_hook_init() to replace original malloc/free with ones defined in this project.
4. In __STATIC_MALLOC, mode Call BigMalloc() before Allocate_tensors() in TFLite to allocate all memory at once.
5. Update malloc_array with data given from python/offseter.py
6. ..  Repeat.
7. Analyse. 
8. .. Repeat until confident with results.
