/*
 * Copyright (c) 2017,2020 Daichi GOTO
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

#define PBUF_SIZE	1024 * 64

static void pbuf_entitycompaction(void);
static void pbuf_escaped_prints(char *);
static void pbuf_escaped_printc(char);

static char pbuf[PBUF_SIZE];
static int pbuf_offset = 0;

static char pbuf2[PBUF_SIZE];

static char pbuf_copied[PBUF_SIZE];

char *
pbuf_getcopied(void)
{
	memcpy(pbuf_copied, pbuf, PBUF_SIZE);
	return (pbuf_copied);
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
pbuf_output(void)
{
	pbuf_entitycompaction();
	pbuf_escaped_prints(pbuf);
}

void
pbuf_outputln(void)
{
	pbuf_escaped_prints(pbuf);
	putchar('\n');
}

void
pbuf_trimoutput(int fw_offset, int bw_offset)
{
	pbuf_entitycompaction();

	int len = strnlen(pbuf, PBUF_SIZE) - fw_offset - bw_offset;

	memset(pbuf2, '\0', PBUF_SIZE);
	memcpy(pbuf2, pbuf + fw_offset, len);

	pbuf_escaped_prints(pbuf2);
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
	memset(pbuf, '\0', PBUF_SIZE);
}

int
pbuf_startwith(char *s)
{
	if (0 == strncmp(pbuf, s, strlen(s)))
		return 1;
	return 0;
}

void
pbuf_entitycompaction(void)
{
	// Nothing to do. libexpat does instead.
}

static void
pbuf_escaped_prints(char *s)
{
	char *p;

	p = s;
	while ('\0' != *p) {
		// ' (' -> '('
		if (' ' == *p && '(' == *(p+1)) {
			++p;
		}
		pbuf_escaped_printc(*p);
		++p;
	}
}

static void
pbuf_escaped_printc(char c)
{
	switch (c) {
	case '(':
		printf("（");
		break;
	case ')':
		printf("）");
		break;
	default:
		putchar(c);
		break;
	}
}
