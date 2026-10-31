#include "TasSTL.h"
#include <algorithm>
#include <iostream>
#include <stdexcept>

void TasSTL::ajouterALaFin(int x) { tas_.push(x); }
void TasSTL::ajusterHaut() {}
void TasSTL::inserer(int x) { tas_.push(x); }
void TasSTL::retirerRacine() { if (!tas_.empty()) tas_.pop(); }
void TasSTL::ajusterBas() {}
int TasSTL::supprimerMax() {
    if (tas_.empty()) throw std::out_of_range("Impossible de supprimer le maximum : le tas est vide.");
    const int resultat = tas_.top(); tas_.pop(); return resultat;
}
int TasSTL::max() const {
    if (tas_.empty()) throw std::out_of_range("Le tas est vide.");
    return tas_.top();
}
bool TasSTL::estVide() const { return tas_.empty(); }
std::size_t TasSTL::taille() const { return tas_.size(); }
void TasSTL::vider() { tas_ = std::priority_queue<int>(); }
void TasSTL::reserver(std::size_t) {}

void TasSTL::afficher() const {
    std::priority_queue<int> copie = tas_;
    std::cout << "Éléments par priorité : [";
    bool premier = true;
    while (!copie.empty()) {
        std::cout << (premier ? "" : ", ") << copie.top();
        premier = false; copie.pop();
    }
    std::cout << "]\n";
}

void TasSTL::construireTas(const std::vector<int>& valeurs) {
    vider();
    for (int valeur : valeurs) tas_.push(valeur);
}

void TasSTL::triParTas(std::vector<int>& valeurs) {
    std::make_heap(valeurs.begin(), valeurs.end());
    std::sort_heap(valeurs.begin(), valeurs.end());
}

std::pair<int, unsigned long long> TasSTL::rechercher(int x) const {
    std::priority_queue<int> copie = tas_;
    unsigned long long examinees = 0;
    while (!copie.empty()) {
        ++examinees;
        if (copie.top() == x) return {static_cast<int>(examinees - 1), examinees};
        copie.pop();
    }
    return {-1, examinees};
}

std::pair<bool, unsigned long long> TasSTL::rechercherDansCopie(int x) {
    unsigned long long examinees = 0;
    while (!tas_.empty()) {
        ++examinees;
        if (tas_.top() == x) return {true, examinees};
        tas_.pop();
    }
    return {false, examinees};
}

bool TasSTL::supprimerValeur(int x) {
    std::priority_queue<int> copie = tas_;
    std::priority_queue<int> nouveau;
    bool supprime = false;
    while (!copie.empty()) {
        const int valeur = copie.top(); copie.pop();
        if (!supprime && valeur == x) supprime = true;
        else nouveau.push(valeur);
    }
    if (supprime) tas_ = nouveau;
    return supprime;
}
