#include "TasScratchVector.h"
#include <iostream>
#include <stdexcept>

TasScratchVector::TasScratchVector()
    : tas_(nullptr), taille_(0), capacite_(0), comparaisons_(0), echanges_(0) {}

TasScratchVector::~TasScratchVector() { delete[] tas_; }

TasScratchVector::TasScratchVector(const TasScratchVector& autre)
    : tas_(autre.capacite_ ? new int[autre.capacite_] : nullptr),
      taille_(autre.taille_), capacite_(autre.capacite_),
      comparaisons_(autre.comparaisons_), echanges_(autre.echanges_) {
    for (std::size_t i = 0; i < taille_; ++i) tas_[i] = autre.tas_[i];
}

TasScratchVector& TasScratchVector::operator=(const TasScratchVector& autre) {
    if (this != &autre) {
        TasScratchVector copie(autre);
        int* ancien = tas_;
        tas_ = copie.tas_; taille_ = copie.taille_; capacite_ = copie.capacite_;
        comparaisons_ = copie.comparaisons_; echanges_ = copie.echanges_;
        copie.tas_ = ancien;
    }
    return *this;
}

TasScratchVector::TasScratchVector(TasScratchVector&& autre) noexcept
    : tas_(autre.tas_), taille_(autre.taille_), capacite_(autre.capacite_),
      comparaisons_(autre.comparaisons_), echanges_(autre.echanges_) {
    autre.tas_ = nullptr; autre.taille_ = autre.capacite_ = 0;
    autre.comparaisons_ = autre.echanges_ = 0;
}

TasScratchVector& TasScratchVector::operator=(TasScratchVector&& autre) noexcept {
    if (this != &autre) {
        delete[] tas_;
        tas_ = autre.tas_; taille_ = autre.taille_; capacite_ = autre.capacite_;
        comparaisons_ = autre.comparaisons_; echanges_ = autre.echanges_;
        autre.tas_ = nullptr; autre.taille_ = autre.capacite_ = 0;
        autre.comparaisons_ = autre.echanges_ = 0;
    }
    return *this;
}

void TasScratchVector::reserver(std::size_t demandee) {
    if (demandee <= capacite_) return;
    std::size_t nouvelle = capacite_ ? capacite_ : 8;
    while (nouvelle < demandee) nouvelle *= 2;
    int* remplacement = new int[nouvelle];
    for (std::size_t i = 0; i < taille_; ++i) remplacement[i] = tas_[i];
    delete[] tas_;
    tas_ = remplacement;
    capacite_ = nouvelle;
}

void TasScratchVector::ajouterALaFin(int x) {
    if (taille_ == capacite_) reserver(capacite_ ? capacite_ * 2 : 8);
    tas_[taille_++] = x;
}

void TasScratchVector::echanger(int i, int j) {
    const int temporaire = tas_[i]; tas_[i] = tas_[j]; tas_[j] = temporaire; ++echanges_;
}

void TasScratchVector::ajusterHaut(int i) {
    while (i > 0) {
        const int parent = (i - 1) / 2;
        ++comparaisons_;
        if (tas_[parent] >= tas_[i]) break;
        echanger(parent, i); i = parent;
    }
}

void TasScratchVector::inserer(int x) { ajouterALaFin(x); ajusterHaut(static_cast<int>(taille_) - 1); }

void TasScratchVector::retirerRacine() {
    if (taille_ == 0) return;
    if (taille_ > 1) tas_[0] = tas_[taille_ - 1];
    --taille_;
}

int TasScratchVector::ajusterBas(int i) {
    const unsigned long long echangesAvant = echanges_;
    const int n = static_cast<int>(taille_);
    while (i < n) {
        int plusGrand = i;
        const int gauche = 2 * i + 1, droite = 2 * i + 2;
        if (gauche < n) { ++comparaisons_; if (tas_[gauche] > tas_[plusGrand]) plusGrand = gauche; }
        if (droite < n) { ++comparaisons_; if (tas_[droite] > tas_[plusGrand]) plusGrand = droite; }
        if (plusGrand == i) break;
        echanger(i, plusGrand); i = plusGrand;
    }
    return static_cast<int>(echanges_ - echangesAvant);
}

int TasScratchVector::supprimerMax() {
    if (taille_ == 0) throw std::out_of_range("Impossible de supprimer le maximum : le tas est vide.");
    const int resultat = tas_[0]; retirerRacine(); if (taille_) ajusterBas(0); return resultat;
}

int TasScratchVector::max() const {
    if (taille_ == 0) throw std::out_of_range("Le tas est vide.");
    return tas_[0];
}

bool TasScratchVector::estVide() const { return taille_ == 0; }
std::size_t TasScratchVector::taille() const { return taille_; }
void TasScratchVector::vider() { taille_ = 0; }
void TasScratchVector::reinitialiserCompteurs() { comparaisons_ = echanges_ = 0; }
unsigned long long TasScratchVector::nbComparaisons() const { return comparaisons_; }
unsigned long long TasScratchVector::nbEchanges() const { return echanges_; }

std::pair<int, unsigned long long> TasScratchVector::rechercher(int x) const {
    unsigned long long comparaisons = 0;
    for (std::size_t i = 0; i < taille_; ++i) {
        ++comparaisons;
        if (tas_[i] == x) return {static_cast<int>(i), comparaisons};
    }
    return {-1, comparaisons};
}

bool TasScratchVector::supprimerValeur(int x) {
    const int indice = rechercher(x).first;
    if (indice < 0) return false;
    const int dernier = static_cast<int>(taille_) - 1;
    if (indice == dernier) { --taille_; return true; }
    tas_[indice] = tas_[dernier]; --taille_;
    if (indice > 0 && tas_[indice] > tas_[(indice - 1) / 2]) ajusterHaut(indice);
    else ajusterBas(indice);
    return true;
}

void TasScratchVector::construireTas(const int* valeurs, std::size_t nombre) {
    reserver(nombre);
    for (std::size_t i = 0; i < nombre; ++i) tas_[i] = valeurs[i];
    taille_ = nombre;
    if (taille_ > 1) for (int i = static_cast<int>(taille_ / 2) - 1; i >= 0; --i) ajusterBas(i);
}

void TasScratchVector::afficher() const {
    std::cout << "Tableau : [";
    for (std::size_t i = 0; i < taille_; ++i) std::cout << (i ? ", " : "") << tas_[i];
    std::cout << "]\nArbre (niveau par niveau) :\n";
    for (std::size_t i = 0, niveau = 0; i < taille_; ++niveau) {
        const std::size_t fin = i + (std::size_t{1} << niveau) < taille_ ? i + (std::size_t{1} << niveau) : taille_;
        for (; i < fin; ++i) std::cout << tas_[i] << ' ';
        std::cout << '\n';
    }
}
