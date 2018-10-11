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
  d.insert(Student(16181, 17));
  d.insert(Student(16176, 17));

  cout << d.select(Student(16179)) << endl;
  cout << d.select(Student(16176)) << endl;

  d.update(Student(16177, 19));
  cout << d.select(Student(16177)) << endl << endl;
  d.remove(Student(16180));

  cout << endl;
  d.print(cout);

  d.~Database();
  remove("data.dat");
  remove("tree.dat");
  return 0;
}