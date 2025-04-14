/*
 * Copyright (c) 2022,2023,2025 Daichi GOTO
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
static bool listtype_order = false;
static int listtype_order_index = 1;
static int docgroup_depth = 0;

static char *p_access_chardata;
static char access_chardata[BUFSIZ];
static char access_ref[BUFSIZ];
static char quote_ref[BUFSIZ];

static int accesslist_count = 0;
static char accesslist_ref[100][BUFSIZ];
static char accesslist_title[100][BUFSIZ];

static bool in_item = false;
static bool in_access = false;
static bool in_quote = false;
static bool in_import = false;

static bool in_note = false;
static int note_count = 0;

static bool doc_started = false;
static bool sub_started = false;

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
		in_access = true;
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
		in_import = true;
		import(attr);
		break;
	case ELEMENT_ITEM:
		in_item = true;
		break;
	case ELEMENT_LIST:
		printf("\n％リスト\n\n");
		listtype_order = false;
		for (int i = 0; attr[i]; i += 2) {
			if (0 == strcmp(attr[i], "type") &&
			    0 == strcmp(attr[i+1], "order")) {
				listtype_order = true;
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
		printf("\n□noborder\n▼▲");
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
			// 2025-04-14 引用記号ではなくカギカッコによる表記に
			// 変えてほしいというITmedia編集部の要望に応えて変換を
			// 変更
			//printf("▼□\n");
			//pbuf_flushln();
			printf("　「");
			pbuf_flush();
			printf("」");

			// 引用元がある場合には引用元を表示
			if ('\0' != quote_ref[0]) {
				output("<br>（");
				output(quote_ref);
				output("より）");
				newline();
				memset(quote_ref, '\0', sizeof(quote_ref));
			}
			else {
				newline();
			}

			// 2025-04-14 引用記号ではなくカギカッコによる表記に
			// 変えてほしいというITmedia編集部の要望に応えて変換を
			// 変更
			//printf("▼□□\n");

			in_quote = false;
			return;
		}

		if (in_note) {
			if (1 == note_count) {
				//printf("（注%d）", note_count);
				printf("（注）");
				pbuf_flush();
			}
			else {
				printf("◆br◆（注%d）", note_count);
				pbuf_flush();
			}
			++note_count;
			return;
		}

		switch((int)pre_element) {
		case ELEMENT_ITEM:
			if (listtype_order) {
				printf("＃");
				++listtype_order_index;
			}
			else {
				output("・");
			}
			// 列挙および枚挙の末尾の「。」は削除する
			pbuf_trimlastdot();

			pbuf_outputln();
			break;
		case ELEMENT_TITLE:
			switch(docgroup_depth) {
			case 1:
				output("##タイトル\n\n");
				// タイトルの「、」は「　」に変換されている
				// ことが多いので、自動変換しておく。
				pbuf_swap("、","　",3);

				pbuf_outputln();
				newline();
				break;
			default:
				if (2 >= docgroup_depth) {
					printf("●");
				}
				else {
					printf("○");
				}
				pbuf_output();
				putchar('\n');

				if (! sub_started) {
					printf("\nsubscription\n");
					sub_started = true;
				}
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
				//=====================================
				// [LINK:][]からURLを取得
				//=====================================
				char ref[BUFSIZ], *p_ref;
				memset(ref, 0, BUFSIZ);
				p_ref = ref;

				char *purl = pbuf_getcopied();
				purl += 6;
				while (']' != *purl) {
					*p_ref = *purl;
					++p_ref;
					++purl;
				}

				//=====================================
				// [LINK:][]からタイトルを取得
				//=====================================
				char title[BUFSIZ], *p_title;
				memset(title, 0, BUFSIZ);
				p_title = title;

				++purl;
				++purl;
				while (']' != *purl) {
					*p_title = *purl;
					++p_title;
					++purl;
				}

				//=====================================
				// アクセスリンクデータセットにリンクを追加
				//=====================================
				bool sameurl = false;
				for (int i=0; i < accesslist_count; i++) {
					if (0 == strcmp(accesslist_ref[i], ref)) {
						sameurl = true;
					}
				}
				if (!sameurl) {
					strcpy(accesslist_ref[accesslist_count], ref);
					strcpy(accesslist_title[accesslist_count], title);
					++accesslist_count;
				}
				break;
			}
			if (pbuf_startwith("[URL:")) {
				break;
			}
			if (pbuf_startwith("[LEAD:")) {
				output("##リード\n\n");
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
				//=====================================
				// [SRC:][]からURLを取得
				//=====================================
				char ref[BUFSIZ], *p_ref;
				memset(ref, 0, BUFSIZ);
				p_ref = ref;

				char *purl = pbuf_getcopied();
				purl += 5;
				while (']' != *purl) {
					*p_ref = *purl;
					++p_ref;
					++purl;
				}

				//=====================================
				// [SRC:][]からタイトルを取得
				//=====================================
				char title[BUFSIZ], *p_title;
				memset(title, 0, BUFSIZ);
				p_title = title;

				++purl;
				++purl;
				while (']' != *purl) {
					*p_title = *purl;
					++p_title;
					++purl;
				}

				//=====================================
				// アクセスリンクデータセットにリンクを追加
				//=====================================
				bool sameurl = false;
				for (int i=0; i < accesslist_count; i++) {
					if (0 == strcmp(accesslist_ref[i], ref)) {
						sameurl = true;
					}
				}
				if (!sameurl) {
					strcpy(accesslist_ref[accesslist_count], ref);
					strcpy(accesslist_title[accesslist_count], title);
					++accesslist_count;
				}

				//===========================================
				// 関連記事を出力(ITmedia記事リンク)
				//===========================================
				newline();
				printf("関連記事\n");

				//-------------------------------------------
				// 本文中access要素のURLとタイトルを関連リンクとして出力
				//-------------------------------------------
				for (int i=0; i < accesslist_count; i++) {
					if (0 == strncmp("https://www.itmedia.co.jp/",accesslist_ref[i],26)) {
						printf("%s,i\n", accesslist_ref[i]);
					}
				}

				//===========================================
				// 関連リンクを出力
				//===========================================
				newline();
				printf("関連リンク\n");

				//-------------------------------------------
				// 本文中access要素のURLとタイトルを関連リンクとして出力
				//-------------------------------------------
				for (int i=0; i < accesslist_count; i++) {
					if (0 != strncmp("https://www.itmedia.co.jp/",accesslist_ref[i],26)) {
						// ITmedia CMSで扱えない文字列のエスケープ処理
						string_processing_for_itmedia_cms(accesslist_title[i]);

						printf("%s\n", accesslist_ref[i]);
						printf("%s\n", accesslist_title[i]);
					}
				}
				break;
			}
			if (pbuf_startwith("[POST:")) {
				break;
			}
			if (in_item) {
				if (listtype_order) {
					printf("＃");
					listtype_order_index++;
				}
				else
					output("・");
				pbuf_outputln();
				in_item = false;
			}
			else {
				if (doc_started) {
					if (in_import) {
						newline();
						pbuf_flushln();
						in_import = false;
					}
					else {
						newline();
						printf("　"); // paragraph indent
						pbuf_flushln();
					}
				}
				else {
					printf("##本文\n\n"); // paragraph indent
					if (in_import) {
						pbuf_flushln();
						in_import = false;
					}
					else {
						printf("　"); // paragraph indent
						pbuf_flushln();
					}
					doc_started = true;
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
		in_access = false;

		//======================================================
		// アクセスリンクデータセットにリンクを追加
		//======================================================
		bool sameurl = false;
		for (int i=0; i < accesslist_count; i++) {
			if (0 == strcmp(accesslist_ref[i], access_ref)) {
				sameurl = true;
			}
		}
		if (!sameurl) {
			strcpy(accesslist_ref[accesslist_count], access_ref);
			strcpy(accesslist_title[accesslist_count], access_chardata);
			++accesslist_count;
		}

		break;
	case ELEMENT_LIST:
		in_item = false;
		break;
	case ELEMENT_DOCGROUP:
		docgroup_depth--;	
		break;
	case ELEMENT_NOTE:
		printf("\n□□\n");
		in_note = false;
		note_count = 0;
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
