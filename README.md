# PlanX

A modern and next gen development kernel

## Setup
1. Install development tools
```
sudo apt install build-essential nasm make gdb
```

2. Install emulation tool to run os
```
sudo apt install qemu-system
```

## Usage


| :exclamation:  Since there is no init process to engage with, it’s recommended to use gdb for running kernel.   |
|----------------------------------------------|
1. Run kernel in QEMU
```
make run
```

2. Run kernel and drop into gdb
```
make debug
```
drop into gdb by
```
sudo gdb kernel.elf
```

3. Clean up
```
make clean
```

## Architecture
![kernel](https://github.com/itsankitkp/PlanX/assets/93041495/fa7936b0-3807-4470-9616-84f08218742e)


