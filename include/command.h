#ifndef SCE_COMMAND_H
#define SCE_COMMAND_H

#include "editor_state.h"
#include "modes.h"

enum sce_mode sce_command(struct sce_state * state, int input);

#endif