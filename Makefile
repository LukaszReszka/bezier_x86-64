CC = gcc
CFLAGS = -pedantic -Wall -std=c99 -m64
NFLAGS = -f elf64
ALLEGRO = allegro-5 allegro_image-5 allegro_font-5 allegro_ttf-5 allegro_primitives-5
TARGET = bezier

all: main.o draw_bezier_curve.o
	$(CC) $(CFLAGS) main.o draw_bezier_curve.o -o $(TARGET) $$(pkg-config --cflags --libs $(ALLEGRO))

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

draw_bezier_curve.o: draw_bezier_curve.s
	nasm $(NFLAGS) -o draw_bezier_curve.o draw_bezier_curve.s

.PHONY: clean
clean:
	rm -f *.o
	rm -f $(TARGET)
	rm -f *.h.gch
