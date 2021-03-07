//
// Created by Luecx on 06.03.2021.
//
#include <iostream>
#include "crossover.h"

Genome crossover(Genome &g1, Genome &g2) {

    // creates a new genome
    // g1 should have the higher score
    //  - take all the genes of a
    //  - if there is a core.genome in a that is also in b, choose randomly
    //  - do not take disjoint genes of b
    //  - take excess genes of a if they exist

    Genome res = g1.emptyCopy();

    auto it1     = g1.connections.begin();
    auto it2     = g2.connections.begin();

    while(it1 != g1.connections.end() && it2 != g2.connections.end()){
        Innovation in1 = it1->innovationNumber;
        Innovation in2 = it2->innovationNumber;

        if(in1 == in2){
            // similar gene
            if(rand() > RAND_MAX/2){
                // use the connection from genome 1
                res.connections.insert(*it1);
                // dont forget to also add the nodes
                NodeGene n1_temp{it1->innovation1};
                NodeGene n2_temp{it1->innovation2};
                res.nodes.insert(*g1.nodes.find(n1_temp));
                res.nodes.insert(*g1.nodes.find(n2_temp));
            }else{
                // use the connection from genome 2
                res.connections.insert(*it2);
                // dont forget to also add the nodes
                NodeGene n1_temp{it1->innovation1};
                NodeGene n2_temp{it1->innovation2};
                res.nodes.insert(*g2.nodes.find(n1_temp));
                res.nodes.insert(*g2.nodes.find(n2_temp));

            }
            it1 ++;
            it2 ++;
        }else if(in1 > in2){
            //disjoint gene of b -> skip
            it2 ++;
        }else{
            //disjoint gene of a -> use
            res.connections.insert(*it1);

            // dont forget to also add the nodes
            NodeGene n1_temp{it1->innovation1};
            NodeGene n2_temp{it1->innovation2};
            res.nodes.insert(*g1.nodes.find(n1_temp));
            res.nodes.insert(*g1.nodes.find(n2_temp));
            it1 ++;
        }
    }
    // all excess genes of A
    while(it1 != g1.connections.end()){
        res.connections.insert(*it1);
        it1++;
    }

    return res;

}
