
CROSS=riscv32-unknown-elf-

ifndef SERV
S = https://github.com/olofk/serv
$(error SERV is not set, see $(S))
endif

CORE=$(SERV)/rtl

PROJ = serv
ADD_SRC = 
ADD_CLEAN = pll.v firmware.elf firmware.bin $(PROJ).bin *_tb $(VCONFIG)
ADD_YOSYS = -dsp
ADD_VERILATOR = $(VCONFIG) +incdir+$(CORE)
ADD_DEPS = pll.v $(VCONFIG)
ADD_IVERILOG = -I $(CORE)
TB_DEPS = pll.v $(VCONFIG)
TB_VERILATOR = /usr/share/yosys/ice40/cells_sim.v sim.v
ADD_TB_IVERILOG = sim.v tb.v

VCONFIG = /tmp/$(PROJ)_config.vlt

PACKAGE = sg48
DEVICE = up5k

PIN_DEF = ../icebreaker.pcf

FILES  = pll.v
FILES += reset.v
FILES += ibus.v
FILES += uart.v
FILES += spi.v
FILES += gpio.v
FILES += chip_select.v
FILES += ram_arb.v
FILES += bus_arb.v
FILES += timer.v
FILES += irq.v

HW_FILES  = ice40up5k_spram.v
HW_FILES += ram.v

# CPU internals
CPU_FILES  = $(CORE)/serv_rf_top.v
CPU_FILES += $(CORE)/serv_rf_ram_if.v
CPU_FILES += $(CORE)/serv_rf_ram.v
CPU_FILES += $(CORE)/serv_rf_if.v
CPU_FILES += $(CORE)/serv_mem_if.v
CPU_FILES += $(CORE)/serv_top.v
CPU_FILES += $(CORE)/serv_state.v
CPU_FILES += $(CORE)/serv_decode.v
CPU_FILES += $(CORE)/serv_bufreg.v
CPU_FILES += $(CORE)/serv_ctrl.v
CPU_FILES += $(CORE)/serv_alu.v
CPU_FILES += $(CORE)/serv_shift.v
CPU_FILES += $(CORE)/serv_csr.v

ADD_SRC+=$(FILES) $(CPU_FILES) $(HW_FILES)
TB_SRC = $(CPU_FILES) $(FILES)


include ../main.mk

prog_firmware: firmware.bin
	iceprog -o 1M $^

all: prog_firmware prog

pll.v : Makefile
	icepll -o 32 -m -f $@
	# Change *_CORE macro to *_PAD macro
	# See SiliconBlue ICE TM Technology Library Version 2.3
	sed -i 's/SB_PLL40_CORE/SB_PLL40_PAD/g' $@
	sed -i 's/.REFERENCECLK/.PACKAGEPIN/g' $@

firmware.bin: firmware.elf
	$(CROSS)objcopy -S -O binary $^ $@

CFLAGS  = -DICEBREAKER
CFLAGS += -march=rv32i
CFLAGS += -ffreestanding
CFLAGS += -nostartfiles
CFLAGS += -Wall -Werror
CFLAGS += -O1
LFLAGS = -Wl,-Bstatic,-T,icebreaker_sections.lds,--strip-debug 

firmware.elf: firmware.c start.s soc.c Makefile icebreaker_sections.lds
	$(CROSS)g++ $(CFLAGS) $(LFLAGS) -o $@ start.s firmware.c soc.c

$(VCONFIG): config.vlt
	envsubst < $^ > $@

tb: serv_tb bus_arb_tb ibus_tb uart_tb reset_tb spi_tb timer_tb irq_tb ram_arb_tb

#	FIN
