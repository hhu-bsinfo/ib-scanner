# IbPerfMonitor

A monitoring tool for Infiniband networks.

This project uses the libibmad- and libibnetdisc-libraries to automatically discover all Infiniband devices in a
network and read their performance counters.

# Build instructions

This project supports Linux only. It uses the libibmad- and libibnetdisc-libraries. CMake is used to generate build
scripts. To compile everything, just run *build.sh*.

# Run instructions

To start the project, just execute the compiled binary *IbPerfMon* with root privileges.

On a Debian-based system, you can run theses commands to build and run the project:

```
sudo apt install cmake libibmad-dev libibumad-dev libibnetdisc-dev libopensm-dev
./build.sh
sudo ./build/bin/IbPerfMon
```