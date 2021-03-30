# Description 
A sum of python and C++ scripts to attach for embedded solutions on TFLite to analyze dynamic and avoid memory allocations, as decribed in this figure.
![transform_allocs_static_workflow](./pics/pic_1.png)

# Requirements
- newlib (modified and compiled with extra hook options) !update link to fork/downloadables!
- python (tested on 3.7) 
- Libre Office, or any _.xlsx_ file editor
- C++11 or newer.

# Who's who?
_/python_ : Python scripts to run and analyze data cought from the hooks running in a specific mode. **Read more inside**.

_/C_ : _.cc_ and _.h_ to include to enable custom malloc(), free() hooks on the C Library. **Read more inside**.

# Getting started
To start using the scripts, clone the repo. Add the correpondant .cc files (if using CMAKE to your makefile) to compile the targeted main.cpp. 
