#include "cli.h"
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <stdint.h>

void cli_nonblocking_terminal(void)
{
    struct termios term;

    tcgetattr(STDIN_FILENO, &term);

    // disable canonical mode & echo
    term.c_lflag &= ~((tcflag_t)(ICANON | ECHO));
    tcsetattr(STDIN_FILENO, TCSANOW, &term);

    // set non-blocking mode
    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
}

void cli_blocking_terminal(void)
{
    struct termios term;
    tcgetattr(STDIN_FILENO, &term);

    // disable canonical mode & echo
    term.c_lflag &= (tcflag_t)(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &term);

    // remove non-blocking flag
    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, flags & ~O_NONBLOCK);
}
