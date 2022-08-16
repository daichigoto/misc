/*
 * Copyright (c) 2022 Daichi GOTO
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

static ELEMENT cur_element;
static ELEMENT pre_element;
static int listtype_order = 0;
static int listtype_order_index = 1;
static int docgroup_depth = 0;

static char *p_access_chardata;
static char access_chardata[BUFSIZ];
static char access_ref[BUFSIZ];
static char quote_ref[BUFSIZ];

static int accesslist_count = 0;
static char accesslist_ref[100][BUFSIZ];
static char accesslist_title[100][BUFSIZ];

static int in_item = 0;
static int in_access = 0;
static int in_quote = 0;
static int in_import = 0;

static int doc_started = 0;

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
		in_access = 1;
		memset(access_chardata, '\0', sizeof(access_chardata));
		p_access_chardata = access_chardata;

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
		in_import = 1;
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
	case ELEMENT_QUOTE:
		in_quote = 1;
		newline();
		for (int i = 0; attr[i]; i += 2)
			if (0 == strcmp(attr[i], "ref")) {
				strncpy(quote_ref, attr[i+1],
					sizeof(quote_ref));
				break;
			}
		break;
	default:
		break;
	}
}

void
el_end_handler(void *data, const XML_Char *name)
{
	cur_element = element_is(name);
	switch((int)cur_element) {
	case ELEMENT_P:
		if (in_quote) {
			output("引用: ");
			if ('\0' != quote_ref[0])
				outputln(quote_ref);
			else
				newline();
			output("　　  ");
			pbuf_flushln();
			memset(quote_ref, '\0', sizeof(quote_ref));
			in_quote = 0;
			return;
		}

		switch((int)pre_element) {
		case ELEMENT_ITEM:
			if (listtype_order)
				printf("%d. ", listtype_order_index++);
			else
				output("● ");
			pbuf_outputln();
			break;
		case ELEMENT_TITLE:
			switch(docgroup_depth) {
			case 1:
				output("＜タイトル＞");
				pbuf_outputln();
				newline();
				break;
			default:
				for (int i = 0; i < docgroup_depth; i++)
					putchar('#');
				pbuf_output();
				break;
			}
			break;
		case ELEMENT_SHORTTITLE:
			break;
		case ELEMENT_AUTHOR:
			break;
		case ELEMENT_FIRSTEDITION:
			break;
		case ELEMENT_LASTMODIFIED:
			break;
		default:
			if (pbuf_startwith("[LINK:")) {
				newline();
				printf("関連リンク\n");

				for (int i=0; i < accesslist_count; i++) {
					printf("<url>%s\n", accesslist_ref[i]);
					printf("<title>%s\n", accesslist_title[i]);
					newline();
				}

				printf("<url>");
				char *purl = pbuf_getcopied();
				purl += 6;
				while (']' != *purl) {
					putchar(*purl);
					++purl;
				}
				newline();

				printf("<title>");
				++purl;
				++purl;
				while (']' != *purl) {
					putchar(*purl);
					++purl;
				}
				newline();
				break;
			}
			if (pbuf_startwith("[URL:")) {
				break;
			}
			if (pbuf_startwith("[LEAD:")) {
				output("＜リード＞");
				pbuf_trimoutputln(6, 1);
				newline();
				break;
			}
			if (pbuf_startwith("[W:")) {
				break;
			}
			if (pbuf_startwith("[TAG:")) {
				break;
			}
			if (pbuf_startwith("[SRC:")) {
				break;
			}
			if (pbuf_startwith("[POST:")) {
				break;
			}
			if (in_item) {
				if (listtype_order)
					printf("%d. ", 
						listtype_order_index++);
				else
					output("・");
				pbuf_outputln();
				in_item = 0;
			}
			else {
				if (doc_started) {
					if (in_import) {
						newline();
						pbuf_flushln();
						in_import = 0;
					}
					else {
						newline();
						printf("　"); // paragraph indent
						pbuf_flushln();
					}
				}
				else {
					printf("＜本文＞\n"); // paragraph indent
					if (in_import) {
						pbuf_flushln();
						in_import = 0;
					}
					else {
						printf("　"); // paragraph indent
						pbuf_flushln();
					}
					doc_started = 1;
				}
			}
			break;
		}
		break;
	case ELEMENT_ACCESS:
		pbuf_add("■", 3);
		pbuf_add(access_ref, strlen(access_ref));
		pbuf_add("□", 3);
		pbuf_add(access_chardata, strlen(access_chardata));
		pbuf_add("■", 3);
		in_access = 0;

		strcpy(accesslist_ref[accesslist_count], access_ref);
		strcpy(accesslist_title[accesslist_count], access_chardata);
		++accesslist_count;

		break;
	case ELEMENT_LIST:
		in_item = 0;
		break;
	case ELEMENT_DOCGROUP:
		docgroup_depth--;	
		break;
	}
}

void
el_chardata_handler(void *data, const XML_Char *cdata, int len)
{
	if (in_access) {
		memcpy(p_access_chardata, cdata, len);
		p_access_chardata += len;
	}
	else {
		pbuf_add(cdata, len);
	}
}
