# This script is meant to be run by
# https://github.com/google/oss-fuzz/blob/master/projects/libspectre/Dockerfile

cd ghostscript-9.50
./configure
make -j$(nproc) soinstall
make -j$(nproc) libgs
cd ..
rm /usr/local/lib/libgs.so*
cp ghostscript-9.50/bin/gs.a /usr/local/lib/libgs.a

./autogen.sh --enable-static --disable-shared
make -j$(nproc)

$CXX $CXXFLAGS $SRC/libspectre/test/spectre_read_fuzzer.c -I. \
    -o $OUT/spectre_read_fuzzer \
    $LIB_FUZZING_ENGINE $SRC/libspectre/libspectre/.libs/libspectre.a \
    $SRC/libspectre/ghostscript-9.50/bin/gs.a

find $SRC/libspectre/ghostscript-9.50 -name "*.ps" | \
     xargs zip $OUT/spectre_read_fuzzer_seed_corpus.zip

cp $SRC/libspectre/test/postscript.dict $OUT/spectre_read_fuzzer.dict

# Needed for coverage builds
cd ghostscript-9.50/obj

ln -s ../jpeg/jdapistd.c jdapistd.c
ln -s ../jpeg/jdarith.c jdarith.c
ln -s ../jpeg/jdcoefct.c jdcoefct.c
ln -s ../jpeg/jdcolor.c jdcolor.c
ln -s ../jpeg/jddctmgr.c jddctmgr.c
ln -s ../jpeg/jdhuff.c jdhuff.c
ln -s ../jpeg/jdinput.c jdinput.c
ln -s ../jpeg/jdmainct.c jdmainct.c
ln -s ../jpeg/jdmarker.c jdmarker.c
ln -s ../jpeg/jdmaster.c jdmaster.c
ln -s ../jpeg/jdpostct.c jdpostct.c
ln -s ../jpeg/jdsample.c jdsample.c
ln -s ../jpeg/jerror.c jerror.c
ln -s ../jpeg/jcsample.c jcsample.c
ln -s ../jpeg/jidctint.c jidctint.c
ln -s ../jpeg/jcarith.c jcarith.c
ln -s ../jpeg/jmemmgr.c jmemmgr.c
ln -s ../jpeg/jcdctmgr.c jcdctmgr.c
ln -s ../jpeg/jcinit.c jcinit.c
ln -s ../jpeg/jutils.c jutils.c
ln -s ../jpeg/jcmaster.c jcmaster.c
ln -s ../jpeg/jcparam.c jcparam.c
ln -s ../jpeg/jfdctint.c jfdctint.c
ln -s ../jpeg/jcapistd.c jcapistd.c
ln -s ../jpeg/jccoefct.c jccoefct.c
ln -s ../jpeg/jchuff.c jchuff.c
ln -s ../jpeg/jcmarker.c jcmarker.c
ln -s ../jpeg/jcomapi.c jcomapi.c
ln -s ../jpeg/jdapimin.c jdapimin.c
ln -s ../jpeg/jcapimin.c jcapimin.c
ln -s ../jpeg/jccolor.c jccolor.c
ln -s ../jpeg/jcprepct.c jcprepct.c
ln -s ../jpeg/jcmainct.c jcmainct.c

cd ../..