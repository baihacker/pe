# Libraries on Win64

This doc tells you how to build and use C/C++ libraries on windows 64, and those libraries provide script to build on linux environment. In most cases, it is also applied to windows 32.

## Build

Please read the README or INSTALL doc of the target library before building it, since it may contain important configuration specifications. If the library provides vc solution/project, you can use it to build. This doc only focuses on a library which can be built by makefile (other building system is not included).

### Prepare
* MinGW64, the 64-bit version of MinGW. You can download the installer [here](https://sourceforge.net/projects/mingw-w64/). The installer provides several install options, and a typical install option is MinGW-x86_64-8.1.0-win32-seh-rt_v6-rev0.

* msys2, its core is an independent rewrite of [MSYS](http://www.mingw.org/wiki/MSYS), based on modern Cygwin (POSIX compatibility layer) and MinGW-w64 with the aim of better interoperability with native Windows software.
  > Please copy the "mingw64" folder under the installation path of MinGW64 to C:/msys64/mingw64 (Assuming the installation path of msys2 is c:/msys64)
  
  > You can run C:/msys64/mingw64.exe to open the terminal.
  
  > Make sure C:/msys64/home/<computer user name>/build is a valid directory.
  
[To be continued]
