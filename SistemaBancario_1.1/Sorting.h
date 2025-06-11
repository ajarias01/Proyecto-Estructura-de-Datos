#ifndef SORTING_H
#define SORTING_H

#include <vector>
#include "Movimiento.h"
#include "Fecha.h"

inline int dateKey(const Fecha& f) {
    return f.get_anuario() * 10000 + f.get_mes() * 100 + f.get_dia();
}

inline int dateKey(const Movimiento& m) {
    return dateKey(m.get_fecha());
}
// LSD Radix Sort base 256 on 32-bit date keys
inline void radixSortMovements(std::vector<Movimiento>& arr) {
    size_t n = arr.size();
    std::vector<Movimiento> buffer(n);
    const int B = 256;
    std::vector<int> count(B);
    for (int shift = 0; shift < 32; shift += 8) {
        std::fill(count.begin(), count.end(), 0);
        // count occurrences
        for (size_t i = 0; i < n; ++i) {
            int key = (dateKey(arr[i]) >> shift) & 0xFF;
            ++count[key];
        }
        // prefix sum
        for (int i = 1; i < B; ++i) count[i] += count[i - 1];
        // build buffer
        for (int i = (int)n - 1; i >= 0; --i) {
            int key = (dateKey(arr[i]) >> shift) & 0xFF;
            buffer[--count[key]] = arr[i];
        }
        // copy back
        for (size_t i = 0; i < n; ++i) arr[i] = buffer[i];
    }
}

// Binary search lower_bound on dateKey
inline int lowerBound(const std::vector<Movimiento>& arr, int target) {
    int lo = 0, hi = (int)arr.size();
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (dateKey(arr[mid]) < target) lo = mid + 1;
        else hi = mid;
    }
    return lo;
}

inline int upperBound(const std::vector<Movimiento>& arr, int target) {
    int lo = 0, hi = (int)arr.size();
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (dateKey(arr[mid]) <= target) lo = mid + 1;
        else hi = mid;
    }
    return lo;
}

#endif