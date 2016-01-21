#include <stdlib.h>
#include <vector>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

int fbfd = 0;
struct fb_var_screeninfo vinfo;
struct fb_fix_screeninfo finfo;
long int screensize = 0;
char *fbp = 0;
int x = 0, y = 0;
long int location = 0;
int blockSize = 10;
int fontWidth = 5;
int fontHeight = 7;


int coordinateA[7][5] = {
    {0, 1, 1, 1, 0},
    {1, 0, 0, 0, 1},
    {1, 1, 1, 1, 1},
    {1, 0, 0, 0, 1},
    {1, 0, 0, 0, 1},
    {1, 0, 0, 0, 1},
    {1, 0, 0, 0, 1}
};

int coordinateB[7][5] = {
    {1, 1, 1, 1, 0},
    {1, 0, 0, 0, 1},
    {1, 0, 0, 0, 1},
    {1, 1, 1, 1, 0},
    {1, 0, 0, 0, 1},
    {1, 0, 0, 0, 1},
    {1, 1, 1, 1, 0}
};


void drawBlock(int offsetX, int offsetY) {
    for (x = 0; x < blockSize; x++) {
        for (y = 0; y < blockSize; y++) {
            location = (offsetX +x + vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
                       (offsetY +y + vinfo.yoffset) * finfo.line_length;

            *(fbp + location) = 255;        // Some blue
            *(fbp + location + 1) = 0;      // A little green
            *(fbp + location + 2) = 0;      // A lot of red
            *(fbp + location + 3) = 0;      // No transparency
        }
    }
}

void copyMatrix(int M1[7][5], int M2[7][5]) {
    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 5; j++) {
            M2[i][j] = M1[i][j];
        }
    }
}

void drawFont(int xPos, int yPos, char c) {
    int coordinate[7][5];

    switch(c) {
        case 'A':
            copyMatrix(coordinateA, coordinate);
            break;
        case 'B':
            copyMatrix(coordinateB, coordinate);
            break;
        case 'C':
            //copyMatrix(coordinateC, coordinate);
            break;
        case 'H':
            //copyMatrix(coordinateH, coordinate);
            break;
        case 'I':
            //copyMatrix(coordinateI, coordinate);
            break;
        case 'M':
            //copyMatrix(coordinateM, coordinate);
            break;
        case 'N':
            //copyMatrix(coordinateN, coordinate);
            break;
        case 'O':
            //copyMatrix(coordinateO, coordinate);
            break;
        case 'R':
            //copyMatrix(coordinateR, coordinate);
            break;
        case 'T':
            //copyMatrix(coordinateT, coordinate);
            break;
        case 'W': 
            //copyMatrix(coordinateW, coordinate);
            break;  
    }
    
    for(int x = 0; x < fontWidth; x++) {
        for(int y = 0; y < fontHeight; y++) {
            if (coordinate[y][x] == 1) {
                drawBlock(xPos + x*blockSize, yPos + y*blockSize);
            }
        }
    }
}



int main()
{
    // Open the file for reading and writing
    fbfd = open("/dev/fb0", O_RDWR);
    if (fbfd == -1) {
        perror("Error: cannot open framebuffer device");
        exit(1);
    }
    printf("The framebuffer device was opened successfully.\n");

    // Get fixed screen information
    if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo) == -1) {
        perror("Error reading fixed information");
        exit(2);
    }

    // Get variable screen information
    if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo) == -1) {
        perror("Error reading variable information");
        exit(3);
    }

    printf("%dx%d, %dbpp\n", vinfo.xres, vinfo.yres, vinfo.bits_per_pixel);

    // Figure out the size of the screen in bytes
    screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;

    // Map the device to memory
    fbp = (char *)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
    if (*fbp - '0' == -1) {
        perror("Error: failed to map framebuffer device to memory");
        exit(4);
    }
    printf("The framebuffer device was mapped to memory successfully.\n");

    drawFont(100, 100, 'A');
    drawFont(200, 200, 'B');
    
    munmap(fbp, screensize);
    close(fbfd);
    return 0;
}
