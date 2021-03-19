#!/home/osm/Documents/EI-EDA/ml_on_mcu/venv/bin/python3.7

# A python script to be ran from terminal.
# Used to plot lifetime of pointers and usage count given from malloc/free hook's database.
# Replace upper command with correspondant venv python exec. 

import pandas as pd
import matplotlib
import matplotlib.pyplot as plt
#from bokeh.plotting import figure, output_file, show
import seaborn as sns
from datetime import date
from datetime import datetime
import argparse


def import_sheet(path):
    df = pd.read_excel(path)  #Check this.
    df.columns = df.columns.str.replace(" ", "")
    df = df.set_index("ID")
    return df


#https://python-graph-gallery.com/184-lollipop-plot-with-2-groups/
def draw_lifetime(df,save,save_format,show_graph):
    now = datetime.now()
    current_time = now.strftime("[%H:%M:%S]")
    if (show_graph):
        print(current_time, ": Running quiet..")
    else: print(current_time, ": Running loud..")
    #Pointers_df= df["Pointer"]
    Allocated_df = df["Allocated"]
    Lifetime_df = df["Lifetime"]
    Reuse_df = df["Reuse"]

    my_range = range(1, len(df.index) + 1)
    fig = plt.figure(figsize=(16, 7))
    plt.hlines(y=my_range,
               xmin=Allocated_df,
               xmax=Lifetime_df,
               color='grey',
               alpha=0.5,
               linewidth=4)
    plt.scatter(Allocated_df,
                my_range,
                alpha=1,
                label='Allocated',
                c=Reuse_df,
                cmap="cool")
    plt.scatter(Lifetime_df,
                my_range,
                alpha=0.5,
                label='Destroyed',
                c=Reuse_df,
                cmap="cool")
    plt.colorbar(label="Reusage times", orientation="vertical")
    plt.legend()

    plt.yticks(my_range, df.index)
    plt.title("Memory Usage", loc='center')
    plt.xlabel('Time (not in seconds)')
    plt.ylabel('IDs to Memory pointers')
    if(save):
        now = datetime.now()
        today = date.today()
        today.strftime("%b_%d_%Y")
        current_time = now.strftime("%H_%M_%S")
        output = "figs/ptr_" + str(current_time) + "_" + str(today)
        plt.savefig(output + "."+save_format, bbox_inches='tight', dpi=100)
        current_time = now.strftime("[%H:%M:%S]")
        print(current_time, ": Plot saved.")
    plt.grid(alpha=0.1, linewidth=0.7)
    if(not show_graph):
        plt.show()


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("input_sheet_file",help="The path to a datasheet containing pointer lifetimes, make sure that the format respects the following layout: 'Pointers|Size|Allocated|Liftime|Reuse|ID' of types 'str|int|int|int|int|int'",type=str)
    parser.add_argument("-s","--save_bool",help="Saves the plot automatically into figs/<current_date.save_format>. Defaults to False",action='store_true')
    parser.add_argument("-q","--quiet",help="Shows the plot at the end automatically. Defaults to False.",action='store_true')
    parser.add_argument("-f","--save_format",help="Image format to save (supported formats: eps, jpeg, jpg, pdf, pgf, png, ps, raw, rgba, svg, svgz, tif, tiff). Defaults to PNG",default="png")
    args = parser.parse_args()
    df = import_sheet(args.input_sheet_file)
    draw_lifetime(df,args.save_bool,args.save_format,args.quiet)

if __name__ == "__main__":
    main()
