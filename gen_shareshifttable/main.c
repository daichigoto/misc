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

#include "gen_shareshifttable.h"

int
main(int argc, char *argv[])
{
	int ch;
	SHARESHIFTDATA shift[100];
	SHAREDATA cur[100], pre[100];

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

	if (3 != argc)
		usage();

	file_2_sharedata(argv[0], cur);
	file_2_sharedata(argv[1], pre);
	sharedata_2_shareshiftdata(shift, cur, pre);
	output_shareshiftdata_style1(shift);

	return(EX_OK);
}

void
usage(void)
{
	fprintf(stderr, 
		"gen_shareshifttable cur.tsv pre.tsv > shareshift.tsv\n");
	exit(EX_USAGE);
}

void
version(void)
{
	fprintf(stderr, "version: " VERSION "\n");
	exit(EX_USAGE);
}
