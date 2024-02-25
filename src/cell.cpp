#include "cell.h"

Cell::Cell() {

}
Cell::Cell(vector<int> value) {
    options = value;
}
Cell::Cell(int value) {
    for(int i=0; i<value; i++) {
        options.push_back(i);
    }
}