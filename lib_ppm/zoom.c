#include <stdio.h>
#include <stdlib.h>
#include "lib_ppm.h"

void generate_zoomed_subpixels(struct pixel_s pixel, struct pixel_s subpixels[9])
{

    for (int i = 0; i < 9; i++) {
        subpixels[i].r = 0;
        subpixels[i].g = 0;
        subpixels[i].b = 0;
    }

    if (pixel.r >= 180) {
        for (int i = 0; i < 3; i++) subpixels[i].r = pixel.r;
    } else if (pixel.r >= 135) {
        subpixels[0].r = pixel.r;
        subpixels[2].r = pixel.r;
    } else if (pixel.r >= 75) {
        subpixels[1].r = pixel.r;
    }

    if (pixel.g >= 180) {
        for (int i = 3; i < 6; i++) subpixels[i].g = pixel.g;
    } else if (pixel.g >= 135) {
        subpixels[3].g = pixel.g;
        subpixels[5].g = pixel.g;
    } else if (pixel.g >= 75) {
        subpixels[4].g = pixel.g;
    }

    if (pixel.b >= 180) {
        for (int i = 6; i < 9; i++) subpixels[i].b = pixel.b;
    } else if (pixel.b >= 135) {
        subpixels[6].b = pixel.b;
        subpixels[8].b = pixel.b;
    } else if (pixel.b >= 75) {
        subpixels[7].b = pixel.b;
    }
}

int main(int argc, char *argv[])
{
    if (argc < 3) {
        printf("Uso: %s <imagem_entrada.ppm> <imagem_saida.ppm>\n", argv[0]);
        return -1;
    }

    struct image_s input_image, output_image;

    if (read_ppm(argv[1], &input_image) != 0) {
        printf("Erro ao ler a imagem %s\n", argv[1]);
        return -1;
    }

    if (new_ppm(&output_image, input_image.width * 3, input_image.height * 3) != 0) {
        printf("Erro ao criar a nova imagem.\n");
        free_ppm(&input_image);
        return -1;
    }

    for (int y = 0; y < input_image.height; y++) {
        for (int x = 0; x < input_image.width; x++) {
            struct pixel_s pixel = input_image.pix[y * input_image.width + x];
            struct pixel_s subpixels[9];
            
            generate_zoomed_subpixels(pixel, subpixels);

            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    output_image.pix[(3 * y + i) * output_image.width + (3 * x + j)] = subpixels[i * 3 + j];
                }
            }
        }
    }

    if (write_ppm(argv[2], &output_image) != 0) {
        printf("Erro ao escrever a imagem %s\n", argv[2]);
    }

    free_ppm(&input_image);
    free_ppm(&output_image);

    return 0;
}
