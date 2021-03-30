#!/home/osm/Documents/EI-EDA/ml_on_mcu/venv/bin/python3.7 
#Change the venv path above.

# Add offset to a given datasheet.
# Two methods : 
#   - Give offset for every ID (every malloc call) --> MORE Memory
#   - Give offset for every pointer (every container) -->Less Memory.
# Datasheet format:
# ID2.0|Size|firstAlloc|lastFree|Pointer

import argparse
import pandas as pd
import numpy as np

# Import the datasheet
def import_sheet(path):
    df = pd.read_excel(path)
    df.columns = df.columns.str.replace(" ", "")  # Clean the datasheet
    df = df.set_index("ID2.0") 
    return df 
# Save the datasheet
def save_sheet(name,df):
    df.to_excel(name[:len(name)-5]+"_updated.xlsx")

#Output offset array on terminal.
def offseter(df):
    #reference adding base pointer
    cell_ref= df['Pointer'].min()
    df['Offset_1'] = 0 #first method
    df['Offset_2'] = 0 #second method

    #Use buffer for every malloc call with appropriate size.
    global_size= 0 
    for index, row in df.iterrows():
        df.loc[index,'Offset_1']= global_size
        global_size+=row['Size']
    output_string =''
    for off in df['Offset_1']:
        output_string+=str(off)+","

    #pretty print for hook
    print('=============================OFFSET_1==================================')
    print(output_string[:len(output_string)-1])
    print("Global Size required: ",global_size)
    print('=============================OFFSET_2==================================')

    #Use buffer for each unique pointer with appropriate size.
    max_mem = 0 
    for i in df['Pointer'].unique():
        df_current = df[df['Pointer'] == i]
        df.loc[(df.Pointer == i) , 'Offset_2'] = max_mem
        max_mem += df_current['Size'].max()

    #pretty print for hook 
    output_string =''
    for off in df['Offset_2']:
        output_string+=str(off)+","
    print(output_string[:len(output_string)-1])
    print("Global Size required: ",max_mem)
    return df

def main ():
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "input_sheet_file",
        help=
        "The path to a datasheet containing pointer lifetimes and sizes, make sure that the format respects the following layout: 'ID2.0|Size|firstAlloc|lastFree|Pointer' of types 'int|int|int|int|str'",
        type=str)
    parser.add_argument(
        "-s",
        "--save_bool",
        help=
        "Saves the updated datasheet automatically into /datasheet_updated.xlsx. Defaults to False.",
        action='store_true')
    args = parser.parse_args()
    df = import_sheet(args.input_sheet_file)
    df = offseter(df)
    if (args.save_bool):
        save_sheet(args.input_sheet_file,df)

if __name__ == "__main__":
    main()