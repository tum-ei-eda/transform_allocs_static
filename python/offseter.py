#!/home/osm/Documents/EI-EDA/ml_on_mcu/venv/bin/python3.7

# Add offset to a given datasheet.
# ID2.0|Size|firstAlloc|lastFree|Pointer

import argparse
import pandas as pd
import numpy as np

def import_sheet(path):
    df = pd.read_excel(path)
    df.columns = df.columns.str.replace(" ", "")  # Clean the datasheet
    df = df.set_index("ID2.0") 
    return df 

def offseter(df):
    #reference adding base pointer
    cell_ref= df['Pointer'].min()
    df['Offset_1'] = 0 #first method
    df['Offset_2'] = 0 #second method

    #Use buffer for every malloc call with appropriate size.
    global_size= 0 
    for index, row in df.iterrows():
        global_size+=row['Size']
        df.loc[index,'Offset_1']= hex((int(cell_ref,16))+global_size) #Check this hex +int ?..
    output_string =''
    for off in df['Offset_1']:
        output_string+=off+","

    #pretty print for hook
    print('=============================OFFSET_1==================================')
    print(output_string[:len(output_string)-1])
    print("Global Size required: ",global_size)
    print('===============================================================')

    #Use buffer for each unique pointer with appropriate size.
    max_mem = 0 
    for i in df['Pointer'].unique():
        df_current = df[df['Pointer'] == i]
        max_mem += df_current['Size'].max()
        df.loc[(df.Pointer == i) , 'Offset_2'] = hex((int(cell_ref,16))+max_mem)

    #pretty print for hook 
    output_string =''
    for off in df['Offset_2']:
        output_string+=off+","
    print(output_string[:len(output_string)-1])
    print("Global Size required: ",max_mem)
    print('=============================OFFSET_2==================================')


def main ():
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "input_sheet_file",
        help=
        "The path to a datasheet containing pointer lifetimes and sizes, make sure that the format respects the following layout: 'ID2.0|Size|firstAlloc|lastFree|Pointer' of types 'int|int|int|int|str'",
        type=str)
    args = parser.parse_args()
    df = import_sheet(args.input_sheet_file)
    offseter(df)

if __name__ == "__main__":
    main()