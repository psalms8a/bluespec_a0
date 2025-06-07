package RiscVCore;

import Prelude::*;

interface CacheMaster;
    method Action req(Bit#(64) addr, Bit#(64) data, Bool write);
    method ActionValue#(Bit#(64)) resp();
endinterface

interface RiscVCoreIfc;
    interface CacheMaster iCache;
    interface CacheMaster dCache;
endinterface

(* synthesize *)
module mkRiscVCore(RiscVCoreIfc);
    Reg#(Bit#(64)) pc <- mkReg(0);

    Reg#(Bool) stage1 <- mkReg(False);
    Reg#(Bool) stage2 <- mkReg(False);

    interface CacheMaster iCache;
        method Action req(Bit#(64) addr, Bit#(64) data, Bool write);
        endmethod
        method ActionValue#(Bit#(64)) resp();
            return 0;
        endmethod
    endinterface

    interface CacheMaster dCache;
        method Action req(Bit#(64) addr, Bit#(64) data, Bool write);
        endmethod
        method ActionValue#(Bit#(64)) resp();
            return 0;
        endmethod
    endinterface
endmodule

endpackage
