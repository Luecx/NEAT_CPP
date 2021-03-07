//
// Created by Luecx on 04.03.2021.
//

#ifndef NEAT_GENE_H
#define NEAT_GENE_H

#include <ostream>
#include "types.h"


struct Gene {

    // innovation number for the gene
    Innovation innovationNumber;

    bool operator==(const Gene &rhs) const;

    bool operator!=(const Gene &rhs) const;

    bool operator<(const Gene &rhs) const;

    bool operator>(const Gene &rhs) const;

    bool operator<=(const Gene &rhs) const;

    bool operator>=(const Gene &rhs) const;

    friend std::ostream &operator<<(std::ostream &os, const Gene &gene);

};

struct NodeGene : public Gene {

    // coordinate of the node in x direction. used for computation order
    mutable Coordinate coordinate = 0.5;

    // node type
    mutable NodeType type = HIDDEN_NODE;

    friend std::ostream &operator<<(std::ostream &os, const NodeGene &gene);

};

struct ConnectionGene : public Gene{

    // Weight for computation
    mutable Weight weight = 0;

    // Innovation number of previous node
    mutable Innovation innovation1 = 0;

    // Innovation number of second node
    mutable Innovation innovation2 = 0;

    // enabled or not for computations (can be disabled)
    mutable bool enabled = true;

    // if this connection is split into 2, this node will be used for the replacement
    mutable Innovation nodeReplaceIndex = 0;


    friend std::ostream &operator<<(std::ostream &os, const ConnectionGene &gene);
};



#endif //NEAT_GENE_H
