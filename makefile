# compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -g

# directories and files
SRC_DIR = src
INC_DIR = include
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:.c=.o)
TARGET = shell

# default target
all: $(TARGET)

# linking the executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# compiling objects
%.o: %.c
	$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@

# clean target removes binaries and object files
clean:
	rm -f $(SRC_DIR)/*.o $(TARGET)
