#include <iostream>
#include <random>
#include <string>
#include <vector>
#include <set>
#include <fstream>


unsigned int RSHash(const char* str, unsigned int length)
{
    unsigned int b    = 378551;
    unsigned int a    = 63689;
    unsigned int hash = 0;
    unsigned int i    = 0;

    for (i = 0; i < length; ++str, ++i)
    {
        hash = hash * a + (*str);
        a    = a * b;
    }

    return hash;
}

unsigned int JSHash(const char* str, unsigned int length)
{
    unsigned int hash = 1315423911;
    unsigned int i    = 0;

    for (i = 0; i < length; ++str, ++i)
    {
        hash ^= ((hash << 5) + (*str) + (hash >> 2));
    }

    return hash;
}

unsigned int PJWHash(const char* str, unsigned int length)
{
    const unsigned int BitsInUnsignedInt = (unsigned int)(sizeof(unsigned int) * 8);
    const unsigned int ThreeQuarters     = (unsigned int)((BitsInUnsignedInt  * 3) / 4);
    const unsigned int OneEighth         = (unsigned int)(BitsInUnsignedInt / 8);
    const unsigned int HighBits          =
            (unsigned int)(0xFFFFFFFF) << (BitsInUnsignedInt - OneEighth);
    unsigned int hash = 0;
    unsigned int test = 0;
    unsigned int i    = 0;

    for (i = 0; i < length; ++str, ++i)
    {
        hash = (hash << OneEighth) + (*str);

        if ((test = hash & HighBits) != 0)
        {
            hash = (( hash ^ (test >> ThreeQuarters)) & (~HighBits));
        }
    }

    return hash;
}

unsigned int ELFHash(const char* str, unsigned int length)
{
    unsigned int hash = 0;
    unsigned int x    = 0;
    unsigned int i    = 0;

    for (i = 0; i < length; ++str, ++i)
    {
        hash = (hash << 4) + (*str);

        if ((x = hash & 0xF0000000L) != 0)
        {
            hash ^= (x >> 24);
        }

        hash &= ~x;
    }

    return hash;
}

unsigned int BKDRHash(const char* str, unsigned int length)
{
    unsigned int seed = 131; /* 31 131 1313 13131 131313 etc.. */
    unsigned int hash = 0;
    unsigned int i    = 0;

    for (i = 0; i < length; ++str, ++i)
    {
        hash = (hash * seed) + (*str);
    }

    return hash;
}

unsigned int SDBMHash(const char* str, unsigned int length)
{
    unsigned int hash = 0;
    unsigned int i    = 0;

    for (i = 0; i < length; ++str, ++i)
    {
        hash = (*str) + (hash << 6) + (hash << 16) - hash;
    }

    return hash;
}

unsigned int DJBHash(const char* str, unsigned int length)
{
    unsigned int hash = 5381;
    unsigned int i    = 0;

    for (i = 0; i < length; ++str, ++i)
    {
        hash = ((hash << 5) + hash) + (*str);
    }

    return hash;
}

unsigned int DEKHash(const char* str, unsigned int length)
{
    unsigned int hash = length;
    unsigned int i    = 0;

    for (i = 0; i < length; ++str, ++i)
    {
        hash = ((hash << 5) ^ (hash >> 27)) ^ (*str);
    }

    return hash;
}

unsigned int APHash(const char* str, unsigned int length)
{
    unsigned int hash = 0xAAAAAAAA;
    unsigned int i    = 0;

    for (i = 0; i < length; ++str, ++i)
    {
        hash ^= ((i & 1) == 0) ? (  (hash <<  7) ^ (*str) * (hash >> 3)) :
                (~((hash << 11) + ((*str) ^ (hash >> 5))));
    }

    return hash;
}


int main () {
    std::vector<std::set<int>> codes (9);
    std::vector<std::string> titles = {"RS", "JS", "PJW", "ELF", "BKDR", "SDBM", "DJB", "DEK", "AP"};

    std::ofstream file ("collisions_2.txt");

    std::vector<std::string> letters(26, "a");
    for (int i = 0; i < 26; i++) {
        letters[i] = 'a' + i;
    }

    std::vector<std::string> words;
    words.reserve(12000000);
    for (const auto& i : letters) {
        for (const auto& j : letters) {
            for (const auto& k : letters) {
                for (const auto& l : letters) {
                    for (const auto& m : letters) words.push_back(i + j + k + l + m);
                }
            }
        }
    }

    int size = 100000;
    for (int i = 0; i < 12; ++i) {
        std::cout << i+1 << "/12\n";
        for (int j = 0; j < 9; j++) codes[j].clear();

        for (int j = 0; j < size; ++j) {
            codes[0].insert(RSHash(words[j].c_str(), words[j].length()));
            codes[1].insert(JSHash(words[j].c_str(), words[j].length()));
            codes[2].insert(PJWHash(words[j].c_str(), words[j].length()));
            codes[3].insert(ELFHash(words[j].c_str(), words[j].length()));
            codes[4].insert(BKDRHash(words[j].c_str(), words[j].length()));
            codes[5].insert(SDBMHash(words[j].c_str(), words[j].length()));
            codes[6].insert(DJBHash(words[j].c_str(), words[j].length()));
            codes[7].insert(DEKHash(words[j].c_str(), words[j].length()));
            codes[8].insert(APHash(words[j].c_str(), words[j].length()));
        }

        for (int k = 0; k < 9; ++k) {
            file << size << " " << titles[k] << ": " << size - codes[k].size() << " ; ";
        }
        file << std::endl;
        
        size += 100000;
        if (i >= 9) size += 1000000;
    }
}