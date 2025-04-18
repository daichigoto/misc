/*
 * Copyright (c) 2017,2022,2023 Daichi GOTO
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

static ELEMENT cur_element;
static ELEMENT pre_element;

static char access_ref[BUFSIZ];

static int docgroup_depth = 0;

static bool in_item = false;
static bool in_quote = false;

static int list_depth = -1;
static int listtype_order[10] = {false};

static bool in_note = false;
static int note_count = 0;

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
		pbuf_add("[", 1);
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
		in_item = true;
		break;
	case ELEMENT_LIST:
		newline();
		++list_depth;
		listtype_order[list_depth] = false;
		for (int i = 0; attr[i]; i += 2) {
			if (0 == strcmp(attr[i], "type") &&
			    0 == strcmp(attr[i+1], "order")) {
				listtype_order[list_depth] = true;
				break;
			}
		}
		break;
	case ELEMENT_DOCGROUP:
		if (0 < docgroup_depth)
			newline();
		docgroup_depth++;	
		break;
	case ELEMENT_QUOTE:
		in_quote = true;
		newline();
		break;
	case ELEMENT_NOTE:
		in_note = true;
		note_count = 1;
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
			if (listtype_order[list_depth])
				output("1. ");
			else
				output("- ");
			// 列挙・枚挙の最後の「。」を削除
			pbuf_trimlastdot();
			pbuf_outputln();

			in_item = false;
			return;
		}
		if (in_quote) {
			output("> ");
			pbuf_outputln();
			in_quote = false;
			return;
		}
		if (in_note) {
			if (1 < note_count) {
				printf("\n");
			}
			printf("(注%d) ", note_count);
			pbuf_outputln();
			++note_count;
			return;
		}

		switch((int)pre_element) {
		case ELEMENT_ITEM:
			if (listtype_order[list_depth])
				output("1. ");
			else
				output("- ");
			pbuf_outputln();
			break;
		case ELEMENT_TITLE:
			escaped_output = 1;
			pbuf_set_parent_element(ELEMENT_TITLE);
			switch(docgroup_depth) {
			case 1:
				pbuf_outputln();
				output("title=");
				pbuf_outputln();
				break;
			default:
				for (int i = 0; i < docgroup_depth; i++)
					putchar('#');
				putchar(' ');
				pbuf_outputln();
				break;
			}
			pbuf_set_parent_element(ELEMENT_DOCUMENT);
			break;
		case ELEMENT_SHORTTITLE:
			escaped_output = 0;
			output("short=");
			pbuf_outputln();
			break;
		case ELEMENT_AUTHOR:
			escaped_output = 0;
			output("author=");
			pbuf_outputln();
			break;
		case ELEMENT_FIRSTEDITION:
			break;
		case ELEMENT_LASTMODIFIED:
			break;
		default:
			if (pbuf_startwith("[MJ:")) {
				pbuf_xxx_linkoutputln();
				break;
			}
			if (pbuf_startwith("[URL:")) {
				pbuf_xxx_siteoutputln();
				break;
			}
			if (pbuf_startwith("[LEAD:")) {
				escaped_output = 0;
				output("lead=");
				pbuf_trimoutputln(6, 1);
				break;
			}
			if (pbuf_startwith("[W:"))
				break;
			if (pbuf_startwith("[TAG:"))
				break;
			if (pbuf_startwith("[SRC:"))
				break;
			if (pbuf_startwith("[POST:"))
				break;

			newline();

			if (pbuf_startwith("#") &&
			    !pbuf_startwith("###### ") &&
			    !pbuf_startwith("##### ") &&
			    !pbuf_startwith("#### ") &&
			    !pbuf_startwith("### ")) {
				// 行頭の「#」はタイトル指定と解釈されるため
				// エスケープする。
				putchar('\\');
			}
			pbuf_flushln();
			break;
		}
		break;
	case ELEMENT_ACCESS:
		pbuf_add("](", 2);
		pbuf_add(access_ref, strlen(access_ref));
		pbuf_add(")", 1);
		break;
	case ELEMENT_LIST:
		in_item = false;
		--list_depth;
		break;
	case ELEMENT_NOTE:
		in_note = false;
		note_count = 0;
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
