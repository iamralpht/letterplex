#!/bin/sh

mkdir out
mkdir work

export LP_ROOT=`pwd`
export LP_OUT=`pwd`/out
export LP_WORK=`pwd`/work

export COMMON_FLAGS="-fPIC -I$LP_OUT/include -O3 -mfloat-abi=softfp -mfpu=vfpv3-d16 -mcpu=cortex-a9 -mthumb"
export CFLAGS="-std=gnu99 $COMMON_FLAGS -I$LP_OUT/lib/libffi-3.1/include"
export CXXFLAGS="-std=c++11 $COMMON_FLAGS"
export LDFLAGS="-L$LP_OUT/lib"

export AR=arm-linux-androideabi-ar
export LD=arm-linux-androideabi-ld
export CXX=arm-linux-androideabi-g++
export CC=arm-linux-androideabi-gcc
export RANLIB=arm-linux-androideabi-ranlib

CONFIGURE_ARGS="--host=arm-linux-androideabi --target=arm-linux-androideabi --with-sysroot=$ANDROID_NDK/platforms/android-9/arch-arm --prefix=$LP_OUT --cache=android.cache --enable-static --disable-shared"

mkdir -p $LP_OUT/bin
mkdir -p $LP_OUT/lib/pkgconfig
mkdir -p $LP_OUT/man/man1
mkdir -p $LP_OUT/include

export PKG_CONFIG_PATH=$LP_OUT/lib/pkgconfig


cd $LP_WORK
rm -rf libiconv-1.14
tar zxf $LP_ROOT/libiconv-1.14.tar.gz
cd $LP_WORK/libiconv-1.14
cp $LP_ROOT/config.sub build-aux
cp $LP_ROOT/config.guess build-aux
./configure $CONFIGURE_ARGS && make && make install
for i in `ls $LP_OUT/lib/*.la`; do sed s/=\\//\\//g < $i > $i.noeq; mv $i.noeq $i; done


cd $LP_WORK
rm -rf tinylibintl
tar zxf $LP_ROOT/tinylibintl.tar.gz
cd $LP_WORK/tinylibintl
make install

cd $LP_WORK
rm -rf libffi-3.1
tar zxf $LP_ROOT/libffi-3.1.tar.gz
cd libffi-3.1
./configure $CONFIGURE_ARGS && make && make install
for i in `ls $LP_OUT/lib/*.la`; do sed s/=\\//\\//g < $i > $i.noeq; mv $i.noeq $i; done

cd $LP_WORK
rm -rf glib-2.41.3
tar Jxf $LP_ROOT/glib-2.41.3.tar.xz
cd glib-2.41.3
cp $LP_ROOT/android.cache .
./configure $CONFIGURE_ARGS && make && make install
for i in `ls $LP_OUT/lib/*.la`; do sed s/=\\//\\//g < $i > $i.noeq; mv $i.noeq $i; done

cd $LP_WORK
rm -rf expat-2.1.0
tar zxf $LP_ROOT/expat-2.1.0.tar.gz
cd expat-2.1.0
./configure $CONFIGURE_ARGS && make && make install
for i in `ls $LP_OUT/lib/*.la`; do sed s/=\\//\\//g < $i > $i.noeq; mv $i.noeq $i; done

cd $LP_WORK
rm -rf freetype-2.5.3
tar zxf $LP_ROOT/freetype-2.5.3.tar.gz
cd freetype-2.5.3
./configure $CONFIGURE_ARGS --with-png=no --with-bzip2=no --with-harfbuzz=no && make -j && make install
for i in `ls $LP_OUT/lib/*.la`; do sed s/=\\//\\//g < $i > $i.noeq; mv $i.noeq $i; done

cd $LP_WORK
rm -rf harfbuzz-0.9.35
tar jxf $LP_ROOT/harfbuzz-0.9.35.tar.bz2
cd harfbuzz-0.9.35
./configure $CONFIGURE_ARGS --with-icu=no --with-cairo=no && make -j && make install
for i in `ls $LP_OUT/lib/*.la`; do sed s/=\\//\\//g < $i > $i.noeq; mv $i.noeq $i; done

cd $LP_WORK
rm -rf fontconfig-2.11.1
tar zxf $LP_ROOT/fontconfig-2.11.1.tar.gz
cd fontconfig-2.11.1
cp $LP_ROOT/fcxml.c src/
./configure $CONFIGURE_ARGS --with-cache-dir="./fontcache/" --with-baseconfigdir="./fontconfig" && make && make install
for i in `ls $LP_OUT/lib/*.la`; do sed s/=\\//\\//g < $i > $i.noeq; mv $i.noeq $i; done

cd $LP_WORK
rm -rf pango-1.36.6
tar Jxf $LP_ROOT/pango-1.36.6.tar.xz
cd pango-1.36.6
./configure $CONFIGURE_ARGS --without-xft --without-cairo --with-included-modules=yes --enable-introspection=no && make && make install
