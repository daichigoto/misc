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

#include "glsd2mn.h"

#define ZIPCMD	"zip -u %s %s > /dev/null"

void copy(char *, char *);
void zip(char *, char *);
void rm(char *);

void
copy(char *file_in, char *file_ou)
{
	int fd_in, fd_ou;
	const int buf_len = 1024 * 1024;
	char buf[buf_len];

	fd_in = open(file_in, O_RDONLY);
	fd_ou = open(file_ou, O_WRONLY|O_CREAT, DEFFILEMODE);
	if (-1 == fd_in)
		err(errno, NULL);
	if (-1 == fd_ou)
		err(errno, NULL);
	
	int rl, wl, off;
	rl = wl = off = 0;
	while ((rl = read(fd_in, buf, buf_len)) > 0)
		for (off = 0; rl; rl -= wl, off += wl)
			if ((wl = write(fd_ou, buf + off, (size_t)rl)) < 0)
				err(errno, NULL);

	close(fd_in);
	close(fd_ou);
}

void
zip(char *file_zip, char *file)
{
	char buf[BUFSIZ] = { '\0' };

	snprintf(buf, BUFSIZ, ZIPCMD, file_zip, file);
	system(buf);
}

void
rm(char *file)
{
	unlink(file);
}
