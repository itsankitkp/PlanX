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
1. Run kernel in QEMU
```
make run
```

2. Run kernel and drop into gdb
```
make debug
```

3. Clean up
```
make clean
```

