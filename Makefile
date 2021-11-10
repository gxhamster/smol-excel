SOURCES=CellGrid.c main.c Stack.c
CFLAGS=-Wall -Wextra -g
OUT=excel

all: $(SOURCES)
	gcc $(CFLAGS) $(SOURCES) -o $(OUT)

clean:
	rm $(OUT)
