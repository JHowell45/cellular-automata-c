#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "raylib.h"

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

#define GRID_X_SIZE 321
#define GRID_Y_SIZE 160

// #define SQUARE_SIZE 51

// #define GRID_X_SIZE SQUARE_SIZE
// #define GRID_Y_SIZE SQUARE_SIZE

#define RECTANGLE_SIZE 5
#define RECTANGLE_BUFFER 1

typedef struct {
    size_t xSize;
    size_t ySize;
    bool *data;
} Grid;

Grid *new_grid(size_t x, size_t y);
void free_grid(Grid *grid);
static int8_t get_ruleset_index(uint8_t left, uint8_t centre, uint8_t right);
static bool get_ruleset_index_bit(uint8_t rulesetNumber, size_t index);
void update_next_row_grid(Grid *grid, size_t evalRow, uint8_t ruleset);

static void UpdateFrame(Grid *grid, size_t evalRow, uint8_t ruleset);
static void DrawGrid(Grid *grid);

int main(int argc, char *argv[]) {
    int ruleset = atoi(argv[1]);
    size_t evalRow = 0;

    Grid *grid = new_grid(GRID_X_SIZE, GRID_Y_SIZE);
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Cellular Automata");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(LIGHTGRAY);
        UpdateFrame(grid, evalRow, ruleset);
        if (evalRow < GRID_Y_SIZE) {
            evalRow++;
        }
        EndDrawing();
    }
    CloseWindow();
    free_grid(grid);
    return 0;
}

static void UpdateFrame(Grid *grid, size_t evalRow, uint8_t ruleset) {
    update_next_row_grid(grid, evalRow, ruleset);
    DrawGrid(grid);
}

static void DrawGrid(Grid *grid) {
    for (int y = 0; y < grid->ySize; y++) {
        for (int x = 0; x < grid->xSize; x++) {
            size_t index = (y * grid->xSize) + x;
            if ((bool)grid->data[index]) {
                int xPos = x * (RECTANGLE_SIZE + RECTANGLE_BUFFER);
                int yPos = y * (RECTANGLE_SIZE + RECTANGLE_BUFFER);
                DrawRectangle(
                    xPos,
                    yPos,
                    RECTANGLE_SIZE,
                    RECTANGLE_SIZE,
                    BLACK
                );
            }            
        }
    }
}

Grid *new_grid(size_t x, size_t y) {
    Grid *new_grid = malloc(sizeof(Grid));
    new_grid->xSize = x;
    new_grid->ySize = y;
    new_grid->data = calloc(x * y, sizeof(bool));
    new_grid->data[x / 2] = true;
    return new_grid;
}

void free_grid(Grid *grid) {
    free(grid->data);
    free(grid);
}

void update_next_row_grid(Grid *grid, size_t evalRow, uint8_t ruleset) {
    if (evalRow >= grid->ySize - 1) {
        return;
    }
    int startIndex = evalRow * grid->xSize;
    grid->data[startIndex + grid->xSize] = grid->data[startIndex];
    grid->data[startIndex + (2 * grid->xSize) - 1] = grid->data[startIndex + grid->xSize - 1];
    for (int i = 1; i < grid->xSize - 1; i++) {
        int index = startIndex + i;
        int8_t rulesetIndex = get_ruleset_index(grid->data[index-1], grid->data[index], grid->data[index + 1]);
        grid->data[index + grid->xSize] = get_ruleset_index_bit(ruleset, 7-rulesetIndex);
    }
    evalRow++;
}

static int8_t get_ruleset_index(uint8_t left, uint8_t centre, uint8_t right) {
    if (left == 1 && centre == 1 && right == 1) {
        return 0;
    }
    if (left == 1 && centre == 1 && right == 0) {
        return 1;
    }
    if (left == 1 && centre == 0 && right == 1) {
        return 2;
    }
    if (left == 1 && centre == 0 && right == 0) {
        return 3;
    }
    if (left == 0 && centre == 1 && right == 1) {
        return 4;
    }
    if (left == 0 && centre == 1 && right == 0) {
        return 5;
    }
    if (left == 0 && centre == 0 && right == 1) {
        return 6;
    }
    if (left == 0 && centre == 0 && right == 0) {
        return 7;
    }
    return -1;
}

static bool get_ruleset_index_bit(uint8_t rulesetNumber, size_t index) {
    // https://stackoverflow.com/questions/2249731/how-do-i-get-bit-by-bit-data-from-an-integer-value-in-c
    return (rulesetNumber & ( (uint8_t)1 << index )) >> index;
}