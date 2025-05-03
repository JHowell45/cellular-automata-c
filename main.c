#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "raylib.h"

typedef struct {
    size_t xSize;
    size_t ySize;
    bool *data;
} Grid;

Grid *new_grid(size_t x, size_t y);
void free_grid(Grid *grid);
static int8_t get_ruleset_index(uint8_t left, uint8_t centre, uint8_t right);
static bool get_ruleset_index_bit(uint8_t rulesetNumber, size_t index);

int main(int argc, char *argv) {
    return 0;
}