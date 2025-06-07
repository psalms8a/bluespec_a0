package TestFifo;

import Fifo::*;
import StmtFSM::*;

(* synthesize *)
module mkTb(Empty);
   FifoIfc#(Bit#(8)) fifo <- mkSimpleFifo;

   Stmt test = seq
       fifo.enq(8'hAA);
       fifo.enq(8'hBB);
       $display("First: %h", fifo.first);
       fifo.deq();
       $display("First after deq: %h", fifo.first);
       fifo.deq();
       $display("Empty: %0d", !fifo.notEmpty);
       $finish;
   endseq;

   mkAutoFSM(test);
endmodule

endpackage
