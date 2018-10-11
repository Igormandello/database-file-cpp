#include <stdexcept>

template <class T>
Database<T>::Database(string dataFile, string treeFile, T defaultValue) {
  const char* dataFileChr = dataFile.c_str();
  this->dataName = strdup(dataFileChr);
  this->dataFile.open(dataFileChr, ios::out | ios::in | ios::binary);
  if (!this->dataFile.is_open()) {
    this->dataFile.open(dataFileChr, ios::out);
    this->dataFile.close();

    this->dataFile.open(dataFileChr, ios::out | ios::in | ios::binary);
  } 

  const char* treeFileChr = treeFile.c_str();
  this->treeName = strdup(treeFileChr);
  this->treeFile.open(treeFileChr, ios::out | ios::in | ios::binary);
  if (!this->treeFile.is_open()) {
    this->treeFile.open(treeFileChr, ios::out);
    this->treeFile.close();

    this->treeFile.open(treeFileChr, ios::out | ios::in | ios::binary);
  } 

  this->defaultValue = new T(defaultValue);
}

template <class T>
Database<T>::~Database() {
  this->dataFile.close();
  this->treeFile.close();
  delete this->dataName;
  delete this->treeName;
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

    delete tBytes;
    delete nodeBytes;

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
      else {
        delete tBytes;
        delete nodeBytes;

        return tmp;
      }
    }
  }

  return T(*this->defaultValue);
}

template <class T>
void Database<T>::update(T data) {
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
      else {
        this->dataFile.seekg(current.data * sizeof(T), this->dataFile.beg);

        tBytes = reinterpret_cast<char*>(&data);
        this->dataFile.write(tBytes, sizeof(T));
        break;
      }
    }
  }
}

template <class T>
void Database<T>::remove(T data) {
  this->treeFile.seekg(0, this->treeFile.end);

  int next = 0, prev = 0;
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
      
      if (data > tmp) {
        prev = next;
        next = current.right;
      } else if (data < tmp) {
        prev = next;
        next = current.left;
      } else {
        if (current.left == -1 && current.right == -1) {
          Node prevNode;
          this->treeFile.seekg(prev * sizeof(Node), this->treeFile.beg);
          this->treeFile.read(nodeBytes, sizeof(Node));
          memcpy(&prevNode, nodeBytes, sizeof(Node));

          this->dataFile.seekg(prevNode.data * sizeof(T), this->dataFile.beg);
          this->dataFile.read(tBytes, sizeof(T));
          memcpy(&tmp, tBytes, sizeof(T));

          if (data > tmp)
            prevNode.right = -1;
          else
            prevNode.left = -1;

          nodeBytes = reinterpret_cast<char*>(&prevNode);
          this->treeFile.seekg(prev * sizeof(Node), this->treeFile.beg);
          this->treeFile.write(nodeBytes, sizeof(Node));

          this->removeBytes(this->treeFile, this->treeName, next * sizeof(Node), sizeof(Node));
          this->removeBytes(this->dataFile, this->dataName, current.data * sizeof(T), sizeof(T));

          int removedData = current.data;
          this->treeFile.seekg(0, this->treeFile.end);
          int last = this->treeFile.tellg();
          for (int i = 0; i < last; i += sizeof(Node)) {
            this->treeFile.seekg(i, this->treeFile.beg);
            this->treeFile.read(nodeBytes, sizeof(Node));
            memcpy(&current, nodeBytes, sizeof(Node));

            if (current.right > next)
              current.right--;
            
            if (current.left > next)
              current.left--;

            if (current.data > removedData)
              current.data--;

            nodeBytes = reinterpret_cast<char*>(&current);
            this->treeFile.seekg(i, this->treeFile.beg);
            this->treeFile.write(nodeBytes, sizeof(Node));
          }

          break;
        } else if (current.right == -1) {
          int prevBig = next;
          int nextBig = current.left;

          Node biggest;
          this->treeFile.seekg(nextBig * sizeof(Node), this->treeFile.beg);
          this->treeFile.read(nodeBytes, sizeof(Node));
          memcpy(&biggest, nodeBytes, sizeof(Node));
          
          while (biggest.right != -1) {
            prevBig = nextBig;
            nextBig = biggest.right;

            this->treeFile.seekg(nextBig * sizeof(Node), this->treeFile.beg);
            this->treeFile.read(nodeBytes, sizeof(Node));
            memcpy(&biggest, nodeBytes, sizeof(Node));
          }

          this->treeFile.seekg(nextBig * sizeof(Node), this->treeFile.beg);
          this->treeFile.read(nodeBytes, sizeof(Node));
          memcpy(&biggest, nodeBytes, sizeof(Node));

          int removedData = current.data;
          current.data = biggest.data;

          if (prevBig == next)
            current.left = -1;
          else {
            Node biggestPrev;
            this->treeFile.seekg(prevBig * sizeof(Node), this->treeFile.beg);
            this->treeFile.read(nodeBytes, sizeof(Node));
            memcpy(&biggestPrev, nodeBytes, sizeof(Node));

            biggestPrev.right = -1;

            nodeBytes = reinterpret_cast<char*>(&biggestPrev);
            this->treeFile.seekg(prevBig * sizeof(Node), this->treeFile.beg);
            this->treeFile.write(nodeBytes, sizeof(Node));
          }

          nodeBytes = reinterpret_cast<char*>(&current);
          this->treeFile.seekg(next * sizeof(Node), this->treeFile.beg);
          this->treeFile.write(nodeBytes, sizeof(Node));

          this->removeBytes(this->treeFile, this->treeName, nextBig * sizeof(Node), sizeof(Node));
          this->removeBytes(this->dataFile, this->dataName, removedData * sizeof(T), sizeof(T));

          this->treeFile.seekg(0, this->treeFile.end);
          int last = this->treeFile.tellg();
          for (int i = 0; i < last; i += sizeof(Node)) {
            this->treeFile.seekg(i, this->treeFile.beg);
            this->treeFile.read(nodeBytes, sizeof(Node));
            memcpy(&current, nodeBytes, sizeof(Node));

            if (current.right > next)
              current.right--;
            
            if (current.left > next)
              current.left--;

            if (current.data > removedData)
              current.data--;

            nodeBytes = reinterpret_cast<char*>(&current);
            this->treeFile.seekg(i, this->treeFile.beg);
            this->treeFile.write(nodeBytes, sizeof(Node));
          }
        } else {
          int prevSmall = next;
          int nextSmall = current.right;

          Node smallest;
          this->treeFile.seekg(nextSmall * sizeof(Node), this->treeFile.beg);
          this->treeFile.read(nodeBytes, sizeof(Node));
          memcpy(&smallest, nodeBytes, sizeof(Node));
          
          while (smallest.left != -1) {
            prevSmall = nextSmall;
            nextSmall = smallest.left;

            this->treeFile.seekg(nextSmall * sizeof(Node), this->treeFile.beg);
            this->treeFile.read(nodeBytes, sizeof(Node));
            memcpy(&smallest, nodeBytes, sizeof(Node));
          }

          this->treeFile.seekg(nextSmall * sizeof(Node), this->treeFile.beg);
          this->treeFile.read(nodeBytes, sizeof(Node));
          memcpy(&smallest, nodeBytes, sizeof(Node));

          int removedData = current.data;
          current.data = smallest.data;

          if (prevSmall == next)
            current.right = -1;
          else {
            Node smallestPrev;
            this->treeFile.seekg(prevSmall * sizeof(Node), this->treeFile.beg);
            this->treeFile.read(nodeBytes, sizeof(Node));
            memcpy(&smallestPrev, nodeBytes, sizeof(Node));

            smallestPrev.left = -1;

            nodeBytes = reinterpret_cast<char*>(&smallestPrev);
            this->treeFile.seekg(prevSmall * sizeof(Node), this->treeFile.beg);
            this->treeFile.write(nodeBytes, sizeof(Node));
          }

          nodeBytes = reinterpret_cast<char*>(&current);
          this->treeFile.seekg(next * sizeof(Node), this->treeFile.beg);
          this->treeFile.write(nodeBytes, sizeof(Node));

          this->removeBytes(this->treeFile, this->treeName, nextSmall * sizeof(Node), sizeof(Node));
          this->removeBytes(this->dataFile, this->dataName, removedData * sizeof(T), sizeof(T));

          this->treeFile.seekg(0, this->treeFile.end);
          int last = this->treeFile.tellg();
          for (int i = 0; i < last; i += sizeof(Node)) {
            this->treeFile.seekg(i, this->treeFile.beg);
            this->treeFile.read(nodeBytes, sizeof(Node));
            memcpy(&current, nodeBytes, sizeof(Node));

            if (current.right > next)
              current.right--;
            
            if (current.left > next)
              current.left--;

            if (current.data > removedData)
              current.data--;

            nodeBytes = reinterpret_cast<char*>(&current);
            this->treeFile.seekg(i, this->treeFile.beg);
            this->treeFile.write(nodeBytes, sizeof(Node));
          }
        }
      }
    }
  }
}

template <class T>
void Database<T>::removeBytes(fstream& file, const char* filename, int firstBytes, int skip) {
  file.seekg(0, file.end);
  int fileSize = file.tellg();
  int lastBytes = fileSize - firstBytes - skip;
  //cout << fileSize << " " << firstBytes << " " << lastBytes << endl;

  char* first = new char[firstBytes];
  char* last = new char[lastBytes];

  file.seekg(0, file.beg);
  file.read(first, firstBytes);
  file.seekg(-lastBytes, file.end);
  file.read(last, lastBytes);
  file.close();

  file.open(filename, ios::out | ios::binary | ios::trunc);
  file.write(first, firstBytes);
  file.write(last, lastBytes);
  file.close();

  file.open(filename, ios::out | ios::in | ios::binary);

  delete first;
  delete last;
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