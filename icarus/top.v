module top;
reg clk;
reg [15:0] test1,test2,test3,test4;

initial begin
  test1 = 16'h2000;
  test2 = 16'h0020;
  test3 = '1;
  test4 = '1;
  clk = 0;
  #1 clk = 1;
  #1 $finish;
end

endmodule
