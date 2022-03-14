/*
 * Copyright (c) 2017,2018,2019-2022 Daichi GOTO
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

#include "glsd2mn.h"

static void import_image(const XML_Char **, char *);
static void import_image_jpeg(const XML_Char **);
static void import_image_png(const XML_Char **);
static void import_text_sourcecode(const XML_Char **);
static void import_text_commandprompt(const XML_Char **);
static void import_text_Xsv(const XML_Char **, const char);
static void import_text_csv(const XML_Char **);
static void import_text_tsv(const XML_Char **);

#define MAPPING_FILE		"mapping.txt"

#define	IMAGE_WIDTH_UNLIMITED	9999
#define	IMAGE_WIDTH_AUTOGEN	1200

#define IMAGE_WIDTH_LARGE	800
#define IMAGE_WIDTH_NORMAL	600
#define IMAGE_WIDTH_MOBILE	360
#define IMAGE_WIDTH_TOP		600

static char mapping_buf[1024 * 16];
static char *mapping_buf_p = mapping_buf;
static int mapping_buf_len = 0;
static int mapping_buf_initialized = 0;

static char type[BUFSIZ];
static char filename[BUFSIZ];
static char caption[BUFSIZ];
static bool index_flag = false;
static bool iphone_flag = false;
static bool autogen_flag = false;

static bool index_image_generated = false;

void
import(const XML_Char **attr)
{
	memset(type, '\0', sizeof(type));
	memset(filename, '\0', sizeof(filename));
	memset(caption, '\0', sizeof(caption));

	index_flag = false;
	iphone_flag = false;
	autogen_flag = false;

	for (int i = 0; attr[i]; i += 2) {
		if (0 == strcmp(attr[i], "type")) {
			if (sizeof(type) < strlen(attr[i+1]))
				return;
			memcpy(type, attr[i+1], strlen(attr[i+1]));
		}
		else if (0 == strcmp(attr[i], "ref")) {
			if (sizeof(filename) < strlen(attr[i+1]))
				return;
			memcpy(filename, attr[i+1], strlen(attr[i+1]));
		}
		else if (0 == strcmp(attr[i], "caption")) {
			if (sizeof(caption) < strlen(attr[i+1]))
				return;
			memcpy(caption, attr[i+1], strlen(attr[i+1]));
		}
		else if (0 == strcmp(attr[i], "index")) {
			index_flag = true;
		}
		else if (0 == strcmp(attr[i], "iphone")) {
			iphone_flag = true;
		}
	}

	if (0 == strcmp(type, "image/jpeg"))
		import_image_jpeg(attr);
	else if (0 == strcmp(type, "image/jpg"))
		import_image_jpeg(attr);
	else if (0 == strcmp(type, "image/png"))
		import_image_png(attr);
	else if (0 == strcmp(type, "text/sourcecode"))
		import_text_sourcecode(attr);
	else if (0 == strcmp(type, "text/command-prompt"))
		import_text_commandprompt(attr);
	else if (0 == strcmp(type, "text/csv"))
		import_text_csv(attr);
	else if (0 == strcmp(type, "text/tsv"))
		import_text_tsv(attr);
}

void
import_finish(void)
{
	int fd;

	fd = open(MAPPING_FILE, O_WRONLY|O_CREAT, DEFFILEMODE);
	if (-1 == fd)
		err(errno, NULL);
	write(fd, mapping_buf, strlen(mapping_buf));
	close(fd);

	zip(IMAGE_ZIPFILE, MAPPING_FILE);
	rm(MAPPING_FILE);
}

static void
import_image(const XML_Char **attr, char *img_type)
{
	char name[BUFSIZ] = {'\0'};
	char namel[BUFSIZ] = {'\0'};
	char escaped_caption[BUFSIZ] = {'\0'};
	char *p, *p_name, *p_namel;
	IMAGE_SIZE size;

	escaped_output = 0;

	p = strstr(filename, "/");
	if (NULL == p)
		p = filename;
	else
		++p;

	/*
	 * 2019/12/23 - Compatible with specification changed.
	 *
	 * Images with the width of 1200 pixels or more are target to 
	 * automatic generation processing.
	 */
	size = get_image_size(filename);
	if (IMAGE_WIDTH_AUTOGEN <= size.width)
		autogen_flag = true;

	p_name = name;
	p_namel = namel;
	while (48 <= *p && *p <= 57) {
		*p_name++ = *p;
		*p_namel++ = *p;
		++p;
	}
	*p_namel++ = 'l';
	while ('\n' != *p && '\0' != *p && '.' != *p)
		p++;
	while ('\n' != *p && '\0' != *p) {
		*p_name++ = *p;
		*p_namel++ = *p;
		++p;
	}

	/*
	 * 2019/12/23 - Compatible with specification changed.
	 *
	 * If the width of the image is 1200 pixels or more, the image 
	 * will be automatically generated, so no conversion processing 
	 * is performed here.
	 */
	if (autogen_flag) {
		if (!index_image_generated || index_flag) {
			image_process(filename, "ogp.jpg",
					IMAGE_ZIPFILE,
					IMAGE_WIDTH_UNLIMITED);
			image_process(filename, "index.jpg", 
					IMAGE_ZIPFILE,
					IMAGE_WIDTH_TOP);
			image_process(filename, name, 
					IMAGE_ZIPFILE,
					IMAGE_WIDTH_UNLIMITED);
			rm(name);
			rm("ogp.jpg");
			rm("index.jpg");
			index_image_generated = true;
		}
		else {
			image_process(filename, name, 
					IMAGE_ZIPFILE,
					IMAGE_WIDTH_UNLIMITED);
			rm(name);
		}
	}
	else {
		if (!index_image_generated || index_flag) {
			image_process(filename, "ogp.jpg",
					IMAGE_ZIPFILE,
					IMAGE_WIDTH_UNLIMITED);
			image_process(filename, namel,
					IMAGE_ZIPFILE,
					IMAGE_WIDTH_LARGE);
			image_process(namel, "index.jpg", 
					IMAGE_ZIPFILE,
					IMAGE_WIDTH_TOP);
			if (iphone_flag)
				size = image_process("index.jpg", name,
					IMAGE_ZIPFILE,
					IMAGE_WIDTH_MOBILE);
			else
				size = image_process("index.jpg", name,
					IMAGE_ZIPFILE,
					IMAGE_WIDTH_TOP);
			rm(namel);
			rm(name);
			rm("ogp.jpg");
			rm("index.jpg");
			index_image_generated = true;
		}
		else {
			image_process(filename, namel, 
					IMAGE_ZIPFILE,
					IMAGE_WIDTH_LARGE);
			if (iphone_flag)
				size = image_process(namel, name, 
					IMAGE_ZIPFILE,
					IMAGE_WIDTH_MOBILE);
			else
				size = image_process(namel, name, 
					IMAGE_ZIPFILE,
					IMAGE_WIDTH_NORMAL);
			rm(namel);
			rm(name);
		}
	}

	if (!mapping_buf_initialized) {
		memset(mapping_buf, '\0', sizeof(mapping_buf));
		mapping_buf_p = mapping_buf;
		mapping_buf_len = 0;
		mapping_buf_initialized = 1;
	}
	if ('\0' != mapping_buf[0]) {
		*mapping_buf_p++ = '\n';
		++mapping_buf_len;
	}

	// Escape treatment for the caption
	for (int i = 0, j = 0; 
	     '\0' != caption[i] && j < (int)sizeof(escaped_caption) - 4; 
	     i++, j++) {
		// Escape , to 、
		if (',' == caption[i]) {
			// 「、」
			escaped_caption[j++] = 0xe3;
			escaped_caption[j++] = 0x80;
			escaped_caption[j] = 0x81;
			if (' ' == caption[i+1])
				++i;
		}
		// Escape | to ｜
		else if ('|' == caption[i]) {
			if (0 < j && ' ' == escaped_caption[j-1])
				--j;
			// 「｜」
			escaped_caption[j++] = 0xef;
			escaped_caption[j++] = 0xbd;
			escaped_caption[j] = 0x9c;
			if (' ' == caption[i+1])
				++i;
		}
		else
			escaped_caption[j] = caption[i];
	}

	if (autogen_flag) {
		mapping_buf_len += snprintf(
			mapping_buf_p, 
			sizeof(mapping_buf) - mapping_buf_len, 
			"%s,,%s,%s,", name,
			escaped_caption, escaped_caption);
	}
	else {
		mapping_buf_len += snprintf(
			mapping_buf_p, 
			sizeof(mapping_buf) - mapping_buf_len, 
			"%s,%s,%s,%s,", name, namel, 
			escaped_caption, escaped_caption);
	}
	mapping_buf_p = mapping_buf + mapping_buf_len;

	if (350 >= size.width)
//		pbuf_add("|photo_right\n", 13);
		pbuf_add("|photo_center\n", 14);
	else
		pbuf_add("|photo_center\n", 14);
	pbuf_add("|I@", 3);
	pbuf_add(name, strlen(name));
	pbuf_add(",", 1);

	p = caption;
	while ('\0' != *p) {
		if ('|' == *p)
			pbuf_add("｜", 3);
		else
			pbuf_add(p, 1);
		++p;
	}
	pbuf_add("|", 1);
}

static void
import_image_jpeg(const XML_Char **attr)
{
	import_image(attr, "jpg");
}

static void
import_image_png(const XML_Char **attr)
{
	import_image(attr, "png");
}

static void
import_text_sourcecode(const XML_Char **attr)
{
	FILE *fp;
	char buf[BUFSIZ] = {'\0'};
	int depth;

	escaped_output = 0;

	depth = get_docgroup_depth();
	if (6 <= depth)
		depth = 5;
	for (int i = 0; i <= depth + 1; i++)
		pbuf_add("#", 1);
	pbuf_add(" ", 1);
	pbuf_addln(pbuf_get_escaped_string(caption), 
		strlen(pbuf_get_escaped_string(caption)));
	pbuf_newline();

	fp = fopen(filename, "r");
	if (NULL == fp)
		return;
	int firstline = 1;
	while (NULL != fgets(buf, sizeof(buf) - 1, fp)) {
		if (firstline)
			firstline = 0;
		else
			pbuf_add("\n", 1);
		pbuf_add("    ", 4);
		pbuf_add(buf, strlen(buf) - 1);
	}
	fclose(fp);
}

static void
import_text_commandprompt(const XML_Char **attr)
{
	import_text_sourcecode(attr);
}

static void
import_text_Xsv(const XML_Char **attr, const char delim)
{
	FILE *fp;
	char buf[BUFSIZ * 10] = {'\0'};
	char *p;
	bool emptycell;

	escaped_output = 0;

	fp = fopen(filename, "r");
	if (NULL == fp)
		return;

	pbuf_add("|table class=\"Table1\" align=\"center\"|\n", 38);
	
	int firstline = 1;
	while (NULL != fgets(buf, sizeof(buf) - 1, fp)) {
		if (!firstline)
			pbuf_add("\n", 1);

		int mode_inbq = 0;	
		p = buf;

		emptycell = true;
		if (firstline)
			pbuf_add("|*", 2);
		else
			pbuf_add("| ", 2);
		if ('"' == *p) {
			++p;
			mode_inbq = 1;
		}
		while ('\n' != *p && '\0' != *p) {
			if (delim == *p) {
				if (firstline) {
					if (emptycell)
						pbuf_add(" |*", 3);
					else
						pbuf_add("|*", 2);
				}
				else {
					if (emptycell)
						pbuf_add(" |", 2);
					else
						pbuf_add("|", 1);
				}
				emptycell = true;
			}
			else if ('"' == *p) {
				if (mode_inbq) {
					if (delim == *(p + 1) ||
					    '\n' == *(p + 1) ||
					    '\0' == *(p + 1))
						mode_inbq = 0;
					else {
						pbuf_add("\"", 1);
						emptycell = false;
					}
				}
				else {
					if (',' == *(p - 1))
						mode_inbq = 1;
					else {
						pbuf_add("\"", 1);
						emptycell = false;
					}
				}
			}
			else if ('|' == *p) {
				pbuf_add("｜", 3);
				emptycell = false;
			}
			else if ('*' == *p) {
				pbuf_add("&lowast;", 8);
				emptycell = false;
			}
			else if ('\r' == *p) {
				// Carriage return will be deleted.
			}
			else {
 				pbuf_add(p, 1);
				emptycell = false;
			}
			++p;
		}
		pbuf_add("|", 1);
		if (firstline)
			firstline = 0;
	}
	fclose(fp);
}

static void
import_text_csv(const XML_Char **attr)
{
	import_text_Xsv(attr, ',');
}

static void
import_text_tsv(const XML_Char **attr)
{
	import_text_Xsv(attr, '\t');
}
