#  Copyright (c) 2021 Daichi GOTO
#  All rights reserved.
#
#  Redistribution and use in source and binary forms, with or without
#  modifigation, are permitted provided that the following conditions are
#  met:
#
#  1. Redistributions of source code must retain the above copyright
#     notice, this list of conditions and the following disclaimer.
#  2. Redistributions in binary form must reproduce the above copyright
#     notice, this list of conditions and the following disclaimer in the
#     documentation and/or other materials provided with the distribution.
#
#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
#  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
#  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
#  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
#  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
#  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
#  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
#  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
#  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
#  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
#  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

SRCS=		$(wildcard *.c)
OBJS=		$(SRCS:.c=.o)

BINPERM=	500

INCLUDEDIR=	/usr/include
LIBDIR=		
CC=		/usr/bin/clang

CFLAGS=		-I${INCLUDEDIR} \
		-O2 \
		-pipe \
		-std=gnu99 \
		-fstack-protector \
		-Qunused-arguments \
		-Werror \
		-Wall \
		-W \
		-Wno-unused-parameter

LINKFLAGS=	

RM=		rm 
COPY=		cp
INSTALL=	install

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

build: install-required-packages $(BINDIR)/$(CMD)

$(BINDIR)/$(CMD): $(OBJS)
	$(CC) $(CFLAGS) -o $(CMD) $(OBJS) $(LINKFLAGS) 

install: build
	$(INSTALL) -m $(BINPERM) $(CMD) $(BINDIR)

uninstall: 
	$(RM) -f $(BINDIR)/$(CMD)

clean:
	$(RM) -f $(CMD)
	$(RM) -f $(OBJS)
