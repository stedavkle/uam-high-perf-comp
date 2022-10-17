#include <emmintrin.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <sys/time.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <x86intrin.h>

static inline void getRGB(uint8_t *im, int width, int height, int nchannels, int x, int y, __m128i *datal, __m128i *datah)
{
    unsigned char *offset = im + (x + width * y) * nchannels;

    
    *datal = _mm_loadl_epi64((__m128i *)offset);
    *datah = _mm_loadl_epi64((__m128i *)(offset + 8));
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
        __m128i datal, datah; 
        __m256i datal32, datah32; 
        __m256 datalf, datahf;
        __m256 weights = _mm256_set_ps(0.2989, 0.5870, 0.1140, 0, 0.2989, 0.5870, 0.1140, 0);
        __m256 result;
        __m128i result_final;

        for (int i = 0; i < width; i+=4)
        {
            for (int j = 0; j < height; j++)
            {
                // get vector of values from pixels
                getRGB(rgb_image, width, height, 4, i, j, &datal, &datah);
                // convert 8 bit to 32 bit - 0 extend all rgba valiues
                datal32 = _mm256_cvtepu8_epi32(datal);
                datah32 = _mm256_cvtepu8_epi32(datah);
                // convert to float
                datalf = _mm256_cvtepi32_ps(datal32);
                datahf = _mm256_cvtepi32_ps(datah32);
                // multiply by weights 
                datalf = _mm256_mul_ps(datalf, weights);
                datahf = _mm256_mul_ps(datahf, weights);
                // horizontal add to get sum of all values
                result = _mm256_hadd_ps(datalf, datahf);
                result = _mm256_hadd_ps(result, result);

                // perm index 0 4 1 5
                // permutate
                result = _mm256_permutevar8x32_ps(result, _mm256_set_epi32(0, 4, 1, 5, 0, 0, 0, 0));
                
                // extract low 128 bits and convert to int
                result_final = _mm_cvtps_epi32(_mm256_extractf128_ps(result, 1));
        
                // store result_final in grey_image at position i,j and further
                grey_image[i + width * j] = _mm_extract_epi8(result_final, 0);
                grey_image[i + width * j + 1] = _mm_extract_epi8(result_final, 4);
                grey_image[i + width * j + 2] = _mm_extract_epi8(result_final, 8);
                grey_image[i + width * j + 3] = _mm_extract_epi8(result_final, 12);
                
            }
        }

        stbi_write_jpg(grey_image_filename, width, height, 1, grey_image, 10);
        free(rgb_image);

        gettimeofday(&fin,NULL);

	    printf("Tiempo: %f\n", ((fin.tv_sec*1000000+fin.tv_usec)-(ini.tv_sec*1000000+ini.tv_usec))*1.0/1000000.0);
        free(grey_image_filename);
    }
}
