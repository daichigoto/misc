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

#include "glsd2in.h"
#include <time.h>

static void import_image(const XML_Char **, char *);
static void import_image_jpeg(const XML_Char **);
static void import_image_png(const XML_Char **);
static void import_text_sourcecode(const XML_Char **);
static void import_text_commandprompt(const XML_Char **);
static void import_text_Xsv(const XML_Char **, const char);
static void import_text_csv(const XML_Char **);
static void import_text_tsv(const XML_Char **);
static void get_date(void);

#define IMAGE_WIDTH_LARGE	800
#define IMAGE_WIDTH_NORMAL	600
#define IMAGE_WIDTH_MOBILE	300
#define IMAGE_WIDTH_TOP		600

#define IMAGE_RESIZED_DIR	"images_resized"

static char type[BUFSIZ];
static char filepath[BUFSIZ];
static char caption[BUFSIZ];
static char date[BUFSIZ];
static int iphone_flag;

void
import(const XML_Char **attr)
{
	memset(type, '\0', sizeof(type));
	memset(filepath, '\0', sizeof(filepath));
	memset(caption, '\0', sizeof(caption));

	iphone_flag = 0;
	for (int i = 0; attr[i]; i += 2) {
		if (0 == strcmp(attr[i], "type")) {
			if (sizeof(type) < strlen(attr[i+1]))
				return;
			memcpy(type, attr[i+1], strlen(attr[i+1]));
		}
		else if (0 == strcmp(attr[i], "ref")) {
			if (sizeof(filepath) < strlen(attr[i+1]))
				return;
			memcpy(filepath, attr[i+1], strlen(attr[i+1]));
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
	IMAGE_SIZE s;
	char name[BUFSIZ] = {'\0'};
	char namel[BUFSIZ] = {'\0'};
	char namem[BUFSIZ] = {'\0'};
	char path[BUFSIZ] = {'\0'};
	char pathl[BUFSIZ] = {'\0'};
	char pathm[BUFSIZ] = {'\0'};
	char wh[BUFSIZ] = {'\0'};
	char *p, *p_name, *p_namel, *p_namem, 
	     *p_path, *p_pathl, *p_pathm, *p_date;

	mkdir(IMAGE_RESIZED_DIR, S_IRWXU|S_IRWXG|S_IRWXO);

	p_name = name;
	p_namel = namel;
	p_namem = namem;
	strcpy(path, IMAGE_RESIZED_DIR);
	strcpy(pathl, IMAGE_RESIZED_DIR);
	strcpy(pathm, IMAGE_RESIZED_DIR);
	p = filepath;
	p_path = path + strlen(IMAGE_RESIZED_DIR);
	p_pathl = pathl + strlen(IMAGE_RESIZED_DIR);
	p_pathm = pathm + strlen(IMAGE_RESIZED_DIR);

	while ('/' != *p)
		++p;

	*p_path++ = '/';
	*p_pathl++ = '/';
	*p_pathm++ = '/';
	++p;

	if (p_flag) {
		strcpy(p_path, "pwmgr_");
		strcpy(p_pathl, "pwmgr_");
		strcpy(p_pathm, "pwmgr_");
		strcpy(p_name, "pwmgr_");
		strcpy(p_namel, "pwmgr_");
		strcpy(p_namem, "pwmgr_");
		p_path += 6;
		p_pathl += 6;
		p_pathm += 6;
		p_name += 6;
		p_namel += 6;
		p_namem += 6;
	}
	else {
		strcpy(p_path, "linux_");
		strcpy(p_pathl, "linux_");
		strcpy(p_pathm, "linux_");
		strcpy(p_name, "linux_");
		strcpy(p_namel, "linux_");
		strcpy(p_namem, "linux_");
		p_path += 6;
		p_pathl += 6;
		p_pathm += 6;
		p_name += 6;
		p_namel += 6;
		p_namem += 6;
	}

	get_date();

	p_date = date;	
	while ('\0' != *p_date) {
		*p_path++ = *p_date;
		*p_pathl++ = *p_date;
		*p_pathm++ = *p_date;
		*p_name++ = *p_date;
		*p_namel++ = *p_date;
		*p_namem++ = *p_date;
		++p_date;
	}
	*p_path++ = '_';
	*p_pathl++ = '_';
	*p_pathm++ = '_';
	*p_name++ = '_';
	*p_namel++ = '_';
	*p_namem++ = '_';

	while (48 <= *p && *p <= 57) {
		*p_path++ = *p;
		*p_pathl++ = *p;
		*p_pathm++ = *p;
		*p_name++ = *p;
		*p_namel++ = *p;
		*p_namem++ = *p;
		++p;
	}
	*p_pathl++ = 'l';
	*p_namel++ = 'l';
	*p_pathm++ = 'm';
	*p_namem++ = 'm';

	while ('\n' != *p && '\0' != *p && '.' != *p)
		p++;
	while ('\n' != *p && '\0' != *p) {
		*p_path++ = *p;
		*p_pathl++ = *p;
		*p_pathm++ = *p;
		*p_name++ = *p;
		*p_namel++ = *p;
		*p_namem++ = *p;
		++p;
	}

	s = image_process(filepath, pathl, IMAGE_WIDTH_LARGE);
	if (iphone_flag)
		image_process(pathl, pathm, IMAGE_WIDTH_MOBILE);
	else
		image_process(pathl, path, IMAGE_WIDTH_NORMAL);

	snprintf(wh, sizeof(wh), "width=%d,height=%d,", s.width, s.height);

	pbuf_addln("<div class=\"center\">", 20);
	pbuf_addln(" <table border=\"0\" cellpadding=\"0\" "
	                   "cellspacing=\"1\" class=\"Photo1\" "
			   "width=\"50\">", 77);
	pbuf_addln("  <tr>", 6);
	pbuf_add("   <td>", 7);
	pbuf_add("<a href=\"https://news.mynavi.jp/itsearch/files/", 47);
	pbuf_add(namel, 23);
	pbuf_add("\" ", 2); 
	pbuf_add("onclick=\"window.open('", 22);
	pbuf_add("\"https://news.mynavi.jp/itsearch/files/", 39);
	pbuf_add(namel, 23);
	pbuf_add("\" ", 2); 
	pbuf_add("','popup','", 11);
	pbuf_add(wh, strlen(wh));
	pbuf_add("scrollbars=yes,resizable=no,toolbar=no,"
	         "directories=no,location=no,menubar=no,status=no,"
		 "left=0,top=0'); return false\">", 117);
	pbuf_add("<img src=\"", 10);
	pbuf_add("https://news.mynavi.jp/itsearch/files/", 38);
	if (iphone_flag)
		pbuf_add(namem, 23);
	else
		pbuf_add(name, 22);
	pbuf_add("\"/>", 3);
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

	fp = fopen(filepath, "r");
	if (NULL == fp)
		return;

	pbuf_add("#### ", 5);
	pbuf_addln(caption, strlen(caption));
	pbuf_newline();

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

	fp = fopen(filepath, "r");
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

static void
get_date(void)
{
	time_t t;
	struct tm *tm;

	t = time(NULL);
	tm = localtime(&t);

	snprintf(date, sizeof(date), "%4d%02d%02d", 
		tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday);
}
