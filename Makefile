CFLAGS = $(shell pkg-config --cflags gtk+-3.0 gtk-layer-shell-0)
LDFLAGS = $(shell pkg-config --libs gtk+-3.0 gtk-layer-shell-0)

all: build

build: src/main.c
	$(CC) $(CFLAGS) -o komarubar src/main.c $(LDFLAGS)

clean:
	rm -f komarubar
