#include "default.h"
#include "cli.h"
#include "snake.h"
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

static void onKeyPress(SnakeGame *game, int val)
{
    SnakeDirection d = game->direction;
    switch (val)
    {
    case '\033':
        if (getchar() != '[')
            break;
        val = getchar() - 'A';
        if (val < 0 || val >= 4)
        {
            return;
        }
        d = (SnakeDirection)(getchar() - 'A');
        break;
    case 'w':
    case 'W':
        d = SnakeDirectionUp;
        break;
    case 's':
    case 'S':
        d = SnakeDirectionDown;
        break;
    case 'a':
    case 'A':
        d = SnakeDirectionLeft;
        break;
    case 'd':
    case 'D':
        d = SnakeDirectionRight;
        break;
    default:
        break;
    }

    game->direction = d;
}

static void startGame(SnakeGame *game)
{
    while (true)
    {
        int ch = 0;
        while ((ch = getchar()) != EOF)
        {
            if (ch == 'q')
            {
                cli_puts_red("Quitting game");
                break;
            }
            onKeyPress(game, ch);
        }

        snake_move(game);
        cli_clear();
        snake_print_game_grid(game);
        if (game->ended)
        {
            cli_puts_green("GAME ENDED");
            break;
        }
        sleep(1);
    }
    cli_printf_green("TOTAL POINTS: %d\n", game->length);
}

int main(int argc, const char *argv[])
{
    srand((uint32_t)time(NULL));
    UNUSED(argc);
    UNUSED(argv);
    printf("Snake game initialization...\n");
    cli_clear();

    SnakeGame game;
    snake_init_game(&game);

    snake_print_game_grid(&game);
    cli_puts_green("Press a key for starting...");
    (void)getchar();

    // cli_cursor_hide();
    // cli_cursor_move_top_left();
    cli_nonblocking_terminal();

    startGame(&game);

    cli_blocking_terminal();
    cli_cursor_show();

    return 0;
}
