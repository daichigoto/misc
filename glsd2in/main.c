/*
 * Copyright (c) 2017 Daichi GOTO
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

#include "glsd2in.h"

int
main(int argc, char *argv[])
{
	FILE *fp;
	int ch, len, done;
	const int bufsize = BUFSIZ;
	char buf[bufsize];
	char *input = "/dev/stdin";

	p_flag = 0;
	while ((ch = getopt(argc, argv, "phv")) != -1)
		switch (ch) {
		case 'h':
			usage();
			break;
		case 'p':
			p_flag = 1;
			break;
		case 'v':
			version();
			break;
		default:
			usage();
		}
	argv += optind;

	XML_Parser ps = XML_ParserCreate(NULL);

	if (1 <= argc - optind)
		input = argv[0];

	fp = fopen(input, "r");
	if (NULL == fp)
		err(errno, "%s", input);

	rm("images.zip");

	XML_SetElementHandler(ps, el_start_handler, el_end_handler);
	XML_SetCharacterDataHandler(ps, el_chardata_handler);
	for (;;) {
		len = fread(buf, 1, bufsize, fp);
		if (ferror(fp)) 
			err(ferror(fp), NULL);
		done = feof(fp);

		if (XML_Parse(ps, buf, len, done) == XML_STATUS_ERROR) {
			fprintf(stderr,
				"Parse error at line %lu --> %s\n",
				XML_GetCurrentLineNumber(ps),
				XML_ErrorString(XML_GetErrorCode(ps)));
			exit(EX_DATAERR);
		}

		if (done)
			break;
	}
	XML_ParserFree(ps);

	return(EX_OK);
}

void
usage(void)
{
	fprintf(stderr, "glsd2mn [-hv] typescript.xml > output.mn\n");
	exit(EX_USAGE);
}

void
version(void)
{
	fprintf(stderr, "version: " VERSION "\n");
	exit(EX_USAGE);
}
