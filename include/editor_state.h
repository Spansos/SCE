#ifndef SCE_EDITOR_STATE
#define SCE_EDITOR_STATE

#include <stdint.h>
#include <stddef.h>

struct sce_line {
    char * text;
    size_t count;
};

struct sce_text {
    struct sce_line * lines;
    size_t count;
};

struct sce_position {
    size_t line;
    size_t column;
};

struct sce_pointer {
    struct sce_position position;
    struct sce_position start_selection;
};

struct sce_state {
    // version
    uint16_t version;
    // null-terminated array of pointers
    struct sce_pointer * pointers;
    size_t pointer_count;
    // text (maybe change to something stream-based so that very large files always work correctly)
    struct sce_text text;
    // amount of scroll
    size_t scroll;
};


#endif