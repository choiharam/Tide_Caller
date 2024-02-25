#include "tile.h"

bool compareEdge(string a, string b){
    string copy(b);
    std::reverse(copy.begin(), copy.end());
    return a==copy;
}

Tile::Tile() {
    
}
//--------------------------------------------------------------
Tile::Tile(ofTexture tx, vector<string> edges, int imgIndex, float limit) {
    this->tex = tx;
    this->edges = edges;
    this->imgIndex = imgIndex;
    this->limit = (limit==-1)?1000:limit;
}

//--------------------------------------------------------------
void Tile::analyze(vector<Tile>& tiles) {
    for(int i=0; i<tiles.size(); ++i){
        Tile t = tiles[i];
        // UP
        if(compareEdge(t.edges[2], this->edges[0])){
            this->up.push_back(i);
        }
        // RIGHT
        if(compareEdge(t.edges[3], this->edges[1])){
            this->right.push_back(i);
        }
        // DOWN
        if(compareEdge(t.edges[0], this->edges[2])){
            this->down.push_back(i);
        }
        // LEFT
        if(compareEdge(t.edges[1], this->edges[3])){
            this->left.push_back(i);
        }
    }
}
//--------------------------------------------------------------
Tile Tile::rotate(int n) {
    ofPixels px;
    this->tex.readToPixels(px);
    px.rotate90(n);
    ofTexture newImage;
    newImage.loadData(px);

    // rotate edges
    int size = this->edges.size();
    vector<string> newEdges(size);
    for(int i=0; i<size; ++i){
        newEdges[i] = this->edges[(i - n + size) % size];
    }
    return Tile(newImage, newEdges, imgIndex, this->limit);
    
}
//--------------------------------------------------------------
Tile Tile::flipLR() {
    ofPixels px;
    this->tex.readToPixels(px);
    px.mirror(false, true);
    ofTexture newImage;
    newImage.loadData(px);
    
    // flip edges
    vector<string> newEdges = this->edges;
    std::swap(newEdges[1], newEdges[3]);
    return Tile(newImage, newEdges, imgIndex, this->limit);
}