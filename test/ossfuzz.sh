# This script is meant to be run by
# https://github.com/google/oss-fuzz/blob/master/projects/libspectre/Dockerfile

wget https://github.com/ArtifexSoftware/ghostpdl-downloads/releases/download/gs950/ghostscript-9.50.tar.gz
tar xvzf ghostscript-9.50.tar.gz
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
