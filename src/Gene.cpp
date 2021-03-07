//
// Created by Luecx on 04.03.2021.
//

#include <iomanip>
#include "Gene.h"

bool Gene::operator==(const Gene &rhs) const {
    return innovationNumber == rhs.innovationNumber;
}

bool Gene::operator!=(const Gene &rhs) const {
    return !(rhs == *this);
}

bool Gene::operator<(const Gene &rhs) const {
    return innovationNumber < rhs.innovationNumber;
}

bool Gene::operator>(const Gene &rhs) const {
    return rhs < *this;
}

bool Gene::operator<=(const Gene &rhs) const {
    return !(rhs < *this);
}

bool Gene::operator>=(const Gene &rhs) const {
    return !(*this < rhs);
}

std::ostream &operator<<(std::ostream &os, const Gene &gene) {
    os << std::left;
    os  << "innovationNumber: " <<  std::setw(10)   << gene.innovationNumber;
    return os;
}

std::ostream &operator<<(std::ostream &os, const NodeGene &gene) {
    os << std::left;
    os << static_cast<const Gene &>(gene)
        << " coordinate: "  << std::setw(10) << gene.coordinate
        << " type: "        << std::setw(10) << gene.type;
    return os;
}

std::ostream &operator<<(std::ostream &os, const ConnectionGene &gene) {
    os << std::left;
    os << static_cast<const Gene &>(gene)
        << " weight: "          <<  std::setw(10)   << gene.weight
        << " innovation1: "     <<  std::setw(10)   << gene.innovation1
        << " innovation2: "     <<  std::setw(10)   << gene.innovation2
        << " enabled: "         <<  std::setw(10)   << gene.enabled
        << " nodeReplaceIndex: "<<  std::setw(10)   << gene.nodeReplaceIndex;
    return os;
}
