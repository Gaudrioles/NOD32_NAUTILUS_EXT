.PHONY: all clean install uninstall

# Variables #
CC = gcc
BIN = nautilus_nod32.so
SRC= $(wildcard *.c)
OBJ= $(SRC:.c=.o)

LIBDIR=/usr/lib/x86_64-linux-gnu

# Flags pour le Compilateur et l'editeur de liens #

CFLAGS = -fPIC -g \
	$(shell pkg-config --cflags libnautilus-extension)

LFLAGS = -shared \
	$(shell pkg-config --libs libnautilus-extension)

all: $(BIN)

%.o: %.c *.h
	$(CC) -c $< $(CFLAGS) -o $@

%.o: %.c
	$(CC) -c $< $(CFLAGS) -o $@

$(BIN): $(OBJ)
	$(CC) -s $^ $(LFLAGS) -o $@

install: all
	mkdir -p $(LIBDIR)/nautilus/extensions-3.0/
	sudo cp $(BIN) $(LIBDIR)/nautilus/extensions-3.0/

uninstall: all
	sudo rm -f $(LIBDIR)/nautilus/extensions-3.0/$(BIN)

# Nettoyage #
clean:
	rm -f $(OBJ) $(BIN)
