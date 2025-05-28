# On Mac OSX gcc is required as clang complains bitterly about the old style
# C delarations used.

CC=gcc
CXX=g++
DEP=zmac.o mio.o doc.o zi80dis.o

# Some systems like CentOS may require this
# YACC=bison -y
# Is there a YACCFLAGS?

zmac.exe: doc.inl zmac_doc.html $(DEP)
	$(CXX) -Wall $(CXXFLAGS) -o ../zmac.exe $(DEP)

doc.exe: doc.c doc.txt
	$(CC) -Wall -DMK_DOC -o doc.exe doc.c

doc.inl: doc.exe
	./doc.exe >/dev/null

zmac.c:
	bison --output=zmac.c zmac.y

zmac_doc.html: doc.exe
	./doc.exe > ../zmac_doc.html

clean:
	rm -f ../zmac.exe ../zmac_doc.html zmac.c doc.exe doc.inl $(DEP)

