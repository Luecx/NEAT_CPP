#include <iostream>

#include "src/distance.h"
#include "src/System.h"
#include "src/crossover.h"
#include "src/mutate.h"
#include "src/calculator.h"

int main() {


    System system{1,2};

    Genome g1 = system.emptyGenome();
    std::cout << g1 << std::endl;
    MutationConfig config1{};
    config1.nodeMutation            = MutationConfigEntry          {0    , FOR_EACH_GENOME};
    config1.linkMutation            = MutationConfigStrengthEntry  {1    , FOR_EACH_GENOME, 1};
    config1.connectionStateToggle   = MutationConfigEntry          {0    , FOR_EACH_GENOME};
    config1.weightChange            = MutationConfigStrengthEntry  {0.25 , FOR_EACH_GENOME, 1};
    config1.weightShift             = MutationConfigStrengthEntry  {0.25 , FOR_EACH_GENOME, 0.2};


    for(int i = 0; i < 10; i++){
        mutate(system, g1, config1);
    }

    config1.nodeMutation            = MutationConfigEntry          {1    , FOR_EACH_GENOME};
    mutate(system, g1, config1);
    std::cout << g1 << std::endl;

    Calculator c{g1};
    Vector<1,Input> input{};
    input.values[0] = 1;
    auto out = c.compute<1,2>(input);
    for(int i = 0; i < 2; i++){
        std::cout << out.values[i] << std::endl;
    }

    return 0;
}
