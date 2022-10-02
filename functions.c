#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "functions.h"

void swap_int(int *a, int *b)
{
	int aux = *a;
	*a = *b;
	*b = aux;
}

int isnumber(char a[])
{
	int m = strlen(a);
	if (!isdigit(a[0]) && a[0] != '+' && a[0] != '-')
		return 0;
	for (int i = 1; i < m; ++i) {
		if (isdigit(a[i]) == 0)
			return 0;
	}
	return 1;
}

unsigned char round_value(double pixel)
{
	double frac = pixel - (int)pixel;
	if (frac <= 0.5)
		return (unsigned char)pixel;
	return (unsigned char)pixel + 1;
}

void iscomment(FILE *file)
{
	char letter;
	fscanf(file, "%c", &letter);
	if (letter == '#') {
		do {
			fscanf(file, "%c", &letter);
		} while (letter != '\n');
	} else {
		fseek(file, -1, SEEK_CUR);
	}
}

pixel_t **alloc_matrix(int xsize, int ysize)
{
	pixel_t **m = (pixel_t **)malloc(ysize * sizeof(pixel_t *));
	if (!m) {
		fprintf(stderr, "Cannot malloc()\n");
		return NULL;
	}
	for (int i  = 0; i < ysize; ++i) {
		m[i] = (pixel_t *)malloc(xsize * sizeof(pixel_t));
		if (!m[i]) {
			fprintf(stderr, "Cannot malloc()\n");
			for (int j = 0; j < i; ++j)
				free(m[j]);
			free(m);
			return NULL;
		}
	}

	return m;
}

void alloc_image(img_t *image)
{
	(*image).matrix = alloc_matrix((*image).size.x, (*image).size.y);
	(*image).alloc = 1; //marking that it is allocated
}

void free_matrix(img_t *memory)
{
	for (int i = 0; i < (*memory).size.y; ++i)
		free((*memory).matrix[i]);
	free((*memory).matrix);
	(*memory).alloc = 0;
}

void read_image(img_t *memory, FILE *filein)
{
	char type[10];
	int binary;
	iscomment(filein);
	fscanf(filein, "%s", type);
	switch (type[1]) {
	case '2':
		binary = 0;
		(*memory).layers = 1;
		break;
	case '3':
		binary = 0;
		(*memory).layers = 3;
		break;
	case '5':
		binary = 1;
		(*memory).layers = 1;
		break;
	case '6':
		binary = 1;
		(*memory).layers = 3;
		break;
	default:
		printf("No data supported\n");
		return;
	}
	iscomment(filein);
	fscanf(filein, "%d %d\n%d\n", &(*memory).size.x, &(*memory).size.y,
		   &(*memory).intensity);
	alloc_image(memory);
	unsigned char aux;
	for (int i = 0; i < (*memory).size.y; ++i) {
		for (int j = 0; j < (*memory).size.x; ++j)
			for (int k = 0; k < (*memory).layers; ++k)
				if (binary == 0) { //is ascii
					iscomment(filein);
					fscanf(filein, "%lf", &(*memory).matrix[i][j].pixel[k]);
				} else {
					fread(&aux, sizeof(unsigned char), 1, filein);
					(*memory).matrix[i][j].pixel[k] = aux;
				}
	}
}

void write_image(img_t *memory, FILE *fileout, int binary)
{
	if (binary == 0) { //is ascii
		if ((*memory).layers == 1)
			fprintf(fileout, "%s", "P2");
		if ((*memory).layers == 3)
			fprintf(fileout, "%s", "P3");
	} else { //is binary
		if ((*memory).layers == 1)
			fprintf(fileout, "%s", "P5");
		if ((*memory).layers == 3)
			fprintf(fileout, "%s", "P6");
	}
	fprintf(fileout, "\n");
	fprintf(fileout, "%d %d\n", (*memory).size.x, (*memory).size.y);
	fprintf(fileout, "%d\n", (*memory).intensity);
	unsigned char aux;
	for (int i = 0; i < (*memory).size.y; ++i) {
		for (int j = 0; j < (*memory).size.x; ++j) {
			for (int k = 0; k < (*memory).layers; ++k)
				if (binary == 0) {
					fprintf(fileout, "%hhu ",
							round_value((*memory).matrix[i][j].pixel[k]));
				} else {
					aux = round_value((*memory).matrix[i][j].pixel[k]);
					fwrite(&aux, sizeof(unsigned char), 1, fileout);
				}
		}
		if (binary == 0)
			fprintf(fileout, "\n");
	}
}

void load_img(img_t *memory, char filein[])
{
	if ((*memory).alloc == 1) //if there is loaded a previous image
		free_matrix(memory); //it is deallocated

	FILE *fin = fopen(filein, "rb");
	if (!fin) {
		printf("Failed to load %s\n", filein);
		return;
	}
	read_image(memory, fin);
	if ((*memory).alloc == 0) {
		printf("Failed to load %s\n", filein);
		return;
	}
	printf("Loaded %s\n", filein);
	(*memory).coord1.x = 0;
	(*memory).coord1.y = 0;
	(*memory).coord2.y = (*memory).size.y;
	(*memory).coord2.x = (*memory).size.x;
	fclose(fin);
}

void save_img(img_t *memory, char filename[], int binary)
{
	if ((*memory).alloc == 0) {
		printf("No image loaded\n");
		return;
	}
	FILE *fout = fopen(filename, "wb");
	if (!fout) {
		printf("Failed to save %s\n", filename);
		return;
	}
	write_image(memory, fout, binary);
	printf("Saved %s\n", filename);
	fclose(fout);
}
