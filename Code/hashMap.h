#include "odom.h"
#include <vector>
#include <stdlib.h>
#include <time.h>

using namespace std;

class hashMap {
private:
    //Vector with open hashing
    vector<Odom*> map = {};
    vector<long> perm = {};
    int s;
    int capacity;

    //Fills the permutation vector with pseudo random values to avoid collision clustering
    void createPerm() {
        perm.resize(capacity);
        srand(time(NULL));
        for (auto p : perm)
            //Sets each value between (1 - capacity)
            p = rand() % capacity + 1;
    }

    //Dynamic hash function, changes whenever the capacity does.
    int hash(int val) {
        return val % capacity;
    }

    //Returns the next empty index in the sequence. Always returns a value
    int collisionpolicy(int val) {
        int i = 0;
        while (true) {
            if(map[hash(val + perm[i])] == nullptr)
                //FIXME: Should always an empty index eventually
                return hash(val + perm[i]);
            i++;
        }
    }

    //Grows size of hash map and assigns all old values new indexes based on new hash function
    void expand() {
        //Increase capacity by 1000%
        capacity *= 10;
        //Create vector and assign old map to it
        vector<Odom*> oldMap = map;
        //Clear and resize map and perm vector
        map.clear();
        map.resize(capacity, nullptr);
        createPerm();
        //Iterate through existing vector and rehash all values to new positions
        for (int i = 0; i < oldMap.size(); i++) {
            if (oldMap[i] != nullptr) {
                int index = hash(oldMap[i]->t_());
                if (map[index] == nullptr)
                    //Insert object into map if nothing is in that position
                    map[index] = map[i];
                else
                    //Call the collision resolver if there is something in that position
                    map[collisionpolicy(index)] = oldMap[i];
            }
        }
    }
    
public:
    hashMap() {
        s = 0;
        int capacity = 100;
        map.resize(capacity, nullptr);
        createPerm();
    }

    //Checks if the map contains the value
    bool has(int val) {
        if(hash(val) < map.size() && map[hash(val)] != nullptr) {
            //If the value is at the hashed index, return true
            if (map[hash(val)]->t_() == val) return true;
            //If its not run it through the permutation vector
            int i = 0;
            while (map[hash(val + perm[i])] != nullptr) {
                if (map[hash(val + perm[i])]->t_() == val) return true;
                i++;
            }
        }
        return false;
     }

    void insert(Odom* data) {
        if (!has(data->t_())) {
            if ((float)s / capacity  > 0.5)  expand();
            if (map[hash(data->t_())] == nullptr)
                map[hash(data->t_())] = data;
            else 
                map[collisionpolicy(data->t_())] = data;
            s++;
        }
    }

    int size() {
        return s;
    }
};