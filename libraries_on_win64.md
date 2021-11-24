# Libraries on Win64

This doc tells you how to build and use C/C++ libraries on windows 64, and those libraries provide script to build on linux environment. In most cases, it is also applied to windows 32.

## General build instruction

Please read the README or INSTALL doc of the target library before building it, since **it may contain important configuration specifications**. If the library provides vc solution/project, you can use it to build. This doc only focuses on a library which can be built by makefile (other building system is not included).

### Prepare
* MinGW64, the 64-bit version of MinGW. You can download the installer [here](https://sourceforge.net/projects/mingw-w64/). The installer provides several install options, and a typical install option is MinGW-x86_64-8.1.0-win32-seh-rt_v6-rev0.
  * Please make a copy of <parent path>/bin/mingw32-make.exe and the new name is make.exe.

* msys2, its core is an independent rewrite of [MSYS](http://www.mingw.org/wiki/MSYS), based on modern Cygwin (POSIX compatibility layer) and MinGW-w64 with the aim of better interoperability with native Windows software.
  * Please copy the "mingw64" folder under the installation path of MinGW64 to C:/msys64/mingw64 (Assuming the installation path of msys2 is c:/msys64)

  * You can run C:/msys64/mingw64.exe to open the terminal.

  * Make sure C:/msys64/home/`<computer user name>`/build is a valid directory.

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

  * -D__USE_MINGW_ANSI_STDIO=0 in C or CXX flags to avoid mingw dependencies.

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

* See [x86 Options](https://gcc.gnu.org/onlinedocs/gcc/x86-Options.html) for more options.

#### Use a library
* CPLUS_INCLUDE_PATH contains the corresponding header files.

  * if "D:/X/Y/zzz.h" is your header file and CPLUS_INCLUDE_PATH contains "D:/X/Y" you can use #include <zzz.h> in your code. If "D:/X" is in it, you can use #include <Y/zzz.h> in your code.

* LIBRARY_PATH contains the corresponding lib files.

  * if "D:/X/Y/libzzz.a" is in LIBRARY_PATH, you can use "-lzzz" to add that in your compile command.

  * some libraries don't have a ".a" file, so just include the header files. e.g. eigen.

* Note for mutiple libraries:
  * The order of included header files is important. Please adjust them if necessaries.

  * The order of libraries (the order of additional library in compile command) is important (it usually happens). If the compiler says it cannot find a symbol, please try to adjust the library order. [Why does the order in which libraries are linked sometimes cause errors in GCC?](https://stackoverflow.com/questions/45135/why-does-the-order-in-which-libraries-are-linked-sometimes-cause-errors-in-gcc/409470#409470)

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
cl test\pe_test.c /TP /GS /GL /W3 /Gy /Zc:wchar_t /Zi /Gm- /O2 /Zc:inline /fp:precise /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /errorReport:prompt /WX- /Zc:forScope /Gd /Oi /MT /openmp /FC /EHsc /nologo /diagnostics:classic /DTEST_ALL /DCONTINUOUS_INTEGRATION_TEST /DENABLE_ASSERT=0 /DTRY_TO_USE_INT128=1 /DENABLE_OPENMP=1 /DENABLE_EIGEN=0 /DENABLE_GMP=0 /DENABLE_FLINT=0 /DENABLE_MPFR=0 /DENABLE_LIBBF=0 /DENABLE_NTL=0 /I "C:\projects\pe"

## Build and use third party libraries in pe

 * Current version
 
   * [gmp 6.2.1](https://gmplib.org/)
   
   * [mpfr 4.1.0](https://www.mpfr.org/mpfr-current/#download)
   
   * [mpir 3.0.0](http://mpir.org/downloads.html) (not used now)
   
   * [flint2 2.8.4](https://github.com/wbhart/flint2)
   
   * [libbf 2020-01-19](https://bellard.org/libbf/)
   
   * [ntl Windows: WinNTL-11_5_1.zip](https://www.shoup.net/ntl/download.html)

 * The compiled binaries (flint, gmp, mpfr, mpir, libbf, libntl) on windows (x64) can be found [here](https://pan.baidu.com/s/1OI-vk3JJevYphIsFoNg_vA) (pwd:x7cg).

 * Library order in compiling command: "-lbf -lgmpxx -lflint -lmpfr -lntl -lgmp"

 * Building script
 
   * makefile_libbf and makefile_ntl are mentioned in the next section
   
   * Please extract the source into the directories mentioned in building script

```cpp
#!/bin/bash

BUILD_ROOT=$(pwd)

GMP_DIR="gmp-6.2.1"
MPFR_DIR="mpfr-4.1.0"
MPIR_DIR="mpir-3.0.0"
LIBBF_DIR="libbf-2020-01-19"
WIN_NTL_DIR="WinNTL-11_5_1"
FLINT_DIR="flint-2.8.4"

BUILD_DIR="build"
TARGET_DIR="$(pwd)/${BUILD_DIR}"
SHARED_FLAGS="-mtune=skylake -march=k8-sse3 -D__USE_MINGW_ANSI_STDIO=0"

function build_gmp(){
  echo "build gmp"
  cd "${GMP_DIR}"
  sed -i 's/ln -s "$ac_rel_source" "$ac_file" 2>\/dev\/null ||//g' configure
  sed -i 's/ln "$ac_source" "$ac_file" 2>\/dev\/null ||//g' configure
  ./configure --disable-shared --enable-static --prefix=${TARGET_DIR} --enable-cxx --host=x86_64-w64-mingw32 CFLAGS="-O3 -m64 ${SHARED_FLAGS}" CXXFLAGS="-O3 -m64 ${SHARED_FLAGS}"
  make -j8
  make install
  cd ..
}

function build_mpfr(){
  echo "build mpfr"
  cd "${MPFR_DIR}"
  ./configure --with-gmp=/usr --enable-static --disable-shared --prefix=${TARGET_DIR} CFLAGS="-O3 -m64 ${SHARED_FLAGS}"
  sed -i 's/DLT_OBJDIR=\\"\.libs\/\\"/DLT_OBJDIR=\.libs/g' makefile
  sed -i 's/DMPFR_PRINTF_MAXLM=\\"j\\"/DLT_OBJDIR=\DMPFR_PRINTF_MAXLM=j/g' makefile
  sed -i 's/DLT_OBJDIR=\\"\.libs\/\\"/DLT_OBJDIR=\.libs/g' src/makefile
  sed -i 's/DMPFR_PRINTF_MAXLM=\\"j\\"/DLT_OBJDIR=\DMPFR_PRINTF_MAXLM=j/g' src/makefile
  make -j8
  make install
  cd ..
}

function build_mpir(){
  echo "build mpir"
  cd "${MPIR_DIR}"
  sed -i 's/ln -s "$ac_rel_source" "$ac_file" 2>\/dev\/null ||//g' configure
  sed -i 's/ln "$ac_source" "$ac_file" 2>\/dev\/null ||//g' configure
  ./configure --disable-shared --enable-static --prefix=${TARGET_DIR} CFLAGS="-m64 -O3 ${SHARED_FLAGS}" CXXFLAGS="-m64 -O3 ${SHARED_FLAGS}"
  make -j8
  make install
  cd ..
}

function build_libbf(){
  echo "build libbf"
  cp makefile_libbf "${LIBBF_DIR}/makefile"
  cd "${LIBBF_DIR}"
  make -j8
  cp libbf.h "${TARGET_DIR}/include/libbf.h"
  cp libbf.avx2.a "${TARGET_DIR}/lib/libbf.avx2.a"
  cp libbf.generic.a "${TARGET_DIR}/lib/libbf.generic.a"
  cd ..
}

function build_ntl(){
  echo "build win ntl"
  cp makefile_ntl "${WIN_NTL_DIR}/src/makefile"
  cd "${WIN_NTL_DIR}"
  cd "include/NTL"
  sed -i 's/\/\* sanity checks \*\//\/\* sanity checks \*\/\n#define NTL_STD_CXX14\n#undef NTL_DISABLE_MOVE_ASSIGN/g' config.h
  cd "../../src"
  make -j8
  cp libntl.a "${TARGET_DIR}/lib/libntl.a"
  cd ..
  cp -r "./include/NTL" "${TARGET_DIR}/include/"
  cd ..
}

function build_flint(){
  echo "build flint"
  cd "${FLINT_DIR}"
  ./configure --disable-shared --enable-static --prefix=${TARGET_DIR} --with-gmp=${TARGET_DIR} --with-mpfr=${TARGET_DIR} --disable-pthread CFLAGS="-ansi -Wno-long-long -Wno-declaration-after-statement -O3 -funroll-loops -mpopcnt ${SHARED_FLAGS}" CXXFLAGS="-ansi -Wno-long-long -Wno-declaration-after-statement -O3 -funroll-loops -mpopcnt ${SHARED_FLAGS}"
  cp  -f ./Makefile.subdirs ./MS
  sed -i '1i\BUILD_DIR = ../build/${MOD}' MS
  sed -i '1i\MOD_DIR = ${MOD}' MS
  sed -i '/^WANT_DEPS=.*/aexport WANTDEPS=$(WANT_DEPS)' makefile
  sed -i '/^libflint.a:.*/a\\t$(AT)$(foreach dir, $(BUILD_DIRS), mkdir -p build/$(dir))' makefile
  sed -i 's/mkdir -p build\/$(dir); WANTDEPS=$(WANT_DEPS); export WANTDEPS; BUILD_DIR=..\/build\/$(dir); export BUILD_DIR; MOD_DIR=$(dir); export MOD_DIR; $(MAKE) -f ..\/Makefile.subdirs/export MOD=$(dir); $(MAKE) -f ..\/MS/g' makefile
  mkdir -p fmpz_mod_mpoly/test
  echo "int main(){return 0;}" > fmpz_mod_mpoly/test/t-1.c
  make -j8
  make install
  cd ..
}

function build_all(){
  build_gmp
  build_mpfr
  build_mpir
  build_libbf
  build_ntl
  build_flint
}

function clean() {
  rm -r -f "${GMP_DIR}"
  rm -r -f "${MPFR_DIR}"
  rm -r -f "${MPIR_DIR}"
  rm -r -f "${LIBBF_DIR}"
  rm -r -f "${WIN_NTL_DIR}"
  rm -r -f "${FLINT_DIR}"
  rm -r -f "./${BUILD_DIR}"
}

function extract_file() {
  #xz -d -k "${GMP_DIR}.tar.xz"
  #tar xf "${GMP_DIR}.tar"
  #rm "${GMP_DIR}.tar"
  tar xf "${GMP_DIR}.tar.xz"
  unzip -o -q "${MPFR_DIR}.zip"
  unzip -o -q "${MPIR_DIR}.zip"
  unzip -o -q "${WIN_NTL_DIR}.zip"
  tar xf "${LIBBF_DIR}.tar.gz"
  unzip -o -q "${FLINT_DIR}.zip"
}

function build_main() {
  clean
  extract_file
  mkdir -p "${TARGET_DIR}/include"
  mkdir -p "${TARGET_DIR}/lib"
  build_all
  cp -f ./METADATA.txt "${TARGET_DIR}/METADATA.txt"
  cp -f ./makefile_ntl "${TARGET_DIR}/makefile_ntl"
  cp -f ./makefile_libbf "${TARGET_DIR}/makefile_libbf"
  cp -f ./build_pe_deps.sh "${TARGET_DIR}/build_pe_deps.sh"
  rm -r -f "${TARGET_DIR}/share"
  rm -r -f "${TARGET_DIR}/lib/pkgconfig"
}

function package_file() {
  RELEASE_FILENAME="pe_dependencies_win64_$(date '+%Y%m%d').zip"
  cd ${BUILD_DIR}
  zip -r -q "${RELEASE_FILENAME}" .
  cd ..
  mv -f "./${BUILD_DIR}/${RELEASE_FILENAME}" ./
}

build_main
package_file

cd ${BUILD_ROOT}
```
 * makefile_libbf. This makefile will generate libbf.avx2.a and libbf.generic.a, please choose one and rename it to libbf.a
```cpp
CC=$(CROSS_PREFIX)gcc
CFLAGS=-Wall
CFLAGS+=-O3
CFLAGS+=-D__MSVCRT_VERSION__=0x1400
CFLAGS+=-Wno-format-extra-args
CFLAGS+=-Wno-format
CFLAGS+=-march=k8-sse3 -mtune=skylake -D__USE_MINGW_ANSI_STDIO=0
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

  * makefile_ntl (windows). Please edit config.h to enable c++14 and move assignment before running the script.
```cpp
CC=$(CROSS_PREFIX)gcc
CFLAGS=-Wall
CFLAGS+=-O3
CFLAGS+=-D__MSVCRT_VERSION__=0x1400
CFLAGS+=-I../include
CFLAGS+=--std=c++17
CFLAGS+=-Wno-maybe-uninitialized
CFLAGS+=-Wno-unused-variable
CFLAGS+=-Wno-unused-function
CFLAGS+=-Wno-unused-but-set-variable
CFLAGS+=-march=k8-sse3 -mtune=skylake -D__USE_MINGW_ANSI_STDIO=0
LDFLAGS=

SOURCE = $(wildcard *.cpp)
OBJS = $(patsubst %.cpp,%.o,$(SOURCE))

PROGS+=libntl.a

all: $(PROGS)

libntl.a : $(OBJS)
	gcc-ar crv libntl.a $(OBJS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(PROGS) *.o *.d *.a *.exe *~
```
