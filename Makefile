obj = 	huffman/huffman_tree.o \
		util/util.o \
		util/bitvector.o

enctest = $(addsuffix .enc, $(addprefix tests/, $(notdir $(wildcard ./input/*))))
dectest = $(enctest:.enc=.dec)

bin = huf
header_size = header_size

encode = ./$(bin) -e
decode = ./$(bin) -d

CC = g++ -g

all: $(bin)

test: $(enctest) $(dectest)
	@tests/ratios

.SUFFIXES: .enc .dec
tests/%.enc: $(bin) input/%
	$(encode) input/$(notdir $*) > $@
%.dec: $(bin) %.enc
	$(decode) $*.enc > $@

$(bin): $(obj) huf.cc
	$(CC) $^ -I. -o $@

$(header_size): header_size.cc
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
	-$(RM) $(bin) $(header_size)
clean-gh:
	-$(RM) tags types_c.taghl
clean-all: clean clean-tst clean-bin clean-gh
