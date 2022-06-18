
interface ISPI();
    logic		sclk;
    logic		mosi;
    logic		miso;
    logic		ssel;
    
    modport Master(output sclk, mosi, ssel, input miso);
    modport Slave(input sclk, mosi, ssel, output miso);

endinterface

typedef enum logic [2:0] {NoHost, MCU, FPGA} SelHost;
typedef enum logic [3:0] {NoSlave, SD0, SD1, ESP32, FPGA_S} SelSlave;


module CPLD (
    // sys
    input		logic		n_softrst_i,
    input		logic		cpld_ssel_i,
    input		logic[1:0]	fpga_sel_i,
    
    // vdac
    input		logic		dac_clk_i,
    input		logic[15:0]	vdat_i,
    input		logic		vsync_i,
    input		logic		hsync_i,
    
    output	logic[7:0]	    dac_r_o,
    output	logic[7:0]	    dac_g_o,
    output	logic[7:0]	    dac_b_o,
    
    // spi
    ISPI.Slave				mcu,
    ISPI.Slave				fpga_m,
    
    ISPI.Master				sd0,
    ISPI.Master				sd1,
    ISPI.Master				esp32,
    ISPI.Master				fpga_s
);

// VDAC *********************************************

wire[4:0]	o_r = vdat_i[4:0];
wire[4:0]	o_g = vdat_i[9:5];
wire[4:0]	o_b = vdat_i[14:10];
wire 		mode = vdat_i[15];


lut lut_r (.mode(mode), .in(o_r), .out(dac_r_o));
lut lut_g (.mode(mode), .in(o_g), .out(dac_g_o));
lut lut_b (.mode(mode), .in(o_b), .out(dac_b_o));

        
// SPI SLAVE ****************************************	
reg [7:0]	spi_reg;
reg [7:0]	cpld_config;

always @ (negedge cpld_ssel_i or negedge n_softrst_i)
if (n_softrst_i == 1'b0)
    cpld_config  <= 0;
else
    cpld_config <= spi_reg;

always @ (posedge mcu.sclk)
if (mcu.ssel == 1'b0 && cpld_ssel_i)
    spi_reg <= {spi_reg[6:0], mcu.mosi};

wire 		sd0_sel = cpld_config[0];
wire 		sd1_sel = cpld_config[1];
wire [1:0]	esp32_slave_sel = cpld_config[5:4];


//assign esp32.ssel = spi_reg[5];


wire	sd0_mcu_sel_n    = !(sd0_sel == 0 && esp32_slave_sel == 2'b00);
wire	sd1_mcu_sel_n    = !(sd1_sel == 0 && esp32_slave_sel == 2'b01);
wire	fpga_mcu_sel_n 	 = !(esp32_slave_sel == 2'b10);

wire	sd0_fpga_sel_n   = !(sd0_sel == 1 && fpga_sel_i == 2'b00);
wire	sd1_fpga_sel_n   = !(sd1_sel == 1 && fpga_sel_i == 2'b01);
wire	eps32_fpga_sel_n = !(fpga_sel_i == 2'b10);




// sd0
assign	sd0.sclk = (mcu.sclk | sd0_mcu_sel_n) & (fpga_m.sclk | sd0_fpga_sel_n);
assign	sd0.mosi = (mcu.mosi | sd0_mcu_sel_n) & (fpga_m.mosi | sd0_fpga_sel_n);
assign	sd0.ssel = (mcu.ssel | sd0_mcu_sel_n) & (fpga_m.ssel | sd0_fpga_sel_n);

// sd1
assign	sd1.sclk = (mcu.sclk | sd1_mcu_sel_n) & (fpga_m.sclk | sd1_fpga_sel_n);
assign	sd1.mosi = (mcu.mosi | sd1_mcu_sel_n) & (fpga_m.mosi | sd1_fpga_sel_n);
assign	sd1.ssel = (mcu.ssel | sd1_mcu_sel_n) & (fpga_m.ssel | sd1_fpga_sel_n);

// fpga slave
assign	fpga_s.sclk = mcu.sclk | fpga_mcu_sel_n;
assign	fpga_s.mosi = mcu.mosi | fpga_mcu_sel_n;
assign	fpga_s.ssel = mcu.ssel | fpga_mcu_sel_n;

// esp32 slave
//assign	esp32.sclk  = fpga_m.sclk | eps32_fpga_sel_n;
//assign	esp32.mosi  = fpga_m.mosi | eps32_fpga_sel_n;
//assign	esp32.ssel  = fpga_m.ssel | eps32_fpga_sel_n;

assign	esp32.sclk  = fpga_s.mosi;
assign	esp32.mosi  = esp32_slave_sel[0];
assign	esp32.ssel  = esp32_slave_sel[1];



reg		mcu_miso, fpga_m_miso;
assign	mcu.miso = mcu_miso;
assign	fpga_m.miso = fpga_m_miso;

always_comb
begin

	case (esp32_slave_sel)
		2'b00:	 mcu_miso = sd0.miso;
		2'b01:	 mcu_miso = sd1.miso;
		2'b10:	 mcu_miso = fpga_s.miso;
		default: mcu_miso = 1'b1;
	endcase
	
	case (fpga_sel_i)
		2'b00:	 fpga_m_miso = sd0.miso;
		2'b01:	 fpga_m_miso = sd1.miso;
		2'b10:	 fpga_m_miso = esp32.miso;
		default: fpga_m_miso = 1'b1;
	endcase
			
end

//assign	mcu.miso = sd0.miso;
//assign	mcu.miso = sd1.miso;

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


/*

interface ISPI();
    logic		sclk;
    logic		mosi;
    logic		miso;
    logic		ssel;
    
    modport Master(output sclk, mosi, ssel, input miso);
    modport Slave(input sclk, mosi, ssel, output miso);

endinterface

typedef enum logic [2:0] {NoHost, MCU, FPGA} SelHost;
typedef enum logic [3:0] {NoSlave, SD0, SD1, ESP32, FPGA_S} SelSlave;


module CPLD (
    // sys
    input		logic		n_softrst_i,
    input		logic		cpld_ssel_i,
    input		logic[1:0]	fpga_sel_i,
    
    // vdac
    input		logic		dac_clk_i,
    input		logic[15:0]	vdat_i,
    input		logic		vsync_i,
    input		logic		hsync_i,
    
    output	logic[7:0]	    dac_r_o,
    output	logic[7:0]	    dac_g_o,
    output	logic[7:0]	    dac_b_o,
    
    // spi
    ISPI.Slave				mcu,
    ISPI.Slave				fpga_m,
    
    ISPI.Master				sd0,
    ISPI.Master				sd1,
    ISPI.Master				esp32,
    ISPI.Master				fpga_s
);

// VDAC *********************************************

wire[4:0]	o_r = vdat_i[4:0];
wire[4:0]	o_g = vdat_i[9:5];
wire[4:0]	o_b = vdat_i[14:10];
wire 		mode = vdat_i[15];


lut lut_r (.mode(mode), .in(o_r), .out(dac_r_o));
lut lut_g (.mode(mode), .in(o_g), .out(dac_g_o));
lut lut_b (.mode(mode), .in(o_b), .out(dac_b_o));

        
// SPI SLAVE ****************************************	
reg [7:0]	spi_reg;
reg [7:0]	cpld_config;

always @ (negedge cpld_ssel_i or negedge n_softrst_i)
if (n_softrst_i == 0)
    cpld_config  <= 0;
else
    cpld_config <= spi_reg;

always @ (posedge mcu.sclk)
if (mcu.ssel == 1'b0 && cpld_ssel_i)
    spi_reg <= {spi_reg[6:0], mcu.mosi};
    
// MUX CONTROL ***************************************

SelSlave	mcu_sel, fpga_m_sel;
SelHost		sd0_sel, sd1_sel, esp32_sel, fpga_s_sel;

mux_control ctrl(
    .cpld_config    (cpld_config),
    .fpga_sel	    (fpga_sel_i),
    
    .mcu			(mcu_sel),
    .fpga_m		    (fpga_m_sel),
    
    .sd0			(sd0_sel),
    .sd1			(sd1_sel),
    .esp32		    (esp32_sel),
    .fpga_s		    (fpga_s_sel)
);

// CROSSBAR MUX ***********************************

always_comb
begin
    
    case (mcu_sel)
        SD0: 		mcu.miso <= sd0.miso;
        SD1:		mcu.miso <= sd1.miso;
        FPGA_S:     mcu.miso <= fpga_s.miso;
        
        default:	mcu.miso <= 1'b1;
    endcase
    
    case (fpga_m_sel)
        SD0: 		fpga_m.miso <= sd0.miso;
        SD1:		fpga_m.miso <= sd1.miso;
        ESP32:	    fpga_m.miso <= esp32.miso;
        
        default:	fpga_m.miso <= 1'b1;
    endcase

    case (sd0_sel)
        MCU:		{sd0.sclk, sd0.mosi, sd0.ssel} <= {mcu.sclk, mcu.mosi, mcu.ssel};
        FPGA:		{sd0.sclk, sd0.mosi, sd0.ssel} <= {fpga_m.sclk, fpga_m.mosi, fpga_m.ssel};
        
        default:	{sd0.sclk, sd0.mosi, sd0.ssel} <= 3'b111;
    endcase
    
    case (sd1_sel)
        MCU:		{sd1.sclk, sd1.mosi, sd1.ssel} <= {mcu.sclk, mcu.mosi, mcu.ssel};
        FPGA:		{sd1.sclk, sd1.mosi, sd1.ssel} <= {fpga_m.sclk, fpga_m.mosi, fpga_m.ssel};
        
        default:	{sd1.sclk, sd1.mosi, sd1.ssel} <= 3'b111;
    endcase
    
    case (esp32_sel)
        MCU:		{esp32.sclk, esp32.mosi, esp32.ssel} <= {mcu.sclk, mcu.mosi, mcu.ssel};
        FPGA:		{esp32.sclk, esp32.mosi, esp32.ssel} <= {fpga_m.sclk, fpga_m.mosi, fpga_m.ssel};
        
        default:	{esp32.sclk, esp32.mosi, esp32.ssel} <= 3'b111;
    endcase
    
    case (fpga_s_sel)
        MCU:		{fpga_s.sclk, fpga_s.mosi, fpga_s.ssel} <= {mcu.sclk, mcu.mosi, mcu.ssel};
        FPGA:		{fpga_s.sclk, fpga_s.mosi, fpga_s.ssel} <= {fpga_m.sclk, fpga_m.mosi, fpga_m.ssel};
        
        default:	{fpga_s.sclk, fpga_s.mosi, fpga_s.ssel} <= 3'b111;
    endcase
end


endmodule


module mux_control(
    input	logic	[7:0]	cpld_config,
    input	logic	[1:0]	fpga_sel,
    
    output	SelSlave		mcu,
    output	SelSlave		fpga_m,
    
    output	SelHost			sd0,
    output	SelHost			sd1,
    output	SelHost			esp32,
    output	SelHost			fpga_s
);

wire		sd0_master_sel	= cpld_config[0];
wire		sd1_master_sel	= cpld_config[1];
wire[1:0]	mcu_slave_sel	= cpld_config[5:4];

always_comb
begin
    // sd0
    casex ({sd0_master_sel, mcu_slave_sel, fpga_sel})
        5'b0_00_xx:	sd0 <= MCU;
        5'b1_xx_00:	sd0 <= FPGA;
        
        default:		sd0 <= NoHost;
    endcase
    
    // sd1
    casex ({sd1_master_sel, mcu_slave_sel, fpga_sel})
        5'b0_01_xx:	sd1 <= MCU;
        5'b1_xx_01:	sd1 <= FPGA;
        
        default:    sd1 <= NoHost;
    endcase
    
    esp32   <= (fpga_sel == 2'b10)    	? FPGA : NoHost;
    fpga_s  <= (mcu_slave_sel == 2'b10)	? MCU  : NoHost;
end

always_comb 
begin
    // fpga host
    casex ({fpga_sel, sd1_master_sel, sd0_master_sel})
        4'b00_x1:	fpga_m <= SD0;
        4'b01_1x:	fpga_m <= SD1;
        4'b10_xx:	fpga_m <= ESP32;
            
        default:	fpga_m <= NoSlave;
    endcase
    
    //mcu host
    casex ({mcu_slave_sel, sd1_master_sel, sd0_master_sel})
        4'b00_x0:	mcu	<= SD0;
        4'b01_0x:	mcu	<= SD1;
        4'b10_xx:	mcu	<= FPGA_S;
        
        default:	mcu	<= NoSlave;
        
    endcase
end
    

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


*/
