#
#  Copyright (c) 2017,2021 Daichi GOTO
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
#

INCLUDEDIR?=	/usr/local/include
LIBDIR?=	/usr/local/lib

OBJS=		main.o output.o pbuf.o element.o handler.o import.o
CMD=		glsd2txt
BINDIR?=	${.CURDIR}/../bin
BINPERM?=	500

CFLAGS+=	-I${INCLUDEDIR} \
		-O2 \
		-pipe \
		-std=gnu99 \
		-fstack-protector \
		-Qunused-arguments \
		-Werror \
		-Wall \
		-W \
		-Wno-unused-parameter

CC?=		cc
MAKE?=		make
RM?=		rm
INSTALL?=	install

.c.o:
	${CC} ${CFLAGS} -c $< -o $@

build: ${OBJS}
	${CC} ${CFLAGS} -L${LIBDIR} -lexpat -o ${CMD} ${OBJS}

install: clean build
	${INSTALL} -m ${BINPERM} ${CMD} ${BINDIR}

uninstall: 
	${RM} -f ${BINDIR}/${CMD}

clean:
	${RM} -f ${CMD} ${OBJS}
