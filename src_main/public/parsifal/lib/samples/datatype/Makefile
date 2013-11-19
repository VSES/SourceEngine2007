APP = datatype
all: $(APP)
## install root location:
PREFIX=/usr/local
## libraries to link with:
LIBS = -lparsifal

CC = gcc
CFLAGS = -Wall -g
CPPFLAGS = -I/usr/include
BIN_INSTALL_DIR=$(PREFIX)/bin

$(APP): $(APP).o
	$(CC) $(CFLAGS) $(LIBS) $(APP).c -o $(APP)

install: $(APP)
	install -D -m555 $(APP) $(BIN_INSTALL_DIR)/$(APP)

uninstall:
	rm -f $(BIN_INSTALL_DIR)/$(APP)

clean:
	rm -f $(APP).o

.PHONY: all clean
