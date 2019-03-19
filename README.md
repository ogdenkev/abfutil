ABF Utilities
==========================================================================

Kevin K. Ogden

June 30, 2016


Overview
--------------------------------------------------------------------------

This goal of this project is to take the ABFFIO.DLL and ABFFIO.LIB files from
the ABF File Support Pack version 2.03 and make them work with non-C++ programs,
specifically ChanneLab and Matlab.


Description
--------------------------------------------------------------------------

The Axon Binary Format is a format developed at Axon for storing
electrophysiological data recorded using Axon's Clampex software.  Axon made
some software developer's kits available to allow third-parties to develop
software to read data in ABF files. Axon was bought by Molecular Devices, but
the SDKs, which are referred to as ABF File Support Packs, are still available.
Initially, the SDK was a set of C++ headers and code that could be used in a
project directly.  Also, the file format of the ABF was well-described, so it
was possible to write your own routines for reading the files, without using the
SDK. However, the header information in the ABF files grew more complex over
time, and around 2007, Axon (or Molecular Devices) no longer provided C++ code
to read the ABF files, but rather supplied a dynamic link library and import
library that could be linked into an application.

The ABF File Support Pack is available for download from
http://mdc.custhelp.com/app/answers/detail/a_id/18881/kw/axon%20file%20support
It seems that you need to register to download the FSPs (but you may not and
it's free anyway).  You can also search for Axon pCLAMP ABF File Support Pack
Download in the Molecular Devices Knowledge Base.


Contents
--------------------------------------------------------------------------

* FSP_ABF_FileSupportPack_2.03.zip, which was download from the above site on
  June 30, 2016 and had an md5 checksum of `c8af567417b1459c1c96974514d6b38e`
* data directory that contains four abf files that were not correctly loaded
  with the FSP from 2007


Problem with reading pCLAMP 10.4+ ABF files
-------------------------------------------

It seems that around 2013, the ABF File Support Pack was updated to correspond
to version 2.03 of the ABF.  With this change, newer versions of Clampex and
pCLAMP (e.g. pCLAMP >= 10.4) saved ABF files that were not compatible with the
earlier Support Pack.

With the ABF header version 2.03, which occurs with pCLAMP 10.4.0.7 and later,
the header structure changed.  For example, the number of DAC channels increased
from 4 to 8 and there were a few other fields added. I believe the mismatches in
size were causing buffer overload errors.
