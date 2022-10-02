#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "functions.h"
#include "effects.h"

void select_img(img_t *memory, char par1[], char par2[],
				char par3[], char par4[])
{
	if ((*memory).alloc == 0) {
		printf("No image loaded\n");
		return;
	}
	if ((par1[0] == '\0' || par2[0] == '\0' || par3[0] == '\0' ||
		 par4[0] == '\0') && strcmp(par1, "ALL")) {
		printf("Invalid command\n");
		return;
	}
	if ((!isnumber(par1)  || !isnumber(par2) || !isnumber(par3) ||
		 !isnumber(par4)) && strcmp(par1, "ALL")) {
		printf("Invalid command\n");
		return;
	}
	int x1;
	int y1;
	int x2;
	int y2;
	if (strcmp(par1, "ALL") == 0) {
		(*memory).coord1.x = 0;
		(*memory).coord1.y = 0;
		(*memory).coord2.x = (*memory).size.x;
		(*memory).coord2.y = (*memory).size.y;
		printf("Selected ALL\n");
		return;
	}
	x1 = atoi(par1);
	y1 = atoi(par2);
	x2 = atoi(par3);
	y2 = atoi(par4);

	if (x1 > x2)
		swap_int(&x1, &x2);
	if (y1 > y2)
		swap_int(&y1, &y2);
	if (x1 < 0 || y1 < 0 || x2 < 0 || y2 < 0 || x2 > (*memory).size.x ||
		y2 > (*memory).size.y || x1 == x2 || y1 == y2) {
		printf("Invalid set of coordinates\n");
		return;
	}
	printf("Selected %d %d %d %d\n", x1, y1, x2, y2);
	(*memory).coord1.x = x1;
	(*memory).coord1.y = y1;
	(*memory).coord2.x = x2;
	(*memory).coord2.y = y2;
}

void rotate_img(img_t *memory, char par[])
{
	if ((*memory).alloc == 0) {
		printf("No image loaded\n");
		return;
	}
	int dim, angle;
	int nr_rot;

	if ((*memory).coord2.x - (*memory).coord1.x !=
		(*memory).coord2.y - (*memory).coord1.y) {
		printf("The selection must be square\n");
		return;
	}
	angle = atoi(par);
	if (angle % 90 != 0) {
		printf("Unsupported rotation angle\n");
		return;
	}
	nr_rot = angle / 90;
	if (nr_rot < 0)
		nr_rot += 4;
	for (int o = 0; o < nr_rot; ++o) { //rotating 90 degrees as much as needed
		dim = (*memory).coord2.x - (*memory).coord1.x;
		pixel_t **aux = alloc_matrix(dim, dim); //auxiliary matrix where the
		for (int i = (*memory).coord1.y, n = dim - 1; //selection is rotated
			 i < (*memory).coord2.y; ++i, --n)
			for (int j = (*memory).coord1.x, m = 0;
				 j < (*memory).coord2.x; ++j, ++m)
				for (int k = 0; k < (*memory).layers; ++k)
					aux[m][n].pixel[k] = (*memory).matrix[i][j].pixel[k];
		for (int i = 0, k = (*memory).coord1.y; i < dim; ++i, ++k) {
			for (int j = 0, l = (*memory).coord1.x; j < dim; ++j, ++l) {
				for (int m = 0; m < (*memory).layers; ++m)
					(*memory).matrix[k][l].pixel[m] = aux[i][j].pixel[m];
			}
		}
		for (int i = 0; i < dim; ++i)
			free(aux[i]);
		free(aux);
		}
		printf("Rotated %d\n", angle);
}

void rotate_all(img_t *memory, char par[])
{
	if ((*memory).alloc == 0) {
		printf("No image loaded\n");
		return;
	}
	int dimx, dimy, angle;
	int nr_rot;

	angle = atoi(par);
	if (angle % 90 != 0) {
		printf("Unsupported rotation angle\n");
		return;
	}
	nr_rot = angle / 90;
	if (nr_rot < 0)
		nr_rot += 4;
	for (int o = 0; o < nr_rot; o++) {
		dimx = (*memory).size.y;
		dimy = (*memory).size.x;
		pixel_t **aux = alloc_matrix(dimx, dimy); //auxiliary matrix
		for (int i = 0, m = dimx - 1; i < (*memory).size.y; ++i, --m)
			for (int j = 0, n = 0; j < (*memory).size.x; ++j, ++n)
				for (int k = 0; k < (*memory).layers; ++k)
					aux[n][m].pixel[k] = (*memory).matrix[i][j].pixel[k];

		free_matrix(memory);
		(*memory).size.x = dimx; //allocating the dimensions and coordinates
		(*memory).size.y = dimy;
		(*memory).coord1.x = 0;
		(*memory).coord1.y = 0;
		(*memory).coord2.x = dimx;
		(*memory).coord2.y = dimy;
		(*memory).alloc = 1; //alloc it
		(*memory).matrix = aux;
	}
	printf("Rotated %d\n", angle);
}

void crop_img(img_t *memory)
{
	if ((*memory).alloc == 0) {
		printf("No image loaded\n");
		return;
	}

	int i, j, k, l;
	int dimx = (*memory).coord2.x - (*memory).coord1.x;
	int dimy = (*memory).coord2.y - (*memory).coord1.y;
	img_t aux;
	aux.size.x = dimx;
	aux.size.y = dimy;
	alloc_image(&aux);

	i = 0;
	k = 0;
	for (i = 0, k = (*memory).coord1.y; i < aux.size.y; ++i, ++k) {
		for (j = 0, l = (*memory).coord1.x; j < aux.size.x; ++j, ++l) {
			for (int m = 0; m < (*memory).layers; ++m)
				aux.matrix[i][j].pixel[m] = (*memory).matrix[k][l].pixel[m];
		}
	}
	free_matrix(memory);
	(*memory).size.x = dimx; //allocating the dimensions and coordinates
	(*memory).size.y = dimy;
	(*memory).coord1.x = 0;
	(*memory).coord1.y = 0;
	(*memory).coord2.x = dimx;
	(*memory).coord2.y = dimy;
	(*memory).alloc = 1;
	(*memory).matrix = aux.matrix;
	printf("Image cropped\n");
}

double clamp(double pixel)
{
	if (pixel < 0)
		return 0;
	if (pixel > 255)
		return 255;
	return pixel;
}

void apply_blur(img_t *memory)
{
	if ((*memory).layers == 1) {
		printf("Easy, Charlie Chaplin\n");
		return;
	}
	img_t copy;
	copy.size.x = (*memory).size.x;
	copy.size.y = (*memory).size.y;
	alloc_image(&copy);
	for (int i = 0; i < (*memory).size.y; ++i) {
		for (int j = 0; j < (*memory).size.x; ++j) {
			for (int k = 0; k < 3; ++k) {
				if (i == 0 || j == 0 || i == (*memory).size.y - 1 ||
					j == (*memory).size.x - 1 || i < (*memory).coord1.y ||
					j < (*memory).coord1.x || i >= (*memory).coord2.y ||
					j >= (*memory).coord2.x) {
					copy.matrix[i][j].pixel[k] =
					(*memory).matrix[i][j].pixel[k];
				} else {
					copy.matrix[i][j].pixel[k] =
						((*memory).matrix[i - 1][j - 1].pixel[k] +
						(*memory).matrix[i - 1][j].pixel[k] +
						(*memory).matrix[i - 1][j + 1].pixel[k] +
						(*memory).matrix[i][j - 1].pixel[k] +
						(*memory).matrix[i][j].pixel[k] +
						(*memory).matrix[i][j + 1].pixel[k] +
						(*memory).matrix[i + 1][j - 1].pixel[k] +
						(*memory).matrix[i + 1][j].pixel[k] +
						(*memory).matrix[i + 1][j + 1].pixel[k]) / 9;
				}
			}
		}
	}
	for (int i = 0; i < (*memory).size.y; ++i) {
		for (int j = 0; j < (*memory).size.x; ++j) {
			for (int k = 0; k < 3; ++k)
				(*memory).matrix[i][j].pixel[k] =
				clamp(copy.matrix[i][j].pixel[k]);
		}
	}
	free_matrix(&copy);
	printf("APPLY BLUR done\n");
}

void apply_sharpen(img_t *memory)
{
	if ((*memory).layers == 1) { //if it is grayscale
		printf("Easy, Charlie Chaplin\n");
		return;
	}
	img_t copy;
	copy.size.x = (*memory).size.x;
	copy.size.y = (*memory).size.y;
	alloc_image(&copy);
	for (int i = 0; i < (*memory).size.y; ++i) {
		for (int j = 0; j < (*memory).size.x; ++j) {
			for (int k = 0; k < 3; ++k) {
				if (i == 0 || j == 0 || i == (*memory).size.y - 1 ||
					j == (*memory).size.x - 1 || i < (*memory).coord1.y ||
					j < (*memory).coord1.x || i >= (*memory).coord2.y ||
					j >= (*memory).coord2.x) {
					copy.matrix[i][j].pixel[k] =
					(*memory).matrix[i][j].pixel[k];
				} else {
					copy.matrix[i][j].pixel[k] =
						(-1) * (*memory).matrix[i - 1][j].pixel[k] +
						(-1) * (*memory).matrix[i][j - 1].pixel[k] +
						(-1) * (*memory).matrix[i][j + 1].pixel[k] +
						(-1) * (*memory).matrix[i + 1][j].pixel[k] +
						5 * (*memory).matrix[i][j].pixel[k];
				}
			}
		}
	}
	for (int i = 0; i < (*memory).size.y; ++i) {
		for (int j = 0; j < (*memory).size.x; ++j) {
			for (int k = 0; k < 3; ++k)
				(*memory).matrix[i][j].pixel[k] =
				clamp(copy.matrix[i][j].pixel[k]);
		}
	}
	free_matrix(&copy);
	printf("APPLY SHARPEN done\n");
}

void apply_edge(img_t *memory)
{
	if ((*memory).layers == 1) {
		printf("Easy, Charlie Chaplin\n");
		return;
	}
	img_t copy;
	copy.size.x = (*memory).size.x;
	copy.size.y = (*memory).size.y;
	alloc_image(&copy);
	for (int i = 0; i < (*memory).size.y; ++i) {
		for (int j = 0; j < (*memory).size.x; ++j) {
			for (int k = 0; k < 3; ++k) {
				if (i == 0 || j == 0 || i == (*memory).size.y - 1 ||
					j == (*memory).size.x - 1 || i < (*memory).coord1.y ||
					j < (*memory).coord1.x || i >= (*memory).coord2.y ||
					j >= (*memory).coord2.x) {
					copy.matrix[i][j].pixel[k] =
					(*memory).matrix[i][j].pixel[k];
				} else {
					copy.matrix[i][j].pixel[k] =
						(-1) * (*memory).matrix[i - 1][j - 1].pixel[k] +
						(-1) * (*memory).matrix[i - 1][j].pixel[k] +
						(-1) * (*memory).matrix[i - 1][j + 1].pixel[k] +
						(-1) * (*memory).matrix[i][j - 1].pixel[k] +
						8 * (*memory).matrix[i][j].pixel[k] +
						(-1) * (*memory).matrix[i][j + 1].pixel[k] +
						(-1) * (*memory).matrix[i + 1][j - 1].pixel[k] +
						(-1) * (*memory).matrix[i + 1][j].pixel[k] +
						(-1) * (*memory).matrix[i + 1][j + 1].pixel[k];
				}
			}
		}
	}
	for (int i = 0; i < (*memory).size.y; ++i) {
		for (int j = 0; j < (*memory).size.x; ++j) {
			for (int k = 0; k < 3; ++k)
				(*memory).matrix[i][j].pixel[k] =
				clamp(copy.matrix[i][j].pixel[k]);
		}
	}
	free_matrix(&copy);
	printf("APPLY EDGE done\n");
}

void apply_gaussianblur(img_t *memory)
{
	if ((*memory).layers == 1) {
		printf("Easy, Charlie Chaplin\n");
		return;
	}
	img_t copy;
	copy.size.x = (*memory).size.x;
	copy.size.y = (*memory).size.y;
	alloc_image(&copy);
	for (int i = 0; i < (*memory).size.y; ++i) {
		for (int j = 0; j < (*memory).size.x; ++j) {
			for (int k = 0; k < 3; ++k) {
				if (i == 0 || j == 0 || i == (*memory).size.y - 1 ||
					j == (*memory).size.x - 1 || i < (*memory).coord1.y ||
					j < (*memory).coord1.x || i >= (*memory).coord2.y ||
					j >= (*memory).coord2.x) {
					copy.matrix[i][j].pixel[k] =
					(*memory).matrix[i][j].pixel[k];
				} else {
					copy.matrix[i][j].pixel[k] =
						((*memory).matrix[i - 1][j - 1].pixel[k] +
						(*memory).matrix[i - 1][j + 1].pixel[k] +
						(*memory).matrix[i + 1][j - 1].pixel[k] +
						(*memory).matrix[i + 1][j + 1].pixel[k] +
						2 * ((*memory).matrix[i - 1][j].pixel[k] +
						(*memory).matrix[i][j - 1].pixel[k] +
						(*memory).matrix[i][j + 1].pixel[k] +
						(*memory).matrix[i + 1][j].pixel[k]) +
						4 * (*memory).matrix[i][j].pixel[k]) / 16;
				}
			}
		}
	}
	for (int i = 0; i < (*memory).size.y; ++i) {
		for (int j = 0; j < (*memory).size.x; ++j) {
			for (int k = 0; k < 3; ++k)
				(*memory).matrix[i][j].pixel[k] =
				clamp(copy.matrix[i][j].pixel[k]);
		}
	}
	free_matrix(&copy);
	printf("APPLY GAUSSIAN_BLUR done\n");
}

void filter_img(img_t *memory, char par[])
{
	if ((*memory).alloc == 0) {
		printf("No image loaded\n");
		return;
	}

	if (strcmp(par, "EDGE") == 0) {
		apply_edge(memory);
	} else {
		if (strcmp(par, "SHARPEN") == 0) {
			apply_sharpen(memory);
		} else {
			if (strcmp(par, "BLUR") == 0) {
				apply_blur(memory);
			} else {
				if (strcmp(par, "GAUSSIAN_BLUR") == 0) {
					apply_gaussianblur(memory);
				} else {
					printf("APPLY parameter invalid\n");
					return;
				}
			}
		}
	}
}
