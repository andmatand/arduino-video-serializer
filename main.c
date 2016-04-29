#include <stdio.h>
#include <unistd.h>
#include "lib/arduino_serial.h"
#include "lib/png_reader.h"

#define DEVICE_NAME "/dev/ttyACM0"
#define BAUD_RATE 115200 //9600

int port;

//const uint8_t MAX_WRITE_RETRIES = 5;

int keep_trying_to_write_byte(int fd, uint8_t byte) {
    //int i;
    //for (i = 0; i < MAX_WRITE_RETRIES; i++) {
    while (1) {
        int result = serialport_writebyte(fd, byte);
        if (result == 0) break;

        // Wait 1 millisecond before trying again
        usleep(1 * 1000);
    }

    //if (i == MAX_WRITE_RETRIES) {
    //    printf("Exceeded max retries for writing\n");
    //    return 1;
    //}

    return 0;
}

void send_pixel(uint8_t color) {
    keep_trying_to_write_byte(port, color);
}

void do_stuff_with_png() {
    coolpng png = read_png_file("asset/test.png");

    for (int y = 0; y < png.height; y++) {
        png_bytep row = png.row_pointers[y];
        for (int x = 0; x < png.width; x++) {
            png_bytep px = &(row[x * 4]);

            uint8_t r = px[0];
            uint8_t g = px[1];
            uint8_t b = px[2];

            // Squash colors
            r = (r / 32) << 5;
            g = (g / 32) << 2;
            b = (b / 64);

            uint8_t combinedColor = r | g | b;

            printf("%d (%d, %d, %d)\n", x, px[0], px[1], px[2]);
            printf("%d (%d, %d, %d)\n", x, r, g, b);

            send_pixel(combinedColor);
        }
    }
}

int main(int argc, char* argv[]) {
    port = serialport_init(DEVICE_NAME, BAUD_RATE);

    printf("Opening serial port...");
    if (port == -1) {
        printf("ERROR\n");
        return 1;
    }
    else {
        printf("Done\n");
    }

    // Sleep
    //int millis = 1000 * 1000;
    //int vanillis = 2;
    //usleep(millis * vanillis);

    printf("Flushing...");
    serialport_flush(port);
    printf("Done\n");

    do_stuff_with_png();


    //const int BUFFER_SIZE = 2;
    //char buffer[BUFFER_SIZE];
    ////              rrrgggbb
    //uint8_t msg = 0b00011111;

    //int numReceived = 0;
    //int numWritten = 0;
    //for (int i = 0; i < 1000; i++) {
    //    // Write
    //    //uint8_t msg = 'a';
    //    //int result = serialport_writebyte(port, msg);
    //    //if (result != 0) {
    //    //    printf("\nError writing\n");
    //    //}

    //    // Keep trying to write
    //    int writeResult = keep_trying_to_write_byte(port, msg);
    //    if (writeResult == 0) {
    //        numWritten ++;
    //    }
    //    printf("\nSent: %d\n", (uint8_t) msg);

    //    // Read
    //    //int readResultCode = serialport_read_until(port, buffer, msg, BUFFER_SIZE, 5000);
    //    //if (readResultCode != 0) {
    //    //    printf("\nError reading (%d)\n", readResultCode);
    //    //    return 1;
    //    //}
    //    //printf("Received: %d\n", (uint8_t) buffer);

    //    //// Verify the receipt of the message
    //    //if (buffer[0] == msg) {
    //    //    numReceived ++;
    //    //}
    //    //else {
    //    //    printf("\nReceived wrong message: %d\n", (uint8_t) buffer);
    //    //}
    //    // frrgggbb
    //    // 01100011

    //    printf("Wrote %d bytes, Received %d bytes\r", numWritten, numReceived);
    //}
    //printf("\n");

    serialport_close(port);

    return 0;
}
