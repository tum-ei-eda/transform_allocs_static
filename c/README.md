# Description
The files introduce malloc and free custom hooks.

The hook workflow is explained here:
![hook_workflow](/pics/pic_6.png)

# Choose a mode 
As the hooks are designed in the purpose of avoiding dynamic memory allocations inside TensorFlow Lite Micro interpreter and optimizing the planned static buffer, there are two methods to use these hooks:

##  **Normal (default)**
This will call for malloc()/free() provided by the toolchain (The GNU C Library/Newlib ..), simply output a print after each call during runtime, and prvoide a mapping of every call to the correspondant pointer, size, and lifetime:

| ID 2.0 | Size | firstAlloc | lastFree | Pointer |
| --- | --- | --- | --- | --- |
| 0 | 20 | 0 | -1 | 0x1307c0 |
| 1 | 112 | 1 | -1 | 0x1307d4 |
| 2 | 32 | 2 | 15 | 0x130844 |
| 3 | 512 | 3 | 53 | 0x130864 | 

_PS: -1 means that the ID/Buffer upto the print time still did not get released, in python scripts this is automnatically set to the last time stamp in the run._

![hook_terminal](/pics/pic_7.png)

## **#define _STATIC_MALLOC** 
This will replace the malloc()/free() calls with a lookup function in a given malloc_array[]([Can be created here](../python/offseter.py)).

In static mode, a call to BigMalloc() is included inside "init_transform_allocs_static" as the goal is to offer TFLite (or any similar API) enough pre-allocatoed memory from the heap to ensure that it has enough room to execute the kernel codes in the Prepare()/Eval() stages. ([Look here for TFLite documentation](https://www.tensorflow.org/lite/guide/ops_custom)). 

However BigFree() is optional to call, as there is no expected shutdown for micro implementations running specific, non-changing pre-implemented ML models.

Additionaly, all the mapping and print options from normal mode are still viable.

# How to use 
0. make/compile using _modified_ newlib.
1. Include malloc_hook.h & malloc_array.h in your main.cpp in TFLite's "app".
2. Use **_STATIC_MALLOC** to enable 2nd method of alloction (Either through CMAKE or #define _STATIC_MALLOC in main.cpp).
3. Call init_transform_allocs_static() to replace original malloc/free with ones defined in this project.
4. In __STATIC_MALLOC mode, Call init_transform_allocs_static() before Allocate_tensors() in TFLite to allocate all memory at once.
5. Update offset_data in malloc_array with data given from python/offseter.py
6. ..  Repeat.
7. Analyse. 
8. .. Repeat until content  with results.
