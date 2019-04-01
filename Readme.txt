Eklavya Patel
Krishna Chamarti
Systems Programming
SPRING 2019
File Compressor

This Program uses Huffman Coding to encrypt words. This encryption is used to compress and decompress files. These operations can be done recursively over multiple files in a directory or a singular file.

The following command line flags perform different operations on the file:

-R	Recursively traverses a directory and performs operations on all files encountered.

-c	compresses given file into .hcz file

-d	decompresses a given file 

-b	builds a Huffman codebook from text

Description:

Filecompressor tokenizes the input file using '\t', '\n' and ' ' as denims. These tokens are stored onto a linked list that is sorted using Mergesort to have the tokens is increasing order of frequency. This makes it easier to build a Huffman Tree out of these tokens and derive their Huffman coding.

Time and Space Complexity Analysis:

In the worst case, FileCompressor receives a directory with n file and n unique tokens in every single file.
The recursive element


