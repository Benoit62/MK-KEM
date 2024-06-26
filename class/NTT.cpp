#include "../headers/NTT.hpp"

#include <cstdint>
#include <cassert>
#include <iostream>
#include <vector>
#include <array>

#include "../headers/BitArray.hpp"
#include "../headers/ByteArray.hpp"
#include "../headers/NTTCoef.hpp"
#include "Zeta.cpp"


    NTT::NTT() {};

    template <std::size_t n>
    NTT::NTT(const Poly& f) {
        // Pourquoi de pas avoir rempli directement l'attribut coefficients avec les coeffs de input ?
        // A la limite passer par un IntArray ou un array de PolyCoef
        std::array<uint16_t, n> f_hat = f;

        uint16_t k = 1;

        for (uint16_t len = 128; len >= 2; len /= 2) {
            for (uint16_t start = 0; start < N; start += 2 * len) {

                k++;

                for (uint16_t j = start; j < start + len; j++) {
                    int32_t t = static_cast<int32_t>(Zeta.getZeta(k)) * static_cast<int32_t>(f_hat[j + len]);
                    // Voir si les modulos sont nécessaires, déjà fait dans les surcharges d'opérateurs
                    // Pourquoi des cast en int32_t alors qu'il faut utiliser les surcharges ????????????????
                    // Pourquoi avoir cast les coef de l'input au lieux de les laisser en COEF ?????
                    // On perd toute l'utilité de la classe NTTCoef et de ses surcharges
                    f_hat[j + len] = static_cast<uint16_t>((static_cast<int32_t>(f_hat[j]) - t % q + q) % q);
                    f_hat[j] = static_cast<uint16_t>((static_cast<int32_t>(f_hat[j]) + t % q) % q);
                }
            }
        }

        coefficients = f_hat;
    }

    template <std::size_t n>
    NTT::NTT(const std::array<NTTCoef, n>& input) {
        //Même remarque que pour le constructeur précédent
        std::array<uint16_t, n> f_hat = {};
        for (size_t i = 0; i < n; i++) {
            f_hat[i] = static_cast<uint16_t>(input[i]);
        }

        uint16_t k = 1;
        for (uint16_t len = 128; len >= 2; len /= 2) {
            for (uint16_t start = 0; start < n; start += 2 * len) {
                k++;
                for (uint16_t j = start; j < start + len; j++) {
                    int32_t t = static_cast<int32_t>(Zeta.getZeta(k)) * static_cast<int32_t>(f_hat[j + len]);
                    //Même remarque que pour le constructeur précédent
                    f_hat[j + len] = static_cast<uint16_t>((static_cast<int32_t>(f_hat[j]) - t % q + q) % q);
                    f_hat[j] = static_cast<uint16_t>((static_cast<int32_t>(f_hat[j]) + t % q) % q);
                }
            }
        }

        //coefficients = f_hat;
        for (size_t i = 0; i < n; i++) {
            coefficients[i] = f_hat[i];
        }
    }

    NTTCoef NTT::get(std::size_t i) const {
        return coefficients[i];
    }

    void NTT::set(std::size_t i, const NTTCoef& coef) {
        coefficients[i] = coef;
    }

    NTT NTT::SampleNTT(XOF B) {
        NTT a;
        uint32_t i = 0;
        uint32_t j = 0;
        uint32_t d1, d2;

         while (j < 256) {
            // Que fait exactement getLastThree ?
             d1 = B.getLastThree(i)[0] + 256 * (B.getLastThree(i)[1] % 16);
             d2 = static_cast<int>(floor(B.getLastThree(i)[1] / 16.0)) + 16 * B.getLastThree(i)[2];

             if (d1 < q) {
                 a.set(j, d1);
                 j++;
             }

             if (d2 < q && j < 256) {
                 a.set(j, d2);
                 j++;
             }
             i += 3;
         }

        return a;
    }

    // Get tab coefficients
    std::array<NTTCoef, n> NTT::getCoef()
    {
        return coefficients;
    }

    // Surcharge = pour NTT
    NTT& NTT::operator=(const NTT& other)
    {
        coefficients = other.coefficients;
        return *this;
    }

    // Mettre directement le code de MultiplyNTTs ici ?
    NTT& NTT::operator*(const NTT& other)
    {
        NTT h = MultiplyNTTs(*this, other);
        return h;
    }

    NTT& NTT::operator+(const NTT& other)
    {
        for (int i = 0; i < coefficients.size(); i++)
        {
            this->coefficients[i] = coefficients[i] + other.coefficients[i];
        }
        return *this;
    }

    // Utilité d'une méthode de test dans la classe ? C'est la seule dans le projet à faire ceci
    // Tests imcomplets, il faudrait tester plus de cas ou changer le nom de la méthode
    void NTT::testNTT() {
        // Générer un tableau de valeurs aléatoires
        std::array<NTTCoef, n> input;
        for (auto& x : input) {
            x = NTTCoef(rand() % q);
        }
        // Créer l'objet NTT à partir du tableau
        NTT ntt(input);
        // Vérifier que la transformation est correcte
        for (size_t i = 0; i < n; i++) {
            assert(ntt.get(i) == input[i]);
        }
        std::cout << "Test NTT réussi !" << std::endl;
    }

    // A passer en privé si on passe par la surcharge de l'opérateur *, ou faire un choix entre les deux 
    NTT NTT::MultiplyNTTs(const NTT& f, const NTT& g)
    {
        NTT h;
        NTTCoef c0;
        NTTCoef c1;

        for (uint16_t i = 0; i < n / 2; i++)
        {
            NTTCoef a0 = f.get(2 * i);
            NTTCoef a1 = f.get(2 * i + 1);
            NTTCoef b0 = g.get(2 * i);
            NTTCoef b1 = g.get(2 * i + 1);
            uint16_t gamma = gammaList[i];
            BaseCaseMultiply(a0, a1, b0, b1, c0, c1, gamma);

            h.set(2 * i, c0);
            h.set(2 * i + 1, c1);
        }
        return h;
    }

    // Devrait être en privé ?
    void NTT::BaseCaseMultiply(const NTTCoef a0, const NTTCoef a1, const NTTCoef b0, const NTTCoef b1, NTTCoef& c0, NTTCoef& c1, const uint16_t gamma)
    {
        c0 = a0 * b0 + a1 * b1 * gamma;
        c1 = a0 * b1 + a1 * b0;
    }

