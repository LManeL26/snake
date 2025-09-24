#include "default.h"
#include "cli.h"
#include "snake.h"
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <signal.h>

static SnakeGame currentGame;

static void endProgram(int signal)
{
    UNUSED(signal);

    cli_puts_green("GAME ENDED");
    cli_printf_green("TOTAL POINTS: %d\n", currentGame.length);

    cli_blocking_terminal();
    cli_cursor_show();

    exit(0);
}
static void onKeyPress(int val, SnakeDirection *d)
{
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
        *d = (SnakeDirection)(val);
        break;
    case 'w':
    case 'W':
        *d = SnakeDirectionUp;
        break;
    case 's':
    case 'S':
        *d = SnakeDirectionDown;
        break;
    case 'a':
    case 'A':
        *d = SnakeDirectionLeft;
        break;
    case 'd':
    case 'D':
        *d = SnakeDirectionRight;
        break;
    default:
        break;
    }
}

static void startGame(SnakeGame *game)
{
    while (true)
    {
        int ch = 0;
        SnakeDirection newDirection = game->direction;
        while ((ch = getchar()) != EOF)
        {
            if (ch == 'q')
            {
                cli_blocking_terminal();
                cli_print_red("Are you sure to quit the game? [Y/n] ");
                ch = getchar();
                if (ch == '\n' || ch == 'y' || ch == 'Y')
                {
                    return;
                }
                cli_nonblocking_terminal();
                cli_wait_eof();
                continue;
            }
            if (ch == 0x03) // CTRL + C
            {
                return;
            }
            onKeyPress(ch, &newDirection);
        }

        if ((newDirection ^ game->direction) >> 1) // Removes same axis movement
        {
            game->direction = newDirection;
        }

        snake_move(game);
        cli_clear();
        snake_print_game_grid(game);
        if (game->ended)
        {
            return;
        }
        usleep(400000);
    }
}

int main(int argc, const char *argv[])
{
    UNUSED(argc);
    UNUSED(argv);

    signal(SIGINT, endProgram);
    signal(SIGKILL, endProgram);
    srand((uint32_t)time(NULL));

    printf("Snake game initialization...\n");
    cli_clear();

    snake_init_game(&currentGame);

    snake_print_game_grid(&currentGame);
    cli_puts_green("Press a key for starting...");
    (void)getchar();

    cli_nonblocking_terminal();

    startGame(&currentGame);

    endProgram(SIGINT);

    return 0;
}
