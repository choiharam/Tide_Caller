#pragma once

#include "ofMain.h"
#include "cell.h"
#include "tile.h"

#define IMG_NUM 35

enum StoryTellerPosition{
	ST_CENTER_LEFT=0,
	ST_CENTER_RIGHT,
	ST_TOP_LEFT,
	ST_TOP_RIGHT,
	ST_BOTTOM_LEFT,
	ST_BOTTOM_RIGHT
};

class ofApp : public ofBaseApp{

public:
	void setup();
	void update();
	void draw();
	void keyPressed(int key);
	void mousePressed(int x, int y, int button);
	void startOver();
	void checkValid(vector<int>& arr, vector<int>& valid);
	void drawLable(Tile _tile, float _x, float _y, int _i, int _j);
	void setInitialBias();
	void limitCounterUpdate(vector<int>& opts);

	// biased idxs -- mainly for the story teller
	vector<int> biasedIdxs;

	int width, height;
	int dim_w, dim_h;
	int gcd;
	vector<Tile> tiles;
	vector<ofTexture> tileImages;
	vector<Cell> grid;
	bool bgrid;
	bool blabel;
	bool bdebug;
	map<int, float> limitCounter;
	
	ofFbo fbo;
	bool bglobalFlipLR;

};
