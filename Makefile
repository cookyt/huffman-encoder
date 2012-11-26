src = 	huffman/huffman_tree.cc \
		huffman/huffman_node.cc \
		util.cc \
		main.cc \

CC = g++ -g

test: $(src:.cc=.o)
	$(CC) $^ -o $@

%.o: %.cc %.h
	$(CC) -c $< -o $@

%.o: %.cc
	$(CC) -c $< -o $@

clean:
	-$(RM) *.o
	-$(RM) huffman/*.o
