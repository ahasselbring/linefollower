CC = clang
CFLAGS = -Wall -Wextra -pedantic -Werror -pedantic-errors -std=c11 -I. -DTARGET_SIMULATOR -D_GNU_SOURCE

SRC=$(shell find -name '*.c')
OBJ=$(SRC:.c=.o)
PROG = prog

all: $(PROG)

$(PROG): $(OBJ)
	$(CC) $(CFLAGS) -lm -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	@rm -f $(OBJ) $(PROG)

.PHONY: all clean
