#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <sys/time.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <x86intrin.h>

static inline void getRGB(uint8_t *im, int width, int height, int nchannels, int x, int y, __m256i *rgba8)
{

    unsigned char *offset = im + (x + width * y) * nchannels;
    int *offset_i = &offset
    // *r = offset[0];
    // *g = offset[1];
    // *b = offset[2];
    
    // only at AVX-512 IS
    //*rgba8 = _mm256_load_epi32(&offset);

    *rgba8 = _mm256_maskload_epi32(offset_i, _mm256_set_epi32(0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff));
}

int main(int nargs, char **argv)
{
    int width, height, nchannels;
    struct timeval fin,ini;

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
        uint8_t *grey_image = malloc(width * height);
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

        if (nchannels != 3 && nchannels != 4)
        {
            printf("[error] Num of channels=%d not supported. Only three (RGB), four (RGBA) are supported.\n", nchannels);
            continue;
        }

        gettimeofday(&ini,NULL);
        // RGB to grey scale
        __m256i rgba8;
        __m512d rgbvec = _mm512_broadcast_f64x4(_mm256_set_pd(0.2989, 0.5870, 0.1140, 0.0));
        __m512d rgba8_dp;
        __m512d result_dp;
        __m256i result;

        for (int i = 0; i < width; i++)
        {
            for (int j = 0; j < height; j++)
            {
                // get vector of values from 8 pixels
                getRGB(rgb_image, width, height, 4, i, j, &rgba8);

                // convert to double precision
                rgba8_dp = _mm512_cvtepi32_pd(rgba8);

                // multiply rgbvec by rgba8_dp
                result_dp = _mm512_mul_pd(rgbvec, rgba8_dp);

                // convert back to integer
                result = _mm512_cvtpd_epi32(result_dp);

                // store the values of 4 pixels
                _mm256_store_epi32(&grey_image[j * width + i], result);
            }
        }

        stbi_write_jpg(grey_image_filename, width, height, 1, grey_image, 10);
        free(rgb_image);

        gettimeofday(&fin,NULL);

	printf("Tiempo: %f\n", ((fin.tv_sec*1000000+fin.tv_usec)-(ini.tv_sec*1000000+ini.tv_usec))*1.0/1000000.0);
        free(grey_image_filename);
    }
}
