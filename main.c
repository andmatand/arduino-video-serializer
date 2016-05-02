#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include "lib/arduino_serial.h"
#include "lib/png_reader.h"

#define DEVICE_NAME "/dev/ttyACM0"
#define BAUD_RATE 115200
#define FRAME_FORMAT "asset/frame/%03d.png"


int port;

int keep_trying_to_write_byte(int fd, uint8_t byte) {
    while (1) {
        int result = serialport_writebyte(fd, byte);
        if (result == 0) break;

        // Wait 1 millisecond before trying again
        usleep(1 * 1000);
    }

    return 0;
}

void send_pixel(uint8_t color) {
    keep_trying_to_write_byte(port, color);
}

void send_image(coolpng png) {
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

            //printf("%d (%d, %d, %d)\n", x, px[0], px[1], px[2]);
            //printf("%d (%d, %d, %d)\n", x, r, g, b);

            send_pixel(combinedColor);
        }
    }
}

// Detect how many frames there are by checking for the existence of
// asset/frame*.png
int count_frames() {
    bool fileExists = true;
    char filename[255];
    int i = 0;
    while (fileExists) {
        sprintf(filename, FRAME_FORMAT, i++);
        fileExists = (access(filename, F_OK) != -1);
    }

    return i - 1;
}

void send_images(int numFrames) {
    for (int i = 0; i < numFrames; i++) {
        char filename[255];
        sprintf(filename, FRAME_FORMAT, i);

        //coolpng png = read_png_file("asset/test.png");
        coolpng png = read_png_file(filename);

        send_image(png);

        // delay to make framerate look correct
        //usleep(50 * 1000);
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

    printf("Flushing...");
    serialport_flush(port);
    printf("Done\n");

    //printf("Sending image...");
    //do_stuff_with_png();
    //printf("Done\n");
    //

    int numFrames = count_frames();
    printf("Detected %d frames\n", numFrames);

    while (1) {
        send_images(numFrames);
    }

    serialport_close(port);

    return 0;
}
