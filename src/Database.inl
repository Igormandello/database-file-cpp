#include <stdexcept>

template <class T>
Database<T>::Database(string dataFile, string treeFile, T defaultValue) {
  const char* dataFileChr = dataFile.c_str();
  this->dataFile.open(dataFileChr, ios::out | ios::in | ios::binary);
  if (!this->dataFile.is_open()) {
    this->dataFile.open(dataFileChr, ios::out);
    this->dataFile.close();

    this->dataFile.open(dataFileChr, ios::out | ios::in | ios::binary);
  } 

  const char* treeFileChr = treeFile.c_str();
  this->treeFile.open(treeFileChr, ios::out | ios::in | ios::binary);
  if (!this->treeFile.is_open()) {
    this->treeFile.open(treeFileChr, ios::out);
    this->treeFile.close();

    this->treeFile.open(treeFileChr, ios::out | ios::in | ios::binary);
  } 

  this->defaultValue = new T(defaultValue);
}

template <class T>
void Database<T>::insert(T data) {
  this->dataFile.seekg(0, this->dataFile.end);
  this->treeFile.seekg(0, this->treeFile.end);

  char* bytes = reinterpret_cast<char*>(&data);
  this->dataFile.write(bytes, sizeof(T));

  int amount = this->dataFile.tellg() / sizeof(T);
  Node n(amount - 1);

  int next = 0;
  if (this->treeFile.tellg() > 0) {
    T tmp;
    Node current;
    char* tBytes = new char[sizeof(T)];
    char* nodeBytes = new char[sizeof(Node)];
    
    while (next != -1) {
      this->treeFile.seekg(next * sizeof(Node), this->treeFile.beg);
      this->treeFile.read(nodeBytes, sizeof(Node));
      memcpy(&current, nodeBytes, sizeof(Node));

      this->dataFile.seekg(current.data * sizeof(T), this->dataFile.beg);
      this->dataFile.read(tBytes, sizeof(T));
      memcpy(&tmp, tBytes, sizeof(T));
      
      if (data > tmp)
        if (current.right == -1) {
          current.right = amount - 1;
          break;
        } else
          next = current.right;
      else if (data < tmp)
        if (current.left == -1) {
          current.left = amount - 1;
          break;
        } else
          next = current.left;
      else
        throw invalid_argument("Data already exists");
    }

    this->treeFile.seekg(next * sizeof(Node), this->treeFile.beg);
    bytes = reinterpret_cast<char*>(&current);
    this->treeFile.write(bytes, sizeof(Node));
  }

  this->treeFile.seekg(0, this->treeFile.end);
  bytes = reinterpret_cast<char*>(&n);
  this->treeFile.write(bytes, sizeof(Node));
}

template <class T>
T Database<T>::select(T data) {
  this->treeFile.seekg(0, this->treeFile.end);

  int next = 0;
  if (this->treeFile.tellg() > 0) {
    T tmp;
    Node current;
    char* tBytes = new char[sizeof(T)];
    char* nodeBytes = new char[sizeof(Node)];
    
    while (next != -1) {
      this->treeFile.seekg(next * sizeof(Node), this->treeFile.beg);
      this->treeFile.read(nodeBytes, sizeof(Node));
      memcpy(&current, nodeBytes, sizeof(Node));

      this->dataFile.seekg(current.data * sizeof(T), this->dataFile.beg);
      this->dataFile.read(tBytes, sizeof(T));
      memcpy(&tmp, tBytes, sizeof(T));
      
      if (data > tmp)
          next = current.right;
      else if (data < tmp)
          next = current.left;
      else
        return tmp;
    }
  }

  return T(*this->defaultValue);
}

template <class T>
void Database<T>::update(T data) {


}

template <class T>
void Database<T>::remove(T data) {


}

template <class T>
void Database<T>::print(ostream& os) {
  char* read = new char[sizeof(T)];
  T tmp;
  int actual = 0;

  os << "dataFile:" << endl;
  this->dataFile.seekg(0, this->dataFile.end);
  int dataLength = this->dataFile.tellg();
  for (int i = 0; i < dataLength; i += sizeof(T)) {
    this->dataFile.seekg(i, this->dataFile.beg);
    this->dataFile.read(read, sizeof(T));
    memcpy(&tmp, read, sizeof(T));

    os << "  " << tmp << endl;
    actual++;
  }

  read = new char[sizeof(Node)];
  Node tmpNode;
  actual = 0;

  os << endl;
  os << "treeFile:" << endl;
  this->treeFile.seekg(0, this->dataFile.end);
  dataLength = this->treeFile.tellg();
  for (int i = 0; i < dataLength; i += sizeof(Node)) {
    this->treeFile.seekg(i, this->treeFile.beg);
    this->treeFile.read(read, sizeof(Node));
    memcpy(&tmpNode, read, sizeof(Node));

    os << "  " << tmpNode << endl;
    actual++;
  }
}