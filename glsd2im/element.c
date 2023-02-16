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

#include "glsd2im.h"

int
element_is(const XML_Char *name)
{
	if (0 == strcmp(name, "p"))
		return ELEMENT_P;
	if (0 == strcmp(name, "access"))
		return ELEMENT_ACCESS;
	if (0 == strcmp(name, "import"))
		return ELEMENT_IMPORT;
	if (0 == strcmp(name, "list"))
		return ELEMENT_LIST;
	if (0 == strcmp(name, "item"))
		return ELEMENT_ITEM;
	if (0 == strcmp(name, "author"))
		return ELEMENT_AUTHOR;
	if (0 == strcmp(name, "title"))
		return ELEMENT_TITLE;
	if (0 == strcmp(name, "shorttitle"))
		return ELEMENT_SHORTTITLE;
	if (0 == strcmp(name, "firstedition"))
		return ELEMENT_FIRSTEDITION;
	if (0 == strcmp(name, "firstedition"))
		return ELEMENT_FIRSTEDITION;
	if (0 == strcmp(name, "lastmodified"))
		return ELEMENT_LASTMODIFIED;
	if (0 == strcmp(name, "docgroup"))
		return ELEMENT_DOCGROUP;
	if (0 == strcmp(name, "quote"))
		return ELEMENT_QUOTE;
	if (0 == strcmp(name, "note"))
		return ELEMENT_NOTE;

	return ELEMENT_UNKOWN;
}
