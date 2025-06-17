/* Copyright EPFL 2022
 * Joshua Klein
 * 
 * Example single 1k*1k perceptron layer using AIMClib using random weights and
 * inputs.  An example of how to compile this example is included within
 * build_example.sh.  The GCC/G++ build command line determines if the AIMClib
 * checker (gem5 model emulation) is used or not.
 *
 */

#include "aimc.hh"
#include <cstdint>
#include <cstdlib>
#include <iostream>

int main(int argc, char * argv[])
{
    // Test bench parameters.
    int n_x         = 32; // MLP input/output dimensions.

    // Set up and initialize vectors/matrices.
    int8_t* input = new int8_t[n_x];
    int8_t* output = new int8_t[n_x];
    std::cout << "input vec:\n";
    for (int i = 0; i < n_x; i++) {
        input[i] = (int8_t)rand();
        std::cout << static_cast<int>(input[i]) << ", ";
    }
    for (int i = 0; i < n_x; i++) {
        output[i] = (int8_t)rand();
    }

    int8_t *  W1        = new int8_t[n_x*n_x];
    for (int i = 0; i < n_x*n_x; i++) {
        W1[i] = (int8_t)rand();
        std::string delim = (i % n_x == 0) ? "\n" : ", ";
        std::cout << static_cast<int>(W1[i]) << delim;
    }

    // Map weights to AIMC tile.
    std::cout << "mapping matrix";
    mapMatrix(0, 0, n_x, n_x, W1);

    // Do inference.
    // Queue input for next inference in first layer.
    std::cout << "queuing vector";
    queueVector(n_x, input);
    
    // Do MVM.
    std::cout << "processing";
    aimcProcess();
    
    // Dequeue output from AIMC tile MVM.
    std::cout << "dequeuing vector";
    dequeueVector(n_x, output);

    std::cout << "output vec:\n";
    for (int i = 0; i < n_x; i++) {
        output[i] = (int8_t)rand();
        std::cout << output[i] << ", ";
    }

    // Cleanup and return.
    delete[] input;
    delete[] output;
    delete[] W1;

    return 0;
}
