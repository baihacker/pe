# Libraries on Win64

This doc tells you how to build and use C/C++ libraries on windows 64, and those libraries provide script to build on linux environment. In most cases, it is also applied to windows 32.

## Build

Please read the README or INSTALL doc of the target library before building it, since it may contain important configuration specifications. If the library provides vc solution/project, you can use it to build. This doc only focuses on a library which can be built by makefile (other building system is not included).

### Prepare
* MinGW64, the 64-bit version of MinGW. You can download the installer [here](https://sourceforge.net/projects/mingw-w64/). The installer provides several install options, and a typical install option is MinGW-x86_64-8.1.0-win32-seh-rt_v6-rev0.
  > Please make a copy of <parent path>/bin/mingw32-make.exe and the new name is make.exe.

* msys2, its core is an independent rewrite of [MSYS](http://www.mingw.org/wiki/MSYS), based on modern Cygwin (POSIX compatibility layer) and MinGW-w64 with the aim of better interoperability with native Windows software.
  > Please copy the "mingw64" folder under the installation path of MinGW64 to C:/msys64/mingw64 (Assuming the installation path of msys2 is c:/msys64)
  
  > You can run C:/msys64/mingw64.exe to open the terminal.
  
  > Make sure C:/msys64/home/<computer user name>/build is a valid directory.

### Build gmp
* Extract the package to build directory and cd the build directory (use msys2 terminal)
* Commands
  > ./configure --disable-shared --enable-static --prefix=/usr --enable-cxx --host=x86_64-w64-mingw32
  
  > make
  
  > make check
  
  > make install
  
* Notes
  > "--disable-shared --enable-static" we only build static library here.
  
     >* sometimes, you can only enable one kind of build type. If you want to build both shared library and static library, please run the command twice.
     
     >* it is a good practice only to build static library.
     
     >* it is tricky if you have both static library and shared libarary and you want to let your building system decide which one to use.
     
  > "--prefix=/usr" is used to specify the install path.
  
     >* You can find the header file (.h or other suffix) under /usr/include and find lib (.a) under /usr/lib.
     
     >* TODO(baihacker): for shared library.
     
  > "--enable-cxx" gmp specified option to support c++ wrapper.
  
  > "--host=x86_64-w64-mingw32" required for building gmp on win64.

  > "make check" is optional but some libraries have this option in their makefile to check the build result.
  
  > "make install" copies the result to target path
  
  > "make clean" cleans intermediate outputs generated in the build process

[To be continued]
