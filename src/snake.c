#include "snake.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define cell_is_cords(cell, x, y) ((cell.x == x) && (cell.y == y))
#define cell_is(c1, c2) ((c1.x == c2.x) && (c1.y == c2.y))
#define grid_at(grid, cell) grid[cell.x][cell.y]
#define history_at(game, index) game->history[(index) % (SNAKE_GRID_SIZE)]

static bool cell_handle_direction(CellLocation *cell, SnakeDirection direction)
{
    switch (direction)
    {
    case SnakeDirectionUp:
        if (cell->y == 0)
            return false;
        cell->y--;
        break;
    case SnakeDirectionDown:
        if (cell->y == SNAKE_HEIGHT - 1)
            return false;
        cell->y++;
        break;
    case SnakeDirectionRight:
        if (cell->x == SNAKE_WIDTH - 1)
            return false;
        cell->x++;
        break;
    case SnakeDirectionLeft:
        if (cell->x == 0)
            return false;
        cell->x--;
        break;
    }
    return true;
}

static bool cell_occupied(SnakeGame *game, uint16_t x, uint16_t y)
{
    if (cell_is_cords(game->tail, x, y) || cell_is_cords(game->head, x, y))
    {
        return true;
    }
    CellLocation cell = game->tail;
    for (uint16_t i = game->historyIndex; i != game->historyIndex + game->length - 2; i++)
    {
        SnakeDirection direction = history_at(game, i);
        cell_handle_direction(&cell, direction);
        if (cell_is_cords(cell, x, y))
        {
            return true;
        }
    }
    return false;
}
static void snake_place_fruit(SnakeGame *game)
{
    uint8_t x, y;
    do
    {
        x = (uint8_t)(rand()) % SNAKE_WIDTH;
        y = (uint8_t)(rand()) % SNAKE_HEIGHT;
    } while (cell_occupied(game, x, y));
    game->fruit = (CellLocation){
        .x = x,
        .y = y,
    };
}

void snake_init_game(SnakeGame *game)
{
    if (game == NULL)
        return;
    game->head = (CellLocation){
        .x = SNAKE_INITIAL_LEN,
        .y = SNAKE_HEIGHT / 2,
    };
    game->tail = (CellLocation){
        .x = 1,
        .y = SNAKE_HEIGHT / 2,
    };
    game->length = SNAKE_INITIAL_LEN;

    game->direction = SnakeDirectionRight;
    for (uint16_t i = 0; i < SNAKE_INITIAL_LEN; i++)
    {
        game->history[i] = SnakeDirectionRight;
    }
    game->historyIndex = 0;

    snake_place_fruit(game);
}
void snake_to_grid(const SnakeGame *game, SnakeGrid grid)
{
    for (size_t i = 0; i < SNAKE_WIDTH; i++)
    {
        for (size_t j = 0; j < SNAKE_HEIGHT; j++)
        {
            grid[i][j] = CELL_EMPTY;
        }
    }
    grid_at(grid, game->fruit) = CELL_FRUIT;
    grid_at(grid, game->tail) = CELL_SNAKE_BODY;

    CellLocation cell = game->tail;
    for (uint16_t i = game->historyIndex; i != game->length + game->historyIndex - 2; i++)
    {
        SnakeDirection direction = history_at(game, i);
        cell_handle_direction(&cell, direction);
        grid_at(grid, cell) = CELL_SNAKE_BODY;
    }
    char snake_head = 'E'; // E rror
    switch (game->direction)
    {
    case SnakeDirectionUp:
        snake_head = CELL_SNAKE_HEAD_UP;
        break;
    case SnakeDirectionDown:
        snake_head = CELL_SNAKE_HEAD_DOWN;
        break;
    case SnakeDirectionRight:
        snake_head = CELL_SNAKE_HEAD_RIGHT;
        break;
    case SnakeDirectionLeft:
        snake_head = CELL_SNAKE_HEAD_LEFT;
        break;
    }
    grid_at(grid, game->head) = snake_head;
}
static void snake_print_line(void)
{
    char line[SNAKE_WIDTH + 3];
    line[SNAKE_WIDTH + 2] = '\0';
    line[0] = line[SNAKE_WIDTH + 1] = '+';
    for (size_t i = 1; i <= SNAKE_WIDTH; i++)
    {
        line[i] = '-';
    }
    puts(line);
}
void snake_print_game_grid(const SnakeGame *game)
{
    if (game == NULL)
        return;
    SnakeGrid grid;
    snake_to_grid(game, grid);

    snake_print_line();
    for (size_t y = 0; y < SNAKE_HEIGHT; y++)
    {
        putchar('|');
        for (size_t x = 0; x < SNAKE_WIDTH; x++)
        {
            putchar(grid[x][y]);
        }
        puts("|");
    }
    snake_print_line();
}

static void snake_history_shift(SnakeGame *game)
{
    cell_handle_direction(&game->tail, game->history[game->historyIndex]);
    history_at(game, game->length + game->historyIndex - 1) = game->direction;
    game->historyIndex++;
    game->historyIndex %= SNAKE_GRID_SIZE;
}
void snake_move(SnakeGame *game)
{
    if (game == NULL || game->ended)
    {
        return;
    }
    CellLocation nextCell = game->head;
    if (!cell_handle_direction(&nextCell, game->direction)) { // Hitted a wall, ending
        game->ended = true;
        return;
    }

    if (cell_is(nextCell, game->fruit))
    {
        game->length++;
        snake_place_fruit(game);
        history_at(game, game->length + game->historyIndex - 2) = game->direction;
    }
    else if (cell_occupied(game, nextCell.x, nextCell.y))
    {
        game->ended = true;
        return;
    }
    else
    {
        snake_history_shift(game);
    }
    game->head = nextCell;
}