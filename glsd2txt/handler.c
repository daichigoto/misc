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

#include "glsd2txt.h"

static ELEMENT cur_element;
static ELEMENT pre_element;
static int listtype_order = 0;
static int listtype_order_index = 1;
static int docgroup_depth = 0;

static char access_ref[BUFSIZ];

static int in_item = 0;

void
el_start_handler(void *data, const XML_Char *name, const XML_Char **attr)
{
	pre_element = cur_element;
	cur_element = element_is(name);

	switch(element_is(name)) {
	case ELEMENT_P:
		pbuf_reset();
		break;
	case ELEMENT_ACCESS:
		memset(access_ref, '\0', sizeof(access_ref));
		for (int i = 0; attr[i]; i += 2) {
			if (0 == strcmp(attr[i], "ref")) {
				memcpy(access_ref, attr[i+1], 
					strlen(attr[i+1]));
				break;
			}
		}
		break;
	case ELEMENT_IMPORT:
		import(attr);
		break;
	case ELEMENT_ITEM:
		in_item = 1;
		break;
	case ELEMENT_LIST:
		newline();
		listtype_order = 0;
		for (int i = 0; attr[i]; i += 2) {
			if (0 == strcmp(attr[i], "type") &&
			    0 == strcmp(attr[i+1], "order")) {
				listtype_order = 1;
				listtype_order_index = 1;
				break;
			}
		}
		break;
	case ELEMENT_DOCGROUP:
		if (0 < docgroup_depth)
			newline();
		docgroup_depth++;	
		break;
	}
}

void
el_end_handler(void *data, const XML_Char *name)
{
	cur_element = element_is(name);
	switch((int)cur_element) {
	case ELEMENT_P:
		switch((int)pre_element) {
		case ELEMENT_ITEM:
			if (listtype_order)
				printf("%d. ", listtype_order_index++);
			else
				output("- ");
			pbuf_outputln();
			break;
		case ELEMENT_TITLE:
			switch(docgroup_depth) {
			case 1:
				output("題名: ");
				pbuf_outputln();
				break;
			default:
				for (int i = 0; i < docgroup_depth; i++)
					putchar('#');
				putchar(' ');
				pbuf_outputln();
				break;
			}
			break;
		case ELEMENT_SHORTTITLE:
			output("短縮: ");
			pbuf_outputln();
			break;
		case ELEMENT_AUTHOR:
			output("著者: ");
			pbuf_outputln();
			break;
		case ELEMENT_FIRSTEDITION:
			break;
		case ELEMENT_LASTMODIFIED:
			break;
		default:
			if (pbuf_startwith("[MJ:")) {
				pbuf_trimoutputln(1, 1);
				newline();
				break;
			}
			if (pbuf_startwith("[URL:")) {
				break;
			}
			if (pbuf_startwith("[LEAD:")) {
				output("導入: ");
				pbuf_trimoutputln(6, 1);
				break;
			}
			if (pbuf_startwith("[W:")) {
				break;
			}
			if (pbuf_startwith("[TAG:")) {
				break;
			}
			if (pbuf_startwith("[SRC:")) {
				pbuf_trimoutputln(1, 1);
				break;
			}
			if (pbuf_startwith("[POST:")) {
				newline();
				pbuf_trimoutputln(1, 1);
				newline();
				break;
			}
			if (in_item) {
				if (listtype_order)
					printf("%d. ", 
						listtype_order_index++);
				else
					output("- ");
				pbuf_outputln();
				in_item = 0;
			}
			else {
				newline();
				pbuf_flushln();
			}
			break;
		}
		break;
	case ELEMENT_ACCESS:
		pbuf_add(" (", 2);
		pbuf_add(access_ref, strlen(access_ref));
		pbuf_add(")", 1);
		break;
	case ELEMENT_LIST:
		break;
	case ELEMENT_DOCGROUP:
		docgroup_depth--;	
		break;
	}
}

void
el_chardata_handler(void *data, const XML_Char *cdata, int len)
{
	pbuf_add(cdata, len);
}
