#pragma once
#include <stdbool.h>
#include <stdint.h>

#define SNAKE_WIDTH 40
#define SNAKE_HEIGHT 11
#define SNAKE_GRID_SIZE SNAKE_WIDTH * SNAKE_HEIGHT

#define SNAKE_INITIAL_LEN 3

#define CELL_SNAKE_BODY '#'
#define CELL_SNAKE_HEAD_UP '^'
#define CELL_SNAKE_HEAD_DOWN 'v'
#define CELL_SNAKE_HEAD_LEFT '<'
#define CELL_SNAKE_HEAD_RIGHT '>'
#define CELL_FRUIT 'O'
#define CELL_EMPTY ' '

typedef enum snake_direction
{
    SnakeDirectionUp,
    SnakeDirectionDown,
    SnakeDirectionRight,
    SnakeDirectionLeft,
} SnakeDirection;
typedef struct cell_location
{
    uint8_t x;
    uint8_t y;
} CellLocation;
typedef char SnakeGrid[SNAKE_WIDTH][SNAKE_HEIGHT];
typedef uint8_t SnakeHistory[SNAKE_GRID_SIZE];

typedef struct snake_game
{
    CellLocation head;
    CellLocation tail;
    CellLocation fruit;

    SnakeHistory history;
    uint16_t historyIndex;

    SnakeDirection direction;
    
    uint16_t length;
    bool ended;
} SnakeGame;

void snake_init_game(SnakeGame *);
bool snake_handle_command(SnakeGame *, char command);
void snake_move(SnakeGame *);

void snake_to_grid(const SnakeGame *game, SnakeGrid grid);
void snake_print_game_grid(const SnakeGame *);