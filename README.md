   Firstly, I created 3 structs which represent the way the image is formatted
The first struct called img_t contains layers (it is 1 if the image is 
grayscale or 3 if it is coloured), intensity, alloc (it is 1 if the image
is allocated and 0 otherwise) and two structs: pixel_t **matrix and 
coord_t size, coord1, coord2. The pixel_t **matrix is the matrix of the
photo and it is made of pixels (pixel[t]); the maximum numbers in pixels
is 3 (if the image is coloured, r, g, b). coord_t is a struct which defines
a point with x and y coordinates; coord_t size is associated with the 
width and height and coord_t coord1 and coord2 are the points the closest and
the most far from the origin.
    
   In this program I used 23 functions and they are structured in:
    1.image_editor.c which contains the functions for reading commands
    2.effects.c which contains the functions for applying effects
    or transforming the photo + effects.h
    3.functions.c which contains the read/write, memory and approximation
    functions +functions.h

   While the user introduces the command which is different from "EXIT", it is
compared to the other possible options. The param[10][30] contains the
parameters which are introduced by the user. At every new command introduced,
the first item of evey param is '\0'.
    i) If the command is LOAD, the first parameter is introduced and the function
    load_img reads the photo from a file and then loads it into memory.
    ii) If the command is SAVE, the first two parameters (the second one is not
    compulsory) saves an image by writing it in a file.
    iii) If the command is SELECT, the function select_img sets the coordinates
    of the picture with the introduced data. If we "SELECT ALL", the set
    coordinates are the origin and the point which is defined by the width
    and the height.
    iv) If the command is ROTATE, the type of images which can be rotated are
    squared selections and all image; when the selection is squared,
    the function rotate_img rotates the photo 90 degrees (multiple times
    if needed). When the selection is all, the function rotate_all rotates the
    image. In both cases, the rotation is saved in an auxiliary matrix and
    after that, the original one is replaced by the new one. If the angle is
    not valid, it is shown "Unsupported rotation angle".
    v) If the command is CROP, crop_img uses the coordinates in memory and
    crops the picture. After that, the original photo is deleted by freeing
    and alocating new memory and it is replaced by the cropped image.
    vi) If the command is APPLY, the function filter_img apllies the filter
    which is introduced from input. For every filter there is a different
    function: apply_sharpen, apply_blur, apply_edge, apply_gaussianblur. If
    the filter introduced is not one of these, the message shown is "APPLY
    parameter invalid".
    vii) If the command is "EXIT", the memory is deallocated
    viii) If the command introduced is different from all of the above, it is
    shown "Invalid command".
