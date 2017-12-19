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

static void import_image(const XML_Char **, char *);
static void import_image_jpeg(const XML_Char **);
static void import_image_png(const XML_Char **);
static void import_text_sourcecode(const XML_Char **);
static void import_text_commandprompt(const XML_Char **);
static void import_text_Xsv(const XML_Char **, const char);
static void import_text_csv(const XML_Char **);
static void import_text_tsv(const XML_Char **);

#define IMAGE_WIDTH_LARGE	800
#define IMAGE_WIDTH_NORMAL	600
#define IMAGE_WIDTH_TOP		600

static char type[BUFSIZ];
static char filename[BUFSIZ];
static char caption[BUFSIZ];

static int index_image_generated = 0;

void
import(const XML_Char **attr)
{
	memset(type, '\0', sizeof(type));
	memset(filename, '\0', sizeof(filename));
	memset(caption, '\0', sizeof(caption));
	
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
	IMAGE_SIZE s;
	char name[BUFSIZ] = {'\0'};
	char namel[BUFSIZ] = {'\0'};
	char wh[BUFSIZ] = {'\0'};
	char *p, *p_name, *p_namel;

	p = strstr(filename, "/");
	if (NULL == p)
		p = filename;
	else
		++p;

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

	if (!index_image_generated) {
		s = image_process(filename, namel, IMAGE_WIDTH_LARGE);
		image_process(namel, "images/index.top.jpg", IMAGE_WIDTH_TOP);
		image_process("images/index.top.jpg", name, IMAGE_WIDTH_TOP);
		image_process(namel, name, IMAGE_WIDTH_NORMAL);
		rm(namel);
		rm(name);
		index_image_generated = 1;
	}
	else {
		s = image_process(filename, namel, IMAGE_WIDTH_LARGE);
		image_process(namel, name, IMAGE_WIDTH_NORMAL);
		rm(namel);
		rm(name);
	}

	snprintf(wh, sizeof(wh), "width=%d,height=%d,", s.width, s.height);

	pbuf_addln("<div class=\"center\">", 20);
	pbuf_addln(" <table border=\"0\" cellpadding=\"0\" "
	                   "cellspacing=\"1\" class=\"Photo1\" "
			   "width=\"50\">", 77);
	pbuf_addln("  <tr>", 6);
	pbuf_add("   <td>", 7);
	pbuf_add("<a href=\"HTMLURL\" " 
	         "onclick=\"window.open('HTMLURL','popup','", 58);
	pbuf_add(wh, strlen(wh));
	pbuf_add("scrollbars=yes,resizable=no,toolbar=no,"
	         "directories=no,location=no,menubar=no,status=no,"
		 "left=0,top=0'); return false\">", 117);
	pbuf_add("<img src=\"IMAURL\"/>", 19);
	pbuf_add("</a>", 4);
	pbuf_addln("</td>", 5);
	pbuf_addln("  </tr>", 7);
	pbuf_addln("  <tr>", 6);
	pbuf_add("   <td><p>", 10);
	pbuf_add(caption, strlen(caption));
	pbuf_addln("</p></td>", 9);
	pbuf_addln("  </tr>", 7);
	pbuf_addln(" </table>", 9);
	pbuf_add("</div>", 6);
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

	fp = fopen(filename, "r");
	if (NULL == fp)
		return;
	int firstline = 1;
	while (NULL != fgets(buf, sizeof(buf) - 1, fp)) {
		if (firstline)
			firstline = 0;
		else
			pbuf_add("\n", 1);
		pbuf_add("\t", 1);
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
	char buf[BUFSIZ] = {'\0'};
	char *p;

	fp = fopen(filename, "r");
	if (NULL == fp)
		return;

	pbuf_addln("<div class=\"center\">", 20);
	pbuf_addln(" <table class=\"Table1\" "
		   "style=\"font-size: 15px; max-width: 700px;\">", 66);
	
	int firstline = 1;
	while (NULL != fgets(buf, sizeof(buf) - 1, fp)) {
		pbuf_addln("  <tr>", 6);

		int mode_inbq = 0;	
		p = buf;
		if (firstline)
			pbuf_add("   <th>", 7);
		else
			pbuf_add("   <td>", 7);
		if ('"' == *p) {
			++p;
			mode_inbq = 1;
		}
		while ('\n' != *p && '\0' != *p) {
			if (delim == *p) {
				if (firstline) {
					pbuf_addln("</th>", 5);
					pbuf_add("   <th>", 7);
				}
				else {
					pbuf_addln("</td>", 5);
					pbuf_add("   <td>", 7);
				}
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
			else if ('<' == *p)
				pbuf_add("&lt;", 4);
			else if ('>' == *p)
				pbuf_add("&gt;", 4);
			else if ('&' == *p)
				pbuf_add("&amp;", 5);
			else
 				pbuf_add(p, 1);
			++p;

		}
		if (firstline) {
			pbuf_addln("</th>", 5);
		}
		else {
			pbuf_addln("</td>", 5);
		}
		pbuf_addln("  </tr>", 7);

		if (firstline)
			firstline = 0;
	}
	fclose(fp);

	pbuf_addln(" </table>", 9);
	pbuf_add("</div>", 6);
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
