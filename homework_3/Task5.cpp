#include <iostream>
#include <string>

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/random_access_index.hpp>

using namespace boost::multi_index;

struct Person {
    std::string surname;
    std::string number;
};

using person_multi_index = multi_index_container <
Person, indexed_by <
        hashed_non_unique <
        member < Person, std::string, &Person::surname > > ,
hashed_non_unique <
member < Person, std::string , &Person::number	> > ,
random_access <> ,
		ordered_non_unique <
			member < Person, std::string , &Person::surname > > > > ;


int main () {
    person_multi_index persons;
    persons.insert({"Petrov", "+79104768042"});
    persons.insert({"Putin", "+79194731666"});
    persons.insert({"Medvedev", "+79152468062"});
    persons.insert({"Navalny", "+79164943013"});
    persons.insert({"Navalny", "+79164747811"});

    std::cout << "Hashed surnames:\n";
    std::cout << persons.count("Navalny") << " " << persons.count("Putin") << " " << persons.count("Volkov")
        << std::endl;

    auto & hashed_surname_index = persons.get < 0 > ();
    std::cout << hashed_surname_index.count("Navalny") << std::endl;

    auto iterator = hashed_surname_index.find("Putin");
    hashed_surname_index.modify(iterator, [](Person & person) { person.number = "+79194631666"; });

    std::cout << hashed_surname_index.find("Putin")->number << std::endl;

    std::cout << "==========\nHashed numbers:\n";

    auto & hashed_number_index = persons.get< 1 > ();

    std::cout << hashed_number_index.find("+79152468062")->surname << "\n";

    std::cout << "==========\nRandom access:\n";

    const auto & random_access_index = persons.get < 2 > ();

    for (const auto& per : random_access_index) {
        std::cout << per.surname << " " << per.number << " ";
    }

    std::cout << "\n==========\nOrdered:\n";


    const auto & ordered_legs_index = persons.get < 3 > ();


    for (const auto& per : ordered_legs_index)
    {
    	std::cout << per.surname << " ";
    }

    std::cout << std::endl;




    return 0;
}