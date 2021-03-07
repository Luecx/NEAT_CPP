//
// Created by Luecx on 07.03.2021.
//

#ifndef NEAT_CALCULATOR_H
#define NEAT_CALCULATOR_H

#include <vector>
#include <cstring>
#include <ostream>
#include <iomanip>
#include "Genome.h"
#include "activation.h"

template <int size, typename T>
struct Vector{
    T values[size];

    friend std::ostream &operator<<(std::ostream &os, const Vector<size,T> &vector) {
        for(int i = 0; i < size; i++){
            os << std::setw(14) << vector.values[i];
        }
        os << std::endl;
        return os;
    }
};

struct Connection{
    Coordinate  outputCoordinate;
    Weight      weight;
    int         inputIndex;
    int         outputIndex;

    bool operator<(const Connection &rhs) const {
        return outputCoordinate < rhs.outputCoordinate;
    }

    bool operator>(const Connection &rhs) const {
        return rhs < *this;
    }

    bool operator<=(const Connection &rhs) const {
        return !(rhs < *this);
    }

    bool operator>=(const Connection &rhs) const {
        return !(*this < rhs);
    }

};

struct Calculator{

private:
    // a list of outputs for each node indexed by an index (not by innovation number)
    Output* outputs = nullptr;
    // a list to check if an output has been fully computed (including activation)
    bool  * finished = nullptr;
    // a list of computational operations starting at the input going to the output nodes
    std::vector<Connection> connections{};

    int input_count  = 0;
    int output_count = 0;
    int node_count   = 0;

public:
    Calculator(Genome &g) {

        std::unordered_map<Innovation, int> innovationIndexMap{};

        int nodeIndex = 0;
        for(auto it = g.nodes.begin(); it != g.nodes.end(); it++){
            if(it->type == INPUT_NODE){
                input_count ++;
            }
            if(it->type == OUTPUT_NODE){
                output_count ++;
            }
            innovationIndexMap.insert({it->innovationNumber, nodeIndex});
            nodeIndex ++;
        }

        for(auto it = g.connections.begin(); it != g.connections.end(); it++){
            if(!it->enabled) continue;
            Coordinate outputCoordinate = g.nodes.find({it->innovation2})->coordinate;
            int  inputIndex = innovationIndexMap.find(it->innovation1)->second;
            int outputIndex = innovationIndexMap.find(it->innovation2)->second;

            Connection con{outputCoordinate, it->weight, inputIndex, outputIndex};

            connections.insert(std::upper_bound(connections.begin(), connections.end(), con), con);
        }

        outputs  = new Output [g.nodes.size()];
        finished = new bool   [g.nodes.size()];

        node_count = g.nodes.size();
    }

    Calculator(const Calculator& g){

        outputs  = new Output [g.node_count];
        finished = new bool   [g.node_count];

        input_count  = g.input_count;
        output_count = g.output_count;
        node_count   = g.node_count;

        for(int i = 0; i < g.connections.size(); i++){
            connections.push_back(g.connections.at(i));
        }
    }

    Calculator& operator=(const Calculator& g){

        if(outputs != nullptr){

            delete[] outputs;
            delete[] finished;
        }

        outputs  = new Output [g.node_count];
        finished = new bool   [g.node_count];

        input_count  = g.input_count;
        output_count = g.output_count;
        node_count   = g.node_count;

        for(int i = 0; i < g.connections.size(); i++){
            connections.push_back(g.connections.at(i));
        }
    }

    virtual ~Calculator() {
        if(outputs != nullptr){
            delete[] outputs;
            delete[] finished;
        }
    }

    template <int I, int O>
    Vector<O,Output> compute(Vector<I, Input> &input){
        // reset all outputs
        std::memset(outputs , 0, node_count);
        std::memset(finished, 0, node_count);

        // assign inputs and mark them as finished so no activation will be applied
        for(int i = 0; i < input_count; i++){
            outputs [i] = input.values[i];
            finished[i] = true;
        }

        // make sure the bias is set to 1
        outputs[input_count + output_count] = 1;

        // compute all connections
        for(Connection& c:connections){
            if(!finished[c.inputIndex]){
                outputs [c.inputIndex] = ReLU(outputs[c.inputIndex]);
                finished[c.inputIndex] = true;
            }
            outputs[c.outputIndex] += c.weight * outputs[c.inputIndex];
        }

        // copy the output
        Vector<O, Output> outValues{};
        for(int i = 0; i <output_count; i++){
            outValues.values[i] = outputs[input_count + i];
        }

        // return the output
        return outValues;
    }

};

#endif //NEAT_CALCULATOR_H
