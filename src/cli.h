#pragma once

#include <stdlib.h>

#define ARROW_UP "\033[A"
#define ARROW_DOWN "\033[B"
#define ARROW_RIGHT "\033[C"
#define ARROW_LEFT "\033[D"

#define CURSOR_SHOW "\033[?25h"
#define CURSOR_HIDE "\033[?25l"
#define CURSOR_MOVE_TOP_LEFT "\033[H"

#define FONT_COLOR_CYAN "\033[36m"
#define FONT_COLOR_RED "\033[31m"
#define FONT_COLOR_GREEN "\033[32m"
#define FONT_COLOR_BROWN "\033[33m"
#define FONT_COLOR_BLUE "\033[34m"
#define FONT_COLOR_PURPLE "\033[35m"
#define FONT_COLOR_RESET "\033[0m"

#define cli_text_cyan(x) FONT_COLOR_CYAN x FONT_COLOR_RESET
#define cli_text_red(x) FONT_COLOR_RED x FONT_COLOR_RESET
#define cli_text_green(x) FONT_COLOR_GREEN x FONT_COLOR_RESET
#define cli_text_brown(x) FONT_COLOR_BROWN x FONT_COLOR_RESET
#define cli_text_blue(x) FONT_COLOR_BLUE x FONT_COLOR_RESET
#define cli_text_purple(x) FONT_COLOR_PURPLE x FONT_COLOR_RESET

#define cli_printf_cyan(fmt, ...) printf(cli_text_cyan(fmt), __VA_ARGS__)
#define cli_printf_red(fmt, ...) printf(cli_text_red(fmt), __VA_ARGS__)
#define cli_printf_green(fmt, ...) printf(cli_text_green(fmt), __VA_ARGS__)
#define cli_printf_brown(fmt, ...) printf(cli_text_brown(fmt), __VA_ARGS__)
#define cli_printf_blue(fmt, ...) printf(cli_text_blue(fmt), __VA_ARGS__)
#define cli_printf_purple(fmt, ...) printf(cli_text_purple(fmt), __VA_ARGS__)
#define cli_printf_cyan(fmt, ...) printf(cli_text_cyan(fmt), __VA_ARGS__)

#define cli_puts_red(text) puts(cli_text_red(text))
#define cli_puts_green(text) puts(cli_text_green(text))
#define cli_puts_brown(text) puts(cli_text_brown(text))
#define cli_puts_blue(text) puts(cli_text_blue(text))
#define cli_puts_purple(text) puts(cli_text_purple(text))


#define cli_cursor_move_top_left() fputs(CURSOR_MOVE_TOP_LEFT, stdout)
#define cli_cursor_hide() fputs(CURSOR_HIDE, stdout)
#define cli_cursor_show() fputs(CURSOR_SHOW, stdout)

#define cli_clear() system("clear")
#define cli_wait_eol()                                \
    {                                                 \
        while (getchar() != '\n' && getchar() != EOF) \
            ;                                         \
    }

void cli_blocking_terminal(void);
void cli_nonblocking_terminal(void);