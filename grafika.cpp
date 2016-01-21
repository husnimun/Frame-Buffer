#include <stdlib.h>
#include <vector>
#include <string>
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
int fontHeight = 9;


int coordinateA[9][5] = {
    {0, 0, 0, 0, 0},
    {0, 1, 1, 1, 0},
    {1, 0, 0, 0, 1},
    {1, 1, 1, 1, 1},
    {1, 0, 0, 0, 1},
    {1, 0, 0, 0, 1},
    {1, 0, 0, 0, 1},
    {1, 0, 0, 0, 1},
    {0, 0, 0, 0, 0}
};

int coordinateB[9][5] = {
    {0, 0, 0, 0, 0},
    {1, 1, 1, 1, 0},
    {1, 0, 0, 0, 1},
    {1, 0, 0, 0, 1},
    {1, 1, 1, 1, 0},
    {1, 0, 0, 0, 1},
    {1, 0, 0, 0, 1},
    {1, 1, 1, 1, 0},
    {0, 0, 0, 0, 0}
};

int coordinateI[9][5] = {
    {0, 0, 0, 0, 0},
    {0, 1, 1, 1, 0},
    {0, 0, 1, 0, 0},
    {0, 0, 1, 0, 0},
    {0, 0, 1, 0, 0},
    {0, 0, 1, 0, 0},
    {0, 0, 1, 0, 0},
    {0, 1, 1, 1, 0},
    {0, 0, 0, 0, 0}
};

int coordinateT[9][5] = {
    {0, 0, 0, 0, 0},
    {1, 1, 1, 1, 1},
    {0, 0, 1, 0, 0},
    {0, 0, 1, 0, 0},
    {0, 0, 1, 0, 0},
    {0, 0, 1, 0, 0},
    {0, 0, 1, 0, 0},
    {0, 0, 1, 0, 0},
    {0, 0, 0, 0, 0}
};

int coordinateN[9][5] = {
    {0, 0, 0, 0, 0},
    {1, 0, 0, 0, 1},
    {1, 1, 0, 0, 1},
    {1, 0, 1, 0, 1},
    {1, 0, 0, 1, 1},
    {1, 0, 0, 0, 1},
    {1, 0, 0, 0, 1},
    {1, 0, 0, 0, 1},
    {0, 0, 0, 0, 0}
};

int coordinateR[9][5] = {
    {0, 0, 0, 0, 0},
    {1, 1, 1, 1, 0},
    {1, 0, 0, 0, 1},
    {1, 0, 0, 0, 1},
    {1, 1, 1, 1, 0},
    {1, 0, 0, 1, 0},
    {1, 0, 0, 0, 1},
    {1, 0, 0, 0, 1},
    {0, 0, 0, 0, 0}
};

int coordinateH[9][5] = {
    {0, 0, 0, 0, 0},
    {1, 0, 0, 0, 1},
    {1, 0, 0, 0, 1},
    {1, 0, 0, 0, 1},
    {1, 1, 1, 1, 1},
    {1, 0, 0, 0, 1},
    {1, 0, 0, 0, 1},
    {1, 0, 0, 0, 1},
    {0, 0, 0, 0, 0}
};

int coordinateM[9][5] = {
    {0, 0, 0, 0, 0},
    {1, 0, 0, 0, 1},
    {1, 1, 0, 1, 1},
    {1, 0, 1, 0, 1},
    {1, 0, 0, 0, 1},
    {1, 0, 0, 0, 1},
    {1, 0, 0, 0, 1},
    {1, 0, 0, 0, 1},
    {0, 0, 0, 0, 0}
};

int coordinateW[9][5] = {
    {0, 0, 0, 0, 0},
    {1, 0, 0, 0, 1},
    {1, 0, 0, 0, 1},
    {1, 0, 0, 0, 1},
    {1, 0, 0, 0, 1},
    {1, 0, 1, 0, 1},
    {1, 1, 0, 1, 1},
    {1, 0, 0, 0, 1},
    {0, 0, 0, 0, 0}
};

int coordinateO[9][5] = {
    {0, 0, 0, 0, 0},
    {0, 1, 1, 1, 0},
    {1, 0, 0, 0, 1},
    {1, 0, 0, 0, 1},
    {1, 0, 0, 0, 1},
    {1, 0, 0, 0, 1},
    {1, 0, 0, 0, 1},
    {0, 1, 1, 1, 0},
    {0, 0, 0, 0, 0}
};

int coordinateC[9][5] = {
    {0, 0, 0, 0, 0},
    {0, 1, 1, 1, 0},
    {1, 0, 0, 0, 1},
    {1, 0, 0, 0, 0},
    {1, 0, 0, 0, 0},
    {1, 0, 0, 0, 0},
    {1, 0, 0, 0, 1},
    {0, 1, 1, 1, 0},
    {0, 0, 0, 0, 0}
};

int coordinateS[9][5] = {
    {0, 0, 0, 0, 0},
    {0, 1, 1, 1, 0},
    {1, 0, 0, 0, 1},
    {1, 0, 0, 0, 0},
    {0, 1, 1, 1, 0},
    {0, 0, 0, 0, 1},
    {1, 0, 0, 0, 1},
    {0, 1, 1, 1, 0},
    {0, 0, 0, 0, 0}
};

int coordinateY[9][5] = {
    {0, 0, 0, 0, 0},
    {1, 0, 0, 0, 1},
    {1, 0, 0, 0, 1},
    {0, 1, 0, 1, 0},
    {0, 0, 1, 0, 0},
    {0, 0, 1, 0, 0},
    {0, 0, 1, 0, 0},
    {0, 0, 1, 0, 0},
    {0, 0, 0, 0, 0}
};

int coordinateU[9][5] = {
    {0, 0, 0, 0, 0},
    {1, 0, 0, 0, 1},
    {1, 0, 0, 0, 1},
    {1, 0, 0, 0, 1},
    {1, 0, 0, 0, 1},
    {1, 0, 0, 0, 1},
    {1, 0, 0, 0, 1},
    {0, 1, 1, 1, 0},
    {0, 0, 0, 0, 0}
};


void drawBlock(int offsetX, int offsetY, int color) {
    int r = rand() % 256;
    int g = rand() % 256;
    int b = rand() % 256; 
    if (offsetY < 600 && offsetY > 0) {
        for (x = 0; x < blockSize; x++) {
            for (y = 0; y < blockSize; y++) {
                location = (offsetX +x + vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
                           (offsetY +y + vinfo.yoffset) * finfo.line_length;

                if (color == 1) {
                    *(fbp + location) = g;            // Some blue
                    *(fbp + location + 1) = b;        // A little green
                    *(fbp + location + 2) = r;        // A lot of red
                    *(fbp + location + 3) = 0;          // No transparency 
                } else {
                    *(fbp + location) = 0;              // Some blue
                    *(fbp + location + 1) = 0;          // A little green
                    *(fbp + location + 2) = 0;          // A lot of red
                    *(fbp + location + 3) = 0;          // No transparency 
                }
                
            }
        }
    } 
}

void copyMatrix(int M1[9][5], int M2[9][5]) {
    for (int i = 0; i < fontHeight; i++) {
        for (int j = 0; j < fontWidth; j++) {
            M2[i][j] = M1[i][j];
        }
    }
}

void drawFont(int xPos, int yPos, char c) {
    int coordinate[9][5];

    switch(c) {
        case 'A':
            copyMatrix(coordinateA, coordinate);
            break;
        case 'B':
            copyMatrix(coordinateB, coordinate);
            break;
        case 'C':
            copyMatrix(coordinateC, coordinate);
            break;
        case 'H':
            copyMatrix(coordinateH, coordinate);
            break;
        case 'I':
            copyMatrix(coordinateI, coordinate);
            break;
        case 'M':
            copyMatrix(coordinateM, coordinate);
            break;
        case 'N':
            copyMatrix(coordinateN, coordinate);
            break;
        case 'O':
            copyMatrix(coordinateO, coordinate);
            break;
        case 'R':
            copyMatrix(coordinateR, coordinate);
            break;
        case 'S':
            copyMatrix(coordinateS, coordinate);
            break;
        case 'T':
            copyMatrix(coordinateT, coordinate);
            break;
        case 'U':
            copyMatrix(coordinateU, coordinate);
            break;
        case 'W': 
            copyMatrix(coordinateW, coordinate);
            break;
        case 'Y': 
            copyMatrix(coordinateY, coordinate);
            break;
    }

    for (int x = 0; x < fontWidth; x++) {
        for (int y = 0; y < fontHeight; y++) {
            drawBlock(xPos + x*blockSize, yPos + y*blockSize, coordinate[y][x]);
        }
    }
}

void drawString(int xPos, int yPos, std::string str) {
    int x = 0;
    int y = 0;
    for (int i = 0; i < str.length(); i++) {
        drawFont(xPos +x, yPos + y, str[i]);
        x += 60;
    }
}

void clear(int xRes, int yRes) {
    for (int x = 0; x < xRes - 10; x++) {
        for (int y = 0; y < yRes - 10; y++) {
            location = (x + vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
                       (y + vinfo.yoffset) * finfo.line_length;

            *(fbp + location) = 0;        // Some blue
            *(fbp + location + 1) = 0;      // A little green
            *(fbp + location + 2) = 0;      // A lot of red
            *(fbp + location + 3) = 0;      // No transparency
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

    clear(vinfo.xres, vinfo.yres);

    for (int i = 50; i > 0; i--) {
        drawString((vinfo.xres - 60*5)/2, i*10 + 0, "NITHO");
        drawString((vinfo.xres - 60*5)/2, i*10 + 100, "HUSNI");
        drawString((vinfo.xres - 60*4)/2, i*10 + 200, "BAYU");
        drawString((vinfo.xres - 60*6)/2, i*10 + 300, "RAHMAN");
        drawString((vinfo.xres - 60*4)/2, i*10 + 400, "ICHA");
        drawString((vinfo.xres - 60*4)/2, i*10 + 500, "BOWO");
        usleep(100000);        
    }
    
    munmap(fbp, screensize);
    close(fbfd);
    return 0;
}
