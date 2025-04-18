/*
 * Copyright (c) 2017,2023 Daichi GOTO
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

#include "glsd2html.h"

static ELEMENT cur_element;
static ELEMENT pre_element;

static int docgroup_depth = 0;

static bool in_import = false;
static bool in_item = false;
static bool in_quote = false;

static int list_depth = -1;
static bool listtype_order[10] = {false};

static bool in_note = false;
static int note_count = 0;

static char quote_ref[BUFSIZ];

#define HTML_START \
	"<!doctype html>\n" \
	"<html lang=\"ja\">\n" \
	"<head>\n" \
	"<meta charset=\"utf-8\">\n" \
	"<meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">\n" \
	"<link rel=\"stylesheet\" " \
	      "href=\"https://maxcdn.bootstrapcdn.com/" \
	              "bootstrap/3.3.7/css/bootstrap.min.css\">\n" \
	"<script src=\"https://ajax.googleapis.com/ajax/libs/" \
	              "jquery/1.12.4/jquery.min.js\"></script>\n" \
	"<script src=\"https://maxcdn.bootstrapcdn.com/" \
	               "bootstrap/3.3.7/js/bootstrap.min.js\"></script>\n" \
	"<script>\n" \
        "  // Firefoxは想定通りに機能しないため除外する\n" \
        "  var agent = window.navigator.userAgent.toLowerCase();\n" \
        "  if (-1 == agent.indexOf(\"firefox\")) {\n" \
        "    // Firefox以外でフォーカス＆リロード\n" \
        "    \n" \
        "    // ウィンドウフォーカス時にリロード\n" \
	"    window.addEventListener('focus', ()=>{\n" \
	"      window.location.reload();\n" \
	"    });\n" \
        "  }\n" \
        "  \n" \
        "  // 2秒ごとにリロード\n" \
        "  // window.addEventListener('load', ()=>{\n" \
        "  //  setTimeout(()=>{window.location.reload()}, 2000);\n" \
        "  //});\n" \
	"</script>\n" \
	"<title>Generated from GLSD</title>\n" \
	"</head>\n" \
	"<body>\n" \
	"<div class=\"container\">"

#define HTML_END \
	"</div>\n" \
	"</body>\n" \
	"</html>"

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
		pbuf_add("<a href=\"", 9);
		for (int i = 0; attr[i]; i += 2)
			if (0 == strcmp(attr[i], "ref"))
				pbuf_add(attr[i+1], strlen(attr[i+1]));
		pbuf_add("\">", 2);
		break;
	case ELEMENT_IMPORT:
		in_import = true;
		import(attr);
		break;
	case ELEMENT_ITEM:
		in_item = true;
		output("<li>");
		break;
	case ELEMENT_LIST:
		newline();
		++list_depth;
		listtype_order[list_depth] = false;
		for (int i = 0; attr[i]; i += 2) {
			if (0 == strcmp(attr[i], "type") &&
			    (0 == strcmp(attr[i+1], "order") || 
			     0 == strcmp(attr[i+1], "quote,order") ||
			     0 == strcmp(attr[i+1], "order,quote") )) {
				listtype_order[list_depth] = true;
				outputln("<ol>");
				break;
			}
		}
		if (!listtype_order[list_depth])
			outputln("<ul>");
		break;
	case ELEMENT_DOCGROUP:
		if (0 < docgroup_depth)
			newline();
		docgroup_depth++;	
		break;
	case ELEMENT_DOCUMENT:
		outputln(HTML_START);
		newline();
		break;
	case ELEMENT_QUOTE:
		in_quote = true;
		newline();
		for (int i = 0; attr[i]; i += 2)
			if (0 == strcmp(attr[i], "ref")) {
				strncpy(quote_ref, attr[i+1], 
					sizeof(quote_ref));
				break;
			}
		break;
	case ELEMENT_NOTE:
		in_note = true;
		note_count = 1;
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
		if (in_import) {
			in_import = false;
			newline();
			pbuf_flushln();
			return;
		}
		if (in_item) {
			output("<p style=\"font-size: 18px;\">");
			pbuf_flush();
			output("</p>");
			in_item = false;
			return;
		}
		if (in_quote) {
			output("<blockquote");
			if ('\0' != quote_ref[0]) {
				output(" cite=\"");
				output(quote_ref);
				output("\">");
			}
			else
				output(">");
			pbuf_flush();
			outputln("</blockquote>");
			memset(quote_ref, '\0', sizeof(quote_ref));
			in_quote = false;
			return;
		}
		if (in_note) {
			printf("<p style=\"font-size: 14px;\">(注%d) ",note_count);
			pbuf_flush();
			output("</p>");
			++note_count;
		 	return;
		}

		switch((int)pre_element) {
		case ELEMENT_TITLE:
			switch(docgroup_depth) {
			case 1:
				output("<h1>");
				pbuf_output();
				outputln("</h1>");
				newline();
				break;
			case 2:
				output("<h2>");
				pbuf_output();
				outputln("</h2>");
				newline();
				break;
			default:
				output("<h3>");
				pbuf_output();
				outputln("</h3>");
				newline();
				break;
			}
			break;
		case ELEMENT_SHORTTITLE:
			switch(docgroup_depth) {
			case 1:
				output("<h2>");
				pbuf_output();
				outputln("</h2>");
				newline();
				break;
			case 2:
				output("<h3>");
				pbuf_output();
				outputln("</h3>");
				newline();
				break;
			default:
				output("<h4>");
				pbuf_output();
				outputln("</h4>");
				newline();
				break;
			}
			break;
		case ELEMENT_AUTHOR:
			output("<p>");
			pbuf_output();
			outputln("</p>");
			newline();
			break;
		case ELEMENT_FIRSTEDITION:
			break;
		case ELEMENT_LASTMODIFIED:
			break;
		default:
			if (pbuf_startwith("[MJ:")) {
				output("<p>");
				pbuf_output();
				outputln("</p>");
				newline();
				break;
			}
			if (pbuf_startwith("[URL:")) {
				output("<p>");
				pbuf_output();
				outputln("</p>");
				newline();
				break;
			}
			if (pbuf_startwith("[LEAD:")) {
				output("<p class=\"\" ");
				output("style=\"\">[LEAD:");
				pbuf_trimoutput(6, 1);
				outputln("]</p>");
				newline();
				break;
			}
			if (pbuf_startwith("[W:")) {
				output("<p>");
				pbuf_output();
				outputln("</p>");
				newline();
				break;
			}
			if (pbuf_startwith("[TAG:")) {
				break;
			}
			if (pbuf_startwith("[SRC:")) {
				output("<p>");
				pbuf_output();
				outputln("</p>");
				newline();
				break;
			}
			if (pbuf_startwith("[POST:")) {
				newline();
				output("<p>");
				pbuf_output();
				outputln("</p>");
				newline();
				break;
			}
			if (pbuf_startwith("[LINK:")) {
				newline();
				output("<p>");
				pbuf_output();
				outputln("</p>");
				newline();
				break;
			}
			newline();
			output("<p style=\"font-size: 18px;\">");
			pbuf_flush();
			outputln("</p>");
			break;
		}
		break;
	case ELEMENT_ACCESS:
		pbuf_add("</a>", 4);
		break;
	case ELEMENT_ITEM:
		outputln("</li>");
		break;
	case ELEMENT_LIST:
		if (listtype_order[list_depth])
			outputln("</ol>");
		else
			outputln("</ul>");
		--list_depth;
		break;
	case ELEMENT_NOTE:
		in_note = false;
		note_count = 0;
		break;
	case ELEMENT_DOCGROUP:
		docgroup_depth--;	
		break;
	case ELEMENT_DOCUMENT:
		outputln(HTML_END);
		break;
	}
}

void
el_chardata_handler(void *data, const XML_Char *cdata, int len)
{
	pbuf_add(cdata, len);
}
