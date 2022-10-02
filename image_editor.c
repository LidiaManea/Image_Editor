#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"
#include "effects.h"

void read_command2(img_t *memory, char param[10][30], char command[],
				   char line[])
{
	int garbage; //the variable which tests sscanf
	if ((strcmp(command, "ROTATE")) == 0) {
		garbage = sscanf(line, "%s", param[0]);
		if ((*memory).coord1.x == 0 && (*memory).coord1.y == 0 &&
			(*memory).coord2.x == (*memory).size.x &&
			(*memory).coord2.y == (*memory).size.y) {
			rotate_all(memory, param[0]);
		} else {
			rotate_img(memory, param[0]);
		}
	} else {
		if ((strcmp(command, "CROP")) == 0) {
			crop_img(memory);
		} else {
			if ((strcmp(command, "APPLY")) == 0) {
				if (sscanf(line, "%s", param[0]) == EOF &&
					(*memory).alloc == 1) {
					printf("Invalid command\n");
				} else {
					garbage = sscanf(line, "%s", param[0]);
					filter_img(memory, param[0]);
				}
			} else {
				printf("Invalid command\n");
			}
		}
	}
	line[0] = garbage;
}

void read_command(img_t *memory)
{
	char command[10], line[100];
	char param[10][30];
	int binary;
	scanf("%s", command);
	int garbage;
	while (strcmp(command, "EXIT")) {
		for (int i = 0; i < 10; ++i) //made for initialising the parameters and
			param[i][0] = '\0'; //making them different from the previous ones
								//at every new command
		fgets(line, 100, stdin);
		if (strcmp(command, "LOAD") == 0) {
			garbage = sscanf(line, "%s", param[0]);
			load_img(memory, param[0]);
		} else {
			if (strcmp(command, "SAVE") == 0) {
				garbage = sscanf(line, "%s %s", param[0], param[1]);
				binary = strcmp(param[1], "ascii");
				save_img(memory, param[0], binary);
			} else {
				if (strcmp(command, "SELECT") == 0) {
					if (sscanf(line, "%s %s %s %s", param[0],
							   param[1], param[2], param[3]) == EOF) {
						printf("Invalid command\n");
					} else {
						garbage = sscanf(line, "%s %s %s %s", param[0],
										 param[1], param[2], param[3]);
						select_img(memory, param[0], param[1],
								   param[2], param[3]);
					}
				} else {
					read_command2(memory, param, command, line);
				}
			}
		}
		scanf("%s", command);
		line[0] = garbage;
		for (int i = 0; i < 100; ++i)
			line[i] = '\0'; //resets the line where
							//the user introduces the input
	}
	if ((*memory).alloc)
		free_matrix(memory);
	else
		printf("No image loaded\n");
}

int main(void)
{
	img_t memory;
	memory.alloc = 0;
	read_command(&memory);
	return 0;
}
