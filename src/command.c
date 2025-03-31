#include "command.h"

#include <stdlib.h>
#include <ctype.h>
#include <string.h>

struct sce_command {
    char * command;
    size_t length;
};

enum sce_mode sce_command(struct sce_state * state, int input) {
    static struct sce_command command = {0};

    (void)state;

    if (isprint(input)) {
        command.length++;
        command.command = realloc(command.command, command.length);
        command.command[command.length-1] = input;
    }

    if (command.length >= 4 && memcmp(command.command, "exit", 4) == 0)
        return SCE_EXIT;

    if (input == 27) {
        free(command.command);
        command.length = 0;
        return SCE_INSERT;
    }
    return SCE_COMMAND;
}