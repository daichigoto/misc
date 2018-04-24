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

static ELEMENT cur_element;
static ELEMENT pre_element;

static int listtype_order = 0;
static int docgroup_depth = 0;
static int docgroup_count = 0;

static int in_item = 0;
static int in_quote = 0;
static int list_finished = 0;

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
		pbuf_add("<a rel=\"nofollow\" href=\"", 24);
		for (int i = 0; attr[i]; i += 2) {
			if (0 == strcmp(attr[i], "ref")) {
				pbuf_add(attr[i+1], strlen(attr[i+1]));
				break;
			}
		}
		pbuf_add("\" target=\"_blank\">", 18);
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
				break;
			}
		}
		break;
	case ELEMENT_DOCGROUP:
		++docgroup_count;
		if (2 < docgroup_count)
			newline();

		++docgroup_depth;	
		break;
	case ELEMENT_QUOTE:
		in_quote = 1;
		newline();
		break;
	}
}

void
el_end_handler(void *data, const XML_Char *name)
{
	cur_element = element_is(name);
	switch((int)cur_element) {
	case ELEMENT_P:
		if (in_item) {
			if (listtype_order)
				output("1. ");
			else
				output("- ");
			pbuf_outputln();
			in_item = 0;
			return;
		}
		if (in_quote) {
			output("> ");
			pbuf_outputln();
			in_quote = 0;
			return;
		}

		switch((int)pre_element) {
		case ELEMENT_ITEM:
			if (listtype_order)
				output("1. ");
			else
				output("- ");
			pbuf_outputln();
			break;
		case ELEMENT_TITLE:
			switch(docgroup_depth) {
			case 1:
				break;
			default:
				for (int i = 0; i <= docgroup_depth; i++)
					putchar('#');
				putchar(' ');
				pbuf_outputln();
				break;
			}
			break;
		case ELEMENT_SHORTTITLE:
			output("short=");
			pbuf_outputln();
			break;
		case ELEMENT_AUTHOR:
			break;
		case ELEMENT_FIRSTEDITION:
			break;
		case ELEMENT_LASTMODIFIED:
			break;
		default:
			if (pbuf_startwith("[タイトル:"))
				break;
			if (pbuf_startwith("[MJ:"))
				break;
			if (pbuf_startwith("[URL:"))
				break;
			if (pbuf_startwith("[LEAD:"))
				break;
			if (pbuf_startwith("[W:"))
				break;
			if (pbuf_startwith("[TAG:"))
				break;
			if (pbuf_startwith("[SRC:"))
				break;
			if (pbuf_startwith("[POST:"))
				break;

			newline();
			if (list_finished) {
				output("<br>");
				list_finished = 0;
			}
			pbuf_flushln();
			break;
		}
		break;
	case ELEMENT_ACCESS:
		pbuf_add("</a>", 4);
		break;
	case ELEMENT_LIST:
		in_item = 0;
		list_finished = 1;
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

int
get_docgroup_depth(void)
{
	return(docgroup_depth);
}
