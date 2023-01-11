
typedef enum logic [2:0] {NoHost, MCU, FPGA} SelHost;
typedef enum logic [3:0] {NoSlave, SD0, SD1, ESP32, FPGA_S} SelSlave;


module CPLD (
    // sys
    input	logic			n_softrst_i,
    input	logic			cpld_ssel_i,
    input	logic[1:0]		fpga_sel_i,
    
    // vdac
    input	logic			dac_clk_i,
    input	logic[15:0]		vdat_i,
    output	logic			vsync_i,
    output	logic			hsync_i,
    
    output	logic[7:0]	    dac_r_o,
    output	logic[7:0]	    dac_g_o,
    output	logic[7:0]	    dac_b_o,
    
    // mcu spi slave ports (use in video mode)
    input	logic			mcu_sclk,	// r0
    input	logic			mcu_mosi,	// g1
    input	logic			mcu_ssel,	// r1
    inout	logic			mcu_miso,	// g0
    
    // fpga spi slave ports
    input	logic			fpga_m_sclk,
    input	logic			fpga_m_mosi,
    input	logic			fpga_m_ssel,
    output	logic			fpga_m_miso,
    
    // sd0 spi master ports
	output	logic			sd0_sclk,
    output	logic			sd0_mosi,
    output	logic			sd0_ssel,
    input	logic			sd0_miso,
    
    // sd1 spi master ports
    output	logic			sd1_sclk,
    output	logic			sd1_mosi,
    output	logic			sd1_ssel,
    input	logic			sd1_miso,
    
    // esp32 spi master ports (use in video mode)
    inout	logic			esp32_sclk, // b0
    inout	logic			esp32_mosi, // h-sync
    inout	logic			esp32_ssel, // b1
    input	logic			esp32_miso, // v-sync
        
    // fpga spi master ports
    output	logic			fpga_s_sclk,
    output	logic			fpga_s_mosi,
    output	logic			fpga_s_ssel,
    input	logic			fpga_s_miso
);

wire		sd0_sel, sd1_sel, video_mode;
wire[1:0]	esp32_slave_sel;

spi_config spi_config_inst(
	.n_softrst_i	(n_softrst_i),
	.cpld_ssel_i	(cpld_ssel_i),
	
	.mcu_sclk		(mcu_sclk),
	.mcu_ssel		(mcu_ssel),
	.mcu_mosi		(mcu_mosi),
	
	.sd0_sel		(sd0_sel),
	.sd1_sel		(sd1_sel),
	.esp32_slave_sel(esp32_slave_sel),
	.video_mode		(video_mode)
);

sdcard_mux sdcard_mux_inst(
	.vmode		(video_mode),
	.sd0_sel	(sd0_sel),
	.sd1_sel	(sd1_sel),
	.esp_sel	(esp32_slave_sel),
	.fpga_sel	(fpga_sel_i),
	
	.mcu_sclk	(mcu_sclk),
	.mcu_mosi	(mcu_mosi),
	.mcu_ssel	(mcu_ssel),
	
	.fpga_sclk	(fpga_m_sclk),
	.fpga_mosi	(fpga_m_mosi),
	.fpga_ssel	(fpga_m_ssel),
	
	.sd0_sclk	(sd0_sclk),
	.sd0_mosi	(sd0_mosi),
	.sd0_ssel	(sd0_ssel),
	
	.sd1_sclk	(sd1_sclk),
	.sd1_mosi	(sd1_mosi),
	.sd1_ssel	(sd1_ssel)
);


// SPI MUX *****************************************

wire	fpga_mcu_sel_n 	 = !(esp32_slave_sel == 2'b10 && video_mode == 1'b0);
wire	eps32_fpga_sel_n = !(fpga_sel_i == 2'b10 && video_mode == 1'b0);


// fpga slave
assign	fpga_s_sclk = mcu_sclk | fpga_mcu_sel_n;
assign	fpga_s_mosi = mcu_mosi | fpga_mcu_sel_n;
assign	fpga_s_ssel = mcu_ssel | fpga_mcu_sel_n;

// esp32 slave
assign	esp32_sclk  = (video_mode == 1'b0) ? (fpga_m_sclk | eps32_fpga_sel_n) : 1'bz;
assign	esp32_mosi  = (video_mode == 1'b0) ? (fpga_m_mosi | eps32_fpga_sel_n) : 1'bz;
assign	esp32_ssel  = (video_mode == 1'b0) ? (fpga_m_ssel | eps32_fpga_sel_n) : 1'bz;

reg		mcu_miso_r, fpga_m_miso_r;
assign	fpga_m_miso = fpga_m_miso_r;
assign	mcu_miso	= (video_mode == 1'b0) ? mcu_miso_r : 1'bz; 


always_comb
begin

	casex (esp32_slave_sel)
		3'b00:		mcu_miso_r = sd0_miso;
		3'b01:		mcu_miso_r = sd1_miso;
		3'b10:		mcu_miso_r = fpga_s_miso;
		default:	mcu_miso_r = 1'b1;
	endcase
	
	case (fpga_sel_i)
		2'b00:	 fpga_m_miso_r = sd0_miso;
		2'b01:	 fpga_m_miso_r = sd1_miso;
		2'b10:	 fpga_m_miso_r = esp32_miso;
		default: fpga_m_miso_r = 1'b1;
	endcase
			
end


// VDAC *********************************************
assign		vsync_i = video_mode ? esp32_miso : 1'bz;
assign		hsync_i = video_mode ? esp32_mosi : 1'bz;

video_mux vmux(
	.video_mode(video_mode),
	.mode	(vdat_i[15]),
	
	.vdac_r	(vdat_i[4:0]),
	.vdac_g	(vdat_i[9:5]),
	.vdac_b	(vdat_i[14:10]),
	
	.esp_r	({mcu_ssel, mcu_sclk}),
	.esp_g	({mcu_mosi, mcu_miso}),
	.esp_b	({esp32_ssel, esp32_sclk}),
	
	.dac_r_o(dac_r_o),
	.dac_g_o(dac_g_o),
	.dac_b_o(dac_b_o)
);

endmodule

module spi_config(
	input	logic		n_softrst_i,
	input	logic		cpld_ssel_i,
	
	input	logic		mcu_sclk,
	input	logic		mcu_ssel,
	input	logic		mcu_mosi,
	
	output	logic		sd0_sel,
	output	logic		sd1_sel,
	output	logic[1:0]	esp32_slave_sel,
	output	logic		video_mode
);

reg [7:0]	spi_reg;
reg [7:0]	cpld_config;

always @ (negedge cpld_ssel_i or negedge n_softrst_i)
if (n_softrst_i == 1'b0)
    cpld_config  <= 0;
else
	cpld_config <= video_mode ? 8'h00 : spi_reg;

always @ (posedge mcu_sclk)
if (mcu_ssel == 1'b0 && cpld_ssel_i && video_mode == 1'b0)
    spi_reg <= {spi_reg[6:0], mcu_mosi};

assign 	sd0_sel = cpld_config[0];
assign 	sd1_sel = cpld_config[1];
assign	esp32_slave_sel = cpld_config[5:4];
assign	video_mode = cpld_config[7];

endmodule

module sdcard_mux(
	input	logic		vmode,
	input	logic		sd0_sel,
	input	logic		sd1_sel,
	input	logic[1:0]	esp_sel,
	input	logic[1:0]	fpga_sel,
	
	input	logic		mcu_sclk,
	input	logic		mcu_mosi,
	input	logic		mcu_ssel,
	
	input	logic		fpga_sclk,
	input	logic		fpga_mosi,
	input	logic		fpga_ssel,
	
	output	logic		sd0_sclk,
	output	logic		sd0_mosi,
	output	logic		sd0_ssel,
	
	output	logic		sd1_sclk,
	output	logic		sd1_mosi,
	output	logic		sd1_ssel
);

wire	sd0_mcu_sel_n    = !(sd0_sel == 0 && esp_sel == 2'b00 && vmode == 1'b0);
wire	sd1_mcu_sel_n    = !(sd0_sel == 0 && esp_sel == 2'b01 && vmode == 1'b0);

wire	sd0_fpga_sel_n   = !(sd0_sel == 1 && fpga_sel == 2'b00 && vmode == 1'b0);
wire	sd1_fpga_sel_n   = !(sd1_sel == 1 && fpga_sel == 2'b01 && vmode == 1'b0);

// sd0
assign	sd0_sclk = (mcu_sclk | sd0_mcu_sel_n) & (fpga_sclk | sd0_fpga_sel_n);
assign	sd0_mosi = (mcu_mosi | sd0_mcu_sel_n) & (fpga_mosi | sd0_fpga_sel_n);
assign	sd0_ssel = (mcu_ssel | sd0_mcu_sel_n) & (fpga_ssel | sd0_fpga_sel_n);

// sd1
assign	sd1_sclk = (mcu_sclk | sd1_mcu_sel_n) & (fpga_sclk | sd1_fpga_sel_n);
assign	sd1_mosi = (mcu_mosi | sd1_mcu_sel_n) & (fpga_mosi | sd1_fpga_sel_n);
assign	sd1_ssel = (mcu_ssel | sd1_mcu_sel_n) & (fpga_ssel | sd1_fpga_sel_n);

endmodule

module video_mux(
	input	logic		video_mode,
	input	logic		mode,
	
	input	logic[4:0]	vdac_r,
	input	logic[4:0]	vdac_g,
	input	logic[4:0]	vdac_b,
	
	input	logic[1:0]	esp_r,
	input	logic[1:0]	esp_g,
	input	logic[1:0]	esp_b,
	
	output	logic[7:0]	dac_r_o,
	output	logic[7:0]	dac_g_o,
	output	logic[7:0]	dac_b_o
);

wire [4:0]	r = video_mode ? {esp_r, 3'b000} : vdac_r;
wire [4:0]	g = video_mode ? {esp_g, 3'b000} : vdac_g;
wire [4:0]	b = video_mode ? {esp_b, 3'b000} : vdac_b;

lut lut_r (.mode(mode), .in(r), .out(dac_r_o));
lut lut_g (.mode(mode), .in(g), .out(dac_g_o));
lut lut_b (.mode(mode), .in(b), .out(dac_b_o));

endmodule


module lut (
    input wire mode,
    input wire [4:0] in,
    output wire [7:0] out
);

    wire [7:0] lut;
    assign out = mode ? {in, 3'b0} : lut;

    always_comb
        case (in)
            5'd0:    lut = 8'd0;
            5'd1:    lut = 8'd10;
            5'd2:    lut = 8'd21;
            5'd3:    lut = 8'd31;
            5'd4:    lut = 8'd42;
            5'd5:    lut = 8'd53;
            5'd6:    lut = 8'd63;
            5'd7:    lut = 8'd74;
            5'd8:    lut = 8'd85;
            5'd9:    lut = 8'd95;
            5'd10:   lut = 8'd106;
            5'd11:   lut = 8'd117;
            5'd12:   lut = 8'd127;
            5'd13:   lut = 8'd138;
            5'd14:   lut = 8'd149;
            5'd15:   lut = 8'd159;
            5'd16:   lut = 8'd170;
            5'd17:   lut = 8'd181;
            5'd18:   lut = 8'd191;
            5'd19:   lut = 8'd202;
            5'd20:   lut = 8'd213;
            5'd21:   lut = 8'd223;
            5'd22:   lut = 8'd234;
            5'd23:   lut = 8'd245;
            5'd24:   lut = 8'd255;
            default: lut = 8'd255;
        endcase

endmodule

