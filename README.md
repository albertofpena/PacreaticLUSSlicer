# PacreaticLUSSlicer

Module for 3D Slicer based on PlusLib

The module takes a directory with LUS images and its correspondant tracker tsv file to generate a 3D volume. User can set the interpolation to trilinear or nearest neighbour. Compounding modes available are mean value and maximum value.

## How to build
Assuming the source code of your module is located in folder <code>PancreaticLUSSlicer</code>, externally building modules could be achieved doing:
<pre>
$ mkdir PancreaticLUSSlicer-build
$ cd PancreaticLUSSlicer-build
$ cmake -DSlicer_DIR:PATH=/path/to/Slicer-Superbuild/Slicer-build ../PancreaticLUSSlicer
$ make
</pre>

## tsv file format
Please, note the tsv parsing is adapted to the tracking files provided by Aurora tracking system. Maybe this doesn't fit your needs.
