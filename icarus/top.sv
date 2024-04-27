module top;
/* verilator lint_off UNUSEDSIGNAL */
reg [15:0] test0,test1,test2,test3,test4;

initial begin
  test0 = 16'h0000;
  test1 = 16'hab00;
  test2 = 16'h00ab;
  test3 = 16'hffff;
  test4 = 16'hffff;
  #1;
  #1 $finish;
end

final begin
  $display("verilog: test0=%x",test0);
  $display("verilog: test1=%x",test1);
  $display("verilog: test2=%x",test2);
  $display("verilog: test3=%x",test3);
  $display("verilog: test4=%x\n",test4);
end

endmodule
