# randomx-stress

Cook your computer.

## Build Guide

### Linux

1. Download, build, and install
   [librandomx](https://github.com/tevador/randomx)

        git clone https://github.com/tevador/randomx.git
        mkdir randomx/build
        cd $_
        cmake .. -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=On
        make
        sudo cp librandx.so /usr/local/lib64/
        sudo cp ../src/randomx.h /usr/local/include/
        sudo ldconfig

2. Download, build, and run randomx-stress

        git clone https://github.com/00-matt/randomx-stress.git
        mkdir randomx-stress/build
        cd $_
        cmake .. -DCMAKE_BUILD_TYPE=Release
        make
        ./randomx-stress 8 # or however many threads you want

## FAQ

### Why do I get a "failed to allocate x" exception?

Ensure that you have huge pages enabled, and that you have enough free
memory.

## License

Released under the terms of the MIT license.
See LICENSE file in project root for more details.
