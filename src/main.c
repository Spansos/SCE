#include <ncurses.h>
#include <locale.h>
#include <stdlib.h>

#include "modes.h"
#include "editor_state.h"
#include "command.h"
#include "insert.h"

#define max(a,b) ({__typeof__(a) _a = (a); __typeof__(b) _b = (b); _a > _b ? _a : _b; })
#define min(a,b) ({__typeof__(a) _a = (a); __typeof__(b) _b = (b); _a < _b ? _a : _b; })

int main() {
    setlocale(LC_ALL, "");

    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();

    intrflush(stdscr, FALSE);

    struct sce_state state = {0};

    state.text = (struct sce_text){.count=1, .lines=malloc(sizeof(struct sce_line))};
    state.text.lines->count = 0;
    state.text.lines->text = NULL;

    state.pointer_count = 1;
    state.pointers = malloc(sizeof(struct sce_pointer));
    state.pointers->position.column = 0;
    state.pointers->position.line = 0;
    state.pointers->start_selection = state.pointers->position;

    enum sce_mode mode = SCE_COMMAND;

    while (true) {
        int input = getch();
        
        // if (c==KEY_UP)
        //     y--;
        // else if (c==KEY_DOWN)
        //     y++;
        if (input==KEY_RIGHT)
            state.pointers->position.column++;
        else if (input==KEY_LEFT)
            state.pointers->position.column--;

        switch (mode) {
            case SCE_INSERT:
                mode = sce_insert(&state, input);
                break;
            case SCE_COMMAND:
                mode = sce_command(&state, input);
                break;
            default:
                fprintf(stderr, "oopsie woopsie\n");
        }

        if (mode == SCE_EXIT)
            break;

        clear();

        // draw text
        size_t line_n = state.scroll;
        unsigned int y = 0;
        while (line_n < state.text.count) {
            struct sce_line * line = state.text.lines + line_n;
            mvaddnstr(y, 0, line->text, line->count);
            y++;
            line_n++;
        }

        refresh();
    }

    endwin();

    // output written text to stdout
    for (size_t line_n=0; line_n<state.text.count; line_n++) {
        // '\n' between lines
        if (line_n)
            fprintf(stdout, "\n");
        fprintf(stdout, "%.*s", (int)state.text.lines[line_n].count, state.text.lines[line_n].text);
    }

    return 0;
}