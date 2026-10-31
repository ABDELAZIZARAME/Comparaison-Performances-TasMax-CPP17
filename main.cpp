#include "TasScratchVector.h"
#include "TasSTL.h"
#include "Benchmark.h"
#include <chrono>
#include <iostream>
#include <limits>
#include <string>
#ifdef _WIN32
#include <windows.h>
#endif

int lireEntier(const std::string& invite){int x;for(;;){std::cout<<invite;if(std::cin>>x)return x;if(std::cin.eof())return 0;std::cout<<"Saisie invalide. Entrez un entier.\n";std::cin.clear();std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');}}
template<class F> double chronometrer(F f){auto d=std::chrono::steady_clock::now();f();return std::chrono::duration<double,std::micro>(std::chrono::steady_clock::now()-d).count();}
int main(){
#ifdef _WIN32
 SetConsoleOutputCP(CP_UTF8);SetConsoleCP(CP_UTF8);
#endif
 TasScratchVector a;TasSTL b;initialiserEtChronometrer(a,b);
 for(;;){std::cout<<"\n=== TAS-MAX ===\n1. Insérer une valeur (à la main)\n2. Rechercher une valeur\n3. Supprimer le max\n4. Supprimer une valeur donnée\n5. Résultats du benchmark (comparaison A vs B)\n0. Quitter\n";int c=lireEntier("Choix : ");if(c==0)break;
  if(c==1){int x=lireEntier("Valeur à insérer : ");a.reinitialiserCompteurs();double ta=chronometrer([&]{a.inserer(x);});double tb=chronometrer([&]{b.inserer(x);});std::cout<<"A : "<<ta<<" µs, échanges "<<a.nbEchanges()<<", taille "<<a.taille()<<"\nB : "<<tb<<" µs, taille "<<b.taille()<<"\n";}
  else if(c==2){int x=lireEntier("Valeur à rechercher : ");std::pair<int,unsigned long long> ra,rb;double ta=chronometrer([&]{ra=a.rechercher(x);});double tb=chronometrer([&]{rb=b.rechercher(x);});std::cout<<"A : "<<(ra.first>=0?"trouvé":"non trouvé")<<", indice "<<ra.first<<", comparaisons "<<ra.second<<", "<<ta<<" µs\nB : "<<(rb.first>=0?"trouvé":"non trouvé")<<", indice "<<rb.first<<", comparaisons "<<rb.second<<", "<<tb<<" µs\n";}
  else if(c==3){if(a.estVide()||b.estVide()){std::cout<<"Suppression impossible : un tas est vide.\n";continue;}int va=0,vb=0;a.reinitialiserCompteurs();double ta=chronometrer([&]{va=a.supprimerMax();});auto e=a.nbEchanges();double tb=chronometrer([&]{vb=b.supprimerMax();});std::cout<<"A : max supprimé "<<va<<", "<<ta<<" µs, échanges "<<e<<"\nB : max supprimé "<<vb<<", "<<tb<<" µs\n";}
  else if(c==4){int x=lireEntier("Valeur à supprimer : ");bool sa=false,sb=false;double ta=chronometrer([&]{sa=a.supprimerValeur(x);});double tb=chronometrer([&]{sb=b.supprimerValeur(x);});std::cout<<"A : "<<(sa?"valeur supprimée":"valeur absente")<<", "<<ta<<" µs, taille "<<a.taille()<<"\nB : "<<(sb?"valeur supprimée":"valeur absente")<<", "<<tb<<" µs, taille "<<b.taille()<<"\n";}
  else if(c==5)menuBenchmark(a,b);else std::cout<<"Choix invalide.\n";
 }
 return 0;
}
