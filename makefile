CC = gcc
CFLAGS = -Wall 
SRCS = command_shell.c parse_func.c 
OBJS = command_shell.o parse_func.o 
TARGET = command_shell

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

%.o: %.c parse_func.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET) 