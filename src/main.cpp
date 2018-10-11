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

  bool operator<(const Student& s2) {
    return this->ra < s2.ra;
  }

  bool operator>(const Student& s2) {
    return this->ra > s2.ra;
  }
};

int main() {
  Database<Student> d ("data.dat", "tree.dat", Student(-1));
  
  d.insert(Student(16177, 17));
  d.insert(Student(16180, 17));
  d.insert(Student(16178, 17));
  d.insert(Student(16176, 17));

  cout << d.select(Student(16179)) << endl;
  cout << d.select(Student(16178)) << endl;

  d.update(Student(16178, 19));
  cout << d.select(Student(16178)) << endl << endl;
  d.print(cout);
  d.remove(Student(16178));

  cout << endl;
  d.print(cout);
  return 0;
}