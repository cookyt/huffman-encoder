obj = 	huffman/huffman_tree.o \
		util/util.o \
		util/bitvector.o

enctest = $(addsuffix .enc, $(addprefix tests/, $(notdir $(wildcard ./input/*))))
dectest = $(enctest:.enc=.dec)

encname = encode
decname = decode

CC = g++ -g

all: $(encname) $(decname)

test: $(enctest) $(dectest)
	@tests/ratios


.SUFFIXES: .enc .dec
tests/%.enc: $(encname) input/%
	./$< input/$(notdir $*) > $@
%.dec: $(decname) %.enc
	./$< $*.enc > $@

$(encname): $(obj) encode.cc
	$(CC) $^ -I. -o $@
$(decname): $(obj) decode.cc
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
	-$(RM) $(encname) $(decname)
clean-gh:
	-$(RM) tags types_c.taghl
clean-all: clean clean-tst clean-bin clean-gh
