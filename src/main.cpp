#include <iostream>
#include <fstream>
#include <string.h>
#include "Database.hpp"

using namespace std;

struct Student {
  int ra;
  int age;

  Student(int ra, int age) {
    this->ra = ra;
    this->age = age;
  }

  Student(int ra) {
    this->ra = ra;
    this->age = 0;
  }

  Student() {
    this->ra = 0;
    this->age = 0;
  }

  friend ostream& operator<<(ostream& os, const Student& s) {
    os << "{ ra: " << s.ra << ", age: " << s.age << " }";
    return os;
  }
};

int main() {
  Database<Student> d ("data.dat", "tree.dat");

  Student s (16179, 17);
  d.insert(s);
  //cout << d.select(Student(16179));

  d.print(cout);

  return 0;
}