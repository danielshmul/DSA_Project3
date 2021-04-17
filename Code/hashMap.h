#include "odom.h"
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <iostream>

using namespace std;

class hashMap {
private:
    //Vector with open hashing
    vector<Odom*> map = {};
    int s;
    int capacity;

    //Dynamic hash function, changes whenever the capacity does.
    int hash(int val) {
        return (val == 0)? 0 : val % capacity;
    }

    //Returns the next empty index in the sequence. Always returns a value
    int collisionpolicy(int val) {
        int i = 1;
        while (true) {
            if(map[hash(val + i)] == nullptr)
                //FIXME: Should always an empty index eventually
                return hash(val + i);
            i++;
        }
    }

    //Grows size of hash map and assigns all old values new indexes based on new hash function
    void expand() {
        //Increase capacity by 1000%
        capacity *= 10;
        //Create vector and assign old map to it
        vector<Odom*> oldMap = map;
        //Clear and resize map
        map.clear();
        map.resize(capacity, nullptr);
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
    }

    //Checks if the map contains the value
    bool has(int val) {
        if(hash(val) < map.size() && map[hash(val)] != nullptr) {
            //If the value is at the hashed index, return true
            if (map[hash(val)]->t_() == val) return true;
            //If its not run it through the permutation vector
            int i = 1;
            while (map[hash(val + i)] != nullptr) {
                if (map[hash(val + i)]->t_() == val) return true;
                i++;
            }
        }
        return false;
     }

    void insert(Odom* data) {
        cout << "here";
        if (!has(data->t_())) {
            if ((float)s / capacity  > 0.5)  expand();
            if (map[hash(data->t_())] == nullptr)
                map[hash(data->t_())] = data;
            else 
                map[collisionpolicy(data->t_())] = data;
            s++;
        }
    }

    Odom* get(int val) {
        if(hash(val) < map.size() && map[hash(val)] != nullptr) {
            //If the value is at the hashed index, return the value
            if (map[hash(val)]->t_() == val) return map[hash(val)];
            //If its not run it through the collision policy
            int i = 1;
            while (map[hash(val + i)] != nullptr) {
                if (map[hash(val + i)]->t_() == val) return map[hash(val + i)];
                i++;
            }
        }
        return nullptr;
    }

    int size() {
        return s;
    }
};