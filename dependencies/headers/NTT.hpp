// ntt.hpp

#ifndef NTT_HPP
#define NTT_HPP

#include <vector>

// Fonction pour calculer la NTT en place
void computeNTT(std::vector<int>& f, int q) {
    int n = f.size();
    int k = 1;

    for (int len = 128; len >= 2; len /= 2) {
        for (int start = 0; start < n; start += 2 * len) {
            int zeta = BitRev7(k) % q;
            k++;

            for (int j = start; j < start + len; j++) {
                int t = (1LL * zeta * f[j + len]) % q; 
                f[j + len] = (f[j] - t + q) % q;       
                f[j] = (f[j] + t) % q;                
            }
        }
    }
}

#endif // NTT_HPP
