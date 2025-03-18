/*
 * Copyright (c) 2023 Daichi GOTO
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

void
swap(char *src, char *pre, char *post)
{
	char *p_pre;
	const size_t len_pre = strlen(pre);
	const size_t len_post = strlen(post);

	p_pre = src;
	while (NULL != (p_pre = strstr(p_pre, pre))) {
		const char *remain = p_pre + len_pre;
		memmove(p_pre + len_post, remain, strlen(remain) + len_pre);
		memcpy(p_pre, post, len_post);
	}
}

void
string_processing_for_itmedia_cms(char *src)
{
 	// 関連リンクの「·」が不適切な変換を受けるので
	// 類似記号へ置き換える
	swap(src, "·", "・");

 	// 関連リンクの「 •」が不適切な変換を受けるので
	// 類似記号へ置き換える
	swap(src, " • ", "・");

 	// 関連リンクの「–」が不適切な変換を受けるので
	// 類似記号へ置き換える
	swap(src, "–", "-");

 	// 関連リンクの「2c a0」(&nbsp;)が不適切な変換を受けるので
	// 類似記号へ置き換える
	swap(src, " ", " ");
}
