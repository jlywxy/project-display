module cy_lvdslcd(
	input clk_in,
	output led,
	output [3:0]txdata,
	output txclk
);

wire c35_locked;
wire sds_locked;

assign led=c35_locked&sds_locked;
wire clk;

lcdpll	lcdpll_inst (
	.inclk0 ( clk_in ),
	.c0 ( clk ),
	.locked ( c35_locked )
);
//assign clk=clk_in;

	
reg [27:0]serdes;
reg [24:0]counter;
initial begin
	serdes<=28'bzzzzzzzzzzzzzzzzzzzzzzzzzzzz;
	counter<=25'b0;
	//led=0;
end
always @(posedge clk)begin
	if(counter<25000000)counter<=counter+1'b1;
	else begin
		counter<=0;
		//led=~led;
	end
end
lcd_serdes_tx	tx_auo (.tx_in ( serdes ),.tx_inclock ( clk ),
	.tx_out ( txdata ),.tx_outclock ( txclk ),.tx_locked ( sds_locked));

wire [23:0]rgb;
wire de;
ltdc_de ltdcrgb(.clk(clk),.rgbdata_out(rgb),.de(de));

always @(posedge clk)begin
	//serdes <= 28'b0 |(de<<18)|(rgb[23:15]<<21)|(rgb[7:0]);
	//if(de==0)serdes<=28'b0000000000000000000000000000;
	//else serdes<=    28'b1000000000100000000000000000;
	//else 
	serdes<=28'b0
		|(de<<17)
		|(rgb[16]<<9)|(rgb[17]<<10)|(rgb[18]<<18)
		|(rgb[19]<<19)|(rgb[20]<<20)|(rgb[21]<<14)|(rgb[22]<<22)
		|(rgb[23]<<23)
		
		|(rgb[8]<<3)|(rgb[9]<<11)|(rgb[10]<<12)|(rgb[11]<<13)
		|(rgb[12]<<7)|(rgb[13]<<8)|(rgb[14]<<27)|(rgb[15]<<21)
		
		|(rgb[0]<<4)|(rgb[1]<<5)|(rgb[2]<<6)|(rgb[3])
		|(rgb[4]<<1)|(rgb[5]<<2)|(rgb[6]<<25)|(rgb[7]<<26);
		;
	//else serdes<=  28'b0000000100000000000000000100;
end
	
endmodule

module ltdc_de(
	input clk,
	output reg [23:0]rgbdata_out,
	output de
);

parameter VTOTAL=508,
	VBP=28,
	HTOTAL=1056,
	HBP=256;

//parameter VTOTAL=5,
//	VBP=2,
//	HTOTAL=5,
//	HBP=2;
	
reg [15:0]h;
reg [15:0]v;
initial begin
	h<=16'b0;
	v<=16'b0;
end
assign de=(h>=HBP&&v>=VBP)?1:0;

reg [3:0]colorstate_frame;
reg [3:0]colorstate_line;
reg [23:0]rgbdata;
reg [23:0]rgbdata_frame;
initial rgbdata_frame=24'h0000ff;
initial rgbdata=24'h0000ff;


always @(posedge clk)begin
	if(h<HTOTAL)begin
		h<=h+1'b1;
	end
	else begin
		h<=0;
		
		if(v<VTOTAL)begin
			v<=v+1'b1;
			
			if(rgbdata[7:0]==8'hff&&rgbdata[15:8]<8'hff&&rgbdata[23:16]==0)colorstate_line=0;
				else if(rgbdata[15:8]==8'hff&&rgbdata[7:0]>0&&rgbdata[23:16]==0)colorstate_line=1;
				else if(rgbdata[15:8]==8'hff&&rgbdata[23:16]<8'hff&&rgbdata[7:0]==0)colorstate_line=2;
				else if(rgbdata[23:16]==8'hff&&rgbdata[15:8]>0&&rgbdata[7:0]==0)colorstate_line=3;
				else if(rgbdata[23:16]==8'hff&&rgbdata[7:0]<8'hff&&rgbdata[15:8]==0)colorstate_line=4;
				else if(rgbdata[7:0]==8'hff&&rgbdata[23:16]>0&&rgbdata[15:8]==0)colorstate_line=5;
				
				if(colorstate_line==0)rgbdata=24'h0000ff|((rgbdata[15:8]+1)<<8);
				else if(colorstate_line==1)rgbdata=(rgbdata[7:0]-1)|24'h00ff00;
				else if(colorstate_line==2)rgbdata=24'h00ff00|((rgbdata[23:16]+1)<<16);
				else if(colorstate_line==3)rgbdata=((rgbdata[15:8]-1)<<8)|24'hff0000;
				else if(colorstate_line==4)rgbdata=24'hff0000|(rgbdata[7:0]+1);
				else if(colorstate_line==5)rgbdata=((rgbdata[23:16]-1)<<16)|24'h0000ff;
				rgbdata_out=rgbdata;
		end
		else begin
			v<=0;
			   if(rgbdata_frame[7:0]==8'hff&&rgbdata_frame[15:8]<8'hff&&rgbdata_frame[23:16]==0)colorstate_frame=0;
				else if(rgbdata_frame[15:8]==8'hff&&rgbdata_frame[7:0]>0&&rgbdata_frame[23:16]==0)colorstate_frame=1;
				else if(rgbdata_frame[15:8]==8'hff&&rgbdata_frame[23:16]<8'hff&&rgbdata_frame[7:0]==0)colorstate_frame=2;
				else if(rgbdata_frame[23:16]==8'hff&&rgbdata_frame[15:8]>0&&rgbdata_frame[7:0]==0)colorstate_frame=3;
				else if(rgbdata_frame[23:16]==8'hff&&rgbdata_frame[7:0]<8'hff&&rgbdata_frame[15:8]==0)colorstate_frame=4;
				else if(rgbdata_frame[7:0]==8'hff&&rgbdata_frame[23:16]>0&&rgbdata_frame[15:8]==0)colorstate_frame=5;
				
				if(colorstate_frame==0)rgbdata_frame=24'h0000ff|((rgbdata_frame[15:8]+1)<<8);
				else if(colorstate_frame==1)rgbdata_frame=(rgbdata_frame[7:0]-1)|24'h00ff00;
				else if(colorstate_frame==2)rgbdata_frame=24'h00ff00|((rgbdata_frame[23:16]+1)<<16);
				else if(colorstate_frame==3)rgbdata_frame=((rgbdata_frame[15:8]-1)<<8)|24'hff0000;
				else if(colorstate_frame==4)rgbdata_frame=24'hff0000|(rgbdata_frame[7:0]+1);
				else if(colorstate_frame==5)rgbdata_frame=((rgbdata_frame[23:16]-1)<<16)|24'h0000ff;
				rgbdata=rgbdata_frame;
		end
	end
end




//always @(posedge clk)begin
//	if(de==1)begin
//		if((v-VBP>=0)&&(v-VBP<128))
//			rgbdata<=(24'h0000ff-(v-VBP)*2)|((v-VBP)<<8)*2;
//		else if((v-VBP>=128)&&(v-VBP<256))
//			rgbdata<=(24'h00ff00-(((v-VBP-128)*2)<<8))|(((v-VBP-128)*2)<<16);
//		else if((v-VBP>=256)&&(v-VBP<384))
//			rgbdata<=(24'hff0000-(((v-VBP-256)*2)<<16))|((v-VBP-256)*2);
//		else if(v>=384)
//			rgbdata<=(24'h0000ff-(v-VBP-384)*2)|((v-VBP-384)<<8)*2;
//		else begin
//			rgbdata<=24'hffffff;
//		end
//	end
//	else rgbdata<=24'h000000;
//end
endmodule