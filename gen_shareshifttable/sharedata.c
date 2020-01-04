/*
 * Copyright (c) 2018,2020 Daichi GOTO
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

#include "gen_shareshifttable.h"

void
file_2_sharedata(char *path, SHAREDATA *data)
{
	const int buf_len = BUFSIZ;
	char buf[buf_len], *buf_p, *name_p;
	char index[buf_len], *index_p;
	FILE *fp;
	int sharedata_index;

	fp = fopen(path, "r");
	if (NULL == fp)
		err(EX_NOINPUT, "%s", path);

	sharedata_index = 1;
	while (NULL != fgets(buf, buf_len - 1, fp)) {
		memset(data[sharedata_index].name, 0, 
			SHAREDATA_NAME_MAXLEN);

		buf_p = buf;
		name_p = data[sharedata_index].name;
		while ('\t' != *buf_p && '\n' != *buf_p && 
		       '\0' != *buf_p) {
			*name_p++ = *buf_p++;
		}

		if ('\t' == *buf_p)
			++buf_p;

		memset(index, 0, buf_len);
		index_p = index;
		while ('\t' != *buf_p && '\n' != *buf_p && 
		       '\0' != *buf_p && '%' != *buf_p) {
			*index_p++ = *buf_p++;
		}

		errno = 0;
		data[sharedata_index].index = 
			strtod(index, (char **)NULL);
		if (0 != errno)
			err(EX_DATAERR, "%s", index);

		if ('\0' != data[sharedata_index].name[0])
			++sharedata_index;
	}
}

void
sharedata_2_shareshiftdata
	(SHARESHIFTDATA *shift, SHAREDATA *cur, SHAREDATA *pre)
{
	int i, j;

	i = 1;
	while ('\0' != cur[i].name[0]) {
		memset(shift[i].name, 0, SHAREDATA_NAME_MAXLEN);
		
		strcpy(shift[i].name, cur[i].name);
		shift[i].cur_index = cur[i].index;

		j = 1;
		shift[i].pre_index = NONE_INDEX_DATA;
		while ('\0' != pre[j].name[0]) {
			if (0 == strcmp(cur[i].name, pre[j].name)) {
				shift[i].pre_index = pre[j].index;
				break;
			}
			++j;
		}
		++i;
	}
}

void
output_shareshiftdata_style1(SHARESHIFTDATA *shift)
{
	int i;

	i = 1;
	while ('\0' != shift[i].name[0]) {
		printf("%d\t%s\t%.02f%%\t", 
			i, shift[i].name, shift[i].cur_index);

		if (NONE_INDEX_DATA == shift[i].pre_index)
			printf("ー\t");
		else
			printf("%.02f%%\t", shift[i].pre_index);

		if (shift[i].cur_index > shift[i].pre_index)
			printf("↑");
		else if (shift[i].cur_index < shift[i].pre_index)
			printf("↓");
		else
			printf("＝");
		putchar('\n');
		++i;
	}
}

void
debug_output_shareshiftdata(SHARESHIFTDATA *shift)
{
	int i;

	i = 1;
	while ('\0' != shift[i].name[0]) {
		fprintf(stderr, "%s\t%f\t%f\n",
			shift[i].name,
			shift[i].cur_index,
			shift[i].pre_index);
		++i;
	}
}

void
debug_output_sharedata(SHAREDATA *data)
{
	int i;

	i = 1;
	while ('\0' != data[i].name[0]) {
		fprintf(stderr, "%s\t%f\n",
			data[i].name,
			data[i].index);
		++i;
	}
}
