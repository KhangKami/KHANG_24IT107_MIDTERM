CC      = gcc
CFLAGS  = -Wall -Wextra -g -Iinclude
TARGET  = myls
OBJS    = src/main.o src/options.o src/entry.o src/utils.o src/list.o src/print.o src/sort.o src/format.o src/ls.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
