#ifndef __EFFECTS__H__
#define __EFFECTS__H__

#include "functions.h"

void select_img(img_t *memory, char par1[], char par2[],
				char par3[], char par4[]);

void rotate_img(img_t *memory, char par[]);

void rotate_all(img_t *memory, char par[]);

void crop_img(img_t *memory);

double clamp(double pixel);

void apply_blur(img_t *memory);

void apply_sharpen(img_t *memory);

void apply_edge(img_t *memory);

void apply_gaussianblur(img_t *memory);

void filter_img(img_t *memory, char par[]);

#endif
