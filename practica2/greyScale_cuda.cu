#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <time.h>
#define STB_IMAGE_IMPLEMENTATION
#include "practica2\stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "practica2\stb_image_write.h"

#define BLOCK_SIZE 16

__global__ void greyConvert(uint8_t *in, uint8_t *out, int width, int height);

int main(int nargs, char **argv)
{
    int width, height, nchannels;
    struct timeval fin, ini;

    if (nargs < 2)
    {
        printf("Usage: %s <image1> [<image2> ...]\n", argv[0]);
    }
    // For each image
    // Bucle 0
    for (int file_i = 1; file_i < nargs; file_i++)
    {
        printf("[info] Processing %s\n", argv[file_i]);
        /****** Reading file ******/
        uint8_t *rgb_image = stbi_load(argv[file_i], &width, &height, &nchannels, 4);
        if (!rgb_image)
        {
            perror("Image could not be opened");
        }

        /****** Allocating memory ******/
        // - RGB2Grey
        uint8_t *grey_image = (uint8_t *)malloc(width * height);
        if (!grey_image)
        {
            perror("Could not allocate memory");
        }

        // - Filenames 
        for (int i = strlen(argv[file_i]) - 1; i >= 0; i--)
        {
            if (argv[file_i][i] == '.')
            {
                argv[file_i][i] = 0;
                break;
            }
        }

        char *grey_image_filename = 0;
        asprintf(&grey_image_filename, "%s_grey.jpg", argv[file_i]);
        if (!grey_image_filename)
        {
            perror("Could not allocate memory");
            exit(-1);
        }

        /****** Computations ******/
        printf("[info] %s: width=%d, height=%d, nchannels=%d\n", argv[file_i], width, height, nchannels);
        //printf("[info] Checking channels\n");
        if (nchannels != 3 && nchannels != 4)
        {
            printf("[error] Num of channels=%d not supported. Only three (RGB), four (RGBA) are supported.\n", nchannels);
            continue;
        }

        //printf("[info] Starting algorithm\n");

        gettimeofday(&ini,NULL);
        // RGB to grey scale

        int imgSize = sizeof(rgb_image);
        int gImgSize = sizeof(grey_image);

        uint8_t *d_img_in;
        uint8_t *d_img_out;

        cudaMalloc((void **) &d_img_in, imgSize);
        cudaMalloc((void **) &d_img_out, gImgSize);

        //printf("[cuda] Allocated memory\n");
        //printf("[info] Ready to run Kernel\n");
        cudaMemcpy(d_img_in, &rgb_image, imgSize, cudaMemcpyHostToDevice);
        //printf("[cuda] Copied image to device\n");

        int blocksPerGrid_x = ceil(width/BLOCK_SIZE);
        int blocksPerGrid_y = ceil(height/BLOCK_SIZE);
        dim3 blocksPerGrid(blocksPerGrid_x, blocksPerGrid_y);
        dim3 threadsPerBlock(16, 16);

        //printf("[cuda] Starting kernel\n");
        greyConvert<<<blocksPerGrid, threadsPerBlock>>>(d_img_in, d_img_out, width, height);
        //printf("[cuda] Kernel finished\n");

        cudaMemcpy(&grey_image, d_img_out, gImgSize, cudaMemcpyDeviceToHost);

        cudaFree(d_img_in), cudaFree(d_img_out);

        //printf("[info] Trying to save\n");
        stbi_write_jpg(grey_image_filename, width, height, 1, grey_image, 10);
        free(rgb_image);
        //printf("[info] Done");
        gettimeofday(&fin,NULL);

	    printf("Tiempo: %f\n", ((fin.tv_sec*1000000+fin.tv_usec)-(ini.tv_sec*1000000+ini.tv_usec))*1.0/1000000.0);
        free(grey_image_filename);
    }
}