// =======================================================
// print.cpp
// CPP file for printing text through kernel to BIOS
//
// Author: Ioane Baidoshvili
// Date 16-Sep-24
// =======================================================

#include <drivers/print/print.hpp>


// Define constants for screen size
const size_t NUM_COLS = 80;
const size_t NUM_ROWS = 25;

// Defining struct for representing character on screen
struct Char {
    uint8_t character;
    uint8_t color;
};

#pragma region Global Variables
Char* buffer = reinterpret_cast<Char*>(0xB8000); // Pointing to start of VGA text mode (0xb8000)

size_t col = 0;
size_t row = 0;

// Setting foreground to white and background to black
uint8_t color = PRINT_COLOR_WHITE | (PRINT_COLOR_BLACK << 4);
#pragma endregion

#pragma region Print Functions

// Used locally for print_clear function
void clear_row(size_t row) {
    // Creating a new struct, filling it with spaces and giving it the current color (In "Global Variables")
    Char empty{' ', color};

    // Iterating and clearing
    for(size_t col = 0; col < NUM_COLS; ++col) {
        buffer[col + NUM_COLS * row] = empty; // The newly created struct "empty"
    }
}

// Clearing text
extern "C" void print_clear() {
    // Iterating through all of the rows, and using clear_row
    for(size_t row = 0; row < NUM_ROWS; ++row) {
        clear_row(row);
    }
}

// Adding new line
void print_newline() {
    col = 0;

    // Only adding new line if possible
    if(row < NUM_ROWS - 1) {
        ++row;
    } else {
        // Scrolling the screen up

        for(size_t r = 1; r < NUM_ROWS; ++r) {
            for(size_t c = 0; c < NUM_COLS; ++c) {
                // Copying this row to the one above
                buffer[c + NUM_COLS * (r - 1)] = buffer[c + NUM_COLS * r];
            }   
        }

        clear_row(NUM_ROWS - 1);
    }
}


// Printing char to the screen
extern "C" void print_char(char character) {

    // Handeling new line character input
    if(character == '\n') {
        print_newline();

        return;
    }

    if(col >= NUM_COLS) {
        print_newline();
    }

    buffer[col + NUM_COLS * row] = {static_cast<uint8_t>(character), color};
    ++col; // Incrementing character number on this line
}


// Printing string to the screen
extern "C" void print_str(const char* str) {
    // Calling the print_char function the same amount of times as the strings length
    for(size_t i = 0; str[i] != '\0'; ++i) {
        print_char(str[i]);
    }
}

// Changing text color
extern "C" void print_set_color(uint8_t foreground, uint8_t background) {
    color = foreground | (background << 4);
}


#pragma endregion
