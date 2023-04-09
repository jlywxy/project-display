module ltdc(
    input clk,
    input rst,
    output reg state,
    output pclk,
    output reg [23:0]rgb888,
    output hsync,
    output vsync
);

parameter HSYNC = 24,
    HACT=480,
    HBP=36,
    HFP=0,
    HTOTAL=600;

parameter VSYNC = 8,
    VACT=800,
    VBP=16,
    VFP=0,
    VTOTAL=828;
	 
reg [10:0]h;
reg [10:0]v;
reg [25:0]counter;

assign pclk=clk;

initial begin
    h=0;
    v=0;
    state=0;
    counter=0;
	rgb888=24'hffffff;
end

always @(posedge pclk) begin
    if(counter>=25000000)begin
        counter<=25'd0;
        state<=~state;
    end
    else counter<=counter+1'b1;
end

always @(negedge pclk) begin
	 if(h<=HTOTAL-1)begin
	 h<=h+1'd1;
	 end
	 else
	 h<=11'd0;
end

assign hsync = (h <= HSYNC - 1'b1) ? 1'b0 : 1'b1;

always @(negedge hsync) begin
	 if(v<=VTOTAL-1)begin
	 v<=v+1'd1;
	 end
	 else
	 v<=11'd0;
end

assign vsync = (v <= VSYNC - 1'b1) ? 1'b0 : 1'b1;

wire [10:0]y;
assign y=v-VSYNC-VBP;

always @(negedge pclk) begin
	
	if((y>=0)&&(y<256))
		rgb888<=24'hff0000-(y<<16);
	else if((y>=256)&&(y<256*2))
	    rgb888<=24'h00ff00-((y-256)<<8);
    else if((y>=256*2)&&(y<256*3))
	    rgb888<=24'h0000ff-(y-256*2);
	else
		rgb888<=24'hffffff;


end
	 
endmodule

