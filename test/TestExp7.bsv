package TestExp7;

import Exp7::*;
import RefFifo::*;
import StmtFSM::*;
import Vector::*;
import Prelude::*;

import "BDPI" function Bit#(32) rand_x();
import "BDPI" function Bit#(32) exp_ref(Bit#(32) x);

(* synthesize *)
(* descending_urgency = "check, drive" *)
module mkTb(Empty);
    Exp7Ifc dut <- mkExp7;
    FifoIfc#(Fix32_8) expectQ <- mkRefFifo;
    FifoIfc#(Fix32_8) xQ <- mkRefFifo;

    Reg#(UInt#(32)) sent <- mkReg(0);
    Reg#(UInt#(32)) received <- mkReg(0);
    Reg#(UInt#(4)) warm <- mkReg(0);

    rule drive (sent < 65536);
        Bit#(32) xb = rand_x();
        Fix32_8 x = unpack(xb);
        Bit#(32) yb = exp_ref(xb);
        expectQ.enq(unpack(yb));
        xQ.enq(x);
        dut.start(x);
        sent <= sent + 1;
    endrule

    rule check (dut.valid);
        Fix32_8 expv = expectQ.first;
        Fix32_8 xval = xQ.first;
        expectQ.deq();
        xQ.deq();
        let diff = dut.result - expv;
        if (diff < 0) diff = -diff;
        if (warm >= 6 && diff > 16) begin
            $display("Mismatch for x=%0d", xval);
            $display("expected %0d got %0d", expv, dut.result);
            $finish(1);
        end
        received <= received + 1;
        warm <= warm + 1;
        if (received == 65535) begin
            $display("All tests passed");
            $finish(0);
        end
    endrule
endmodule

endpackage
