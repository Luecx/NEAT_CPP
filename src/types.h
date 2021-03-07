//
// Created by Luecx on 04.03.2021.
//

#ifndef NEAT_TYPES_H
#define NEAT_TYPES_H

#include <cstdint>

#define PROB(x) static_cast<float>(rand()) / RAND_MAX < x
#define RNG_RANGE(a,b) static_cast<float>(rand()) / RAND_MAX * (b - a) + a

typedef uint64_t Innovation;
typedef float    Coordinate;
typedef float    Weight;
typedef float    Output;
typedef float    Input;
typedef float    Distance;
typedef float    FloatSetting;
typedef float    IntSetting;

typedef Output (*Activation) (Output);

enum NodeType{
    INPUT_NODE,
    HIDDEN_NODE,
    OUTPUT_NODE,
    BIAS_NODE
};


#endif //NEAT_TYPES_H
