#include "grid.h"
#include "cell.h"
#include "subject.h"
#include <iostream>
#include <vector>
#include <string>
using namespace std;

// see "grid.h"
Grid::~Grid() {
    if (occ == 0) { /* do nothing since not start yet*/ } else {
        delete td;
    }
}

// see "grid.h"
void Grid::printCellNeighbours() {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            std::cout << "(" << i << "," << j << ")" << ": ";
            for (int x = 0; x < size; x++) {
                for (int y = 0; y < size; y++) {
                    if ((!(x == i && y == j)) && 
                        ((-1 <= x - i) && (x - i) <= 1) &&
                        ((-1 <= y - j) && (y - j) <= 1)) {
                            std::cout << "(" << x << "," << y << ") ";
                        }
                }
            }
            std::cout << "TD " << std::endl;
        }
    }
}

// see "grid.h"
void Grid::init( int n ) {
    // invalid n
    if (n < 1) {
        throw std::invalid_argument( "ERROR: grid size < 1" );
    }
    // valid n
    if (occ != 0) { // has a grid already 
        // free the previous one
        for (auto n : theGrid) {
            n.clear();
        }
        theGrid.clear();
        delete td;
    }

    // store the information
    occ++;
    size = n;
    td = new TextDisplay{(size_t)n};

    // create a new grid
    for (int i = 0; i < size; i++) {
        vector<Cell> hori;
        for (int j = 0; j < size; j++) {
            Cell cell = Cell(i, j);
            hori.emplace_back(cell);
        }
        theGrid.emplace_back(hori);
    }
    // attach to the textdisplayer first
    theGrid[0][0].attach(td);

    if (n > 1) {
        // corners
        theGrid[0][0].attach(&theGrid[1][0]);
        theGrid[0][0].attach(&theGrid[0][1]);
        theGrid[0][0].attach(&theGrid[1][1]);
        theGrid[n-1][0].attach(&theGrid[n-2][0]);
        theGrid[n-1][0].attach(&theGrid[n-2][1]);
        theGrid[n-1][0].attach(&theGrid[n-1][1]);
        theGrid[n-1][n-1].attach(&theGrid[n-2][n-1]);
        theGrid[n-1][n-1].attach(&theGrid[n-1][n-2]);
        theGrid[n-1][n-1].attach(&theGrid[n-2][n-2]);
        theGrid[0][n-1].attach(&theGrid[0][n-2]);
        theGrid[0][n-1].attach(&theGrid[1][n-1]);
        theGrid[0][n-1].attach(&theGrid[1][n-2]);
        // boundaries
        for (int i = 1; i < n - 1; i++) {
            // top
            theGrid[0][i].attach(&theGrid[0][i-1]);
            theGrid[0][i].attach(&theGrid[0][i+1]);
            theGrid[0][i].attach(&theGrid[1][i-1]);
            theGrid[0][i].attach(&theGrid[1][i]);
            theGrid[0][i].attach(&theGrid[1][i+1]);
            // left
            theGrid[i][0].attach(&theGrid[i-1][0]);
            theGrid[i][0].attach(&theGrid[i+1][0]);
            theGrid[i][0].attach(&theGrid[i+1][1]);
            theGrid[i][0].attach(&theGrid[i][1]);
            theGrid[i][0].attach(&theGrid[i-1][1]);
            // right
            theGrid[i][n-1].attach(&theGrid[i-1][n-1]);
            theGrid[i][n-1].attach(&theGrid[i+1][n-1]);
            theGrid[i][n-1].attach(&theGrid[i-1][n-2]);
            theGrid[i][n-1].attach(&theGrid[i][n-2]);
            theGrid[i][n-1].attach(&theGrid[i+1][n-2]);
            // bottom
            theGrid[n-1][i].attach(&theGrid[n-1][i-1]);
            theGrid[n-1][i].attach(&theGrid[n-1][i+1]);
            theGrid[n-1][i].attach(&theGrid[n-2][i-1]);
            theGrid[n-1][i].attach(&theGrid[n-2][i]);
            theGrid[n-1][i].attach(&theGrid[n-2][i+1]);
        }
        // body
        for (int i = 1; i < n - 1; i++) {
            for (int j = 1; j < n - 1; j++) {
                theGrid[i][j].attach(&theGrid[i-1][j-1]);
                theGrid[i][j].attach(&theGrid[i-1][j]);
                theGrid[i][j].attach(&theGrid[i-1][j+1]);
                theGrid[i][j].attach(&theGrid[i][j-1]);
                theGrid[i][j].attach(&theGrid[i][j+1]);
                theGrid[i][j].attach(&theGrid[i+1][j-1]);
                theGrid[i][j].attach(&theGrid[i+1][j]);
                theGrid[i][j].attach(&theGrid[i+1][j+1]);
            }
        }
    }


    
}

// see "grid.h"
void Grid::turnOn( int r, int c ) {
    if (r < 0 || r >= size || c < 0 || c >= size) {
        string mess = "ERROR: coordinates (" + to_string(r) + "," + to_string(c) + 
            ") are not in the range 0 to " + to_string(size - 1);
            throw std::invalid_argument( mess );
    }

    theGrid[r][c].setLiving();
    td->notify(theGrid[r][c]);
} 

// see "grid.h"
void Grid::tick() {
    for (int x = 0; x < size; x++) {
        for (int y = 0; y < size; y++) {
            theGrid[x][y].reset();
        }
    }
    // notify the neighbours first
    for (int x = 0; x < size; x++) {
        for (int y = 0; y < size; y++) {
            theGrid[x][y].broadcastIfAlive();
        }
    }
    // recalculate the grid & notify
    for (int x = 0; x < size; x++) {
        for (int y = 0; y < size; y++) {
            theGrid[x][y].recalculate();
        }
    }
    for (int x = 0; x < size; x++) {
        for (int y = 0; y < size; y++) {
            td->notify(theGrid[x][y]);
       }
    }
}

// see "grid.h"
std::ostream & operator<<( std::ostream & out, const Grid & g ) {
    out << *(g.td);
    return out;
}

