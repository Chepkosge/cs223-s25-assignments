CC=gcc
SOURCES=decipher.c shot.c madlibs.c stripes.c
FILES := $(subst .c,,$(SOURCES))
FLAGS=-g -Wall -Wvla -Werror -Wno-unused-variable -Wno-unused-but-set-variable

all: $(FILES)

stripes :: stripes.c
	$(CC) $(FLAGS) $< write_ppm.c -o $@ -lpthread -lm

% :: %.c
	$(CC) $(FLAGS) $< -o $@ -lpthread -lm

clean:
	rm -rf $(FILES)



