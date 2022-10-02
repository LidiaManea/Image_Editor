#ifndef __FUNCTIONS__H__
#define __FUNCTIONS__H__

typedef struct {
	double pixel[3];
} pixel_t;

typedef struct {
	int x, y;
} coord_t;

typedef struct {
	int layers;
	int intensity;
	pixel_t **matrix;
	coord_t size, coord1, coord2;
	int alloc;
} img_t;

void swap_int(int *a, int *b);

int isnumber(char a[]);

unsigned char round_value(double pixel);

void iscomment(FILE *file);

pixel_t **alloc_matrix(int xsize, int ysize);

void alloc_image(img_t *image);

void free_matrix(img_t *memory);

void read_image(img_t *memory, FILE *filein);

void write_image(img_t *memory, FILE *fileout, int binary);

void load_img(img_t *memory, char filein[]);

void save_img(img_t *memory, char filename[], int binary);

#endif
