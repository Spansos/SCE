#include "insert.h"

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <ncurses.h>

enum sce_mode sce_insert(struct sce_state * state, int input) {
    // add (printable) input to all current pointers
    if (isprint(input)) {
        for (size_t i=0; i<state->pointer_count; i++) {
            struct sce_pointer * pointer = state->pointers+i;
            struct sce_line * line = state->text.lines + pointer->position.line;
            // realloc for 1 char (maybe do capacity+size shenanigans later)
            line->count++;
            line->text = realloc(line->text, line->count);
            // insert character
            memmove(line->text+pointer->position.column+1, line->text+pointer->position.column, line->count-pointer->position.column);
            line->text[pointer->position.column] = input;
            pointer->position.column++;
            
            // update on screen
            mvwaddch(stdscr, pointer->position.line-state->scroll, pointer->position.column, input);
        }
    }
    
    // split the line if new line
    if (input == '\n') {
        // could be refactored to take some things out of loop to only do one, but im lazy
        for (size_t i=0; i<state->pointer_count; i++) {
            struct sce_pointer * pointer = state->pointers+i;

            // resize for the new line
            state->text.count++;
            state->text.lines = realloc(state->text.lines, state->text.count * sizeof(*state->text.lines));

            // move all lines after cursor one to the right
            memmove(state->text.lines+pointer->position.line+2, state->text.lines+pointer->position.line+1, state->text.count-pointer->position.line-2);

            // move chars to new line
            struct sce_line * cur_line = state->text.lines + pointer->position.line;
            struct sce_line * next_line = cur_line + 1;
            size_t copy_count = cur_line->count - pointer->position.column;
            next_line->text = malloc(copy_count);
            memcpy(next_line->text, cur_line->text+pointer->position.column, copy_count);
            next_line->count = copy_count;
            cur_line->count -= copy_count;

            // move current pointer
            pointer->position.column = 0;
            pointer->position.line++;

            // move later pointers
            for (size_t j=0; j<state->pointer_count; j++)
                if (state->pointers[j].position.line > pointer->position.line)
                    state->pointers[j].position.line++;
        }
    }

    // go to command if escape key
    if (input == 27)
        return SCE_COMMAND;
    return SCE_INSERT;
}