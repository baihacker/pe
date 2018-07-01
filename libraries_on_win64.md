# Libraries on Win64

This doc tells you how to build and use C/C++ libraries on windows 64, and those libraries provide script to build on linux environment. In most cases, it is also applied to windows 32.

## Build

Please read the README or INSTALL doc of the target library before building it, since it may contain important configuration specifications. If the library provides vc solution/project, you can use it to build. This doc only focuses on a library which can be built by makefile (other building system is not included).

### Prepare
* MinGW64, the 64-bit version of MinGW. You can download the installer [here](https://sourceforge.net/projects/mingw-w64/). The installer provides several install options, and a typical install option is MinGW-x86_64-8.1.0-win32-seh-rt_v6-rev0.
  * Please make a copy of <parent path>/bin/mingw32-make.exe and the new name is make.exe.

* msys2, its core is an independent rewrite of [MSYS](http://www.mingw.org/wiki/MSYS), based on modern Cygwin (POSIX compatibility layer) and MinGW-w64 with the aim of better interoperability with native Windows software.
  * Please copy the "mingw64" folder under the installation path of MinGW64 to C:/msys64/mingw64 (Assuming the installation path of msys2 is c:/msys64)
  
  * You can run C:/msys64/mingw64.exe to open the terminal.
  
  * Make sure C:/msys64/home/<computer user name>/build is a valid directory.

### Build gmp
* Extract the package to build directory and cd the build directory (use msys2 terminal)
* Commands
  * ./configure --disable-shared --enable-static --prefix=/usr --enable-cxx --host=x86_64-w64-mingw32
  
  * make
  
  * make check
  
  * make install
  
* Notes
  * "--disable-shared --enable-static" we only build static library here.
  
     * sometimes, you can only enable one kind of build type. If you want to build both shared library and static library, please run the command twice.
     
     * it is a good practice only to build static library.
     
     * it is tricky if you have both static library and shared libarary and you want to let your building system decide which one to use.
     
  * "--prefix=/usr" is used to specify the install path.
  
     * You can find the header file (.h or other suffix) under /usr/include and find lib (.a) under /usr/lib.
     
     * TODO(baihacker): for shared library.
     
  * "--enable-cxx" gmp specified option to support c++ wrapper.
  
  * "--host=x86_64-w64-mingw32" required for building gmp on win64.

  * "make check" is optional but some libraries have this option in their makefile to check the build result.
  
  * "make install" copies the result to target path
  
  * "make clean" cleans intermediate outputs generated in the build process

## Use
### MinGW
#### General use
 * Make sure the parent folder of "g++.exe" is in environment PATH.
 
 * Use "g++ source_code.cpp" to compile your code.
 
 * Usually, I add "--std=c++11 -O3 -march=native -mtune=native -Wl,--stack,268435456" to compile my code.
 
 * Please add "-fopenmp" if you want to use openmp.
 
#### Use a library
 * CPLUS_INCLUDE_PATH contains the corresponding header files.
 
   * if "D:/X/Y/zzz.h" is your header file and CPLUS_INCLUDE_PATH contains "D:/X/Y" you can use #include <zzz.h> in your code. If "D:/X" is in it, you can use #include <Y/zzz.h> in your code.
   
 * LIBRARY_PATH contains the corresponding lib files.
 
   * if "D:/X/Y/libzzz.a" is in LIBRARY_PATH, you can use "-lzzz" to add that in your compile command.
   
   * some libraries don't have a ".a" file, so just include the header files. e.g. eigen.
   
 * Note for mutiple libraries:
   * The order of included header files is important. Please adjust them if necessaries.

   * The order of libraries (the order of additional library in compile command) is important (it usually happens). If the compiler says it cannot find a symbol, please try to adjust the library order. (Also try to adjust the order for other cases)
 
### MSVC
 * You can edit the include path and lib path in the solution property dialog.
 
 * In the solution property dialog, you can also add dependent libraries (in linker option section). Unlike that in MinGW, you need to specify libzzz.a instead of zzz.
 
   * You can also use #pragma comment(lib, "libzzz.lib") in your code to add dependent libraries.
   
 * If the library is built by MinGW
 
   * Use /MT (release) or /MTd (debug) to specify c-runtime.
   
   * Add libgcc_s.a;libgcc.a; to dependent library list. (They can be found in the MinGW directory)
   
   * Sometimes legacy_stdio_definitions.lib; is also necessary to add to dependent library list.
   
 * Enable other options if you want, e.g. openmp
 
 * Sample options to compile a code using pe.
 
   * Release: /GS /GL /W3 /Gy /Zc:wchar_t /Zi /Gm- /O2 /Fd"x64\Release\vc141.pdb" /Zc:inline /fp:precise /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /errorReport:prompt /WX- /Zc:forScope /Gd /Oi /MT /openmp /FC /Fa"x64\Release\" /EHsc /nologo /Fo"x64\Release\" /Fp"x64\Release\pe.pch" /diagnostics:classic
   
   * Debug: /GS /W3 /Zc:wchar_t /ZI /Gm- /Od /Fd"x64\Debug\vc141.pdb" /Zc:inline /fp:precise /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /errorReport:prompt /WX- /Zc:forScope /RTC1 /Gd /MTd /openmp /FC /Fa"x64\Debug\" /EHsc /nologo /Fo"x64\Debug\" /Fp"x64\Debug\pe.pch" /diagnostics:classic
   
   * Build test on AppVeyor: "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvars64.bat"
cl test\pe_test.c /TP /GS /GL /W3 /Gy /Zc:wchar_t /Zi /Gm- /O2 /Zc:inline /fp:precise /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /errorReport:prompt /WX- /Zc:forScope /Gd /Oi /MT /openmp /FC /EHsc /nologo /diagnostics:classic /DTEST_ALL /DCONTINUOUS_INTEGRATION_TEST /DENABLE_ASSERT=0 /DTRY_TO_USE_INT128=1 /DENABLE_OPENMP=1 /DENABLE_EIGEN=0 /DENABLE_GMP=0 /DENABLE_FLINT=0 /DENABLE_MPFR=0 /DENABLE_MPIR=0 /DENABLE_LIBBF=0 /I "C:\projects\pe"

## Build and use third party library in pe
 * Build gmp, mpir, mpfr and flint
 
   * gmp
   
     1. /configure --disable-shared --enable-static --prefix=/usr --enable-cxx --host=x86_64-w64-mingw32
     
     2. make
     
     3. make install
     
   * mpir
   
     1. ./configure --disable-shared --enable-static --prefix=/usr
     
     2. make
     
     3. make install
     
   * mpfr
   
     1. ./configure --with-gmp=/usr --enable-static --disable-shared --prefix=/usr
     
     2. Fix error in makefile
     
       * "rm: unknown option -- c": caused by argument passing when sh.exe is executing libtool. Please replace -DLT_OBJDIR=\".libs/\" in variable DEFS by -DLT_OBJDIR=.libs Meanwhile, -DMPFR_PRINTF_MAXLM=\"ll\" is replaced by -DMPFR_PRINTF_MAXLM=ll
       
     3. make
     
     4. make install
     
   * flint
   
     1. ./configure --disable-shared --enable-static --prefix=/usr --with-mpir=/usr --with-mpfr=/usr
     
     2. Fix error in make file
     
       * Replace BUILD_DIR=../build/$(dir); by BUILD_DIR=$(CURDIR)/build/$(dir); in the build command of target libflint.a.
       
     3. make
     
     4. The previous make may be failed: some object file can not be found because the corresponding module is not built. The corresponding fold has no object file. Please modify BUILD_DIRS to build those missing module only, and use make -B to build these modules.
     
     5. Revert the change in makefile in the previous step and try to make -B again. If the error specified in the previous step occurs again, use repeat the fix step.
     
     6. make install

 * Build libbf, use the following makefile. It will generate libbf.avx2.a and libbf.generic.a, please choose one and rename it to libbf.a
 ```cpp
 CC=$(CROSS_PREFIX)gcc
CFLAGS=-Wall
CFLAGS+=-O3
LDFLAGS=

PROGS+=libbf.generic.a libbf.avx2.a

all: $(PROGS)

libbf.generic.a : libbf.o  cutils.o
	gcc-ar crv libbf.generic.a cutils.o libbf.o

libbf.avx2.a : libbf.avx2.o  cutils.avx2.o
	gcc-ar crv libbf.avx2.a cutils.avx2.o libbf.avx2.o

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

%.avx2.o: %.c
	$(CC) $(CFLAGS) -mavx -mavx2 -mfma -mbmi2 -c -o $@ $<

clean:
	rm -f $(PROGS) *.o *.d *.a *.exe *~

-include $(wildcard *.d)
 ```
   * Library order: "-lbf -lgmpxx -lflint -lgmp -lmpfr -lmpir"
   
   * The compiled binaries (flint, gmp, mpfr, mpir, libbf) on windows (x64) can be found [here](https://pan.baidu.com/s/1OI-vk3JJevYphIsFoNg_vA) (pwd:x7cg)
