#include <bits/stdc++.h>
using namespace std;

template<typename V>
class Node{
    public:
    string key;
    V value;
    Node<V>* next;

    Node(){
        key = 0;
        value = 0;
        next = NULL;
    }

    Node(string key, V value){
        this->key = key;
        this->value = value;
        next = NULL;
    }
    ~Node(){
        delete this->next;
    }
};

template<typename V>
class Hashmap{
    private:
    Node<V>** bucketArray;
    int capacity;
    int size;
    const double LOAD_FACTOR_LIMIT = 0.7;
    const int INITIAL_BUCKET_CAPACITY = 16;
    const int MULTIPLICITY_FACTOR = 2;

    private:
    int compress(int x){
        return x%capacity;
    }

    int hashFunction(string s){
        const int p = 37; // prime
        int hashcode = 0;
        int i=0;
        int power = 1;
        for(auto it = s.rbegin(); it!=s.rend();it++){
            hashcode += (capacity+ ((int)(*it)%capacity*power)) % capacity;  // may go out of range of int
            power = (power*p)%capacity;
            //assert(hashcode>0);
            if(hashcode<0){
                cout << "HashCode<0 : " << s << endl;
            }
            hashcode = (capacity+hashcode) % capacity; //or use compress()
        }
        int idx = compress(hashcode);
        return idx;
    }
    
    public:
    Hashmap(){
      // Initial Capacity which doubles to maintain the load factor < 0.7
        size = 0;
        bucketArray = new Node<V>*[INITIAL_BUCKET_CAPACITY];
        for(int i=0;i<capacity;i++){
            bucketArray[i] = NULL;
        }
    }
    private:

    void rehash(){
        //Todo : Safety check for new capacity to not go beyond INT_MAX but lite for now.
        Node<V>** temp = bucketArray;
        bucketArray = new Node<V>*[MULTIPLICITY_FACTOR*capacity];
        for(int i=0;i<MULTIPLICITY_FACTOR*capacity;i++){
            bucketArray[i] = NULL;
        }
        size = 0;
        int oldCapacity = capacity;
        capacity = oldCapacity*MULTIPLICITY_FACTOR;

        for(int i=0;i<oldCapacity;i++){
            Node<V>* curr = temp[i];
            while(curr!=NULL){
                string key = curr->key;
                V value = curr->value;
                insert(key,value);
                curr = curr->next;
            }
        }
        for(int i=0;i<oldCapacity;i++){
            Node<V>* head = temp[i];
            delete head;
        }
        delete []temp;
    }

    public:
    double getLoadFactor(){
        double loadFactor = (1.0*size)/capacity;
        return loadFactor;
    }

    int getSize(){
        return size;
    }

    int getCapacity(){
        return capacity;
    }
    private:
    void maintainLoadFactor(){
        if(getLoadFactor()>=LOAD_FACTOR_LIMIT){
            cout << "Rehashing on size = " << size << endl;
            rehash();
        }
    }

    public:
    void insert(string key, V value){
        int idx = hashFunction(key);
        //If current bucketIndex is pointing to NULL, add entry there
        if(bucketArray[idx]==NULL){
            Node<V>* newNode = new Node<V>(key,value);
            bucketArray[idx] = newNode; size++; maintainLoadFactor(); return;
        }
        assert(bucketArray[idx]!=NULL);
        //Check whether key is already present, if so update
        Node<V>* curr = bucketArray[idx];
        while(curr!=NULL){
            if(curr->key == key){
                curr->value = value; maintainLoadFactor(); return;  //Update value
            }
            curr = curr->next;
        }
        //If key not present, insert at starting of linkedList
        Node<V>* newNode = new Node<V>(key,value);
        newNode->next = bucketArray[idx];
        bucketArray[idx] = newNode; size++;

        maintainLoadFactor(); 
    }

    V getValue(string key){
        int idx = hashFunction(key);
        if(bucketArray[idx]==NULL){
            //cout << "Key not present!" << endl;
            return 0;
        }
        Node<V>* curr = bucketArray[idx];
        while(curr!=NULL){
            if(curr->key == key){
                return curr->value;
            }
            curr = curr->next;
        }
        //Control should not reach here
        assert(false);
        return 0;
    }

    V remove(string key){
        int idx = hashFunction(key);
        Node<V>* curr = bucketArray[idx];
        Node<V>* prev = NULL;
        while(curr!=NULL){
            if(curr->key == key){
                if(prev==NULL){
                    bucketArray[idx] = curr->next;
                }
                else prev->next = curr->next;
                V value = curr->value;
                curr->next = NULL; // prevent recursive destructive calls
                delete curr;
                size--;
                return value;
            }
            prev = curr;
            curr = curr->next;
        }
        return 0;
    }
};

int main(){
    Hashmap<int> m;
    string base = "key";
    cout << m.getLoadFactor() << " " << m.getCapacity() << endl;
    for(int i=0;i<5000;i++){
        string key = base+to_string(i);
        int value = i+1;
        m.insert(key,value);
        cout << key << ": " << m.getValue(key) 
        <<" ( Load Factor = " << m.getLoadFactor() << " )"
        << " Capacity = " << m.getCapacity() << endl;
    }
    cout << "Size: " << m.getSize() << endl;
    m.remove(base+"5");
    m.remove(base+"9");
    cout << "Size: " << m.getSize() << endl;
    delete &m;
    return 0;
}