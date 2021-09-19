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

#--------------------------------------------------------------------
# CFLAGS
#--------------------------------------------------------------------
.if defined(NEED_LIBJPEG)
. if ${OS} == "FreeBSD"
CFLAGS+=	-ljpeg
. endif
. if ${OS} == "Darwin"
.  if exists(/opt/homebrew/lib/)
CFLAGS+=	-I/opt/homebrew/include/ \
		-L/opt/homebrew/lib/ \
		-ljpeg
.  elif exists(/usr/local/opt/jpeg-turbo/)
CFLAGS+=	-I/usr/local/opt/jpeg-turbo/include/ \
		-L/usr/local/opt/jpeg-turbo/lib/ \
		-ljpeg
.  endif
. endif
.endif

.if defined(NEED_LIBEXPAT)
CFLAGS+=	-lexpat
.endif

install-required-packages:
#--------------------------------------------------------------------
# expat (XML 1.0 parser written in C)
#--------------------------------------------------------------------
.if defined(NEED_LIBEXPAT)
. if ${OS} == "FreeBSD"
.  if !exists(${LIBDIR}/libexpat.so)
	sudo pkg install -y expat
.  endif
. endif
.endif
#--------------------------------------------------------------------
# libjpeg compatible library
#--------------------------------------------------------------------
.if defined(NEED_LIBJPEG)
. if ${OS} == "FreeBSD"
.  if !exists(${LIBDIR}/libjpeg.so)
	sudo pkg install -y jpeg-turbo
.  endif
. endif
. if ${OS} == "Darwin"
.  if !exists(/opt/homebrew/lib/libjpeg.dylib) && !exists(/usr/local/opt/jpeg-turbo/lib/libjpeg.dylib)
	brew install jpeg-turbo
.  endif
. endif
.endif
#--------------------------------------------------------------------
# ZIP
#--------------------------------------------------------------------
.if defined(NEED_ZIP)
. if ${OS} == "FreeBSD"
.  if !exists(/usr/local/bin/zip)
	sudo pkg install -y zip
.  endif
. endif
.endif
#--------------------------------------------------------------------
# GraphicsMagick
#--------------------------------------------------------------------
.if defined(NEED_GM)
. if ${OS} == "FreeBSD"
.  if !exists(/usr/local/bin/gm)
	sudo pkg install -y GraphicsMagick
.  endif
. endif
. if ${OS} == "Darwin"
.  if !exists(/opt/homebrew/bin/gm) && !exists(/usr/local/bin/gm)
	brew install graphicsmagick
.  endif
. endif
.endif
