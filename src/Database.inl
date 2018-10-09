template <class T>
Database<T>::Database(string dataFile, string treeFile) {
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
}

template <class T>
void Database<T>::insert(T data) {
  char* bytes = reinterpret_cast<char*>(&data);
  this->dataFile.write(bytes, sizeof(data));
}

template <class T>
T Database<T>::select(T data) {
  char* read = new char[sizeof(data)];
  this->dataFile.read(read, sizeof(data));

  T tmp;
  memcpy(&tmp, read, sizeof(data));

  return tmp;
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