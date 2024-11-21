/*
 * Copyright (c) 2017-2019,2022,2023 Daichi GOTO
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

#define PBUF_SIZE	1024 * 64 * 10

static void pbuf_entitycompaction(void);
static void pbuf_escaped_prints(char *);
static void pbuf_escaped_printc(char);

static char pbuf[PBUF_SIZE];
static int pbuf_offset = 0;

static char pbuf_temp[PBUF_SIZE];

static int link_outputed = 0;
static int site_outputed = 0;

static ELEMENT parent_element;

void
pbuf_set_parent_element(const ELEMENT ele)
{
	parent_element = ele;
}

void
pbuf_add(const XML_Char *cdata, int len)
{
	if (pbuf_offset + len + 1 >= PBUF_SIZE)
		exit(EX__BASE);
	
	memcpy(pbuf + pbuf_offset, cdata, len);
	pbuf_offset += len;
}

void
pbuf_addln(const XML_Char *cdata, int len)
{
	if (pbuf_offset + len + 2 >= PBUF_SIZE)
		exit(EX__BASE);
	
	memcpy(pbuf + pbuf_offset, cdata, len);
	pbuf_offset += len;

	*(pbuf + pbuf_offset) = '\n';
	++pbuf_offset;
}

void
pbuf_newline()
{
	if (pbuf_offset + 1 >= PBUF_SIZE)
		exit(EX__BASE);
	
	*(pbuf + pbuf_offset) = '\n';
	++pbuf_offset;
}

void
pbuf_trimlastdot()
{
	char *p_pbuf = pbuf + pbuf_offset - 3;

	if (0 == strncmp(p_pbuf, "。", 3)) {
		*p_pbuf++ = '\0';
		*p_pbuf++ = '\0';
		*p_pbuf = '\0';

		pbuf_offset -= 3;
	}
}

void
pbuf_output(void)
{
	pbuf_entitycompaction();
	pbuf_escaped_prints(pbuf);
}

void
pbuf_outputln(void)
{
	pbuf_output();
	putchar('\n');
}

void
pbuf_trimoutput(int fw_offset, int bw_offset)
{
	pbuf_entitycompaction();
	int len = strnlen(pbuf, PBUF_SIZE) - fw_offset - bw_offset;
	char *p = pbuf;
	for (int i = 0; i < fw_offset; i++)
		++p;
	for (int i = 0; i < len; i++)
		pbuf_escaped_printc(*p++);
}

void
pbuf_trimoutputln(int fw_offset, int bw_offset)
{
	pbuf_trimoutput(fw_offset, bw_offset);
	putchar('\n');
}

void
pbuf_flush(void)
{
	pbuf_entitycompaction();
	pbuf_escaped_prints(pbuf);
	pbuf_reset();
}

void
pbuf_flushln(void)
{
	int nl = 1;
	if ('\0' == pbuf[0])
		nl = 0;	

	pbuf_flush();
	if (nl)
		putchar('\n');
}

void
pbuf_reset(void)
{
	pbuf_offset = 0;
	escaped_output = 1;
	memset(pbuf, '\0', PBUF_SIZE);
}

int
pbuf_startwith(char *s)
{
	if (0 == strncmp(pbuf, s, strlen(s)))
		return 1;
	return 0;
}

char *
pbuf_get_escaped_string(char *s)
{
	char *p, *n;
	int s_len;
	memset(pbuf_temp, '\0', PBUF_SIZE);

	p = s;
	n = pbuf_temp;
	s_len = strlen(s);
	for (int i = 0; i < s_len; i++, p++) {
		switch (*p) {
		case '_':
			switch (parent_element) {
			case ELEMENT_TITLE:
				// タイトルでは「_」はそのまま出力
				*n = *p;
				++n;
			 	break;
			default:
				*n = '\\';
				++n;
				*n = '_';
				++n;
				break;
			}
			break;
		case '*':
			*n = '\\';
			++n;
			*n = '*';
			++n;
			break;
		case '<':
			*n = '&';
			++n;
			*n = 'l';
			++n;
			*n = 't';
			++n;
			*n = ';';
			++n;
			break;
		case '>':
			*n = '&';
			++n;
			*n = 'g';
			++n;
			*n = 't';
			++n;
			*n = ';';
			++n;
			break;
		case '&':
			*n = '&';
			++n;
			*n = 'a';
			++n;
			*n = 'm';
			++n;
			*n = 'p';
			++n;
			*n = ';';
			++n;
			break;
		case '#':
			// 行頭が「#」だとタイトルだと解釈されるため、行頭だった
			// 場合のみ「\#」へとエスケープする。
			if (0 == i) {
				*n = '\\';
				++n;
				*n = '#';
				++n;
			}
			break;
		default:
			*n = *p;
			++n;
			break;
		}
	}

	return (pbuf_temp);
}

void
pbuf_xxx_linkoutput(void)
{
	pbuf_entitycompaction();
	/*
	 * 2019/12/23 - Compatible with specification changed.
	 */
	// char *p = strstr(pbuf, "/article/");
	char *p = strstr(pbuf, "http");
	if (NULL != p) {
		if (!link_outputed) {
			newline();
			outputln(";;link;;");
			link_outputed = 1;
		}
		while (']' != *p && '\0' != *p)
			putchar(*p++);
	}
}

void
pbuf_xxx_linkoutputln(void)
{
	char *p = strstr(pbuf, "/article/");
	if (NULL != p) {
		pbuf_xxx_linkoutput();
		putchar('\n');
	}
}

void
pbuf_xxx_siteoutput(void)
{
	pbuf_entitycompaction();
	if (!site_outputed) {
		newline();
		outputln(";;site;;");
		site_outputed = 1;
	}

	char *p = strstr(pbuf, "][");
	if (NULL == p)
		return;
	++p;
	++p;
	while (']' != *p && '\0' != *p)
		putchar(*p++);
	putchar('\n');

	p = pbuf;
	p += 5;
	while (']' != *p && '\0' != *p)
		putchar(*p++);
}

void
pbuf_xxx_siteoutputln(void)
{
	pbuf_xxx_siteoutput();
	putchar('\n');
}

static void
pbuf_entitycompaction(void)
{
	// Nothing to do. libexpat does instead.
}

static void
pbuf_escaped_prints(char *s)
{
	char *p;

	p = s;
	while ('\0' != *p)
		pbuf_escaped_printc(*p++);
}

static void
pbuf_escaped_printc(char c)
{
	if (escaped_output) {
		switch (c) {
		case '_':
			switch (parent_element) {
			case ELEMENT_TITLE:
				// タイトルでは「_」はそのまま出力
				putchar('_');
			 	break;
			default:
				putchar('\\');
				putchar('_');
				break;
			}
			break;
		case '*':
			putchar('\\');
			putchar('*');
			break;
		case '&':
			printf("&amp;");
			break;
		case '<':
			printf("&lt;");
			break;
		case '>':
			printf("&gt;");
			break;
		default:
			putchar(c);
			break;
		}
	}
	else
		putchar(c);
}
