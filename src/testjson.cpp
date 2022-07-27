#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include "nlohmann/json.hpp"
#include "twisted-edwards/Point.h"
using json = nlohmann::json;
using namespace std;

int main()
{
    NTL::Point p1,p2;
    json j;
    j["x"] = {};
    j["y"] = {};

    j["x"].emplace_back(p1.X);
    j["y"].emplace_back(p2.X);

    ofstream f;
    f.open("test.json");
    // f << j;
    cout << j.at("x");
    f.close();
}