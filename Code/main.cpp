#include "odom.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <map>
#include "avl.h"
#include "hashMap.h"
#include <unordered_map>

using namespace std;


int main()
{
    ifstream ifs;
    ifs.open("../odom.csv");

    cout << "What would you wish to do?" << endl;
    string operation;
    cin >> operation;

    Node* tree = new Node(); //avl tree
    hashMap u_map; //hashmap

    while (operation != "stop")
    {

        if (operation == "load")
        {
            string lineFromFile;
            stringstream line;
            vector<Odom*> listOdom;
            double px, py, pz, lvx, lvy, lvz, avx, avy, avz;
            int t = 0; // time variable

            //obtain all values from csv and push them to a vector to avoid including file reading from duration
            while (getline(ifs, lineFromFile))
            {
                line.str(lineFromFile);
                line >> px >> py >> pz >> lvx >> lvy >> lvz >> avx >> avy >> avz;

                Odom* point = new Odom(px, py, pz, lvx, lvy, lvz, avx, avy, avz, t+=10);
                listOdom.push_back(point);

            }
            cout << "The number of elements in this dataset: " << listOdom.size() << endl;
            ifs.close();

            //----------------------insert elements into avl tree----------------------//
            auto o_map_start = chrono::system_clock::now();
            for (int i = 0; i < listOdom.size(); i++) {
                if (listOdom[i]->t_() % 50000 == 0) {
                    cout << i+1 << endl;
                }
                tree->insert(listOdom[i], listOdom[i]->t_());
            }
            auto o_map_stop = chrono::system_clock::now();
            double o_map_duration = chrono::duration_cast<chrono::milliseconds>(o_map_stop - o_map_start).count();
            cout << "o_map load dur," << o_map_duration << endl;

            //----------------------insert elements into unordered map (hashMap)-----------------------//
            auto u_map_start = chrono::system_clock::now();
            for (int i = 0; i < listOdom.size(); i++) {
                u_map.insert(listOdom[i]);
            }
            auto u_map_stop = chrono::system_clock::now();
            double u_map_duration = chrono::duration_cast<chrono::milliseconds>(u_map_stop - u_map_start).count();
            cout << "u_map load dur," << u_map_duration << endl;
        }

        cout << "What would you wish to do now?" << endl;
        cin >> operation;
    }

    return 0;
}