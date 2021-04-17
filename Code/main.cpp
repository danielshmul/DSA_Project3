#include "odom.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <cfloat>
#include "avl.h"
#include "hashMap.h"

using namespace std;

//Helper function: Returns the updated root of the tree
Node* callInsert(Node* root, Odom* name, int id);
void findMin(Node* root, hashMap* u_map, string option, int max_time_);
void findMax(Node* root, hashMap* u_map, string option, int max_time_);
void findConstant(Node* root, hashMap* u_map, string option, int max_time);

int main()
{
    Node* root = nullptr; //avl tree root node
    hashMap u_map; //hashmap
    vector<Odom*> listOdom;
    string operation;

    while (true)
    {
        cout << "What would you wish to do? Please select a number." << endl;
        cout << "0. Stop Program" << endl;
        cout << "1. Load Data" << endl;
        cout << "2. Find Min of a value" << endl;
        cout << "3. Find Max of a value" << endl;
        cout << "4. Find Average of a value" << endl;
        cout << "5. Find Velocity at a specific time" << endl;
        // cout << "6. Find time ranges of dataset where velocity is constant and non-zero" << endl;
        cout << endl << "Selection: ";
        cin >> operation;

        if (operation == "0") {
            cout << "Thank you!" << endl;
            break;
        }
        else if (operation == "1") //load data into data structures
        {

            ifstream ifs;
            string file;
            cout << "What file would you like to load?" << endl;
            cin >> file;
            ifs.open(file);
            if (!ifs) {
                cout << "Invalid file" << endl << endl;
            }
            else {

                string lineFromFile;
                istringstream line;
                double px, py, pz, lvx, lvy, lvz, avx, avy, avz;
                double maxlvx = 0;
                int t = 0; // time variable

                // gets column titles 
                getline(ifs, lineFromFile);

                //obtain all values from csv and push them to a vector to avoid file reading in timing
                while (getline(ifs, lineFromFile))
                {
                    line.str(lineFromFile);

                    string temp;
                    getline(line, temp, ',');
                    px = stod(temp);
                    getline(line, temp, ',');
                    py = stod(temp);
                    getline(line, temp, ',');
                    pz = stod(temp);
                    getline(line, temp, ',');
                    lvx = stod(temp);
                    getline(line, temp, ',');
                    lvy = stod(temp);
                    getline(line, temp, ',');
                    lvz = stod(temp);
                    getline(line, temp, ',');
                    avx = stod(temp);
                    getline(line, temp, ',');
                    avy = stod(temp);
                    getline(line, temp, ',');
                    avz = stod(temp);

                    Odom* point = new Odom(px, py, pz, lvx, lvy, lvz, avx, avy, avz, t);
                    listOdom.push_back(point);
                    t += 10;
                }
                cout << "The number of elements in this dataset: " << listOdom.size() << endl;
                ifs.close();

                //----------------------insert elements into avl tree----------------------//
                auto o_map_start = chrono::system_clock::now();
                for (int i = 0; i < listOdom.size(); i++) {
                    root = callInsert(root, listOdom[i], listOdom[i]->t_());
                }
                cout << root->findMax(root->left, root->right) << endl;
                auto o_map_stop = chrono::system_clock::now();
                double o_map_duration = chrono::duration_cast<chrono::milliseconds>(o_map_stop - o_map_start).count();
                cout << "avl tree load duration: " << o_map_duration << " ms" << endl;

                //----------------------insert elements into unordered map (hashMap)-----------------------//
                auto u_map_start = chrono::system_clock::now();
                for (int i = 0; i < listOdom.size(); i++) {
                    u_map.insert(listOdom[i]);
                }
                auto u_map_stop = chrono::system_clock::now();
                double u_map_duration = chrono::duration_cast<chrono::milliseconds>(u_map_stop - u_map_start).count();
                cout << "hashmap load duration: " << u_map_duration << " ms" << endl << endl;
            }
        }
        else if (operation == "2") { //find the min 
            int min_time = 0;
            int max_time = listOdom.size() * 10;
            cout << endl;
            cout << "Which value would you like to find the minimum for?" << endl;
            cout << "1. Position X" << endl;
            cout << "2. Position Y" << endl;
            cout << "3. Position Z" << endl;
            cout << "4. Linear Velocity X" << endl;
            cout << "5. Linear Velocity Y" << endl;
            cout << "6. Linear Velocity Z" << endl;
            cout << "7. Angular Velocity X" << endl;
            cout << "8. Angular Velocity Y" << endl;
            cout << "9. Angular Velocity Z" << endl;
            cin >> operation;

            findMin(root, &u_map, operation, listOdom.size() * 10);
        }
        else if (operation == "3") {
            int min_time = 0;
            int max_time = listOdom.size() * 10;
            cout << endl;
            cout << "Which value would you like to find the maximum for?" << endl;
            cout << "1. Position X" << endl;
            cout << "2. Position Y" << endl;
            cout << "3. Position Z" << endl;
            cout << "4. Linear Velocity X" << endl;
            cout << "5. Linear Velocity Y" << endl;
            cout << "6. Linear Velocity Z" << endl;
            cout << "7. Angular Velocity X" << endl;
            cout << "8. Angular Velocity Y" << endl;
            cout << "9. Angular Velocity Z" << endl;
            cin >> operation;

            findMax(root, &u_map, operation, listOdom.size() * 10);
        }
        else if (operation == "5") {
            int time;
            cout << "Enter time:" << endl;
            cin >> time;
            cout << endl;
            
            // auto avl_start = chrono::system_clock::now();
            // Odom* avl_data = root->search(time);
            // auto avl_stop = chrono::system_clock::now();
            // double avl_duration = chrono::duration_cast<chrono::milliseconds>(avl_stop - avl_start).count();

            // cout << "Position X: " << avl_data->px_() << "\tLinear Velocity X: " << avl_data->lvx_() << "\tAngular Velocity X: " << avl_data->avx_() << endl;
            // cout << "Position Y: " << avl_data->py_() << "\tLinear Velocity Y: " << avl_data->lvy_() << "\tAngular Velocity Y: " << avl_data->avy_() << endl;
            // cout << "Position Z: " << avl_data->pz_() << "\tLinear Velocity Z: " << avl_data->lvz_() << "\tAngular Velocity Z: " << avl_data->avz_() << endl;

            // cout << "AVL Tree search duration: " << avl_duration << " ms" << endl << endl;

            auto hash_start = chrono::system_clock::now();
            Odom* hash_data = u_map.get(time);
            auto hash_stop = chrono::system_clock::now();
            double hash_duration = chrono::duration_cast<chrono::microseconds>(hash_stop - hash_start).count();

            cout << "Position X: " << hash_data->px_() << "\tLinear Velocity X: " << hash_data->lvx_() << "\tAngular Velocity X: " << hash_data->avx_() << endl;
            cout << "Position Y: " << hash_data->py_() << "\tLinear Velocity Y: " << hash_data->lvy_() << "\tAngular Velocity Y: " << hash_data->avy_() << endl;
            cout << "Position Z: " << hash_data->pz_() << "\tLinear Velocity Z: " << hash_data->lvz_() << "\tAngular Velocity Z: " << hash_data->avz_() << endl;

            cout << "Hashmap search duration: " << hash_duration << " ms" << endl << endl;        
        }
        // else if (operation == "6") {
        //     string option;
            
        //     cout << endl;
        //     cout << "Which value would you like to find the maximum for?" << endl;
        //     cout << "1. Position X" << endl;
        //     cout << "2. Position Y" << endl;
        //     cout << "3. Position Z" << endl;
        //     cout << "4. Linear Velocity X" << endl;
        //     cout << "5. Linear Velocity Y" << endl;
        //     cout << "6. Linear Velocity Z" << endl;
        //     cout << "7. Angular Velocity X" << endl;
        //     cout << "8. Angular Velocity Y" << endl;
        //     cout << "9. Angular Velocity Z" << endl;
        //     cin >> option;

            
        // }
        else if (false) {
        
        }
        else {
            cout << "Invalid Option. Please Try Again." << endl << endl;
        }
    }

    return 0;
}

Node* callInsert(Node* root, Odom* name, int id) {

    if (!root) { //If the root is null, create the node outside of the insert function
        //std::cout << "successful" << std::endl;
        return new Node(name, id);
    }
    //Finally, call the function
    //O(logN)
    return root->insert(name, id);
}

void findMin(Node* root, hashMap* u_map, string option, int max_time_) {
    int min_time = 0;
    int max_time = 0;

    cout << endl << "How many milliseconds into the simulation would you like to start?" << endl;
    cin >> min_time;
    cout << endl << "How many milliseconds into the simulation would you like to start? (Select -1 for end of simulation)" << endl;
    cin >> max_time;

    min_time = (min_time / 10) * 10; //allows us to truncate any value not divisible by ten
    if (max_time != -1)
        max_time = (max_time / 10) * 10; //allows us to truncate any value not divisible by ten
    else
        max_time = max_time_;

    //Option is based on which physics value we want to find the min of
    if (option == "1" || option == "2" || option == "3" || option == "4" || 
        option == "5" || option == "6" || option == "7" || option == "8" || option == "9") {

        cout << "The min value for the avl tree is: " << root->minInorder(DBL_MAX, option, min_time, max_time) << endl;

        double min = DBL_MAX; 
        double val = 0;
        for (int i = min_time; i < max_time; i += 10) {
            if (option == "1") {
                val = u_map->get(i)->px_();
            }
            else if (option == "2") {
                val = u_map->get(i)->py_();
            }
            else if (option == "3") {
                val = u_map->get(i)->pz_();
            }
            else if (option == "4") {
                val = u_map->get(i)->lvx_();
            }
            else if (option == "5") {
                val = u_map->get(i)->lvy_();
            }
            else if (option == "6") {
                val = u_map->get(i)->lvz_();
            }
            else if (option == "7") {
                val = u_map->get(i)->avx_();
            }
            else if (option == "8") {
                val = u_map->get(i)->avy_();
            }
            else if (option == "9") {
                val = u_map->get(i)->avz_();
            }

            if (val < min)
                min = val;
        }
        cout << "The min value for the hashmap is: " << min << endl;
    }
    else {
        cout << "Invalid Selection" << endl << endl;
    }
}

void findMax(Node* root, hashMap* u_map, string option, int max_time_) {
    int min_time = 0;
    int max_time = 0;

    cout << endl << "How many milliseconds into the simulation would you like to start?" << endl;
    cin >> min_time;
    cout << endl << "How many milliseconds into the simulation would you like to start? (Select -1 for end of simulation)" << endl;
    cin >> max_time;

    min_time = (min_time / 10) * 10; //allows us to truncate any value not divisible by ten
    if (max_time != -1)
        max_time = (max_time / 10) * 10; //allows us to truncate any value not divisible by ten
    else
        max_time = max_time_;

    //Option is based on which physics value we want to find the min of
    if (option == "1" || option == "2" || option == "3" || option == "4" ||
        option == "5" || option == "6" || option == "7" || option == "8" || option == "9") {

        cout << "The max value for the avl tree is: " << root->maxInorder(0, option, min_time, max_time) << endl;

        double max = 0;
        double val = 0;
        for (int i = min_time; i < max_time; i += 10) {
            if (option == "1") {
                val = u_map->get(i)->px_();
            }
            else if (option == "2") {
                val = u_map->get(i)->py_();
            }
            else if (option == "3") {
                val = u_map->get(i)->pz_();
            }
            else if (option == "4") {
                val = u_map->get(i)->lvx_();
            }
            else if (option == "5") {
                val = u_map->get(i)->lvy_();
            }
            else if (option == "6") {
                val = u_map->get(i)->lvz_();
            }
            else if (option == "7") {
                val = u_map->get(i)->avx_();
            }
            else if (option == "8") {
                val = u_map->get(i)->avy_();
            }
            else if (option == "9") {
                val = u_map->get(i)->avz_();
            }

            if (val > max) {
                cout << "old max: " << max << " new max: " << val << endl;
                max = val;
            }
        }
        cout << "The max value for the hashmap is: " << max << endl;
    }
    else {
        cout << "Invalid Selection" << endl << endl;
    }
}

// void findConstant(Node* root, hashMap* u_map, string option, int max_time) {
//     if (option == "1" || option == "2" || option == "3" || option == "4" || 
//         option == "5" || option == "6" || option == "7" || option == "8" || option == "9") {

//         // cout << "The min value for the avl tree is: " << root->minInorder(DBL_MAX, option, max_time) << endl;
        
//         int count = 0;
//         double temp = 0;
//         double val = 0;
//         int start = 0;
//         int end = 0;
//         for (int i = 1; i < max_time; i += 10) {
//             int t = u_map->get(i)->t_(); // time of element

//             if (option == "1") {
//                 val = u_map->get(i)->px_();
//                 temp = u_map->get(i-1)->px_();
//             }
//             else if (option == "2") {
//                 val = u_map->get(i)->py_();
//                 temp = u_map->get(i-1)->py_();
//             }
//             else if (option == "3") {
//                 val = u_map->get(i)->pz_();
//                 temp = u_map->get(i-1)->pz_();
//             }
//             else if (option == "4") {
//                 val = u_map->get(i)->lvx_();
//                 temp = u_map->get(i-1)->lvx_();
//             }
//             else if (option == "5") {
//                 val = u_map->get(i)->lvy_();
//                 temp = u_map->get(i-1)->lvy_();
//             }
//             else if (option == "6") {
//                 val = u_map->get(i)->lvz_();
//                 temp = u_map->get(i-1)->lvz_();
//             }
//             else if (option == "7") {
//                 val = u_map->get(i)->avx_();
//                 temp = u_map->get(i-1)->avx_();
//             }
//             else if (option == "8") {
//                 val = u_map->get(i)->avy_();
//                 temp = u_map->get(i-1)->avy_();
//             }
//             else if (option == "9") {
//                 val = u_map->get(i)->avz_();
//                 temp = u_map->get(i-1)->avz_();
//             }

//             if (val < temp*0.9 || val > temp*1.1)
//             {
//                 count++;
//                 if(val == 0)
//                 {
//                     count = 0;
//                     start = t;
//                 }
//                 else if(count > 250) 
//                 {
//                     count = 0;
//                     end = t;
//                     cout << "Constant of ~ " << val << "from t: " << start << " to " << end << endl;
//                     start = t;
//                 }
//             }
//         }
//     }
//     else {
//         cout << "Invalid Selection" << endl << endl;
//     }   
// }