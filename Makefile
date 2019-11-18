CC := gcc
CFLAGS := -O0

$(phony all): visbuf

visbuf: ./*.o
	$(CC) $(CFLAGS) -o $(output) $(inputs)

%.o: src/%.c
	$(CC) $(CFLAGS) -o $(output) -c $(input)
