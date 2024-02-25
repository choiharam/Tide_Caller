#pragma once
#include "ofMain.h"

class Tile {
public:
    Tile();
    Tile(ofTexture tx, vector<string> edges, int imgIndex, float limit=-1);
    void analyze(vector<Tile>& tiles);
    Tile rotate(int n);
    Tile flipLR();

    int imgIndex;
    float limit;
    ofTexture tex;
    vector<string> edges;
    vector<int> up, right, down, left;

};