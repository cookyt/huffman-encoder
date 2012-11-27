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

test: $(addprefix tests/, $(test:.orig=.enc)) $(addprefix tests/, $(test:.orig=.dec)) ratios

ratios:
	@tests/ratios

.SUFFIXES: .enc .dec .tree
tests/%.enc tests/%.tree: encode input/%
	./encode input/$(notdir $*) tests/$*.tree > $@
%.dec: decode %.enc %.tree
	./decode $*.enc $*.tree > $@

encode: $(obj) encode.cc
	$(CC) $^ -o $@
decode: $(obj) decode.cc
	$(CC) $^ -o $@

%.o: %.cc %.h
	$(CC) -c $< -o $@
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
