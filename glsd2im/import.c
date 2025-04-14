/*
 * Copyright (c) 2017,2023,2024,2025 Daichi GOTO
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

#include "glsd2im.h"

static void import_image(const XML_Char **, char *);
static void import_image_jpeg(const XML_Char **);
static void import_image_png(const XML_Char **);
static void import_text_sourcecode(const XML_Char **);
static void import_text_commandprompt(const XML_Char **);
static void import_text_Xsv(const XML_Char **, const char);
static void import_text_csv(const XML_Char **);
static void import_text_tsv(const XML_Char **);

static char type[BUFSIZ];
static char filename[BUFSIZ];
static char caption[BUFSIZ];

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
	// ITmedia CMSで扱えない文字列のエスケープ処理
	string_processing_for_itmedia_cms(caption);
	swap(caption, " ", " ");

	// ラージ画像用インポート指定
	pbuf_add("↓l", 4);
	pbuf_add(filename+7, strlen(filename)-7-4); // exclude 'images'
	pbuf_add(".jpg", 4);
	pbuf_add(",", 1);
	pbuf_add(",", 1);
	pbuf_add(caption, strlen(caption));

	pbuf_add("\n\n", 2);

	// スモール画像用インポート指定
	pbuf_add("↓s", 4);
	pbuf_add(filename+7, strlen(filename)-7-4); // exclude 'images'
	pbuf_add(".jpg", 4);
	pbuf_add(",", 1);
	pbuf_add(",", 1);
	pbuf_add(caption, strlen(caption));
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

	pbuf_add("コード: ", 11);
	pbuf_addln(caption, strlen(caption));

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
	char buf[BUFSIZ] = {'\0'};
	char *p;

	fp = fopen(filename, "r");
	if (NULL == fp)
		return;

	int firstline = 1;
	while (NULL != fgets(buf, sizeof(buf) - 1, fp)) {
		if (!firstline)
			pbuf_add("\n", 1);

		int mode_inbq = 0;	
		p = buf;
		if (firstline)
			pbuf_add("    |", 5);
		else
			pbuf_add("    |", 5);
		if ('"' == *p) {
			++p;
			mode_inbq = 1;
		}
		while ('\n' != *p && '\0' != *p) {
			if (delim == *p) {
				if (firstline)
					pbuf_add("|", 1);
				else
					pbuf_add("|", 1);
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
