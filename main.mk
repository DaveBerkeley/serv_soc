
all: $(PROJ).rpt $(PROJ).bin

%.blif: %.v $(ADD_SRC) $(ADD_DEPS)
	yosys -ql $*.log $(if $(USE_ARACHNEPNR),-DUSE_ARACHNEPNR) -p 'synth_ice40 -top top -blif $@' $< $(ADD_SRC)

VERILATOR_SRC?=$(ADD_SRC)

%.json: %.v $(ADD_SRC) $(ADD_DEPS)
	verilator --top-module top $(ADD_VERILATOR) $< $(VERILATOR_SRC) /usr/share/yosys/ice40/cells_sim.v --lint-only -Wall 
	yosys -ql $*.log $(if $(USE_ARACHNEPNR),-DUSE_ARACHNEPNR) -p 'synth_ice40 $(ADD_YOSYS) -top top -json $@' $< $(ADD_SRC)

ifeq ($(USE_ARACHNEPNR),)
%.asc: $(PIN_DEF) %.json
	nextpnr-ice40 --$(DEVICE) $(if $(PACKAGE),--package $(PACKAGE)) $(if $(FREQ),--freq $(FREQ)) --json $(filter-out $<,$^) --pcf $< --asc $@
else
%.asc: $(PIN_DEF) %.blif
	arachne-pnr -d $(subst up,,$(subst hx,,$(subst lp,,$(DEVICE)))) $(if $(PACKAGE),-P $(PACKAGE)) -o $@ -p $^
endif


%.bin: %.asc
	icepack $< $@

%.rpt: %.asc
	icetime $(if $(FREQ),-c $(FREQ)) -d $(DEVICE) -mtr $@ $<

%_tb: %_tb.v %.v $(TB_DEPS)
	verilator --top-module top $(ADD_VERILATOR) $(TB_VERILATOR) $(TB_SRC) $(PROJ).v --lint-only -Wall
	iverilog -DSIMULATION -g2012 $(ADD_IVERILOG) -o $@ $(TB_SRC) $@.v $(ADD_TB_IVERILOG)
	./$@
	#gtkwave $(PROJ).vcd $@.gtkw &

%_tb.vcd: %_tb
	vvp -N $< +vcd=$@

%_syn.v: %.blif
	yosys -p 'read_blif -wideports $^; write_verilog $@'

%_syntb: %_tb.v %_syn.v
	verilator --top-module top $< $(ADD_SRC) --lint-only -Wall -Wno-DECLFILENAME
	iverilog -o $@ $^ `yosys-config --datdir/ice40/cells_sim.v`

%_syntb.vcd: %_syntb
	vvp -N $< +vcd=$@

prog: $(PROJ).bin
	iceprog $<

clean:
	rm -f $(PROJ).blif $(PROJ).asc $(PROJ).rpt $(PROJ).bin $(PROJ).json $(PROJ).log *.vvp *.vcd $(ADD_CLEAN) *~

.SECONDARY:
.PHONY: all prog clean
