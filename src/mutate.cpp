//
// Created by Luecx on 06.03.2021.
//
#include <mmcobj.h>
#include "mutate.h"

void mutateLink                 (System &system, Genome &g, MutationConfigStrengthEntry &config);
void mutateNode                 (System &system, Genome &g, MutationConfigEntry &config);
void mutateWeightShift          (System &system, Genome &g, MutationConfigStrengthEntry &config);
void mutateWeightChange         (System &system, Genome &g, MutationConfigStrengthEntry &config);
void mutateConnectionStateToggle(System &system, Genome &g, MutationConfigEntry &config);

void mutate(System &system, Genome &g, MutationConfig &config) {
    mutateLink                  (system, g, config.linkMutation);
    mutateNode                  (system, g, config.nodeMutation);
    mutateWeightShift           (system, g, config.weightShift);
    mutateWeightChange          (system, g, config.weightChange);
    mutateConnectionStateToggle (system, g, config.connectionStateToggle);
}

void mutateLink                 (System &system, Genome &g, MutationConfigStrengthEntry& config){
    if (config.type == FOR_EACH_GENE) {
        // loop over all node genes and try to find a connection to a previous node
    }else if(config.type == FOR_EACH_GENOME){
        // probability probing to find a single link to create
        if(PROB(config.probability)){

            for(int attempts = 0; attempts < 20; attempts++){
                int index1 = std::min(RNG_RANGE(0,g.nodes.size()),(float)g.nodes.size());
                int index2 = std::min(RNG_RANGE(0,g.nodes.size()),(float)g.nodes.size());

                // dont create a link between the same nodes
                if(index1 == index2) continue;

                auto it1 = g.nodes.begin();
                std::advance(it1, index1);
                auto it2 = g.nodes.begin();
                std::advance(it2, index2);

                Coordinate x1 = it1->coordinate;
                Coordinate x2 = it2->coordinate;

                // skip if the coordinate is the same
                if(x1 == x2) continue;

                Innovation i1 = it1->innovationNumber;
                Innovation i2 = it2->innovationNumber;

                if(x1 > x2){
                    // swap the innovation numbers
                    Innovation h = i1;
                    i1 = i2;
                    i2 = h;
                }

                // get the connection between i1 and i2
                ConnectionGene con = system.newConnectionGene(i1,i2);

                // check if the connection already exists
                if(g.connections.find(con) != g.connections.end()) continue;

                con.weight         = RNG_RANGE(-config.strength, config.strength);
                g.connections.insert(con);
                break;
            }

        }
    }
}
void mutateNode                 (System &system, Genome &g, MutationConfigEntry& config){
    // if the probability accounts for each gene, we need to iterate through all genes
    if (config.type == FOR_EACH_GENE) {
        // loop over all genes
        int count = g.connections.size();
        int idx   = 0;
        for (auto it = g.connections.begin(); idx < count; idx++) {
            // probability probing
            if (PROB(config.probability)) {
                // replace the connection
                Innovation i1 = it->innovation1;
                Innovation i2 = it->innovation2;

                // retrieve the position of the left and right side
                Coordinate p1 = g.nodes.find({i1})->coordinate;
                Coordinate p2 = g.nodes.find({i2})->coordinate;

                auto replaceIterator = system.globalConnectionGenes.find({it->innovationNumber});

                NodeGene ng;
                ConnectionGene c1,c2;

                // check if this connection has already been replaced before
                if(replaceIterator->nodeReplaceIndex == 0){
                    // if it has not been replaced, create a new node

                    // compute a new position. I choose the half +- a small margin
                    Coordinate pn = (p2 + p1) / 2 + RNG_RANGE(-(p2-p1)/10.0, (p2-p1)/10.0);

                    // create a new node
                    ng = NodeGene{system.newNodeGene(pn)};

                    // create new connections
                    c1 = system.newConnectionGene(i1, ng.innovationNumber);
                    c2 = system.newConnectionGene(ng.innovationNumber, i2);

                    // assign the replace index in the global system
                    replaceIterator->nodeReplaceIndex = ng.innovationNumber;
                }else{
                    // create an iterator of the existing gene
                    auto nodeIterator = system.globalNodeGenes.find({replaceIterator->nodeReplaceIndex});
                    // create a gene based of it
                    ng = NodeGene{nodeIterator->innovationNumber, nodeIterator->coordinate};
                    // copy the two existing connection genes
                    c1 = system.connectionGeneMap.find(HASH_CONNECTION(i1, ng.innovationNumber))->second;
                    c2 = system.connectionGeneMap.find(HASH_CONNECTION(i1, ng.innovationNumber))->second;
                }

                // make sure to keep the weights
                c1.weight = 1;
                c2.weight = it->weight;

                // remove old connection
                g.connections.erase(it++);

                // add new connections and node
                g.nodes.insert(ng);
                g.connections.insert(c1);
                g.connections.insert(c2);
            }else{
                it++;
            }
        }
    }else if(config.type == FOR_EACH_GENOME){
        // otherwise, we do only adjust a single weight if some probability probing is larger than config.probability.
        if(PROB(config.probability)){

            // we need to find the gene which we need to adjust. Just to make sure things do not break, std::min
            // it with the amount of connections since i do not trust my RNG_RANGE function :)
            int gene_index = std::min(RNG_RANGE(0,g.connections.size()), (float)g.connections.size());
            // create an iterator to access the elements
            auto it = g.connections.begin();
            // advance the iterator to the specified gene
            std::advance(it, gene_index);

            // replace the connection
            Innovation i1 = it->innovation1;
            Innovation i2 = it->innovation2;

            // retrieve the position of the left and right side
            Coordinate p1 = g.nodes.find({i1})->coordinate;
            Coordinate p2 = g.nodes.find({i2})->coordinate;

            auto replaceIterator = system.globalConnectionGenes.find({it->innovationNumber});

            ConnectionGene c1,c2;
            NodeGene ng;

            // check if this connection has already been replaced before
            if(replaceIterator->nodeReplaceIndex == 0){
                // if it has not been replaced, create a new node

                // compute a new position. I choose the half +- a small margin
                Coordinate pn = (p2 + p1) / 2 + RNG_RANGE(-(p2-p1)/10.0, (p2-p1)/10.0);

                // create a new node
                ng = system.newNodeGene(pn);

                // create new connections
                c1 = system.newConnectionGene(i1, ng.innovationNumber);
                c2 = system.newConnectionGene(ng.innovationNumber, i2);

                // assign the replace index in the global system
                replaceIterator->nodeReplaceIndex = ng.innovationNumber;
            }else{
                // create an iterator of the existing gen
                auto nodeIterator = system.globalNodeGenes.find({replaceIterator->nodeReplaceIndex});
                // create a gene based of it
                ng = NodeGene{nodeIterator->innovationNumber, nodeIterator->coordinate};
                // copy the two existing connection genes
                c1 = system.connectionGeneMap.find(HASH_CONNECTION(i1, ng.innovationNumber))->second;
                c2 = system.connectionGeneMap.find(HASH_CONNECTION(ng.innovationNumber, i2))->second;

            }

            // also adjust the weights
            c1.weight = 1;
            c2.weight = it->weight;

            // remove old connection
            g.connections.erase(it++);


            // add new connections and node
            g.nodes.insert(ng);
            g.connections.insert(c1);
            g.connections.insert(c2);
        }
    }
}
void mutateWeightShift          (System &system, Genome &g, MutationConfigStrengthEntry &config) {
    // if the probability accounts for each gene, we need to iterate through all genes
    if (config.type == FOR_EACH_GENE) {
        // loop over all genes
        for (auto it = g.connections.begin(); it != g.connections.end(); it++) {
            // probability probing
            if (PROB(config.probability)) {
                // adjusting the weight
                it->weight *= RNG_RANGE(-config.strength, config.strength);
            }
        }
    }else if(config.type == FOR_EACH_GENOME){
        // otherwise, we do only adjust a single weight if some probability probing is larger than config.probability.
        if(PROB(config.probability)){
            // we need to find the gene which we need to adjust. Just to make sure things do not break, std::min
            // it with the amount of connections since i do not trust my RNG_RANGE function :)
            int gene_index = std::min(RNG_RANGE(0,g.connections.size()), (float)g.connections.size());
            // create an iterator to access the elements
            auto it = g.connections.begin();
            // advance the iterator to the specified gene
            std::advance(it, gene_index);
            // adjust the weight
            it->weight *= RNG_RANGE(-config.strength, config.strength);
        }
    }
}
void mutateWeightChange         (System &system, Genome &g, MutationConfigStrengthEntry &config) {
    // if the probability accounts for each gene, we need to iterate through all genes
    if (config.type == FOR_EACH_GENE) {
        // loop over all genes
        for (auto it = g.connections.begin(); it != g.connections.end(); it++) {
            // probability probing
            if (PROB(config.probability)) {
                // adjusting the weight
                it->weight = RNG_RANGE(-config.strength, config.strength);
            }
        }
    }else if(config.type == FOR_EACH_GENOME){
        // otherwise, we do only adjust a single weight if some probability probing is larger than config.probability.
        if(PROB(config.probability)){
            // we need to find the gene which we need to adjust. Just to make sure things do not break, std::min
            // it with the amount of connections since i do not trust my RNG_RANGE function :)
            int gene_index = std::min(RNG_RANGE(0,g.connections.size()), (float)g.connections.size());
            // create an iterator to access the elements
            auto it = g.connections.begin();
            // advance the iterator to the specified gene
            std::advance(it, gene_index);
            // adjust the weight
            it->weight = RNG_RANGE(-config.strength, config.strength);
        }
    }
}
void mutateConnectionStateToggle(System &system, Genome &g, MutationConfigEntry &config) {
    // if the probability accounts for each gene, we need to iterate through all genes
    if (config.type == FOR_EACH_GENE) {
        // loop over all genes
        for (auto it = g.connections.begin(); it != g.connections.end(); it++) {
            // probability probing
            if (PROB(config.probability)) {
                // adjusting the weight
                it->enabled = !it->enabled;
            }
        }
    }else if(config.type == FOR_EACH_GENOME){
        // otherwise, we do only adjust a single weight if some probability probing is larger than config.probability.
        if(PROB(config.probability)){
            // we need to find the gene which we need to adjust. Just to make sure things do not break, std::min
            // it with the amount of connections since i do not trust my RNG_RANGE function :)
            int gene_index = std::min(RNG_RANGE(0,g.connections.size()), (float)g.connections.size());
            // create an iterator to access the elements
            auto it = g.connections.begin();
            // advance the iterator to the specified gene
            std::advance(it, gene_index);
            // adjust the weight
            it->enabled = !it->enabled;
        }
    }
}


