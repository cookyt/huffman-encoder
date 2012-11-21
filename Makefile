CC = g++ -g

test: main.cpp huffman_node.o huffman_tree.o
	$(CC) huffman_node.o huffman_tree.o main.cpp -o test

huffman_node.o: huffman_node.cpp huffman_node.hpp
	$(CC) -c huffman_node.cpp

huffman_tree.o: huffman_tree.cpp huffman_tree.hpp
	$(CC) -c huffman_tree.cpp

clean:
	-$(RM) *.o
