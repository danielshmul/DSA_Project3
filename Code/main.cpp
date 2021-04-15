#include "odom.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <map>
#include <unordered_map>

using namespace std;


int main() 
{   
    ifstream ifs;
    ifs.open("../odom.csv");

    ifstream input;
    input.open("frontend_in.txt");

    ofstream output;
    output.open("backend_out.txt");

    // may want to be bigger depending on max time?
    // make a destructor?
    map<int, Odom*> o_map;
    unordered_map<int, Odom*> u_map;

    string operation;
    cin >> operation;

    while(operation != "stop")
    {
        if(operation == "load")
        {
            string lineFromFile;
            stringstream line;
            double px, py, pz, lvx, lvy, lvz, avx, avy, avz;
            int t = 0; // time variable

            auto o_map_start = chrono::system_clock::now();
            while(getline(ifs, lineFromFile))
            {
                line.str(lineFromFile);
                line >> px >> py >> pz >> lvx >> lvy >> lvz >> avx >> avy >> avz;

                o_map[t++*10] = new Odom(px, py, pz, lvx, lvy, lvz, avx, avy, avz);
            }
            auto o_map_stop = chrono::system_clock::now();
            int o_map_duration = chrono::duration_cast<chrono::milliseconds>(o_map_stop - o_map_start).count();

            output << "o_map load dur," << o_map_duration << endl;

            ifs.seekg(0, ios::beg);
            t = 0;

            auto u_map_start = chrono::system_clock::now();
            while(getline(ifs, lineFromFile))
            {
                line.str(lineFromFile);
                line >> px >> py >> pz >> lvx >> lvy >> lvz >> avx >> avy >> avz;

                u_map[t++*10] = new Odom(px, py, pz, lvx, lvy, lvz, avx, avy, avz);
            }
            auto u_map_stop = chrono::system_clock::now();
            int u_map_duration = chrono::duration_cast<chrono::milliseconds>(u_map_stop - u_map_start).count();

            output << "u_map load dur," << u_map_duration << endl;
            // output time range
            output << "t range,1," << t << endl;
        }

        cin >> operation;
    }
    
    


    return 0;
}