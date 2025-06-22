package RefFifo;

import Prelude::*;
import Vector::*;

interface FifoIfc#(type a);
   method Action enq(a data);
   method Action deq;
   method a first;
   method Bool notEmpty;
endinterface

module mkRefFifo(FifoIfc#(a)) provisos(Bits#(a,width));
   Vector#(16, Reg#(a)) mem <- replicateM(mkRegU);
   Reg#(UInt#(4)) head <- mkReg(0);
   Reg#(UInt#(4)) tail <- mkReg(0);
   Reg#(UInt#(5)) count <- mkReg(0);

   method Action enq(a data) if (count < 16);
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
