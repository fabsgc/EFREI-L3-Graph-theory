#ifndef MOTEUR_H_INCLUDED
#define MOTEUR_H_INCLUDED

#include <iostream>
#include "fonctions.h"
#include "Graphe.h"
#include "Tache.h"

using namespace std;

void menuPrincipal();
void menuEditeurProjet(Graphe& graphe);
void creationGrapheClassique();
void creationGrapheContrainte();
void lireGrapheClassique(Graphe& graphe);
void lireGrapheContrainte(Graphe& graphe);
void afficherMatrice(Graphe& graphe);
void afficherArcs(Graphe& graphe);
void afficherRang(Graphe& graphe);
void creerGrapheTransitif(Graphe &transitif, bool afficherMat = true);
bool detectionCircuit(Graphe& graphe);
void rang(map<int, Sommet>& mSommets, map<int, Sommet>& sommetsMerde, bool afficher = true, int rangSommet = 1, int ordreSommet = 0);
void rangSupprimerPredecceseurs(map<int, Sommet>& sommets, int sommet);
bool verifierEntreeUnique(Graphe& graphe);
bool verifierSortieUnique(Graphe& graphe);
bool verifierGraphe(Graphe& graphe, bool afficher = true);
void dateAuPlusTot(Graphe& graphe, bool afficher = true);
void dateAuPlusTard(Graphe& graphe, bool afficher = true);
bool verifierEntreeSommetSortiePossible(Graphe& graphe);
void ajouterTache(Graphe& graphe);
void supprimerTache(Graphe& graphe);
void ajouterContrainte(Graphe& graphe);
void supprimerContrainte(Graphe& graphe);
void modifierDureeExecution(Graphe& graphe);
void modifierDateFinProjet(Graphe& graphe);
int getDateFinProjet(Graphe& graphe);
int getEntree(Graphe& graphe);
int getSortie(Graphe& graphe);
Sommet& getSommet(Graphe& graphe);

#endif // MOTEUR_H_INCLUDED
