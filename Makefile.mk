# Makefile para Windows

PROG = quadtree.exe
FONTES = main.c quadtree.c lib\SOIL\image_DXT.c lib\SOIL\image_helper.c lib\SOIL\SOIL.c lib\SOIL\stb_image_aug.c
OBJETOS = $(FONTES:.c=.o)
CFLAGS = -g -Wall -Iinclude #-g  # Todas as warnings, infos de debug
LDFLAGS = -Llib\GL -lfreeglut -lopengl32 -lglu32 -lm
CC = gcc

$(PROG): $(OBJETOS)
	gcc $(CFLAGS) $(OBJETOS) -o $@ $(LDFLAGS)

clean:
	-@ del $(OBJETOS) $(PROG)