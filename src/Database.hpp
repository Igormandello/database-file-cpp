#ifndef DATABASE_INCLUDED
#define DATABASE_INCLUDED

#include <iostream>
#include <string>

using namespace std;

template <class T>
class Database {
  public:
    Database(string, string);
    void insert(T);
    T select(T);
    void update(T);
    void remove(T);

  private:
    fstream dataFile;
    fstream treeFile;
};

#include "Database.inl"

#endif