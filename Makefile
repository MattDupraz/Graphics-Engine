OBJDIR = obj
INCDIR = inc
SRCDIR = src

CC = g++
CFLAGS = -I./$(INCDIR)
LIBS = -lncursesw -lsfml-graphics -lsfml-window -lsfml-system


_HEADERS = vect.h matrix.h renderer.h sfml_raster.h
HEADERS = $(patsubst %,$(INCDIR)/%,$(_HEADERS))

_OBJECTS = main.o renderer.o sfml_app.o sfml_raster.o
OBJECTS = $(patsubst %,$(OBJDIR)/%,$(_OBJECTS))

TARGET = test

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(HEADERS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(TARGET): $(OBJECTS)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(OBJDIR)/*.o
