#!/bin/bash
# BASED ON osdev TUTORIAL: https://wiki.osdev.org/GCC_Cross-Compiler#Preparing_for_the_build
# DEPENDENCIES MUST BE ISNTALLED SEPARATELY
export PREFIX="$PWD/i686-elf/opt/cross"
export TARGET=i686-elf
export PATH="$PREFIX/bin:$PATH"

#CHECK IF VERSION EXISTS/CORRECT USAGE
binutils_file=$(curl -I https://ftp.gnu.org/gnu/binutils/binutils-$1.tar.bz2 | grep "200 OK")
gcc_file=$(curl -I ftp://ftp.mirrorservice.org/sites/sourceware.org/pub/gcc/releases/gcc-$2/gcc-$2.tar.xz | grep "200 OK")
if [ ${#binutils_file} -gt 0 ]
then
  # DOWNLOADING BINUTILS
  wget https://ftp.gnu.org/gnu/binutils/binutils-$1.tar.bz2 -O binutils-$1.tar.bz2
  printf "Binutils downloaded successfully\n"
else
  printf "Could not get the binutils file\n"
  printf "Usage: ./$0 <binutils_ver> <gcc_ver>\n"
  exit
fi
if [ ${#binutils_file} -gt 0 ]
then
  # DOWNLOADING GCC
  wget ftp://ftp.mirrorservice.org/sites/sourceware.org/pub/gcc/releases/gcc-$2/gcc-$2.tar.xz -O gcc-$2.tar.xz
  printf "Gcc downloaded successfully\n"
else
  printf "Could not get the gcc file\n"
  printf "Usage: ./$0 <binutils_ver> <gcc_ver>\n"
  exit
fi

#BUILDING BINUTILS
tar -jxvf binutils-$1.tar.bz2
mkdir build-binutils
cd build-binutils
../binutils-$1/configure --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror
make
make install
cd ..

#BUILDING GCC
tar -xvf gcc-$2.tar.xz
which -- $TARGET-as || echo $TARGET-as is not in the PATH
mkdir build-gcc
cd build-gcc
../gcc-$2/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --enable-languages=c,c++ --without-headers
make all-gcc
make all-target-libgcc
make install-gcc
make install-target-libgcc
cd ..

printf "FILES INSTALLED UNDER $PWD/opt/cross/\n"
