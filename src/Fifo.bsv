package Fifo;

import Vector::*;
import Prelude::*;

interface FifoIfc#(type a);
   method Action enq(a data);
   method Action deq;
   method a first;
   method Bool notEmpty;
endinterface

module mkSimpleFifo(FifoIfc#(a)) provisos(Bits#(a,width));
   Vector#(4, Reg#(a)) mem <- replicateM(mkRegU);
   Reg#(UInt#(2)) head <- mkReg(0);
   Reg#(UInt#(2)) tail <- mkReg(0);
   Reg#(UInt#(3)) count <- mkReg(0);

   method Action enq(a data) if (count < 4);
       mem[tail] <= data;
       tail <= tail + 1;
       count <= count + 1;
   endmethod

   method Action deq if (count > 0);
       head <= head + 1;
       count <= count - 1;
   endmethod

   method a first if (count > 0);
       return mem[head];
   endmethod

   method Bool notEmpty;
       return count > 0;
   endmethod
endmodule

endpackage
