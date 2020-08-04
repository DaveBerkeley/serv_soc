.section .text

start:

# zero-initialize register file
addi x1, zero, 0
#
addi x3, zero, 0
addi x4, zero, 0
addi x5, zero, 0
addi x6, zero, 0
addi x7, zero, 0
addi x8, zero, 0
addi x9, zero, 0
addi x10, zero, 0
addi x11, zero, 0
addi x12, zero, 0
addi x13, zero, 0
addi x14, zero, 0
addi x15, zero, 0
addi x16, zero, 0
addi x17, zero, 0
addi x18, zero, 0
addi x19, zero, 0
addi x20, zero, 0
addi x21, zero, 0
addi x22, zero, 0
addi x23, zero, 0
addi x24, zero, 0
addi x25, zero, 0
addi x26, zero, 0
addi x27, zero, 0
addi x28, zero, 0
addi x29, zero, 0
addi x30, zero, 0
addi x31, zero, 0

# Point sp to the end of RAM
la sp, _estack

# Copy initialised data from flash to RAM
#
# ibus_read device is mapped to 0x70000000

# Write the address to flash bridge dev
li a0, 0x70000000 
la a1, _sidata # start of .data section in ROM
sw a1, 0(a0)

la a2, _sdata # start of .data section in RAM
la a3, _edata # end of .data section in RAM
bge a2, a3, end_init_data
loop_init_data:
# read data from ROM. Addr is incremented in h/w.
lw a1, 0(a0)
# save in RAM
sw a1, 0(a2)
addi a2, a2, 4
blt a2, a3, loop_init_data

end_init_data:


# zero-init bss section
la a0, _sbss
la a1, _ebss
bge a0, a1, end_init_bss
loop_init_bss:
sw zero, 0(a0)
addi a0, a0, 4
blt a0, a1, loop_init_bss
end_init_bss:

# call main

call main
loop:
j loop

# FIN
