# ib-scanner: A terminal based monitoring tool for InfiniBand

<p align="center">
<img src="logo.svg" height=175>
</p>

<p align="center">
<a href="https://travis-ci.org/hhu-bsinfo/ib-scanner"><img src="https://travis-ci.org/hhu-bsinfo/ib-scanner.svg?branch=master"></a>
  <a href="https://isocpp.org/"><img src="https://img.shields.io/badge/C++-11-blue.svg"></a>
<a href="https://github.com/hhu-bsinfo/observatory/blob/master/LICENSE"><img src="https://img.shields.io/badge/license-GPLv3-orange.svg"></a>
</p>

This project is based on <a href="https://github.com/hhu-bsinfo/detector">Detector</a>, which uses the libibmad- and libibnetdisc-libraries to automatically discover all InfiniBand devices in a
network and read their performance counters.

The goal of ib-scanner is to provide a simple to use frontend for Detector, allowing InfiniBand users to monitor multiple NICs at once and getting diagnostic information about their network.

# Build instructions

This project supports Linux only. It uses the libibmad- and libibnetdisc-libraries. CMake is used to generate build
scripts. To compile everything, just run *build.sh*.

# Run instructions

To start the project, just execute the compiled binary *scanner* with root privileges.

On a Debian-based system, you can run theses commands to build and run the project:

```
sudo apt install cmake libibmad-dev libibumad-dev libibnetdisc-dev libopensm-dev
./build.sh
sudo ./build/bin/scanner
```

It is also possible to start ib-scanner in compatibility mode, where it does not need root privileges. However, it will only monitor local HCAs, by reading their counters from the filesystem, in this mode.

```
./build/bin/scanner --mode compat
```

To get more information about the parameters, run:

````
./build/bin/scanner --help
````
