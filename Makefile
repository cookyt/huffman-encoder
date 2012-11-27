obj = 	huffman/huffman_tree.o \
		huffman/huffman_node.o \
		util/util.o \
		util/bitvector.o

test =	hamlet.orig \
		juliuscaesar.orig \
		macbeth.orig \
		othello.orig

CC = g++ -g

all: encode decode

test: $(addprefix tests/, $(test:.orig=.enc)) $(addprefix tests/, $(test:.orig=.dec))
	@tests/ratios


.SUFFIXES: .enc .dec
tests/%.enc: encode input/%
	./encode input/$(notdir $*) > $@
%.dec: decode %.enc
	./decode $*.enc > $@

encode: $(obj) encode.cc
	$(CC) $^ -I. -o $@
decode: $(obj) decode.cc
	$(CC) $^ -I. -o $@

%.o: %.cc %.h
	$(CC) -c $< -I. -o $@
%.o: %.cc
	$(CC) -c $< -o $@

clean:
	-$(RM) *.o huffman/*.o util/*.o
clean-tst:
	-$(RM) tests/*.dec tests/*.enc tests/*.tree
clean-bin:
	-$(RM) decode encode
clean-gh:
	-$(RM) tags types_c.taghl
clean-all: clean clean-tst clean-bin clean-gh
