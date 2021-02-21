#include <iostream>
#include <random>
#include <string>
#include <vector>
#include <set>
#include <fstream>
#include <chrono>


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

std::set <std::string> makeRandomWords (size_t length) {
    std::uniform_int_distribution letter(97, 122);
    std::default_random_engine e(static_cast <std::size_t>
                                 (std::chrono::system_clock::now().time_since_epoch().count()));

    std::set <std::string> words;

    for (std::string str(length, '_'); words.size() < 5000000u; words.insert(str)) {
        for (auto &c : str)
            c = letter(e);
    }

    return words;
}



int main () {
    std::vector<std::set<int>> codes (9);
    std::vector<std::string> titles = {"RS", "JS", "PJW", "ELF", "BKDR", "SDBM", "DJB", "DEK", "AP"};

    std::ofstream file ("collisions_2.txt");

    std::set<std::string> words = makeRandomWords(10);

    int size = 100000;
    for (int i = 0; i < 11; ++i) {
        std::cout << i+1 << "/11\n";
        for (auto& code : codes) code.clear();

        auto it = words.begin();
        for (int j = 0; j < size; ++j) {
            codes[0].insert(RSHash((*it).c_str(), (*it).length()));
            codes[1].insert(JSHash((*it).c_str(), (*it).length()));
            codes[2].insert(PJWHash((*it).c_str(), (*it).length()));
            codes[3].insert(ELFHash((*it).c_str(), (*it).length()));
            codes[4].insert(BKDRHash((*it).c_str(), (*it).length()));
            codes[5].insert(SDBMHash((*it).c_str(), (*it).length()));
            codes[6].insert(DJBHash((*it).c_str(), (*it).length()));
            codes[7].insert(DEKHash((*it).c_str(), (*it).length()));
            codes[8].insert(APHash((*it).c_str(), (*it).length()));

            it++;
        }

        for (int k = 0; k < 9; ++k) {
            file << size << " " << titles[k] << ": " << size - codes[k].size() << " ; ";
        }
        file << std::endl;

        size += 100000;
    }

    // График в файле graph_task_4
    // PJW и ELF сверху, совпадают, следующая после них - DEK, остальные близки к друг другу
    // и с меньшим числом коллизий, чем предыдущие три.
}