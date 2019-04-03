all:
	gcc -c codebook.c && gcc -c compress.c && gcc -c fileCompressor.c && gcc compress.o codebook.o fileCompressor.o -o fileCompressor
