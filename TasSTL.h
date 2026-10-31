#ifndef TAS_STL_H
#define TAS_STL_H
#include <cstddef>
#include <queue>
#include <utility>
#include <vector>

class TasSTL {
public:
    void ajouterALaFin(int x);
    void ajusterHaut();
    void inserer(int x);
    void retirerRacine();
    void ajusterBas();
    int supprimerMax();
    int max() const;
    bool estVide() const;
    std::size_t taille() const;
    void vider();
    void afficher() const;
    void construireTas(const std::vector<int>& valeurs);
    static void triParTas(std::vector<int>& valeurs);
    void reserver(std::size_t n);
    std::pair<int, unsigned long long> rechercher(int x) const;
    std::pair<bool, unsigned long long> rechercherDansCopie(int x);
    bool supprimerValeur(int x);

private:
    std::priority_queue<int> tas_;
};
#endif
