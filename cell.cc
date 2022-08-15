#include "cell.h"
#include "subject.h"
#include "observer.h"
#include "textdisplay.h"
#include <iostream>
#include <cstddef>

// see "cell.h"
Cell::Cell(size_t r, size_t c) : r{r}, c{c} {}

// see "cell.h"
void Cell::setLiving() {
    state = State::Alive;
}

// see "cell.h"
void Cell::reset() {
    numAliveNeighbours = 0;
}

// see "cell.h"
std::string Cell::getName() {
    std::string str = "(";
    std::string cc;
    cc += c;
    std::string rr;
    rr += r;
    str.append(rr);
    str.append(",");
    str.append(cc);
    str.append(")");
    return str;
}

// see "cell.h"
void Cell::broadcastIfAlive() {
    if (state == State::Alive) {
        notifyObservers();
    }
}

// cell.h
void Cell::notify( Subject & whoNotified ) {
    Info sub_info = whoNotified.getInfo();
    if (sub_info.state == State::Alive) {
        numAliveNeighbours++;
    }
}

// cell.h
void Cell::recalculate() {
    if (state == State::Dead) {
        if (numAliveNeighbours == 3) {
            state = State::Alive;
        }
    } else { // Alive
        if (numAliveNeighbours < 2 || numAliveNeighbours > 3) {
            state = State::Dead;
        }
    }
}

// cell.h
Info Cell::getInfo() const {
    Info info;
    info.state = state;
    info.row = r;
    info.col = c;
    return info;
}
