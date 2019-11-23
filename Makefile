CC := gcc
CFLAGS := -O0 -Werror -Wall
LIBS := -lm

$(phony all): visbuf

visbuf: ./*.o
	$(CC) $(CFLAGS) -o $(output) $(inputs) $(LIBS)

%.o: src/%.c
	$(CC) $(CFLAGS) -o $(output) -c $(input)
