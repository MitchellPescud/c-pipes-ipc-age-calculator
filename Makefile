# Compiler
CC = gcc
# Compiler flags
CFLAGS = -Wall -Wextra -pedantic
# Source files
SRCS = main.c date_utils.c file_utils.c
# Object files
OBJS = $(SRCS:.c=.o)
# Executable name
TARGET = age_calculator

# Build the executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Compile Source File 
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
