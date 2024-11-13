CC=gcc
AR=ar

OUTDIR=.

SRC=$(shell find src -name "*.c")
OBJ=$(SRC:%=%.o)

OPTIONS=

FLAGS=-g -Wall $(OPTIONS)
CFLAGS=$(FLAGS)

LDFLAGS=-lm

APP=mandel

.PHONY: app clean

tests: $(APP) $(TEST_SRC)

app: $(APP)

clean:
	rm -f $(OBJ) $(APP)

$(APP): $(OBJ)
	$(CC) $^ $(LDFLAGS) -o $@

%.c.o: %.c
	$(CC) -c $< $(CFLAGS) -o $@
