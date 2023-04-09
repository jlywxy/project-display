module auo(
	input clk_in,
	output reg [7:0]rgb8,
	output reg hsync,
	output reg vsync,
	output reg pclk
);
reg clk;
reg hblank,vblank;
reg fcount;
reg [24:0] hcount;
reg [24:0] vcount; 
initial begin
	clk<=0;
	rgb8<=8'h00;
	hsync<=0;
	vsync<=0;
	hcount<=0;
	vcount<=0;
	fcount<=0;
end
always @(posedge clk_in)begin
	clk=~clk;
	pclk=clk;
end
wire hsyncon,hsyncoff,hreset,hblankon;
wire vsyncon,vsyncoff,vreset,vblankon;
always @(negedge clk)begin
	hcount<=hreset?0:hcount+1;
	hsync<=hsyncon?0:hsyncoff?1:hsync;
	
	vcount<=hreset?(vreset?0:vcount+1):vcount;
	vsync<=vsyncon?0:vsyncoff?1:vsync;
	
	if ((vcount>28&&vcount<255))
		if(hcount>166&&hcount<=422)
			if(vcount%2==0)
				if(hcount%3==0)
					rgb8<=8'hff-(hcount-166);
				else
					rgb8<=8'h0;
			else
				if(hcount%3==2)
					rgb8<=8'hff-(hcount-166);
				else
					rgb8<=8'h0;
		else if(hcount>422&&hcount<=678)
			if(vcount%2==0)
				if(hcount%3==1)
					rgb8<=8'hff-(hcount-422);
				else
					rgb8<=8'h0;
			else
				
					if(hcount%3==0)
					rgb8<=8'hff-(hcount-422);
				else
					rgb8<=8'h0;
		else if(hcount>678&&hcount<=934)
			if(vcount%2==0)
				if(hcount%3==2)
					rgb8<=8'hff-(hcount-678);
				else
					rgb8<=8'h0;
			else
				
					if(hcount%3==1)
					rgb8<=8'hff-(hcount-678);
				else
					rgb8<=8'h0;
		else 
			rgb8<=8'h00;
	else
		rgb8<=8'hff;
end
always @(posedge vreset)fcount=~fcount;
assign hsyncon=(hcount==0);
assign hsyncoff=(hcount==1);
assign hreset=(hcount==1716);

assign vsyncon=hreset&(vcount==0);
assign vsyncoff=hreset&(vcount==1);
assign vreset=hreset&(vcount==262);

endmodule