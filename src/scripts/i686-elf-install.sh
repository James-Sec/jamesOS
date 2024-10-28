#!/bin/bash
# BASED ON osdev TUTORIAL: https://wiki.osdev.org/GCC_Cross-Compiler#Preparing_for_the_build
# DEPENDENCIES MUST BE ISNTALLED SEPARATELY
export PREFIX="$PWD/i686-elf/opt/cross"
export TARGET=i686-elf
export PATH="$PREFIX/bin:$PATH"
export CXXCPP=/usr/bin/cpp

binutils_ver=$1
gcc_ver=$2
gdb_ver=$3
#CHECK IF VERSION EXISTS/CORRECT USAGE
if [ ${#1} -eq 0 ]
then
  binutils_ver="2.36.1"
fi
if [ ${#2} -eq 0 ]
then
  gcc_ver="10.2.0"
fi
if [ ${#3} -eq 0 ]
then
  gdb_ver="10.1"
fi
binutils_file=$(curl -I https://ftp.gnu.org/gnu/binutils/binutils-$binutils_ver.tar.bz2 | grep "200 OK")
gcc_file=$(curl -I https://ftp.mirrorservice.org/sites/sourceware.org/pub/gcc/releases/gcc-$gcc_ver/gcc-$gcc_ver.tar.xz | grep "200 OK")
gdb_file=$(curl -I https://ftp.gnu.org/gnu/gdb/gdb-$gdb_ver.tar.gz | grep "200 OK")
if [ ${#binutils_file} -gt 0 ]
then
  # DOWNLOADING BINUTILS
  #wget https://ftp.gnu.org/gnu/binutils/binutils-$binutils_ver.tar.bz2 -O binutils-$binutils_ver.tar.bz2
  printf "Binutils downloaded successfully\n"
else
  printf "Could not get the binutils file\n"
  printf "Usage: ./$0 <binutils_ver> <gcc_ver>\n"
  exit
fi
if [ ${#gcc_file} -gt 0 ]
then
  # DOWNLOADING GCC
  #wget https://ftp.mirrorservice.org/sites/sourceware.org/pub/gcc/releases/gcc-$gcc_ver/gcc-$gcc_ver.tar.xz -O gcc-$gcc_ver.tar.xz
  printf "Gcc downloaded successfully\n"
else
  printf "Could not get the gcc file\n"
  printf "Usage: ./$0 <binutils_ver> <gcc_ver>\n"
  exit
fi
if [ ${#gdb_file} -gt 0 ]
then
  # DOWNLOADING GDB
  wget https://ftp.gnu.org/gnu/gdb/gdb-$gdb_ver.tar.gz -O gdb-$gdb_ver.tar.gz
  printf "Gdb downloaded successfully\n"
else
  printf "Could not get the gdb file\n"
  printf "Usage: ./$0 <binutils_ver> <gcc_ver> <gdb_ver>\n"
  exit
fi

##BUILDING BINUTILS
#tar -jxvf binutils-$binutils_ver.tar.bz2
#mkdir build-binutils
#cd build-binutils
#../binutils-$binutils_ver/configure --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror
#make
#make install
#cd ..
#
##BUILDING GCC
#tar -xvf gcc-$gcc_ver.tar.xz
#which -- $TARGET-as || echo $TARGET-as is not in the PATH
#mkdir build-gcc
#cd build-gcc
#../gcc-$gcc_ver/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --enable-languages=c,c++ --without-headers
#make all-gcc
#make all-target-libgcc
#make install-gcc
#make install-target-libgcc
#cd ..

#BUILDING GDB
tar -xvf gdb-$gdb_ver.tar.gz
which -- $TARGET-as || echo $TARGET-as is not in the PATH
mkdir build-gdb
cd build-gdb
../gdb-$gdb_ver/configure --target=$TARGET --prefix="$PREFIX" --disable-werror --with-python=/usr/bin/python3
make all-gdb
make install-gdb
cd ..

#Copying to /opt/i686
sudo cp -r i686-elf/opt/cross /opt/i686-elf

#cleaning
printf "PLEASE, ONLY CLEAN IF CP COMMAND WAS SUCCESSFULL\n"
rm -r -I --one-file-system binutils* build* gcc* gdb*

printf "FILES INSTALLED UNDER /opt/i686-elf/\n"
printf "Please, add /opt/i686-elf/bin to your \$PATH\n"
