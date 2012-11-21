test: main.cpp huffman_node.o huffman_tree.o
	g++ huffman_node.o huffman_tree.o main.cpp -o test

huffman_node.o: huffman_node.cpp huffman_node.hpp
	g++ -c huffman_node.cpp

huffman_tree.o: huffman_tree.cpp huffman_tree.hpp
	g++ -c huffman_tree.cpp
