#include <cmath>     // for std::sqrt and std::pow
#include <algorithm> // for std::sort

float calculate_average(const float array[], int size) {
    float sum = 0.0;
    for (int i = 0; i < size; ++i) {
        sum += array[i];
    }
    return static_cast<float>(sum / size);
}

float calculate_stddev(const float array[], int size) {
    float mean = calculate_average(array, size);
    float variance = 0.0;

    for (int i = 0; i < size; ++i)
        variance += std::pow(array[i] - mean, 2);

    variance /= size;
    return std::sqrt(variance);
}

float min_element(const float array[], int size) {
    return *std::min_element(array, array + size);
}

float max_element(const float array[], int size) {
    return *std::max_element(array, array + size);
}