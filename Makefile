obj = 	huffman/huffman_tree.o \
		huffman/huffman_node.o \
		util.o \

CC = g++ -g

all: encode decode

encode: $(obj) encode.cc
	$(CC) $^ -o $@

decode: $(obj) decode.cc
	$(CC) $^ -o $@

%.o: %.cc %.h
	$(CC) -c $< -o $@

%.o: %.cc
	$(CC) -c $< -o $@

clean:
	-$(RM) *.o
	-$(RM) huffman/*.o
