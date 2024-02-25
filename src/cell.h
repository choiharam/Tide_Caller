#pragma once
#include "ofMain.h"

class Cell {
public:
    Cell();
    Cell(vector<int> value);
    Cell(int value);


    int gridIdx;
    bool collapsed = false;
    vector<int> options;

};