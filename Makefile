obj = 	huffman/huffman_tree.o \
		util/util.o \
		util/bitvector.o

test =	hamlet.orig \
		juliuscaesar.orig \
		macbeth.orig \
		othello.orig

CC = g++ -g

all: huffmanencode huffmandecode

test: $(addprefix tests/, $(test:.orig=.enc)) $(addprefix tests/, $(test:.orig=.dec))
	@tests/ratios


.SUFFIXES: .enc .dec
tests/%.enc: huffmanencode input/%
	./$< input/$(notdir $*) > $@
%.dec: huffmandecode %.enc
	./$< $*.enc > $@

huffmanencode: $(obj) encode.cc
	$(CC) $^ -I. -o $@
huffmandecode: $(obj) decode.cc
	$(CC) $^ -I. -o $@

%.o: %.cc %.h
	$(CC) -c $< -I. -o $@
%.o: %.cc
	$(CC) -c $< -o $@

clean:
	-$(RM) *.o huffman/*.o util/*.o
clean-tst:
	-$(RM) tests/*.dec tests/*.enc
clean-bin:
	-$(RM) huffmandecode huffmanencode
clean-gh:
	-$(RM) tags types_c.taghl
clean-all: clean clean-tst clean-bin clean-gh
