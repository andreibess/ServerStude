#include "VectorProcessor.h"

double VectorProcessor::sumOfSquares(const std::vector<double>& vec) {
    double sum = 0.0;
    for (double val : vec) {
        sum += val * val;
    }
    return sum;
}
