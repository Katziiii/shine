CC = gcc
CFLAGS = -Wall -Wextra

SRC = main.c rom.c cpu.c memory.c bus.c
OBJ = $(SRC:.c=.o)

TARGET = shine

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o $(TARGET)
