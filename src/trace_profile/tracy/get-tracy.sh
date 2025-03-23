#!/bin/bash

# -- DOWNLOAD -------------------------------------------------------------------

test -f tracy.json || curl -s -L https://api.github.com/repos/wolfpld/tracy/releases/latest > tracy.json
TRACY_URL=$(cat tracy.json | jq -r .tarball_url)
TRACY_TAG=$(cat tracy.json | jq -r .tag_name)

echo TRACY_URL $TRACY_URL
echo TRACY_TAG $TRACY_TAG

if ! test -d tracy; then
    wget --no-clobber -O tracy-$TRACY_TAG.tgz $TRACY_URL
    mkdir tracy
    tar xf tracy-$TRACY_TAG.tgz -C tracy --strip-component 1
    echo $TRACY_TAG > tracy/VERSION
elif ! test $TRACY_TAG = $(cat tracy/VERSION); then
    echo -e "\e[1;33mWARN\e[0m: new tracy version available have:$(cat tracy/VERSION) new:$TRACY_TAG"
fi

export CC=$(which clang)
export CXX=$(which clang++)

# -- BUILD PROFILER -------------------------------------------------------------

if ! test -d build-profiler; then
    # Legacy -> x11
    cmake -B build-profiler -S tracy/profiler -DLEGACY=ON
fi

cmake --build build-profiler -j$(nproc)

# -- BUILD CAPTURE --------------------------------------------------------------

if ! test -d build-capture; then
    cmake -B build-capture -S tracy/capture
fi

cmake --build build-capture/ -j$(nproc)
