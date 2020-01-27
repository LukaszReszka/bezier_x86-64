CC = gcc
CFLAGS = -pedantic -Wall -m64
NFLAGS = -f elf64
ALLEGRO = allegro-5 allegro_image-5 allegro_font-5 allegro_ttf-5 allegro_primitives-5
TARGET = bezier

all: main.o draw_bezier.o
	$(CC) main.o draw_bezier.o -o $(TARGET) $$(pkg-config --cflags --libs $(ALLEGRO))

main.o: main.c
	$(CC) -c main.c

draw_bezier.o: draw_bezier.s
	nasm $(NFLAGS) -o draw_bezier.o draw_bezier.s

.PHONY: clean
clean:
	rm -f *.o
	rm -f $(TARGET)
	rm -f *.h.gch
