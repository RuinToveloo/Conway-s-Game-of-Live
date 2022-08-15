#include <cstddef>
#include <iostream>
#include <vector>
#include "textdisplay.h"
#include "observer.h"
#include "cell.h"
#include "info.h"
using namespace std;

// see "testdisplay.h"
TextDisplay::TextDisplay(size_t n){
    for (size_t i = 0; i < n; i++) {
        vector<char> hori;
        for (size_t j = 0; j < n; j++) {
            hori.emplace_back('_');
        }
        theDisplay.emplace_back(hori);
    }
}

// see "testdisplay.h"
void TextDisplay::notify( Subject & whoNotified ) {
    Info cell = whoNotified.getInfo();
    size_t i = cell.row;
    size_t j = cell.col; 
    if (cell.state == State::Dead) {
        theDisplay[i][j] = '_';
    } else {
        theDisplay[i][j] = 'X';
    }
}

// see "testdisplay.h"
std::string TextDisplay::getName() {
    return "TD";
}

// see "testdisplay.h"
std::ostream &operator<<(std::ostream &out, const TextDisplay &td) {
    for (auto hori: td.theDisplay) {
        for (auto verti : hori) {
            out << verti;
        }
        out << "" << endl;
    }
    return out;
}
