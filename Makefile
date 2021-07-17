TARGET_DIRS=	gen_shareshifttable	\
		gen_shareshifttable_12	\
		glsd2gm	\
		glsd2html	\
		glsd2im	\
		glsd2in	\
		glsd2mn	\
		glsd2txt

all: install

install: 
.for i in ${TARGET_DIRS}
	cd ${i}; make install
.endfor

uninstall: 
.for i in ${TARGET_DIRS}
	cd ${i}; make uninstall
.endfor

clean:
.for i in ${TARGET_DIRS}
	cd ${i}; make clean
.endfor
