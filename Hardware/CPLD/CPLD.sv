
interface ISPI();
	logic		sclk;
	logic		mosi;
	logic		miso;
	logic		ssel;
	
	modport Master(output sclk, mosi, ssel, input miso);
	modport Slave(input sclk, mosi, ssel, output miso);

endinterface



module CPLD (
	// sys
	input		logic			n_softrst_i,
	input		logic			cpld_ssel_i,
	input		logic[1:0]	fpga_sel_i,
	
	// vdac
	input		logic			dac_clk_i,
	input		logic[15:0]	vdat_i,
	input		logic			vsync_i,
	input		logic			hsync_i,
	
	output	logic[7:0]	dac_r_o,
	output	logic[7:0]	dac_g_o,
	output	logic[7:0]	dac_b_o,
	
	// spi
	ISPI.Slave				arm,
	ISPI.Master				eps32,
	ISPI.Master				sd0,
	ISPI.Master				sd1,
	ISPI.Slave				fpga_master,
	ISPI.Master				fpga_slave

);



endmodule
