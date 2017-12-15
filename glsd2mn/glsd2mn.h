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

#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <err.h>
#include <errno.h>
#include <string.h>
#include <sysexits.h>
#include <expat.h>

#define VERSION			"20171215"
#define IMAGE_ZIPFILE		"images.zip"

typedef enum el {
	ELEMENT_DOCUMENT,
	ELEMENT_DOCGROUP,
	ELEMENT_DOCINFO,
	ELEMENT_AUTHOR,
	ELEMENT_TITLE,
	ELEMENT_SHORTTITLE,
	ELEMENT_FIRSTEDITION,
	ELEMENT_LASTMODIFIED,
	ELEMENT_LIST,
	ELEMENT_ITEM,
	ELEMENT_P,
	ELEMENT_ACCESS,
	ELEMENT_IMPORT,
	ELEMENT_QUOTE,
	ELEMENT_UNKOWN
} ELEMENT;

void el_start_handler(void *, const XML_Char *, const XML_Char **);
void el_end_handler(void *, const XML_Char *);
void el_chardata_handler(void *, const XML_Char *, int len);
int get_docgroup_depth(void);

int element_is(const XML_Char *);

void pbuf_add(const XML_Char *, int);
void pbuf_addln(const XML_Char *, int);
void pbuf_newline(void);
void pbuf_output(void);
void pbuf_outputln(void);
void pbuf_trimoutput(int, int);
void pbuf_trimoutputln(int, int);
void pbuf_flush(void);
void pbuf_flushln(void);
void pbuf_reset(void);
int pbuf_startwith(char *);

void pbuf_xxx_linkoutput(void);
void pbuf_xxx_linkoutputln(void);
void pbuf_xxx_siteoutput(void);
void pbuf_xxx_siteoutputln(void);

void import(const XML_Char **);
void import_finish(void);

void image_process(char *, char *, char *, int);

void copy(char *, char *);
void zip(char *, char *);
void rm(char *);

void output(char *);
void outputln(char *);
void newline(void);

void usage(void);
void version(void);
