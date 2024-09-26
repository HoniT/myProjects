// =====================================
// kernel_main.cpp
// Main file for the kernel
//
// Author: Ioane Baidoshvili
// Date 06-Sep-24
// =====================================

#include <kernel_main.hpp> 

extern "C" void kernel_main() {

    print_str("Hello There!\nHelp me\n");


    // Infinite loop
    while(true);
    return;
}
