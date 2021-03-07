//
// Created by Luecx on 06.03.2021.
//
#include "distance.h"

Distance distance(Genome &g1, Genome &g2, DistanceConfig &config) {

    // find the maximum innovation number of both genomes
    Innovation g1_max_inno = 0;
    if (g1.connections.size()) g1_max_inno = g1.connections.rbegin()->innovationNumber;
    Innovation g2_max_inno = 0;
    if (g2.connections.size()) g2_max_inno = g2.connections.rbegin()->innovationNumber;

    // to correctly count excess and disjoint genes, g1 shall
    // have more the excess genes (= higher max innovation number)
    if(g1_max_inno < g2_max_inno){
        Genome temp = g1;
        g1 = g2;
        g2 = temp;
    }

    // indices of the two connection to compare
    int     index_g1    = 0;
    int     index_g2    = 0;

    // counting the excess, disjoint and similar genes together with their weight difference
    int     disjoint    = 0;
    int     excess      = 0;
    int     similar     = 0;
    Weight  weight_diff = 0;

    // use iterators for the sets
    auto it1 = g1.connections.begin();
    auto it2 = g2.connections.begin();

    // stop if we reached the end of one of the genomes
    while(index_g1 < g1.connections.size() && index_g2 < g2.connections.size()){

        int in1 = it1->innovationNumber;
        int in2 = it2->innovationNumber;

        if(in1 == in2){
            // if the two innovations are the same, its a similar gene
            similar     ++;
            weight_diff += std::abs(it1->weight - it2->weight);

            // we can increment the connection we look at for both genomes
            index_g1++;
            index_g2++;
            std::advance(it1, 1);
            std::advance(it2, 1);
        }else if(in1 > in2){
            //disjoint gene of b
            disjoint ++;

            // we will look at the gene of node 1 once more to see if it matches another gene in genome 2
            index_g2++;
            std::advance(it2, 1);
        }else{
            //disjoint gene of a
            disjoint ++;

            // we will look at the gene of node 2 once more to see if it matches another gene in genome 1
            index_g1 ++;
            std::advance(it1, 1);
        }
    }

    // since we assume that g1 will contain the excess genes, we can compute it after we have reached the end of g2
    weight_diff /= std::max(1, similar);
    excess       = g1.connections.size() - index_g1;

    double N     = std::max(g1.connections.size(), g2.connections.size());
    if(N < 20) N = 1;

    return  config.C1 * disjoint    / N +
            config.C2 * excess      / N +
            config.C3 * weight_diff;
}
