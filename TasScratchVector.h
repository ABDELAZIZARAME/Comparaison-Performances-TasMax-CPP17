#ifndef TAS_SCRATCH_VECTOR_H
#define TAS_SCRATCH_VECTOR_H
#include <cstddef>
#include <utility>

class TasScratchVector {
public:
    TasScratchVector();
    ~TasScratchVector();
    TasScratchVector(const TasScratchVector& autre);
    TasScratchVector& operator=(const TasScratchVector& autre);
    TasScratchVector(TasScratchVector&& autre) noexcept;
    TasScratchVector& operator=(TasScratchVector&& autre) noexcept;

    void ajouterALaFin(int x);
    void ajusterHaut(int i);
    void inserer(int x);
    void retirerRacine();
    int ajusterBas(int i);
    int supprimerMax();
    int max() const;
    bool estVide() const;
    std::size_t taille() const;
    void vider();
    void afficher() const;
    void construireTas(const int* valeurs, std::size_t nombre);
    void reserver(std::size_t capaciteDemandee);
    void reinitialiserCompteurs();
    unsigned long long nbComparaisons() const;
    unsigned long long nbEchanges() const;
    std::pair<int, unsigned long long> rechercher(int x) const;
    bool supprimerValeur(int x);

private:
    int* tas_;
    std::size_t taille_;
    std::size_t capacite_;
    unsigned long long comparaisons_;
    unsigned long long echanges_;
    void echanger(int i, int j);
};
#endif
