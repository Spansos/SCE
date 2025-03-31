CC     := gcc
CFLAGS := -g -Wall -Wextra -Wpedantic -Werror -Og -Iinclude

# c and header files from own project
CPP_FILES := $(wildcard src/*/*.c) $(wildcard src/*.c)
HEADERS   := $(wildcard include/*/*.h) $(wildcard include/*.h)

# packages from package manager
PKG_LIBS := -lncurses

# obj files. dependant on c files, so last
OBJ_FILES := $(patsubst src/%.c,obj/%.o,$(CPP_FILES))

.PHONY: clean run

all: bin/main.out

run: bin/main.out
	./bin/main.out

clean:
	rm obj/* -rf
	rm bin/* -rf

obj/%.o: src/%.c $(HEADERS)
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

bin/main.out: $(OBJ_FILES)
	$(CC) $(CFLAGS) $(PKG_LIBS) $^ -o $@