//
// Created by Luecx on 04.03.2021.
//

#ifndef NEAT_GENOME_H
#define NEAT_GENOME_H


#include <set>
#include <ostream>
#include "Gene.h"

struct Genome {

    // all node genes in this genome
    std::set<NodeGene> nodes{};

    // all connection genes in this genome
    std::set<ConnectionGene> connections{};

    friend std::ostream &operator<<(std::ostream &os, const Genome &genome);

    Genome emptyCopy(){
        Genome g{};
        auto it = nodes.begin();
        while(it->type != HIDDEN_NODE && it != nodes.end()){
            g.nodes.insert(*it);
            it++;
        }
        return g;
    }
};


#endif //NEAT_GENOME_H
