/*
 * Copyright (c) 2017 Daichi GOTO
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "glsd2in.h"

#include <string.h>
#include <jpeglib.h>

#define CONVCMD	"gm convert -resize %dx%d %s %s"

static IMAGE_SIZE shrink(char *, char *, int);

IMAGE_SIZE
image_process(char *file_in, char *file_ou, int width)
{
	/*
	 * Shrink image.
	 */
	return shrink(file_in, file_ou, width);
}

static IMAGE_SIZE
shrink(char *file_in, char *file_ou, int width)
{
	FILE *fp;
	struct jpeg_decompress_struct jpg;
	struct jpeg_error_mgr jpg_err;
	int height, ori_width, ori_height;
	char buf[BUFSIZ] = { '\0' };
	IMAGE_SIZE size;

	fp = fopen(file_in, "r");
	if (NULL == fp) {
		size.width = 0;
		size.height = 0;
		return(size);
	}

	jpg.err = jpeg_std_error(&jpg_err);
	jpeg_create_decompress(&jpg);
	jpeg_stdio_src(&jpg, fp);
	jpeg_read_header(&jpg, TRUE);
        jpeg_calc_output_dimensions(&jpg);

	ori_width = jpg.output_width;
	ori_height = jpg.output_height;
	height = ori_height * width / ori_width;

	jpeg_destroy_decompress(&jpg);
	fclose(fp);

	/*
	 * The image size is smaller than the specified size. So, 
	 * copy the image without converting.
	 */
	if (width >= ori_width) {
		copy(file_in, file_ou);
		size.width = ori_width;
		size.height = ori_height;
		return(size);
	}

	snprintf(buf, BUFSIZ, CONVCMD, width, height, file_in, file_ou);
	system(buf);

	size.width = width;
	size.height = height;
	return(size);
}
