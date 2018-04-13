/*
 * Copyright (c) 2018 Daichi GOTO
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

#define SHAREDATA_NAME_MAXLEN	100
#define NONE_INDEX_DATA		-1

typedef struct sharedata {
	char	name[SHAREDATA_NAME_MAXLEN];
	double	index;
} SHAREDATA;

typedef struct shareshiftdata {
	char	name[SHAREDATA_NAME_MAXLEN];
	double	cur_index;
	double	pre_index;
} SHARESHIFTDATA;

typedef struct shareshift12data {
	char	name[SHAREDATA_NAME_MAXLEN];
	double	m1_index;
	double	m2_index;
	double	m3_index;
	double	m4_index;
	double	m5_index;
	double	m6_index;
	double	m7_index;
	double	m8_index;
	double	m9_index;
	double	m10_index;
	double	m11_index;
	double	m12_index;
} SHARESHIFT12DATA;

void file_2_sharedata(char *, SHAREDATA *);
void share12data_2_shareshift12data(SHARESHIFT12DATA *, 
	SHAREDATA *, SHAREDATA *, SHAREDATA *, SHAREDATA *, SHAREDATA *, 
	SHAREDATA *, SHAREDATA *, SHAREDATA *, SHAREDATA *, SHAREDATA *, 
	SHAREDATA *, SHAREDATA *);
void sharedata_2_shareshiftdata(SHARESHIFTDATA *, 
	SHAREDATA *, SHAREDATA *);

void output_shareshift12data_style1(SHARESHIFT12DATA *);
void output_shareshiftdata_style1(SHARESHIFTDATA *);

void debug_output_shareshift12data(SHARESHIFT12DATA *);
void debug_output_shareshiftdata(SHARESHIFTDATA *);
void debug_output_sharedata(SHAREDATA *);
