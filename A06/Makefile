CC=gcc
SOURCES=tree_tests dependency match_braces
FILES := $(subst .c,,$(SOURCES))
FLAGS=-g -Wall -Wvla -Werror -Wno-unused-variable -Wno-unused-but-set-variable

# By default, make runs the first target in the file
all: $(FILES)

% :: %.c tree.c 
	$(CC) $(FLAGS) $< tree.c -o $@

clean:
	rm -rf $(FILES)

