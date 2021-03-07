//
// Created by Luecx on 05.03.2021.
//

#ifndef NEAT_SYSTEM_H
#define NEAT_SYSTEM_H

#include <unordered_map>
#include "Genome.h"
#include <iostream>

#define MAX_NODES (1 << 20)
#define HASH_CONNECTION(i1,i2) ((i1) * MAX_NODES + (i2))

class System{
private:
    int inputNodes;
    int outputNodes;
public:
    // all nodes across all genomes
    std::set<NodeGene> globalNodeGenes{};

    // all connections across all genomes
    std::set<ConnectionGene> globalConnectionGenes{};

    // all connections mapped with both nodes to the connection gene
    std::unordered_map<Innovation, ConnectionGene> connectionGeneMap{};

    // constructor to create the input nodes, output nodes and the bias node
    System(int I, int O) {
        inputNodes = I;
        outputNodes = O;
        for(int i = 0; i < I; i++){
            newNodeGene(0, INPUT_NODE);
        }
        for(int i = 0; i < O; i++){
            newNodeGene(1, OUTPUT_NODE);
        }
        newNodeGene(0, BIAS_NODE);
    }

    // creates a new node global node gene and assigns as an innovation number
    NodeGene        newNodeGene(Coordinate position, NodeType type=HIDDEN_NODE){
        NodeGene gene{globalNodeGenes.size(),position,type};
        globalNodeGenes.insert(gene);
        return gene;
    }

    // creates a new connection gene. if no global connection gene like this exists, it will be made global
    ConnectionGene  newConnectionGene(Innovation i1, Innovation i2){
        Innovation hash = HASH_CONNECTION(i1,i2);
        auto pos = connectionGeneMap.find(hash);
        if(pos == connectionGeneMap.end()){
            // no connection gene found -> create new one
            ConnectionGene c1{globalConnectionGenes.size()+1, 0, i1, i2};

            connectionGeneMap.insert({hash, c1});
            globalConnectionGenes.insert(c1);
            return c1;
        }
        // connection has been found -> reuse that innovation number
        return pos->second;
    }

    // creates am empty genome which will receive the input, output and bias nodes
    Genome          emptyGenome() const{
        Genome g{};
        auto it = globalNodeGenes.begin();
        for(int i = 0; i < inputNodes + outputNodes + 1; i++){
            g.nodes.insert(*it);
            it ++;
        }
        return g;
    }

};

#endif //NEAT_SYSTEM_H
