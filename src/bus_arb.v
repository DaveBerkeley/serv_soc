
   /*
    *   iBus Arbitration
    */

module bus_arb(
    input wire wb_clk,
    // Device A
    input wire a_cyc,
    input wire [31:0] a_adr,
    output wire a_ack,
    output wire [31:0] a_rdt,
    // Device B
    input wire b_cyc,
    input wire [31:0] b_adr,
    output wire b_ack,
    output wire [31:0] b_rdt,
    // Controlled Device
    output wire x_cyc,
    output wire [31:0] x_adr,
    input wire x_ack,
    input wire [31:0] x_rdt,
    output wire busy
);

    reg dev_a = 0; // Device A granted control
    reg dev_b = 0; // Device B granted control
    reg pause = 0; // Pause after x_ack
    wire cycle_busy;

    assign cycle_busy = dev_a | dev_b | pause;
    assign busy = cycle_busy | a_cyc | b_cyc;

    // Detect start of device request
    wire a_start;
    assign a_start = a_cyc & !cycle_busy;
    wire b_start;
    assign b_start = b_cyc & !(a_cyc | cycle_busy);

    always @(posedge wb_clk) begin

        if (x_ack) begin
            // End of request, clear busy
            dev_a <= 0;
            dev_b <= 0;
        end

        pause <= x_ack;

        if (a_start) begin
            // Grant the bus to dev A
            dev_a <= 1;
        end

        if (b_start) begin
            // Grant the bus to dev A
            dev_b <= 1;
        end

    end

    // TODO : can this cause a glitch on the adr lines?
    assign x_cyc = (dev_a & a_cyc) | (dev_b & b_cyc) | ((a_cyc | b_cyc) & !cycle_busy);
    assign x_adr = ((a_start | dev_a) ? a_adr : 0) | ((b_start | dev_b) ? b_adr : 0);

    assign a_ack = dev_a & x_ack;
    assign b_ack = dev_b & x_ack;

    assign a_rdt = dev_a ? x_rdt : 0;
    assign b_rdt = dev_b ? x_rdt : 0;

endmodule

