TARGET = programa

CC = gcc 

CFLAGS = -Wall -g -Iheaders/

SRC = main.c list.c type.c

$(TARGET): $(SRC)
		$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean: 
	rm -f $(TARGET)

