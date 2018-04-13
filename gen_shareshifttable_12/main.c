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

#include "gen_shareshifttable_12.h"

int
main(int argc, char *argv[])
{
	int ch;
	SHARESHIFT12DATA shift[100];
	SHAREDATA m1[100], m2[100], m3[100], m4[100], m5[100], m6[100],
	          m7[100], m8[100], m9[100], m10[100], m11[100], m12[100];

	while ((ch = getopt(argc, argv, "hv")) != -1)
		switch (ch) {
		case 'h':
			usage();
			break;
		case 'v':
			version();
			break;
		default:
			usage();
		}
	argv += optind;

	if (13 != argc)
		usage();

	file_2_sharedata(argv[0], m1);
	file_2_sharedata(argv[1], m2);
	file_2_sharedata(argv[2], m3);
	file_2_sharedata(argv[3], m4);
	file_2_sharedata(argv[4], m5);
	file_2_sharedata(argv[5], m6);
	file_2_sharedata(argv[6], m7);
	file_2_sharedata(argv[7], m8);
	file_2_sharedata(argv[8], m9);
	file_2_sharedata(argv[9], m10);
	file_2_sharedata(argv[10], m11);
	file_2_sharedata(argv[11], m12);

	share12data_2_shareshift12data(shift, 
		m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11, m12);
	output_shareshift12data_style1(shift);

	return(EX_OK);
}

void
usage(void)
{
	fprintf(stderr, 
		"gen_shareshifttable_12 m1.tsv m2.tsv m3.tsv m4.tsv "
		"m5.tsv m6.tsv m7.tsv m8.tsv m9.tsv m10.tsv m11.tsv "
		"m12.tsv > shareshift.tsv\n");
	exit(EX_USAGE);
}

void
version(void)
{
	fprintf(stderr, "version: " VERSION "\n");
	exit(EX_USAGE);
}
