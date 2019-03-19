ABFDLL
==========================================================================

This directory contains C++ headers and source code used to wrap the ABFFIO.dll
from the ABF FSP 2.01 (circa 2007) into C code for use in ChanneLab.  These
files were initially copied over from a copy of the ABFDLL project on an
external hard drive, and I assume are the most recent versions used for
ChanneLab, but cannot verify this.

Update July 3, 2016
-------------------

The exported functions need to use the `__stdcall` calling convention to be
compatible with the OldABFDLL, which is used by ChanneLab. This can be
accomplished using the `/Gz` compiler option.

Note that the  ABFFIOforCL.h file needs to use the `BOOL` typedef to be compatible with C.

To use this library in ChanneLab
---------------------------------

1. Include the `ABFFIOforCL.h` and `ABFConstants.h` files in ChanneLab project.
2. Update `ABFFIOforCL.h` to use the `BOOL` typedef insted of the C++ data type `bool`.
3. Add the `ABFFIO.dll` from the Axon ABF File Support Pack to the ChanneLab project.
4. Add the `ABFFIOforCL.lib` and `ABFFIOforCL.dll` files to the ChanneLab project.
5. Make sure LabWindows/CVI uses the `__stdcall` calling convention.

Compile and link commands
-------------------------

The target machine is x86, which is accomplished by running `vcvars32.bat`.
This seemed to work when just the Microsoft Visual Studio 10.0 directory and subdirectories were on the Path.

```
cl /EHsc /c /Gz ABFFIOforCL.cpp
link /DLL ..\..\FSP_ABF_FileSupportPack_2.03\AxonDev\Lib\abffio.lib ABFFIOforCL.obj
```
