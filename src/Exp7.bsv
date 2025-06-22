package Exp7;

import Vector::*;
import Prelude::*;

// Fixed-point type with 8 integer bits and 24 fractional bits
typedef Int#(32) Fix32_8;


// Fixed-point multiply helper
function Fix32_8 fxMul(Fix32_8 a, Fix32_8 b);
    Int#(64) prod = signExtend(a) * signExtend(b);
    return truncate(prod >> 24);
endfunction

// Accessor for polynomial coefficients
function Fix32_8 coeff(Integer idx);
    case(idx)
        0: return (Fix32_8)'(32'sh01000000);
        1: return (Fix32_8)'(32'sh0100001F);
        2: return (Fix32_8)'(32'sh00800009);
        3: return (Fix32_8)'(32'sh002AA86E);
        4: return (Fix32_8)'(32'sh000AAA42);
        5: return (Fix32_8)'(32'sh00022C54);
        default: return (Fix32_8)'(32'sh00005C7E);
    endcase
endfunction

interface Exp7Ifc;
    method Action start(Fix32_8 x);
    method Bool valid;
    method Fix32_8 result;
endinterface

(* synthesize *)
module mkExp7(Exp7Ifc);
    Vector#(6, Reg#(Fix32_8)) xpipe <- replicateM(mkReg(0));
    Reg#(Fix32_8) t0 <- mkReg(0);
    Reg#(Fix32_8) t1 <- mkReg(0);
    Reg#(Fix32_8) t2 <- mkReg(0);
    Reg#(Fix32_8) t3 <- mkReg(0);
    Reg#(Fix32_8) t4 <- mkReg(0);
    Reg#(Fix32_8) t5 <- mkReg(0);
    Reg#(Fix32_8) y  <- mkReg(0);

    Vector#(7, Reg#(Bool)) vpipe <- replicateM(mkReg(False));
    Reg#(Fix32_8) in_reg <- mkReg(0);
    Reg#(Bool)    in_valid <- mkReg(False);

    // Main pipeline rule
    rule do_pipeline;
        t0 <= coeff(6);
        t1 <= fxMul(t0, xpipe[0]) + coeff(5);
        t2 <= fxMul(t1, xpipe[1]) + coeff(4);
        t3 <= fxMul(t2, xpipe[2]) + coeff(3);
        t4 <= fxMul(t3, xpipe[3]) + coeff(2);
        t5 <= fxMul(t4, xpipe[4]) + coeff(1);
        y  <= fxMul(t5, xpipe[5]) + coeff(0);

        // shift x pipeline
        for(Integer i=5; i>0; i=i-1) begin
            xpipe[i] <= xpipe[i-1];
        end
        xpipe[0] <= in_reg;

        // shift valid pipeline
        for(Integer i=6; i>0; i=i-1) begin
            vpipe[i] <= vpipe[i-1];
        end
        vpipe[0] <= in_valid;
        in_valid <= False;
    endrule

    method Action start(Fix32_8 x);
        in_reg <= x;
        in_valid <= True;
    endmethod

    method Bool valid;
        return vpipe[6];
    endmethod

    method Fix32_8 result if (vpipe[6]);
        return y;
    endmethod
endmodule

endpackage
