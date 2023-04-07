# Cloning with Submodules and Building for the First Time
 1. ```git clone --recurse-submodules https://github.com/StephenBerkner/Renode-STM32-Hello-World.git``` (If you skipped the ```--recurse-submodules``` it can be fixed with ```git submodule update --init```)
 2. ```cd Renode-STM32-Hello-World```
 3. ```make -C libopencm3 # (Only needed once)```
 4. ```make -C d5-hello-world```

The last step will produce d5-hello-world/d5-hello-world.elf which should be placed in your renode install location as follows:

Renode-Install-Folder:
    - renode binary
    - d5-hello-world.elf
    - ...

# Source Code Directories
* d5-hello-world contains firmware for generating elf to be loaded onto Renode Emulator
* libopencm3 contains libopencm3 submodule (https://github.com/libopencm3)
* common contains common code needed by both libopencm3 and d5-hello-world firmware


# Fixing Issues with the Renode Memory ModelModel
In the STM32F4 memory map, 0x10XXXXXX is in the CCM region which is NOT implemented by Renode. To fix this we will add CCM RAM at 0x10000000 by creating a repl file with the following:

ccm: Memory.MappedMemory @ sysbus 0x10000000
    size: 0x10000

For conveinience this file has been include in the include directory and can simply be copied into the appropriate location. This repl file should be placed in your renode install location as follows:

Renode-Install-Folder:
    - renode binary
    - d5-hello-world.elf
    - ccm.repl
    - ...

This repl file will need to be added to the machine order to get our program to run. Since it is directly in the install directory we can add it to our Renode machine from the monitor via `machine LoadPlatformDescription @ccm.repl`


# Running our Firwmare on the Renode Emulator
(monitor) mach create
(machine-0) machine LoadPlatformDescription @platforms/boards/stm32f4_discovery-kit.repl
(machine-0) sysbus LoadELF @d5-hello-world.elf
(machine-0) machine LoadPlatformDescription @ccm.repl
(machine-0) showAnalyzer sysbus.uart2
(machine-0) start