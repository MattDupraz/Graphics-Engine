OBJDIR = obj
INCDIR = inc
SRCDIR = src

CC = g++
CFLAGS = -I./$(INCDIR)
LIBS = -lncurses


_HEADERS = vect.h
HEADERS = $(patsubst %,$(INCDIR)/%,$(_HEADERS))

_OBJECTS = main.o
OBJECTS = $(patsubst %,$(OBJDIR)/%,$(_OBJECTS))

TARGET = test

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(HEADERS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(TARGET): $(OBJECTS)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(OBJDIR)/*.o
