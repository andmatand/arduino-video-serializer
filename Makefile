LIBDIR=lib/
CFLAGS=-O -Wall -std=gnu99
TARGET=video-serializer

all: arduino_serial.o png_reader.o main

arduino_serial.o:
	$(CC) $(CFLAGS) -c $(LIBDIR)arduino_serial.c -o $(LIBDIR)arduino_serial.o

png_reader.o:
	$(CC) $(CFLAGS) -c $(LIBDIR)png_reader.c -o $(LIBDIR)png_reader.o

main: main.c
	$(CC) $(CFLAGS) main.c -o $(TARGET) $(LIBDIR)*.o -lpng

clean:
	rm -f $(LIBDIR)*.o
	rm -f *.o
	rm -f $(TARGET)
