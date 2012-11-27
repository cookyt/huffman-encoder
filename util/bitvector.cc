#include <cassert>
#include "bitvector.h"

//Constructs an empty bitvector
bitvector::bitvector()
{
    len = 0;
}

//Constructs a bitvector. It reads the contents of the input file into the
//bitvector. It expects the format of the file to be: a single byte (0-7)
//representing the offset followed by the contents of the vector.
bitvector::bitvector(FILE *fin)
{
    int offset = fgetc(fin);
    assert(offset <= 7 && offset >= 0);
    for (;;)
    {
        char c = fgetc(fin);
        if (feof(fin))
            break;

        bits.push_back(c);
    }
    len = bits.size()*8 - offset;
}


// Returns the number of bits in the vector
int bitvector::size() const
{
    return len;
}

// Returns the number of bytes used to represent this vector
// i.e. ceil(size()/8)
int bitvector::char_size() const
{
    return bits.size();
}

// Returns a pointer to a char array representing the bits. These characters
// should not be changed.
const char *bitvector::c_array() const
{
    return &bits[0];
}

// returns a string representation of this bitvector containg ASCII 1's and 0's
std::string bitvector::to_string() const
{
    std::string str = "";
    for (int i=0; i<len; i++)
    {
        if (i%8 == 0 && i!=0)
            str += ' ';
        if ((*this)[i])
            str += '1';
        else
            str += '0';
    }
    return str;
}


// Writes the entire vector to file. It prepends a single byte representing the
// offset amount to the beginning of the file. The offset is the number of
// unused bits in the last byte of the bitvector (0-7).
void bitvector::to_file(FILE *fout) const
{
    int offset = (7-(len-1)%8);
    fputc(offset, fout);
    fwrite(c_array(), sizeof(char), char_size(), fout);
}

// Appends the contents of another bit vector to this one.
bitvector& bitvector::operator +=(const bitvector& rhs)
{
    for (int i=0; i<rhs.size(); i++)
    {
        *this += rhs[i];
    }
    return *this;
}

// Appends a bit to this vector. The rhs should be either a 1 or a 0.
bitvector& bitvector::operator +=(const bool rhs)
{
    len++;
    if (len%8 == 1)
        bits.push_back('\0');

    if (rhs)
        bits[(len-1)/8] |= (1 << (7-(len-1)%8));

    return *this;
}

// Returns the value of a single bit as a bool. This bool has no connection to
// the underlying data structure.
bool bitvector::operator [](const int i) const
{
    assert(i < len);
    if ((bits[i/8]&(1<<(7-(i%8)))) == 0)
        return false;
    return true;
}