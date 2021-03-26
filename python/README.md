# How to use : 
Use to get more information on the required inputs: 
```
./id_sep.py --help 
./offseter.py --help
```
If the commands are not supported/not found in terminal under linux, please check how to setup _chmod_.

This would also work:
```
<path_to_venv_python3.7> /<py_file.py> --args
```

_datasheet_new.xlsx_ template datasheets can be used respectively.

To create a new datasheet, copy the mapping from the hook "pretty_print" in the terminal into a sheet editor, then export as _.xlsx

Setup a correspondant venv and replace the path. Needed modules are (Use pip to install):
- matplotlib
- pandas 
- numpy
- datetime
- argparse

Figures plotted are generated in /figs, if '-s' is given.

# id_sep.py 
Use this to plot **StatBufMapper** information from the hook process.

The plots from this script give a better prespective on the allocated pointers. (Notice the colormap used). Please look into [/figs](python/figs) for an example

# offseter.py
Return offset in two different methods from datasheet used on **ptr_total.py**. 
Comments inside script explain the point of every method.
