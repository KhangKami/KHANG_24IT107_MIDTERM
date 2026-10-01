CC      = gcc
CFLAGS  = -Wall -Wextra -g -Iinclude
TARGET  = myls
OBJS    = src/main.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
