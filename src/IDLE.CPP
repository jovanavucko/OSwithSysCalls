#include "idle.h"

Idle::Idle(): Thread(256,1) {};

void Idle::run() { while (1); };
