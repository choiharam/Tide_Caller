#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    ofSetVerticalSync(false);
    ofBackground(0);
    
    width = ofGetWidth();
    height = ofGetHeight();
    ofSetWindowShape(width*2, height*2);
    width = ofGetWidth();
    height = ofGetHeight();
    ofSetWindowShape(1920,1080);

    gcd = std::gcd(width, height);
    gcd /= 5;
    dim_w = width/gcd;
    dim_h = height/gcd;

    fbo.allocate(dim_w*gcd, dim_h*gcd, GL_RGBA);

    // load images
    tileImages.resize(IMG_NUM);
    for(int i=0; i<IMG_NUM; ++i){
        string path = "geo_water/" + ofToString(i) + ".png";
        ofLoadImage(tileImages[i], path);
    }
    
    tiles.resize(IMG_NUM);
    // create and label the tiles
    tiles[0] = Tile(tileImages[0], {"AAA", "AAA", "AAA", "AAA"}, 0);
    tiles[1] = Tile(tileImages[1], {"AAA", "AAA", "AAA", "AAA"}, 1, 0.01 * dim_w*dim_h);
    tiles[2] = Tile(tileImages[2], {"AAA", "AAA", "AAA", "AAA"}, 2, 0.02 * dim_w*dim_h);
    tiles[3] = Tile(tileImages[3], {"AAA", "AAA", "AAA", "AAA"}, 3, 0.02 * dim_w*dim_h);
    tiles[4] = Tile(tileImages[4], {"AAA", "AAA", "AAA", "AAA"}, 4, 0.02 * dim_w*dim_h);
    tiles[5] = Tile(tileImages[5], {"AAA", "AAA", "AAA", "AAA"}, 5, 0.02 * dim_w*dim_h);
    tiles[6] = Tile(tileImages[6], {"BBB", "BDA", "AAA", "ADB"}, 6);
    tiles[7] = Tile(tileImages[7], {"ADB", "AAA", "BDA", "BBB"}, 7);
    tiles[8] = Tile(tileImages[8], {"BBB", "BBB", "BDA", "ADB"}, 8);
    tiles[9] = Tile(tileImages[9], {"ADB", "BDA", "AAA", "AAA"}, 9);
    tiles[10] = Tile(tileImages[10], {"AAA", "AEC", "CCC", "CEA"}, 10);
    tiles[11] = Tile(tileImages[11], {"CCC", "CEE", "AAA", "EEC"}, 11);
    tiles[12] = Tile(tileImages[12], {"CEA", "AEC", "CCC", "CCC"}, 12);
    tiles[13] = Tile(tileImages[13], {"CCC", "CEE", "AEC", "CCC"}, 13);
    tiles[14] = Tile(tileImages[14], {"CEA", "AAA", "AAA", "EEC"}, 14);
    tiles[15] = Tile(tileImages[15], {"AAA", "AAA", "AEC", "CEA"}, 15);
    tiles[16] = Tile(tileImages[16], {"CCC", "CCC", "CEA", "EEC"}, 16);
    tiles[17] = Tile(tileImages[17], {"AEC", "CEE", "AAA", "AAA"}, 17);
    tiles[18] = Tile(tileImages[18], {"CCC", "CCC", "CCC", "CCC"}, 18);
    tiles[19] = Tile(tileImages[19], {"CCC", "CCC", "CCC", "CCC"}, 19, 0.02 * dim_w*dim_h);
    tiles[20] = Tile(tileImages[20], {"CCC", "CCC", "CCC", "CCC"}, 20, 0.05 * dim_w*dim_h);
    tiles[21] = Tile(tileImages[21], {"CCC", "CCC", "CCC", "CCC"}, 21, 0.05 * dim_w*dim_h);
    tiles[22] = Tile(tileImages[22], {"BBB", "BBB", "BBB", "BBB"}, 22);
    tiles[23] = Tile(tileImages[23], {"BBB", "BBB", "BBB", "BBB"}, 23, 0.1 * float(dim_w)*float(dim_h));
    tiles[24] = Tile(tileImages[24], {"AAA", "AAA", "AAA", "AAA"}, 24, 0);

    // tide caller
    tiles[25] = Tile(tileImages[25], {"BBB", "GGG", "HHH", "BBB"}, 25, 0); // 0
    tiles[26] = Tile(tileImages[26], {"BBB", "BBB", "BBB", "GGG"}, 26, 0); // 1
    tiles[27] = Tile(tileImages[27], {"BBB", "III", "BBB", "BBB"}, 27, 0); // 2
    tiles[28] = Tile(tileImages[28], {"HHH", "BBB", "JJJ", "III"}, 28, 0); // 3
    tiles[29] = Tile(tileImages[29], {"JJJ", "KKK", "LLL", "BBB"}, 29, 0); // 4
    tiles[30] = Tile(tileImages[30], {"BBB", "BBB", "MMM", "KKK"}, 30, 0); // 5
    tiles[31] = Tile(tileImages[31], {"LLL", "NNN", "OOO", "BBB"}, 31, 0); // 6
    tiles[32] = Tile(tileImages[32], {"MMM", "BBB", "PPP", "NNN"}, 32, 0); // 7
    tiles[33] = Tile(tileImages[33], {"OOO", "QQQ", "BBB", "BBB"}, 33, 0); // 8
    tiles[34] = Tile(tileImages[34], {"PPP", "BBB", "BBB", "QQQ"}, 34, 0); // 9

    vector<int> filpLR = {1,2,3,4,5,6,7,8,9,10,11,12,14,15,16,17, 24};
    vector<int> rotate = {8};

    // flip and rotate tiles
    for(int i=0; i<IMG_NUM; ++i){
        if(std::find(filpLR.begin(), filpLR.end(), i) != filpLR.end()){
            tiles.push_back(tiles[i].flipLR());
        }
        // if(std::find(rotate.begin(), rotate.end(), i) != rotate.end()){
        //     tiles.push_back(tiles[i].rotate(1));
        // }
    }

    // Generate the adjacency rules based on edges
    for(int i=0; i<tiles.size(); ++i){
        tiles[i].analyze(tiles);
    }

    // initialize limitCounter
    for(int i=0; i<IMG_NUM; ++i){
        limitCounter[i] = 0;
    }


    // inizialize the grid
    grid.resize(dim_w*dim_h);

    // set the initial bias
    biasedIdxs.clear();
    startOver();
}

//--------------------------------------------------------------
void ofApp::update(){
    // make a list of references to the original array
    vector<Cell*> gridCopy(grid.size());
    for(int i=0; i<grid.size(); ++i){
        gridCopy[i] = &grid[i];
    }

    // remove any collapsed cell
    for(int i = gridCopy.size() - 1; i >= 0; --i){
        if(gridCopy[i]->collapsed){
            gridCopy.erase(gridCopy.begin() + i);
        }
    }

    // if the grid is empty, then it is finished, start counting the clock
    if(!bisFinished){
        if(gridCopy.empty()){ // is finished
            bisFinished = true;
            startTime = ofGetElapsedTimef();
            return;
        }
    }else{
        if(!bhasToRollBack){ // then count the clock
        cout << "counting the clock" << endl;
            float t = ofGetElapsedTimef();
            if(t - startTime > delay){
                bhasToRollBack = true;
                prevTime = t;
            }
        }
        return;
    }

    // find the cell with least entropy

    // sort by entropy
    std::sort(gridCopy.begin(), gridCopy.end(), [](const Cell* a, const Cell* b){
        return a->options.size() < b->options.size();
    });

    // get the cell with least entropy
    int leastSize = gridCopy[0]->options.size();
    int stopIndex = 0;
    for(int i = 0; i < gridCopy.size(); ++i){
        if(gridCopy[i]->options.size() > leastSize){
            stopIndex = i;
            break;
        }
    }
    if (stopIndex > 0){
        gridCopy.resize(stopIndex); // Keep only the cells with the least entropy
    }

    // collapse a cell
    int idx = ofRandom(gridCopy.size());
    if(gridCopy[idx]->options.empty()){
        startOver();
        return;
    }
    gridCopy[idx]->collapsed = true;

    // restrict the count of each tile
    vector<int>& opts = gridCopy[idx]->options;
    for(int i=0; i<opts.size(); ++i){
        if(limitCounter[tiles[opts[i]].imgIndex] >= tiles[opts[i]].limit){
            opts.erase(opts.begin() + i);
            i--;
        }
    }    
    int pick;
    // if possible, select the one
    if(std::find(opts.begin(), opts.end(), 24) != opts.end()){
        pick = 24;
    }else{
        // pick a random tile from available options
        pick = gridCopy[idx]->options[ofRandom(gridCopy[idx]->options.size())];
    }

    gridCopy[idx]->options = {pick};
    
    // update the determined map
    PreMap pm;
    pm.gridIdx = idx;
    pm.tileIdx = pick;
    premap.push_back(pm);
    
    // count Collapsed
    limitCounter[tiles[pick].imgIndex] += 1;

    // update the grid (re-calculate the entropy)
    vector<Cell> newGrid(dim_w*dim_h);
    for(int j=0; j<dim_h; ++j){
        for(int i=0; i<dim_w; ++i){
            int idx = i + j*dim_w;
            // check if the cell is biased or collapsed
            // if so, keep the cell as is
            auto isBiased = std::find(biasedIdxs.begin(), biasedIdxs.end(), idx);
            if(grid[idx].collapsed || isBiased != biasedIdxs.end()){
                    newGrid[idx] = grid[idx];
            }else{
                vector<int> options;
                for(int i=0; i<tiles.size(); ++i) options.push_back(i);
                // look up
                if(j>0){
                    Cell up = grid[i + (j-1)*dim_w];
                    vector<int> validOptions;
                    for(int option : up.options){
                        validOptions.insert(validOptions.end(), tiles[option].down.begin(), tiles[option].down.end());
                    }
                    checkValid(options, validOptions);
                }
                // look right
                if(i<dim_w-1){
                    Cell right = grid[i+1 + j*dim_w];
                    vector<int> validOptions;
                    for(int option : right.options){
                        validOptions.insert(validOptions.end(), tiles[option].left.begin(), tiles[option].left.end());
                    }
                    checkValid(options, validOptions);
                }
                // look down
                if(j<dim_h-1){
                    Cell down = grid[i + (j+1)*dim_w];
                    vector<int> validOptions;
                    for(int option : down.options){
                        validOptions.insert(validOptions.end(), tiles[option].up.begin(), tiles[option].up.end());
                    }
                    checkValid(options, validOptions);
                }
                // look left
                if(i>0){
                    Cell left = grid[i-1 + j*dim_w];
                    vector<int> validOptions;
                    for(int option : left.options){
                        validOptions.insert(validOptions.end(), tiles[option].right.begin(), tiles[option].right.end());
                    }
                    checkValid(options, validOptions);
                }
                newGrid[idx] = Cell(options);
            }
        }
    }
    grid = newGrid;

}

//--------------------------------------------------------------
void ofApp::draw(){
    fbo.begin();
    
        if(!bhasToRollBack){
            ofClear(0);
            //draw the grid
            float cellSize = gcd;
            for(int j=0; j<dim_h; ++j){
                for(int i=0; i<dim_w; ++i){
                    int idx = i + j*dim_w;
                    Cell cell = grid[idx];
                    if(cell.collapsed){
                        int whichTile = cell.options[0];
                        tiles[whichTile].tex.draw(i*cellSize, j*cellSize, cellSize, cellSize);
                        if(blabel)
                            drawLable(tiles[whichTile], i*cellSize, j*cellSize, i, j);
                    }else{
                        ofPushStyle();
                        ofSetColor(0,140,179);
                        ofNoFill();
                        ofDrawRectangle(i*cellSize, j*cellSize, cellSize, cellSize);
                        ofPopStyle();
                    }

                    // draw the grid
                    if(!bgrid) continue;
                    ofPushStyle();
                    ofSetColor(255,0,0, 80);
                    ofNoFill();
                    ofDrawRectangle(i*cellSize, j*cellSize, cellSize, cellSize);
                    ofPopStyle();
                }
            }
        }else{
            int x = premap.back().gridIdx % dim_w;
            int y = premap.back().gridIdx / dim_w;
            ofPushStyle();
            ofSetColor(0);
            ofFill();
            ofDrawRectangle(x*gcd, y*gcd, gcd, gcd);
            ofSetColor(0,140,179);
            ofNoFill();
            ofDrawRectangle(x*gcd, y*gcd, gcd, gcd);
            ofPopStyle();
            premap.pop_back();
            if(premap.empty()){
                startOver();
            }
        }

    fbo.end();

    if(bglobalFlipLR){
        fbo.draw(width,0,-width,height);
    }else{
        fbo.draw(0,0,width,height);
    }

    if(bdebug){
        std::stringstream ss;
        ss << "fps: " << ofGetFrameRate() << '\n';
        ss << "res: " << width <<","<<height<<'\n';
        ss << "dim: " << dim_w << "," << dim_h << '\n';
        ss << "gcd: " << gcd << '\n';
        ss << "flip: " << bglobalFlipLR << '\n';
        ss << "bisFinished: " << (bisFinished?"True":"False") << '\n';
        ss << "bhasToRollBack: " << (bhasToRollBack?"True":"False") << '\n';
        ss << "rollBackIdx: " << rollBackIdx << '\n';
        ss << "premap.size(): " << premap.size() << '\n';
        ss << "time: " << ofGetElapsedTimef() << '\n';
        ss << "starttime: " << startTime << '\n';
        ss << "Tdiff: " << ofGetElapsedTimef()-startTime << '\n';
        ofDrawBitmapStringHighlight(ss.str(), gcd*dim_w+10, 30);

        std::stringstream sss;
        bool nl = false;
        for(int i=0; i<premap.size(); ++i){
            sss << premap[i].gridIdx << ", ";
            if(i%20==0){
                sss << '\n';
            }
        }
        ofDrawBitmapStringHighlight(sss.str(), gcd*dim_w+200, 20);
    }
    

    // std::stringstream sss;
    // for(int i=0; i<IMG_NUM; ++i){
    //     sss << i << '\t' << limitCounter[i] << '\t' << tiles[i].limit << '\n';
    // }
    // ofDrawBitmapStringHighlight(sss.str(), width+10, 30);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == 'r'){
        startOver();
    }
    if(key == 'g'){
        bgrid = !bgrid;
    }
    if(key == 'l'){
        blabel = !blabel;
    }
    if(key == 'd'){
        bdebug = !bdebug;
    }
}
//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
}
//--------------------------------------------------------------
void ofApp::startOver(){
    cout << "Starting Over" << endl;
    bisFinished = false;
    bhasToRollBack = false;
    rollBackIdx = 0;
    premap.clear();
    bglobalFlipLR = (ofRandomuf()>0.5?true:false);
    for(int i=0; i<dim_w*dim_h; ++i){
        grid[i] = Cell(tiles.size());
    }
    for(int i=0; i<tiles.size(); ++i){
        limitCounter[i] = 0;
    }
    setInitialBias();

}
//--------------------------------------------------------------
void ofApp::checkValid(vector<int>& arr, vector<int>& valid){
    for(int i=arr.size()-1; i>=0; --i){
        if(std::find(valid.begin(), valid.end(), arr[i]) == valid.end()){
            arr.erase(arr.begin()+i);
        }
    }
}
//--------------------------------------------------------------
void ofApp::drawLable(Tile _tile, float _x, float _y, int _i, int _j){
    int idx = _tile.imgIndex;
    stringstream s;
    s << idx << '\n';
    s << _i << "," << _j;
    ofDrawBitmapStringHighlight(s.str(), _x+10, _y+10);

}
//--------------------------------------------------------------
void ofApp::setInitialBias(){
    biasedIdxs.clear();

/*
    -------------- Story Teller bias
*/
    // set the center cell to 24
    int x_idx = dim_w * 0.5 - 1;
    x_idx -= ofRandomf() * 0.5 * (dim_w-x_idx); // randomize the center horizontally
    int idx = x_idx + (dim_h-2) * 0.5 * dim_w + dim_w*0.5;
    idx += (ofRandomuf()>0.5?dim_w:-dim_w); // randomize the center vertically by 1 cell
    // grid[idx].collapsed = false;
    grid[idx].options = {24};
    
    // find 7 without 7
    int idx7=-1;
    for(int i=0; i<tiles.size(); ++i){
        if(i!=7 && tiles[i].imgIndex==7){
            idx7 = i;
            break;
        }
    }
    // set the cell to the right of the center cell to 7 (sand and water)
    grid[idx+1].collapsed = false;
    grid[idx+1].options = {idx7};

    biasedIdxs.push_back(idx);
    biasedIdxs.push_back(idx+1);


/*
    -------------- Tide Caller bias
*/
    vector<int> occupationIdx = { // pre-occupied tiles by the tide caller
        -dim_w+1, // 0
        -dim_w+2, // 1
        0, // 2
        1, // 3
        dim_w+1, // 4
        dim_w+2, // 5
        2*dim_w+1, // 6
        2*dim_w+2, // 7
        3*dim_w+1, // 8
        3*dim_w+2, // 9
    };
    // set the hand of the tide caller one next to the story teller
    int idx27 = idx + 2;
    // if the story teller makes the ocean too big, then put the tide caller at the center of the ocean
    if(x_idx< dim_w * 0.6){
        idx27 = idx + float(dim_w-x_idx)*0.5 - 1;
    }
    if(ofRandomuf()>0.5){
        idx27 += (ofRandomuf()>0.5?dim_w:-dim_w); // randomize the center vertically by 1 cell
    }
    grid[idx27].collapsed = false;
    grid[idx27].options = {27};
    biasedIdxs.push_back(idx27);
    for(int i=0; i<occupationIdx.size(); ++i){
        occupationIdx[i] += idx27;
    }

/*
    -------------- Ocean bias
*/
    // set all the cell to the right of the center and cell 7 to 22 or 23
    for(int i=x_idx+2; i<dim_w; ++i){
        for(int j=0; j<dim_h; ++j){
            int idx = i + j*dim_w;
            if(grid[idx].collapsed) continue;
            if(std::find(occupationIdx.begin(), occupationIdx.end(), idx) != occupationIdx.end()) continue;
            grid[idx].options = (ofRandomuf()>0.01?vector<int>{22,22}:vector<int>{23,22});
            limitCounterUpdate(grid[idx].options);
            // count
            limitCounter[grid[idx].options[0]] += 1;
        }
    }
}
//--------------------------------------------------------------
void ofApp::limitCounterUpdate(vector<int>& opts){
    for(int i=0; i<opts.size(); ++i){
        if(limitCounter[tiles[opts[i]].imgIndex] >= tiles[opts[i]].limit){
            opts.erase(opts.begin() + i);
            i--;
        }
    }
}