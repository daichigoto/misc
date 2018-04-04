/*
 * Copyright (c) 2017,2018 Daichi GOTO
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

#include "glsd2html.h"

static void import_image(const XML_Char **, char *);
static void import_image_jpeg(const XML_Char **);
static void import_image_png(const XML_Char **);
static void import_text_sourcecode(const XML_Char **);
static void import_text_commandprompt(const XML_Char **);
static void import_text_Xsv(const XML_Char **, const char);
static void import_text_csv(const XML_Char **);
static void import_text_tsv(const XML_Char **);

#define IMAGE_WIDTH_NORMAL	600
#define IMAGE_WIDTH_MOBILE	300

#define IMAGE_RESIZED_DIR	"images_resized"

static char type[BUFSIZ];
static char filename[BUFSIZ];
static char caption[BUFSIZ];
static int iphone_flag;

static int img_counter = 1;
static int tbl_counter = 1;
static int src_counter = 1;

void
import(const XML_Char **attr)
{
	memset(type, '\0', sizeof(type));
	memset(filename, '\0', sizeof(filename));
	memset(caption, '\0', sizeof(caption));

	iphone_flag = 0;
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
		else if (0 == strcmp(attr[i], "iphone")) {
			iphone_flag = 1;
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

static void
import_image(const XML_Char **attr, char *img_type)
{
	int w, h;
	char buf[BUFSIZ], numbuf[BUFSIZ];
	IMAGE_SIZE siz;
	
	siz = image_size(filename);
	if (iphone_flag) {
		w = IMAGE_WIDTH_MOBILE;
		h = (int)((double)siz.height * 
			(double)IMAGE_WIDTH_MOBILE / (double)siz.width);
	}
	else if (IMAGE_WIDTH_NORMAL < siz.width) {
		w = IMAGE_WIDTH_NORMAL;
		h = (int)((double)siz.height * 
			(double)IMAGE_WIDTH_NORMAL / (double)siz.width);
	}
	else {
		w = siz.width;
		h = siz.height;
	}
	snprintf(buf, BUFSIZ, "width=\"%dpx\" height=\"%dpx\" ", w, h);

	pbuf_addln("<br>", 4);

	pbuf_add("<div class=\"container\">", 23);
	pbuf_add("<div class=\"row\">", 17);
	pbuf_add("<div class=\"col-md-1\"></div>", 28);
	pbuf_add("<div class=\"col-md-10\">", 23);
	pbuf_add("<a href=\"", 9);
	pbuf_add(filename, strlen(filename));
	pbuf_add("\"/>", 3);

	pbuf_add("<img src=\"", 10);
	pbuf_add(filename, strlen(filename));
	pbuf_add("\" ", 2);
	pbuf_add("alt=\"", 5);
	pbuf_add(caption, strlen(caption));
	pbuf_add("\" ", 2);
//	pbuf_add(buf, strlen(buf));
	pbuf_add("class=\"img-responsive\" ", 23);
	pbuf_add("/>", 2);

	pbuf_add("</a>", 4);
	pbuf_add("</div>", 6);
	pbuf_add("<div class=\"col-md-1\"></div>", 28);
	pbuf_add("</div>", 6);
	pbuf_add("</div>", 6);

	snprintf(numbuf, sizeof(numbuf), "%d", img_counter);
	pbuf_add("<p class=\"text-center\">", 23);
	pbuf_add("図", 3);
	pbuf_add(numbuf, strlen(numbuf));
	++img_counter;
	pbuf_add(" ", 1);
	pbuf_add(caption, strlen(caption));
	pbuf_addln("</p>", 4);
	pbuf_addln("<br>", 4);
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
	char buf[BUFSIZ], numbuf[BUFSIZ];

	fp = fopen(filename, "r");
	if (NULL == fp)
		return;

	pbuf_add("<pre><code>", 11);
	int firstline = 1;
	while (NULL != fgets(buf, sizeof(buf) - 1, fp)) {
		if (firstline)
			firstline = 0;
		else
			pbuf_add("\n", 1);
		pbuf_add(buf, strlen(buf) - 1);
	}

	fclose(fp);

	pbuf_add("</code></pre>", 13);
	pbuf_newline();

	snprintf(numbuf, sizeof(numbuf), "%d", src_counter);
	pbuf_add("<p class=\"text-center\">", 23);
	pbuf_add("リスト", 9);
	pbuf_add(numbuf, strlen(numbuf));
	++src_counter;
	pbuf_add(" ", 1);
	pbuf_add(caption, strlen(caption));
	pbuf_addln("</p>", 4);
	pbuf_addln("<br>", 4);
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
	char buf[BUFSIZ], numbuf[BUFSIZ];
	char *p;

	fp = fopen(filename, "r");
	if (NULL == fp)
		return;
	
	pbuf_addln("<br>", 4);
	pbuf_addln("<table class=\"table table-striped\">", 35);

	int firstline = 1;
	while (NULL != fgets(buf, sizeof(buf) - 1, fp)) {
		pbuf_add("<tr>", 4);

		int mode_inbq = 0;	
		p = buf;
		if (firstline)
			pbuf_add("<th>", 4);
		else
			pbuf_add("<td>", 4);
		if ('"' == *p) {
			++p;
			mode_inbq = 1;
		}
		while ('\n' != *p && '\0' != *p) {
			if (delim == *p) {
				if (firstline)
					pbuf_add("</th><th>", 9);
				else
					pbuf_add("</td><td>", 9);
			}
			else if ('"' == *p) {
				if (mode_inbq) {
					if (delim == *(p + 1) ||
					    '\n' == *(p + 1) ||
					    '\0' == *(p + 1))
						mode_inbq = 0;
					else
						pbuf_add("\"", 1);
				}
				else {
					if (',' == *(p - 1))
						mode_inbq = 1;
					else
						pbuf_add("\"", 1);
				}
			}
			else if ('|' == *p)
				pbuf_add("｜", 3);
			else
 				pbuf_add(p, 1);
			++p;

		}
		if (firstline)
			pbuf_add("</th>", 5);
		else
			pbuf_add("</td>", 5);

		if (firstline)
			firstline = 0;

		pbuf_addln("</tr>", 5);
	}
	fclose(fp);

	pbuf_addln("</table>", 8);

	snprintf(numbuf, sizeof(numbuf), "%d", tbl_counter);
	pbuf_add("<p class=\"text-center\">", 23);
	pbuf_add("表", 3);
	pbuf_add(numbuf, strlen(numbuf));
	++tbl_counter;
	pbuf_add(" ", 1);
	pbuf_add(caption, strlen(caption));
	pbuf_addln("</p>", 4);
	pbuf_addln("<br>", 4);

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
