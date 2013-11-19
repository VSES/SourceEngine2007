APP = xmlplint

## install root location:
PREFIX=/usr/local
CURL_LIBS = -lcurl -lidn -lssl -lcrypto -ldl -lssl -lcrypto -ldl -lz
LIBS = -L/usr/lib $(CURL_LIBS) -lparsifal
OBJ = catalogs.o curlread.o events.o rtrip.o stimer.o vfilter.o xmlplint.o
CC = gcc
CPPFLAGS = -I/usr/include
CFLAGS = -Wall -g

BIN_INSTALL_DIR=$(PREFIX)/bin

$(APP): $(OBJ)
	$(CC) $(OBJ) -o $(APP) $(LIBS)

install: $(APP)
	install -D -m555 $(APP) $(BIN_INSTALL_DIR)/$(APP)

uninstall:
	rm -f $(BIN_INSTALL_DIR)/$(APP)

clean:
	rm -f $(OBJ)

.PHONY: all all-before all-after clean clean-custom

all: all-before $(APP) all-after

catalogs.o: catalogs.c
	$(CC) -c catalogs.c -o catalogs.o $(CFLAGS)

curlread.o: curlread.c
	$(CC) -c curlread.c -o curlread.o $(CFLAGS)

events.o: events.c
	$(CC) -c events.c -o events.o $(CFLAGS)

rtrip.o: rtrip.c
	$(CC) -c rtrip.c -o rtrip.o $(CFLAGS)

stimer.o: stimer.c
	$(CC) -c stimer.c -o stimer.o $(CFLAGS)

vfilter.o: vfilter.c
	$(CC) -c vfilter.c -o vfilter.o $(CFLAGS)

xmlplint.o: xmlplint.c
	$(CC) -c xmlplint.c -o xmlplint.o $(CFLAGS)

