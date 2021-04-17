#include "odom.h"
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <iostream>
using namespace std;

class hashMap {
private:
    //Vector with open hashing policy
    vector<Odom*> map = {};
    int s, capacity;

    //Dynamic hash function, changes whenever the capacity does.
    int hash(int val) { return (val == 0) ? 0 : val % capacity; }

    //Returns the next empty index in the sequence. Always returns a value
    int collisionpolicy(int val) {
        int i = 0;
        while (i++ < capacity) {
            if (map[hash(val + i)] == nullptr) return hash(val + i);
        }
        return -1;
    }

    //Grows size of hash map by 1000% and assigns all old values new indexes based on new hash function
    void expand() {
        capacity *= 10;
        //Assign  map to temorary vector and clear its contents
        vector<Odom*> oldMap = map;
        map.clear();
        map.resize(capacity, nullptr);
        //Iterate through existing vector and rehash all values to new positions
        for (int i = 0; i < oldMap.size(); i++) {
            if (oldMap[i] != nullptr) {
                int index = hash(oldMap[i]->t_());
                //Insert object into map if nothing is in new index
                if (map[index] == nullptr) map[index] = oldMap[i];
                //Call the collision resolver if something is in that position
                else map[collisionpolicy(index)] = oldMap[i];
            }
        }
    }

public:
    hashMap() {
        s = 0;
        capacity = 100;
        map.resize(capacity, nullptr);
    }

    //Checks if the map contains the value
    bool has(int val) {
        if (hash(val) < map.size() && map[hash(val)] != nullptr) {
            //If the value is at the hashed index, return true
            if (map[hash(val)]->t_() == val) return true;
            //If its not run it through the collision sequence
            int i = 1;
            while (map[hash(val + i)] != nullptr) {
                if (map[hash(val + i)]->t_() == val) return true;
                i++;
            }
        }
        return false;
    }

    void insert(Odom* data) {
        if (!has(data->t_())) {
            if ((float)s / capacity > 0.5)  expand();
            if (map[hash(data->t_())] == nullptr) map[hash(data->t_())] = data;
            else map[collisionpolicy(data->t_())] = data;
            s++;
        }
    }

    Odom* get(int val) {

        if (hash(val) < map.size() && map[hash(val)] != nullptr) {
            //If the value is at the hashed index, return the value
            if (map[hash(val)]->t_() == val) return map[hash(val)];
            //If its not run it through the collision policy
            int i = 1;
            while (map[hash(val + i)] != nullptr) {
                if (map[hash(val + i)]->t_() == val) return map[hash(val + i)];
                i++;
            }
        }
        cout << "hello" << endl;
        return nullptr;
    }

    int size() { return s; }
};