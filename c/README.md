# Description
Not done! 
STATIC METHOD NOT FULLY FUNCTIONNING!
--> BIND with OFFSETER.
--> TESTING STILL REQUIRED.

# How to use 
0. make/compile using _modified_ newlib.
1. Include malloc_hook.cc & malloc_array.cc in your main.cpp in TFLite's "app".
2. Use **_STATIC_MALLOC** to enable 2nd method of alloction (Either through CMAKE ARGs or #define _STATIC_MALLOC in main).
3. Call cus_hook_init() to replace original malloc/free with ones defined in this project
4. In __STATIC_MALLOC, mode Call BigMalloc() before Allocate_tensors() in TFLite to allocate all memory at once.
5. Update malloc_array with data given from python/offseter.py
6. ..  
