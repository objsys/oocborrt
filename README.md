# oocborrt
## Objective Open CBOR Run-time (oocborrt)

The Objective Open CBOR Run-time is an open source C run-time 
library implementation of the Concise Binary Object Representation 
(CBOR) format as documented in IETF RFC 7049.  It contains functions
for encoding and decoding data to and from CBOR format.

### Building on Windows using Visual Studio

To build the library for Windows, unzip the package and cd to the build
subdirectory.  Execute the 'nmake' command to run the Visual Studio nmake 
utility to build the package. Visual Studio C/C++ compilation flags are 
located in the platform.mk file in the root directory.

### Building on Linux

To build the package on Linux or UNIX, rename or delete the platform.mk 
file in the main directory and then rename the platform.gnu file to 
platform.mk:

```
rm -f platform.mk
mv platform.gnu platform.mk
```

Then cd to the build subdirectory and execute make.

Note that source files in the package are stored in Windows (DOS) format. 
It make be necessary to convert to UNIX format on some systems. 
A utility program such as dos2unix can be used to convert the files 
to UNIX format if necessary (Google dos2unix for details).

### Using the compiled library

The unit test programs in unit_cbor subdirectory provide an example of 
how to include oocborrt in your own programs.  There are also the 
cbor2json (CBOR-to-JSON) and json2cbor (JSON-to-CBOR) programs in 
the util subdirectory.

### License

The COPYING.txt file within this package contains information on the 
open source license.  This license allows open source redistribution 
of the source and binary files in the library.

