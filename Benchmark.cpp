#include "Benchmark.h"
#include <algorithm>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <queue>
#include <vector>

namespace {
using Horloge = std::chrono::steady_clock;
struct Mesure { double total=0, placement=0, extraction=-1, retrait=0; unsigned long long echanges=0, comparaisons=0, echangesExtraction=0; };
Mesure mesureA, mesureB;
constexpr int N=1000000;
void afficherTableau(double ap,double bp,double aa,double ba,double at,double bt,double ae,double be){
 std::cout<<std::fixed<<std::setprecision(3)<<"\nÉtape                         | A manuel (int*) (ms) | B STL (ms) | Écart (%) | Plus rapide\n";
  auto ligne=[](const char* nom,double a,double b){double ec=(b<=0||a<0)?0:100.0*(a-b)/b;std::cout<<std::left<<std::setw(31)<<nom<<"| "<<std::right;if(a<0)std::cout<<std::setw(21)<<"N/A";else std::cout<<std::setw(21)<<a;std::cout<<" | ";if(b<0)std::cout<<std::setw(10)<<"N/A";else std::cout<<std::setw(10)<<b;std::cout<<" | "<<std::setw(9)<<(b<0||a<0?"N/A":std::to_string(ec))<<" | "<<(a>=0&&b>=0?(a<=b?"A":"B"):"N/A")<<'\n';};
 ligne("Insertion : placement",ap,bp);ligne("Insertion : ajustement arbre",aa,ba);ligne("Insertion : TOTAL",at,bt);ligne("Extraction : TOTAL",ae,be);
}
void extraction(const TasScratchVector& sourceA,const TasSTL& sourceB){
 TasScratchVector a(sourceA);TasSTL b(sourceB);
 if(a.estVide()||b.estVide()){std::cout<<"Extraction impossible : un tas est vide.\n";return;}
 a.reinitialiserCompteurs();
 const int avantA=a.max(),avantB=b.max();
 int maxA=0,maxB=0;
 const auto a0=Horloge::now();
 maxA=a.supprimerMax();
 const auto a1=Horloge::now();
 const auto b0=Horloge::now();
 maxB=b.supprimerMax();
 const auto b1=Horloge::now();
 const double nsA=std::chrono::duration<double,std::nano>(a1-a0).count();
 const double nsB=std::chrono::duration<double,std::nano>(b1-b0).count();
 const int apresA=a.estVide()?0:a.max(),apresB=b.estVide()?0:b.max();
 mesureA.extraction=nsA/1.0e6;mesureB.extraction=nsB/1.0e6;mesureA.echangesExtraction=a.nbEchanges();
 std::cout<<"\n=== Benchmark : Extraction du maximum ===\n\nNombre de valeurs : "<<sourceA.taille()<<"\n\n"
          <<"Méthode A : Tas-Max manuel (int*)\n  Max avant extraction : "<<avantA<<"\n  Max extrait          : "<<maxA<<"\n  Temps d'extraction   : "<<nsA<<" ns\n  Nouveau maximum      : "<<apresA<<"\n  Taille restante      : "<<a.taille()<<"\n  Échanges             : "<<a.nbEchanges()<<"\n  Comparaisons         : "<<a.nbComparaisons()<<"\n\n"
          <<"Méthode B : Tas-Max (priority_queue)\n  Max avant extraction : "<<avantB<<"\n  Max extrait          : "<<maxB<<"\n  Temps d'extraction   : "<<nsB<<" ns\n  Nouveau maximum      : "<<apresB<<"\n  Taille restante      : "<<b.taille()<<"\n\n"
          <<"Résultats identiques : "<<(maxA==maxB?"OUI":"NON")<<'\n';
 std::ofstream f("resultats.csv",std::ios::app);f<<"extraction_une_racine,A,"<<nsA<<"_ns,max="<<maxA<<",echanges="<<a.nbEchanges()<<",comparaisons="<<a.nbComparaisons()<<'\n'<<"extraction_une_racine,B,"<<nsB<<"_ns,max="<<maxB<<'\n';
 }
void recherche(const TasScratchVector& a,const TasSTL& b){
 int cible;
 std::cout<<"\n=== Benchmark : Recherche d'un nombre ===\n\nNombre de valeurs : "<<a.taille()<<"\nEntrez le nombre à rechercher : ";
 while(!(std::cin>>cible)){if(std::cin.eof())return;std::cout<<"Saisie invalide. Entrez un entier : ";std::cin.clear();std::cin.ignore(100000,'\n');}

 // La copie de la priority_queue reste hors du chronométrage.
 TasSTL copieB(b);
 std::pair<int,unsigned long long> resultatA;
 const auto debutA=Horloge::now();
 resultatA=a.rechercher(cible);
 const auto finA=Horloge::now();
 const double nsA=std::chrono::duration<double,std::nano>(finA-debutA).count();

 unsigned long long examinesB=0;
 bool trouveB=false;
 const auto debutB=Horloge::now();
 const auto resultatB=copieB.rechercherDansCopie(cible);
 const auto finB=Horloge::now();
 const double nsB=std::chrono::duration<double,std::nano>(finB-debutB).count();
 trouveB=resultatB.first;
 examinesB=resultatB.second;
 const bool trouveA=resultatA.first>=0;

  std::cout<<"\nMéthode A : From Scratch (tableau int*)\n  Valeur recherchée    : "<<cible
          <<"\n  Résultat             : "<<(trouveA?"TROUVÉ":"NON TROUVÉ")
          <<"\n  Temps de recherche   : "<<nsA<<" ns\n  Comparaisons         : "<<resultatA.second
          <<"\n\nMéthode B : STL (priority_queue)\n  Valeur recherchée    : "<<cible
          <<"\n  Résultat             : "<<(trouveB?"TROUVÉ":"NON TROUVÉ")
          <<"\n  Temps de recherche   : "<<nsB<<" ns\n  Valeurs examinées    : "<<examinesB
          <<"\n\nRésultats identiques : "<<(trouveA==trouveB?"OUI":"NON")<<'\n';
  if(nsA<nsB)std::cout<<"Plus rapide pour cette mesure : Méthode A (tableau int*).\n";
 else if(nsB<nsA)std::cout<<"Plus rapide pour cette mesure : Méthode B (priority_queue).\n";
 else std::cout<<"Temps identiques selon la précision de l'horloge.\n";
  std::cout<<"Le tableau manuel est parcouru en O(n). La priority_queue extrait les maxima un à un, en O(n log n) au pire ; la performance mesurée dépend de la valeur recherchée et de la machine.\n";
 std::ofstream f("resultats.csv",std::ios::app);
 f<<"recherche,A,"<<cible<<','<<nsA<<"_ns,"<<(trouveA?"trouve":"absent")<<",comparaisons="<<resultatA.second<<'\n'
  <<"recherche,B,"<<cible<<','<<nsB<<"_ns,"<<(trouveB?"trouve":"absent")<<",examines="<<examinesB<<'\n';
}
}

void initialiserEtChronometrer(TasScratchVector& a,TasSTL& b){
 std::cout<<"Insertion de 1 000 000 de valeurs..."<<std::flush;
 std::mt19937 gen(42);std::uniform_int_distribution<int>d(-1000000,1000000);std::vector<int> valeurs(N);for(int&x:valeurs)x=d(gen);
 a.reinitialiserCompteurs();auto t=Horloge::now();for(int x:valeurs)a.inserer(x);mesureA.total=std::chrono::duration<double,std::milli>(Horloge::now()-t).count();mesureA.echanges=a.nbEchanges();mesureA.comparaisons=a.nbComparaisons();
 t=Horloge::now();for(int x:valeurs)b.inserer(x);mesureB.total=std::chrono::duration<double,std::milli>(Horloge::now()-t).count();
 TasScratchVector placementA;placementA.reserver(N);t=Horloge::now();for(int x:valeurs)placementA.ajouterALaFin(x);mesureA.placement=std::chrono::duration<double,std::milli>(Horloge::now()-t).count();
  // priority_queue::push effectue le placement et le réajustement ensemble.
  mesureB.placement=-1;
  std::cout<<" terminé.\n";
  std::ifstream existant("resultats.csv");const bool fichierExiste=existant.good();existant.close();
  std::ofstream f("resultats.csv",std::ios::app);if(!fichierExiste)f<<"etape,version,total_ms,placement_ms,ajustement_ms,echanges,comparaisons\n";
  f<<"insertion,A,"<<mesureA.total<<','<<mesureA.placement<<','<<mesureA.total-mesureA.placement<<','<<mesureA.echanges<<','<<mesureA.comparaisons<<'\n';
  f<<"insertion,B,"<<mesureB.total<<",N/A,N/A,N/A,N/A\n";
}

void menuBenchmark(const TasScratchVector& a,const TasSTL& b){
 for(;;){
  std::cout<<"\n=== Résultats benchmark ===\n1. Afficher les résultats d'insertion\n2. Lancer le test d'extraction\n3. Lancer le test de recherche\n0. Retour\n";
  int c;std::cin>>c;
  if(!std::cin){std::cin.clear();std::cin.ignore(100000,'\n');std::cout<<"Saisie invalide.\n";continue;}
  if(c==0)return;
  if(c==1){
   const double aa=mesureA.total-mesureA.placement;
   afficherTableau(mesureA.placement,mesureB.placement,aa,-1,mesureA.total,mesureB.total,mesureA.extraction,mesureB.extraction);
   std::cout<<"\nInsertion A : "<<mesureA.total<<" ms ("<<mesureA.total/1000<<" s), "<<mesureA.total*1e6/N<<" ns/insertion, "<<N/(mesureA.total/1000)<<" insertions/s\nPlacement A : "<<mesureA.placement<<" ms ; ajustement : "<<aa<<" ms ("<<100*aa/mesureA.total<<" %)\nÉchanges : "<<mesureA.echanges<<" total, "<<static_cast<double>(mesureA.echanges)/N<<" moyens ; comparaisons : "<<mesureA.comparaisons<<" total, "<<static_cast<double>(mesureA.comparaisons)/N<<" moyennes\nInsertion B : "<<mesureB.total<<" ms ("<<mesureB.total/1000<<" s), "<<mesureB.total*1e6/N<<" ns/insertion, "<<N/(mesureB.total/1000)<<" insertions/s\nPlacement/ajustement B : non séparables (priority_queue::push réalise les deux).\n";
  }else if(c==2)extraction(a,b);
  else if(c==3)recherche(a,b);
  else std::cout<<"Choix invalide.\n";
 }
}
