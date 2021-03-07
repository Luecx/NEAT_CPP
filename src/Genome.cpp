//
// Created by Luecx on 04.03.2021.
//

#include "Genome.h"

std::ostream &operator<<(std::ostream &os, const Genome &genome) {

    os << "Genome:\n";
    for(auto it = genome.nodes.begin(); it != genome.nodes.end(); it++){
        os << *it << "\n";
    }
    for(auto it = genome.connections.begin(); it != genome.connections.end(); it++){
        os << *it << "\n";
    }
    return os;
}
