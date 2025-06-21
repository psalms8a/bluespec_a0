#include <iostream>
#include "../include/ptx_builder.h"

// Build a simple AXPY kernel using the PTX builder API.
// The kernel performs: y = a * x + y for a single element.
// This example generates PTX text and prints it to stdout.

std::string build_axpy_kernel() {
    using namespace ptx;
    Builder b;
    Reg rA("%f0");
    Reg rX("%f1");
    Reg rY("%f2");

    // rY = a * x + y
    b.mad(rY, rA, rX, rY);
    b.ret();

    std::ostringstream oss;
    oss << ".version 8.7\n";
    oss << ".target sm_80\n";
    oss << ".address_size 64\n\n";
    oss << ".visible .entry axpy_one(\n";
    oss << "    .param .f32 a,\n";
    oss << "    .param .f32 x,\n";
    oss << "    .param .f32 y\n";
    oss << ")\n";
    oss << "{\n";
    oss << "    .reg .f32 %f0, %f1, %f2;\n";
    oss << b.str();
    oss << "}\n";
    return oss.str();
}

int main() {
    std::cout << build_axpy_kernel();
    return 0;
}

