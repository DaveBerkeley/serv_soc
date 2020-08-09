
SERV SoC
========

Please see my [blog page](https://www.rotwang.co.uk/projects/serv.html) for more information.

SoC based on SERV, Olof Kindgren's bit-serial RISC-V processor. It provides Execute in Place (XiP) from Flash. This improves the usability of the CPU core, at the cost of added complexity and slower execution speed.

Instead of fetching code from DP_RAM, the SoC has a SPI interface that reads the Flash chip. Instruction fetches from the processor are directed to the Flash.

Written in Verilog, targetted initially at the 
[Icebreaker](https://1bitsquared.com/products/icebreaker) 
FPGA development board.

SERV is the World's smallest RISC-V processor. See [github](https://github.com/olofk/serv) for more details. This clever design uses a bit-serial architecture and some cunning coding to shrink the footprint down to a few hundred LUTs.

I wanted to add XiP to allow program data to be stored in flash rather than preloaded into DPRAM. This gives much more code space and faster turn around for software development - as it removes the need to re-synthesise on every C code change. I've also used SPRAM for CPU RAM. This frees up valuable DPRAM for use by whatever else you are building.

SERV uses a stripped down 
[Wishbone](https://en.wikipedia.org/wiki/Wishbone_(computer_bus))
bus.

----

Make
----

To build the code you need to be able to build SERV. See the build instructions there.

You need the risc-v **gcc** cross compiler tools. I used C Wolf's instructions 
[here](https://github.com/cliffordwolf/picorv32#building-a-pure-rv32i-toolchain).

You will need also need the synthesis and place-and-route tools, **yosys**, **nextpnr-ice40**.

For development I also use **verilator**, as a lint tool, **vvp** to run test scripts and **gtkwave** to view the resulting waveforms.

You need to set the environment variable SERV to point at your copy of the SERV git repository.

You will also need the **gcc** risc-v tool chain on your PATH.

eg:

    export SERV=~/Desktop/serv
    export PATH=/opt/riscv32i/bin:$PATH

You can then run

    make all

To build and program just the C code, run

    make prog_firmware

There is an asciinema of the process 
[here](https://asciinema.org/a/351518)

I've added a 64-bit counter / timer, which is greedy in terms of resources. There is a also a simple Tx-only UART set to 115200 Baud. 
Plus an interrupt controller and simple GPIO.
This should give the basics of what people need to build their own SoCs.

I've added code to allow **malloc()** and **free()** to work,
the linker script icebreaker_sections.lds is based on code in 
Claire Wolf's excellent
[picosoc processor](https://github.com/cliffordwolf/picorv32/tree/master/picosoc).

----

TODO

* use a higher clock speed to drive the Flash SPI interface. The chip can go up to 50MHz. This will improve the ibus fetch rate by up to 4 times.
* use DSPI or QSPI to fetch data from the Flash. This could double or quadruple the ibus fetch speed.
* Add DMA, to allow the host RAM space to be shared by other parts of the FPGA.

----

I'd like to thank Olof Kindgren and Claire Wolf for their amazing work.
The availability of open source CPU cores open up a whole world of
possibilities for anyone doing FPGA development. 
Reading their code and using their work has been 
both educational and inspirational.

----

