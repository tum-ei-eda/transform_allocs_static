#!/home/osm/Documents/EI-EDA/ml_on_mcu/venv/bin/python3.7

# A python script to be ran from terminal.
# Used to plot lifetime of pointers given from malloc/free hook's database.
# Replace upper command with correspondant venv python exec.

import pandas as pd
import matplotlib
import matplotlib.pyplot as plt
from matplotlib import cm
from matplotlib.colors import ListedColormap, LinearSegmentedColormap
from datetime import date
from datetime import datetime
import argparse
import numpy as np
from matplotlib.lines import Line2D


def import_sheet(path):
    df = pd.read_excel(path)  #Import datasheet.
    df.columns = df.columns.str.replace(" ", "")  # Clean the datasheet
    df = df.set_index("ID2.0")  # set index.
    df['lastFree'].replace(-1, df['lastFree'].max(),
                           inplace=True)  # Clean -1 to be replaced with max.
    return df


def draw_lifetime(df, save, save_format, show_graph):
    now = datetime.now()
    current_time = now.strftime("[%H:%M:%S]")
    if (show_graph):
        print(current_time, ": Running quiet..")
    else:
        print(current_time, ": Running loud..")

    fig = plt.figure(figsize=(30, 30))
    j = 0  # simple loop counter
    plt.xticks(np.arange(0, df['lastFree'].max(), 10))
    global_size = 0
    #Colormap configuration
    viridis = cm.get_cmap('viridis', 256)
    newcolors = viridis(np.linspace(0, 1, 256))
    pink = np.array([248 / 256, 24 / 256, 148 / 256, 1])
    olivegreen = np.array([215 / 256, 255 / 256, 135 / 256, 1])
    orchid = np.array([175 / 256, 95 / 256, 215 / 256, 1])
    goldenrod = np.array([175 / 256, 135 / 256, 0 / 256, 1])
    grey = np.array([38 / 256, 38 / 256, 38 / 256, 1])
    newcolors[:1, :] = pink
    newcolors[1:2, :] = olivegreen
    newcolors[2:5, :] = orchid
    newcolors[5:128, :] = goldenrod
    newcolors[128:, :] = grey
    newcmp = ListedColormap(newcolors)
    #Clean the size for legends
    df['Size_reduced'] = df['Size'] / df['Size'].mean()
    if df.Size_reduced[df.Size_reduced < 1].count() > df.Size_reduced[
            df.Size_reduced > 1].count():
        df['Size_reduced'] += 1

    for i in df['Pointer'].unique():
        df_current = df[df['Pointer'] == i]
        #plot lifetime
        ax0 = plt.hlines(y=np.full(shape=df_current.shape[0], fill_value=j),
                         xmin=df_current['firstAlloc'],
                         xmax=df_current['lastFree'],
                         linewidth=0.85,
                         colors='black')

        #plot sizes
        ax1 = plt.scatter(df_current['firstAlloc'],
                          np.full(shape=df_current.shape[0], fill_value=j),
                          alpha=0.8,
                          label='Size',
                          c=df['Size'][df['Pointer'] == i],
                          cmap=newcmp,
                          vmin=df['Size'].min(),
                          vmax=df['Size'].max(),
                          s=df['Size_reduced'][df['Pointer'] == i])
        global_size += df['Size'][df['Pointer'] == i].max()
        j = j + 1
    plt.yticks(np.arange(0, j, 3))
    plt.tick_params(axis="x", rotation=-45)
    plt.title("Pointer lifetimes", size=20)
    plt.xlabel("Ticks")
    plt.ylabel("Unique Pointers")
    plt.grid(True, alpha=0.25)
    cbar = plt.colorbar()
    cbar.set_label("Size", rotation=90)
    cbar.set_ticks([
        int(1 / 256 * df['Size'].max()),
        int(5 / 256 * df['Size'].max()),
        int(128 / 256 * df['Size'].max()),
        int(256 / 256 * df['Size'].max())
    ])
    if (save):
        now = datetime.now()
        today = date.today()
        today.strftime("%b_%d_%Y")
        current_time = now.strftime("%H_%M_%S")
        output = "figs/id_" + str(current_time) + "_" + str(today)
        plt.savefig(output + "." + save_format, bbox_inches='tight', dpi=100)
        current_time = now.strftime("[%H:%M:%S]")
        print(current_time, ": Plot saved.")
    if (not show_graph):
        #hard coded size legend
        leg_elems = [
            Line2D([0], [0],
                   marker='o',
                   color='w',
                   label='Small ~ <250 ',
                   markerfacecolor=pink,
                   markersize=2),
            Line2D([0], [0],
                   marker='o',
                   color='w',
                   label='Medium ~ <1250',
                   markerfacecolor=orchid,
                   markersize=5),
            Line2D([0], [0],
                   marker='o',
                   color='w',
                   label='Big ~ >32000',
                   markerfacecolor=grey,
                   markersize=15),
            Line2D([0], [0], color='black', label='Lifetime',
                   markersize=0.085),
            Line2D([0], [0],
                   color='w',
                   label=('Full allocated size : ' + str(global_size)+'Bytes'),
                   markersize=0)
        ]
        plt.legend(handles=leg_elems,
                   title='Size allocated',
                   shadow=True,
                   loc='upper left',
                   fontsize='large')
        plt.show()


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "input_sheet_file",
        help=
        "The path to a datasheet containing pointer lifetimes, make sure that the format respects the following layout: 'ID2.0|Size|firstAlloc|lastFree|Pointer' of types 'int|int|int|int|str'",
        type=str)
    parser.add_argument(
        "-s",
        "--save_bool",
        help=
        "Saves the plot automatically into figs/<current_date.save_format>. Defaults to False",
        action='store_true')
    parser.add_argument(
        "-q",
        "--quiet",
        help="Shows the plot at the end automatically. Defaults to False.",
        action='store_true')
    parser.add_argument(
        "-f",
        "--save_format",
        help=
        "Image format to save (supported formats: eps, jpeg, jpg, pdf, pgf, png, ps, raw, rgba, svg, svgz, tif, tiff). Defaults to PNG",
        default="png")
    args = parser.parse_args()
    df = import_sheet(args.input_sheet_file)
    draw_lifetime(df, args.save_bool, args.save_format, args.quiet)


if __name__ == "__main__":
    main()
