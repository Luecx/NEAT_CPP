//
// Created by Luecx on 07.03.2021.
//
#include "activation.h"

Output ReLU(Output o) {
    return std::max(o,0.0f);
}
