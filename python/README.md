# How to use : 
Use to get more information on the required inputs: 
```
./id_sep.py --help 
./ptr_total.py --help 
```
If the commands are not supported /not found in terminal under linux, please check how to setup _chmod_.

datasheet.xlsx & datasheet_new.xlsx template datasheets and can be used respectively.

To create a new datasheet, copy the mapping from the hook "pretty_print" in the terminal into a sheet editor.

Setup a correspondant venv python env and replace. Needed modules are (Use pip to install):
- matplotlib
- pandas 
- numpy
- datetime
- argparse

Figures plotted are generated in /figs, if '-s' is given.

# id_sep.py 
Use this to plot **StatBufMapper** information from the hook process.

# ptr_total.py 
Use this to plot **StatIDMapper** information from the hook process.

The plots from this script give a better prespective on the allocated pointers. (Notice the colormap used)
