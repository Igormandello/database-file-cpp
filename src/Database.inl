template <class T>
Database<T>::Database(string dataFile, string treeFile) {
  this->dataFile.open(dataFile, ios::out | ios::in | ios::binary);
  if (!this->dataFile.is_open()) {
    this->dataFile.open(dataFile, ios::out);
    this->dataFile.close();

    this->dataFile.open(dataFile, ios::out | ios::in | ios::binary);
  } 

  this->treeFile.open(treeFile, ios::out | ios::in | ios::binary);
  if (!this->treeFile.is_open()) {
    this->treeFile.open(treeFile, ios::out);
    this->treeFile.close();

    this->treeFile.open(treeFile, ios::out | ios::in | ios::binary);
  } 
}

template <class T>
void Database<T>::insert(T data) {


}

template <class T>
T Database<T>::select(T data) {


}

template <class T>
void Database<T>::update(T data) {


}

template <class T>
void Database<T>::remove(T data) {


}