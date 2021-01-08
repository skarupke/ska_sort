#!/bin/sh

if [ -d prefix ]; then
    exit 0
fi

set -eux

git clone https://github.com/onqtam/doctest doctest -b 2.4.4 --depth 1 -q \
    2>/dev/null # cloning with -b is very noisy, just void it

cmake -S doctest -B doctest/build \
    -D CMAKE_BUILD_TYPE=Release \
    -D DOCTEST_WITH_MAIN_IN_STATIC_LIB=NO \
    -D DOCTEST_WITH_TESTS=NO

cmake --build doctest/build --config Release

cmake --install doctest/build --config Release --prefix prefix

rm -rf doctest
