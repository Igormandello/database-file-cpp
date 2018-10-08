#include <iostream>
#include <fstream>
#include "Database.hpp"

using namespace std;

struct Student {
  int ra;
  string name;

  Student(int ra, string name) {
    this->ra = ra;
    this->name = name;
  }

  Student(int ra) {
    this->ra = ra;
    this->name = "";
  }

  Student() {
    this->ra = 0;
    this->name = "";
  }

  void swap(Student& other) {
    std::swap(this->ra, other.ra);
    std::swap(this->name, other.name);
  }

  friend ostream& operator<<(ostream& os, const Student& s) {
    os << s.ra << " " 
      << s.name.size() << ":" 
      << s.name;
    return os;
  }

  friend istream& operator>>(istream& is, Student& s) {
    Student tmp;
    size_t size;
    if (is >> tmp.ra >> size) {
        tmp.name.resize(size);

        is.ignore(1); //Ignore the ':' char
        if (is.read(&tmp.name[0], size))
          s.swap(tmp);
    }

    return is;
  }
};

int main() {
  Database<Student> d ("data.dat", "tree.dat");

  return 0;
}