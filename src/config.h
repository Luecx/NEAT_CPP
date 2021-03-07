//
// Created by Luecx on 04.03.2021.
//

#ifndef NEAT_CONFIG_H
#define NEAT_CONFIG_H

#include "types.h"

struct DistanceConfig{
    FloatSetting C1;            // weight for disjoint connections count
    FloatSetting C2;            // weight for excess connections count
    FloatSetting C3;            // weight for average weight difference
};

enum MutationConfigEntryType{
    FOR_EACH_GENE,
    FOR_EACH_GENOME
};

struct MutationConfigEntry{
    FloatSetting                    probability = 0;
    MutationConfigEntryType         type = FOR_EACH_GENE;
};

struct MutationConfigStrengthEntry : MutationConfigEntry{
    FloatSetting                    strength = 0;
};

struct MutationConfig{
    MutationConfigStrengthEntry weightShift{};              // applies *= rand(1-strength,1+strength)
    MutationConfigStrengthEntry weightChange{};             // applies  = rand(-1,1) * strength
    MutationConfigEntry         connectionStateToggle{};    // changes enabled/disabled state
    MutationConfigStrengthEntry linkMutation{};             // creates a new link with the given probability and
                                                            // the new weight will be rand(-1, 1) * strength
    MutationConfigEntry         nodeMutation{};             // creates a new node with the given probability
};


#endif //NEAT_CONFIG_H
