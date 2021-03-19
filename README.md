# Description 
A sum of python scripts and C++ scripts to attach for embedded solutions on TFLite to analyze dynamic and avoid memory allocations.

# Requirements
- newlib (modified and compiled with extra hook options) !update link to fork/downloadables!
- python (tested on 3.7) 
- Libre Office, or any _.xlsx_ file editor


# Who's who?
_/python_ : Python scripts to run to analyze data cought from the hooks running in a specific mode. Read more inside

_C_ : _.cc_ and _.h_ to include to enable custom malloc(), free() hooks on newlib. Read more inside.
