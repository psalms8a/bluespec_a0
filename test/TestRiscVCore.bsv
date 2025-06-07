package TestRiscVCore;

import RiscVCore::*;
import StmtFSM::*;

(* synthesize *)
module mkTb(Empty);
    RiscVCoreIfc dut <- mkRiscVCore;

    Stmt test = seq
        $display("PC: %0d", 0);
        $finish;
    endseq;

    mkAutoFSM(test);
endmodule

endpackage
