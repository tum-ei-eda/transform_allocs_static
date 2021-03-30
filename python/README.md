# How to use : 
## How to use the python files.
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

_datasheet_new.xlsx_ template datasheet can be used respectively.


## How to make a datesheet
To create a new datasheet, copy the mapping from the hook "pretty_print" in the terminal into a sheet editor, then export as _.xlsx as shown below:
![step1](/pics/pic_2.png)
Copy all the content of the map (including the column names)

![step2](/pics/pic_3.png)
Paste inside a Sheet editor of choice. 
In LibreOffice, you can decide when each column starts and ends from the "Text Import" window.
Spacing does not matter. The script will do the "clean-up".

![step2_1](/pics/pic_4.png)
![step3](/pics/pic_5.png)
Export as .xlsx file.

## How to setup venv
Setup a correspondant venv and replace the path at the beginning of both .py files if using _chmod_ to run.
 
Needed modules are (Use pip to install):
- matplotlib
- pandas 
- numpy
- datetime
- argparse

Figures plotted are generated in /figs, if '-s' is given.
# Files description
## id_sep.py 
Use this to plot **StatBufMapper** information from the hook process.

The plots from this script give a better prespective on the allocated pointers. (Notice the colormap used). Please look into [/figs](python/figs) for a concrete example.

## offseter.py
Return offset in two different methods from datasheet used on **ptr_total.py**. 

Comments inside script explain the point of every method. It is usually recommended to use the 2nd method. 

Offset can also save the new datasheet in the current dir.

## /figs
Plots are stored (with a time_stample) in this folder. Keep this folder in the same directory as the python files.
