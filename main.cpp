#include <iostream>
#include "Controller.h"

int main(int argc, char **argv) {
    auto *c = new Controller("C8Programs/PONG");
    c->start(false);
    // Once emulation has ended (window was closed), we will return here
    return 0;
}
