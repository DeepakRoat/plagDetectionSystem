#include "helper.h"

// Calculate Jaccard similarity between two sets of tokens
double calculate_jaccard_similarity(int common, int total1, int total2) {
    //corner cases
    if(total1 == 0 && total2 == 0) return 100.0; // Both are empty, hence fully similar
    if(total1 == 0 || total2 == 0) return 0.0; // One is empty, the other is not
    // Jaccard similarity = (Intersection / Union) * 100
    double union_count = (double)(total1 + total2 - common);
    if (union_count == 0) return 100.0;  // Avoid division by zero
    return (common / union_count) * 100;
}

// Find the minimum of two integers
int min(int a, int b) {
    return (a < b) ? a : b;
}

// Find the maximum of two integers
int max(int a, int b) {
    return (a > b) ? a : b;
}
