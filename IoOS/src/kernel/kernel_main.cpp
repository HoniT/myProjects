// =====================================
// kernel_main.cpp
// Main file for the kernel
//
// Author: Ioane Baidoshvili
// Date 06-Sep-24
// =====================================

#include <kernel_main.hpp> 

extern "C" {

    // Defining magic number
    const unsigned int KERNEL_MAGIC_NUMBER __attribute__((section(".magic"))) = 0xDEADBEEF;
}

extern "C" void kernel_main() {

    // Printing text
    print_clear();
    print_set_color(PRINT_COLOR_WHITE, PRINT_COLOR_RED);
    //print_str("IoOS kernel is running...");
    print_char('R');


    // Infinite loop
    while(true);
    return;
}
