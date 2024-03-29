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
#
include		$(MKDIR)/win.cmds.mk

#--------------------------------------------------------------------
# CFLAGS / LINKFLAGS
#--------------------------------------------------------------------
ifdef NEED_LIBJPEG
LIBJPEGDIR=	$(CONTRIBDIR)/libjpeg/
LIBJPEGA=	$(CONTRIBDIR)/libjpeg/.libs/libjpeg.a

CFLAGS+=	-I$(CONTRIBDIR)/libjpeg/
LINKFLAGS+=	$(LIBJPEGA)
endif

ifdef NEED_LIBEXPAT
LINKFLAGS+=	-lexpat
endif

# MSYS2
install-required-packages:
#--------------------------------------------------------------------
# LLVM Clang
#--------------------------------------------------------------------
ifneq ($(wildcard $(CC)),$(CC))
	pacman -S --noconfirm clang
endif
#--------------------------------------------------------------------
# expat (XML 1.0 parser written in C)
#--------------------------------------------------------------------
ifdef NEED_LIBEXPAT
 ifneq ($(wildcard /usr/include/expat.h), /usr/include/expat.h)
	pacman -S --noconfirm libexpat-devel
 endif
endif
#--------------------------------------------------------------------
# libjpeg
#--------------------------------------------------------------------
ifdef NEED_LIBJPEG
 ifneq ($(wildcard $(LIBJPEGA)), $(LIBJPEGA)) 
	(cd $(LIBJPEGDIR); $(MAKE) build)
 endif
endif
#--------------------------------------------------------------------
# ZIP
#--------------------------------------------------------------------
ifdef NEED_ZIP
 ifneq ($(wildcard /usr/bin/zip.exe), /usr/bin/zip.exe)
	pacman -S --noconfirm zip
 endif
endif
#--------------------------------------------------------------------
# GraphicsMagick
#--------------------------------------------------------------------
ifdef NEED_GM
 ifneq ($(wildcard /mingw64/bin/gm.exe), /mingw64/bin/gm.exe)
	pacman -S --noconfirm mingw-w64-x86_64-graphicsmagick
 endif
endif
