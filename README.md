# randomx-stress

Cook your computer.

## Usage

    randomx-stress [-t <threads>] [-H]

Options:

* **-t** - Set the number of mining threads (see FAQ for optimal amount)
* **-H** - Allocate RandomX dataset with hugepages

Both flags are optional.
Defaults to using half of your CPUs threads with no huge pages.

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

### How many threads should I use?

It depends on the amount of cache your CPU has.

Take the amount of L3 cache (in MiB) and divide by two to find the optimal
number of threads.
e.g. The Ryzen 1700 has 16 MiB of L3 cache in total, so 8 threads should be
used.

### Why do I get a "failed to allocate x" exception?

Ensure that you have huge pages enabled in your operating system if
you pass the `-H` option, and that you have enough free memory.

##### Windows

## Resources

* [integralfx's DDR4 OC Guide](https://github.com/integralfx/MemTestHelper/blob/oc-guide/DDR4%20OC%20Guide.md)
* [Overclocking Discord](https://discord.gg/pRTpebr)

## License

Released under the terms of the MIT license.
See LICENSE file in project root for more details.
