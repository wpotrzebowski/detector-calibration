# Scripts for reading loki readouts from HDF5 files created by EFU

## strawpos.py
The only necessary argument is the filename. This will generate a png file with
a plot of position vs straw. Data is aggregated over all rings and all fens.

    > python3 strawpos.py inputfile

For options on how to limit the plot to a specifig ring and fen use -h command
line argument

    > python3 strawpos.py -h


## amplitudes.py
An example of how to plot amplitudes against each other. Not currently used but
could be a starting point for investigating thresholds.
