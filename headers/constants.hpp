#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <cstdint>
#include <vector>

// Définition des constantes ML-KEM-512 
const uint16_t q = 3329;
const uint16_t n = 256;
const uint8_t k = 2;
const uint8_t eta1 = 3;
const uint8_t eta2 = 2;
const uint8_t du = 10;
const uint8_t dv = 4;

extern std::vector<uint16_t> zetaList;
extern std::vector<uint16_t> gammaList;

#endif // CONSTANTS_HPP