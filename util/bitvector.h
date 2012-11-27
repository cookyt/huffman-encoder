#ifndef BITVECTOR_H
#define BITVECTOR_H

#include <vector>
#include <string>
#include <cstdio>

class bitvector
{
  private:
    int len;
    std::vector<char> bits;
  public:
    bitvector();
    bitvector(FILE *fin);

    int size() const;
    int char_size() const;

    const char *c_array() const;
    std::string to_string() const;

    void to_file(FILE *fout) const;

    bitvector& operator +=(const bitvector& rhs);
    bitvector& operator +=(const bool rhs);
    bool operator [](const int i) const;
};

inline bitvector operator+(bitvector lhs, const bitvector& rhs)
{
    lhs += rhs;
    return lhs;
}
#endif
