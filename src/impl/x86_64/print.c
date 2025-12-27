#include "print.h"

const static size_t NUM_COLS = 80;
const static size_t NUM_ROWS = 25;

struct Char {
    uint8_t ascii;
    uint8_t color;
};

volatile struct Char* const video_memory = (struct Char*)0xb8000;

size_t terminal_row = 0;
size_t terminal_column = 0;
unsigned char terminal_color = 0x0F; // White on black

void print_newline() {
    terminal_column = 0;
    terminal_row++;
    if (terminal_row >= NUM_ROWS) {
        terminal_row = 0;
    }

    for (size_t row = 0; row < NUM_ROWS; row++) {
        for (size_t col = 0; col < NUM_COLS; col++) {
            struct Char current = video_memory[(row + 1) % NUM_ROWS * NUM_COLS + col];
            video_memory[row * NUM_COLS + col] = current;
        }
    }

    clear_row(NUM_ROWS - 1);
}

void print_clear() {
    for (size_t y = 0; y < NUM_ROWS; y++) {
        for (size_t x = 0; x < NUM_COLS; x++) {
            const size_t index = y * NUM_COLS + x;
            video_memory[index].ascii = ' ';
            video_memory[index].color = terminal_color;
        }
    }
    terminal_row = 0;
    terminal_column = 0;
}

void clear_row(size_t row) {
    struct Char empty = (struct Char) {
        .ascii = ' ',
        .color = terminal_color
    };
    for (size_t x = 0; x < NUM_COLS; x++) {
        const size_t index = row * NUM_COLS + x;
        video_memory[index] = empty;
    }
}

void print_char(char character) {
    if (character == '\n') {
        terminal_column = 0;
        return;
    }

    if (terminal_column >= NUM_COLS) {
        print_newline();
    }

    video_memory[terminal_row * NUM_COLS + terminal_column] = (struct Char) {
        .ascii =(uint8_t) character,
        .color = terminal_color
    };

    terminal_column++;
}

void print_str(char* str) {
    for (size_t i = 0; str[i] != '\0'; i++) {
        print_char((uint8_t) str[i]);
    }
}

void print_set_color(unsigned char fg, unsigned char bg) {
    terminal_color = (bg << 4) | (fg & 0x0F);
} 