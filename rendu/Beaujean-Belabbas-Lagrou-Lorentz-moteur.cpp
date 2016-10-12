#include "Beaujean-Belabbas-Lagrou-Lorentz-moteur.h"

void menuPrincipal()
{
    system("CLS");
    cout << "##############################################" << endl;
    cout << "################### GRAPHE ###################" << endl;
    cout << "##############################################" << endl;

    cout << " 1. Lecture d'un graphe classique" << endl;
    cout << " 2. Lecture d'un graphe avec contrainte" << endl;
    cout << " 3. Quitter" << endl;

    cout << endl << "Votre choix: ";

    int numero = 3;

    cin >> numero;

    switch(numero)
    {
        case 1:
            creationGrapheClassique();
        break;

        case 2:
            creationGrapheContrainte();
        break;

        case 3:
            return;
        break;

        default:
            return;
        break;
    }
}

void creationGrapheClassique()
{
    Graphe graphe;

    lireGrapheClassique(graphe);
    cout << "#### Matrice d'indidence" << endl << endl;
    afficherMatrice(graphe);
    //cout << endl << "#### Liste des arcs" << endl << endl;
    //afficherArcs(graphe);

    Graphe transitif = graphe;
    cout << endl << "#### Creation de la fermeture transitive" << endl << endl;
    creerGrapheTransitif(transitif);
    //cout << "#### Matrice d'indidence" << endl << endl;
    //afficherMatrice(transitif);
    //cout << endl << "#### Liste des arcs" << endl << endl;
    //afficherArcs(transitif);

    cout << endl << "#### Detection de circuits" << endl << endl;

    if(detectionCircuit(transitif) == false)
    {
        cout << "# Ce graphe possede au moins un circuit, le calcul du rang n'est pas possible" << endl;
    }
    else
    {
        cout << "# Ce graphe ne possede aucun circuit" << endl;

        cout << endl << "#### Calcul du rang" << endl << endl;

        map<int, Sommet> sommetsTmp = graphe.mSommets;
        rang(sommetsTmp, graphe.mSommets);

        afficherRang(graphe);

        cout << endl << "#### Dates au plus tot" << endl << endl;
        dateAuPlusTot(graphe);

        cout << endl << "#### Dates au plus tard" << endl << endl;
        dateAuPlusTard(graphe);

        cout << endl;

        if(verifierGraphe(graphe, false))
        {
            menuEditeurProjet(graphe);
        }
    }
}

void creationGrapheContrainte()
{
    Graphe graphe;
    lireGrapheContrainte(graphe);
    cout << "#### Matrice d'indidence" << endl << endl;
    afficherMatrice(graphe);
    cout << endl << "#### Liste des arcs" << endl << endl;
    afficherArcs(graphe);
    graphe.mCircuits = false;

    Graphe transitif = graphe;
    cout << endl << "#### Creation de la fermeture transitive" << endl << endl;
    creerGrapheTransitif(transitif);
    cout << "#### Matrice d'indidence" << endl << endl;
    afficherMatrice(transitif);

    cout << endl << "#### Calcul du rang" << endl << endl;

    map<int, Sommet> sommetsTmp = graphe.mSommets;
    rang(sommetsTmp, graphe.mSommets);

    afficherRang(graphe);

    cout << endl << "#### Dates au plus tot" << endl << endl;
    dateAuPlusTot(graphe);

    cout << endl << "#### Dates au plus tard" << endl << endl;
    dateAuPlusTard(graphe);

    menuEditeurProjet(graphe);
}

/**
 * @brief Lecture d'un fichier classique (sans contrainte) et création de l'objet Graphe
 * @param Graphe& graphe : référence vers l'objet Graphe contenant la matrice d'incidene
 * @return void
*/

void lireGrapheClassique(Graphe& graphe)
{
    string numero;

    cout << "Nom du graphe : ";
    cin >> numero;
    cout << endl;

    string nomFichier = numero + ".txt";
    ifstream file(nomFichier);

    file >> graphe.mNSommets;

    int extInit ;
    int extTerm ;
    int valeur ;
    file >> extInit ;

    while (extInit != -1)
    {
        file >> extTerm;
        file >> valeur;

        //on créé les arcs
        Arc arc(valeur, extInit, extTerm);
        graphe.mAdjacente[extInit][extTerm] = arc;
        graphe.mArcs++;

        //on ajoute le sommet courant à la liste et on ajoute ses successeurs.
        //on ajoutera ensuite ses prédécesseurs

        //si le sommet n'a pas encore été enregistré
        if(!existe(graphe.mSommets, extInit))
        {
            Sommet sommet;
            sommet.mValue = extInit;
            graphe.mSommets.insert(pair<int, Sommet>(extInit, sommet));
        }

        if(!existe(graphe.mSommets, extTerm))
        {
            Sommet sommet;
            sommet.mValue = extTerm;
            graphe.mSommets.insert(pair<int, Sommet>(extTerm, sommet));
        }

        graphe.mSommets[extInit].mSuccesseurs.push_back(extTerm);
        graphe.mSommets[extTerm].mPredecesseurs.push_back(extInit);

        file >> extInit ;
    }
}

/**
 * @brief Lecture d'un fichier avec contrainte et création de l'objet Graphe
 * @param Graphe& graphe : référence vers l'objet Graphe contenant la matrice d'incidence
 * @return void
*/

void lireGrapheContrainte(Graphe& graphe)
{
    graphe.mArcs = 0;
    string numero;
    map<int, Tache> taches;

    cout << "Nom du graphe : ";
    cin >> numero;
    cout << endl;

    string nomFichier = numero + ".txt";
    ifstream file(nomFichier);

    file >> graphe.mNSommets;

    int extInit;
    int duree;
    int contrainte;

    file >> extInit ;

    while (extInit != -1)
    {
        contrainte = 0;

        file >> duree;

        Tache tache;
        tache.mNom = extInit;
        tache.mDuree = duree;

        while(contrainte != -1)
        {
            file >> contrainte;

            if(contrainte != -1)
                tache.mContraintes.push_back(contrainte);
        }

        cout << "La tache " << tache.mNom << " dure " << tache.mDuree << " et depend des taches: " << endl;

        for(auto it = tache.mContraintes.begin(); it != tache.mContraintes.end(); it++)
        {
            cout << "  - " << *it << endl;
        }

        file >> extInit;

        taches[tache.mNom] = tache;
    }

    //une fois qu'on a lu toutes les tâches, il faut construire le graphe correspondant
    for(auto it = taches.begin(); it != taches.end(); it++)
    {
        for(auto it2 = it->second.mContraintes.begin(); it2 != it->second.mContraintes.end(); it2++)
        {
            Arc arc(taches[*it2].mDuree, *it2, it->second.mNom);
            graphe.mAdjacente[*it2].insert(pair<int, Arc>(it->second.mNom, arc));
            graphe.mArcs++;

            //on ajoute le sommet courant à la liste et on ajoute ses successeurs.
            //on ajoutera ensuite ses prédécesseurs

            //si le sommet n'a pas encore été enregistré
            if(!existe(graphe.mSommets, *it2))
            {
                Sommet sommet;
                sommet.mValue = *it2;
                sommet.mDuree = taches[sommet.mValue].mDuree;
                graphe.mSommets.insert(pair<int, Sommet>(*it2, sommet));
            }

            if(!existe(graphe.mSommets, it->second.mNom))
            {
                Sommet sommet;
                sommet.mValue = it->second.mNom;
                sommet.mDuree = taches[sommet.mValue].mDuree;
                graphe.mSommets.insert(pair<int, Sommet>(it->second.mNom, sommet));
            }

            graphe.mSommets[*it2].mSuccesseurs.push_back(it->second.mNom);
            graphe.mSommets[it->second.mNom].mPredecesseurs.push_back(*it2);
        }
    }

    //On crée le sommet initial
    Sommet sommet;
    sommet.mValue = 0;
    sommet.mDuree = 0;
    graphe.mSommets.insert(pair<int, Sommet>(0, sommet));

    //On crée le sommet terminal
    sommet.mValue = graphe.mArcs;
    sommet.mDuree = 0;
    graphe.mSommets.insert(pair<int, Sommet>(graphe.mArcs, sommet));

    //on créé les deux arcs restants
    for(auto it = graphe.mSommets.begin(); it != graphe.mSommets.end(); it++)
    {
        //on doit relier la tâche 0 initiale à la tâche qui n'a pas de prédecesseurs
        if(it->second.mPredecesseurs.size() == 0 && it->second.mValue != 0 && it->second.mValue != sommet.mValue)
        {
            cout << "sommet sans predecesseur :" << it->second.mValue << endl;
            Arc arc(0, 0, it->second.mValue);
            graphe.mAdjacente[0][it->second.mValue] = arc;
            graphe.mSommets[it->second.mValue].mPredecesseurs.push_back(0);
            graphe.mSommets[0].mSuccesseurs.push_back(it->second.mValue);
            graphe.mArcs++;
        }

        //on doit relier la tâche n+1 finale à la tâche qui n'a pas de successeurs
        if(it->second.mSuccesseurs.size() == 0  && it->second.mValue != 0 && it->second.mValue != sommet.mValue)
        {
            cout << "sommet sans successeur :" << it->second.mValue << endl;
            Arc arc(taches[it->second.mValue].mDuree, it->second.mValue, sommet.mValue);
            graphe.mAdjacente[it->second.mValue][sommet.mValue] = arc;
            graphe.mSommets[sommet.mValue].mPredecesseurs.push_back(it->second.mValue);
            graphe.mSommets[it->second.mValue].mSuccesseurs.push_back(sommet.mValue);
            graphe.mArcs++;
        }
    }

    //il faut mettre à jour le nombre de sommets total
    graphe.mNSommets+=2;
}

/**
 * @brief Affiche la matrice d'incidence d'un graphe
 * @param Graphe& graphe : référence vers l'objet Graphe contenant la matrice d'incidene
 * @return void
*/

void afficherMatrice(Graphe& graphe)
{
    cout << "    ";

    for(auto it = graphe.mSommets.begin(); it != graphe.mSommets.end(); it++)
    {
        if(it->first < 10)
            cout << it->first << "  ";
        else
            cout << it->first << " ";
    }

    cout << endl << "     ";

    for(int i = 0; i < graphe.mNSommets;i++)
    {
        cout << "-- ";
    }

    cout << endl;

    for(auto it = graphe.mSommets.begin(); it != graphe.mSommets.end(); it++)
    {
        if(it->first < 10)
            cout << it->first << "  | ";
        else
            cout << it->first << " | ";

        for(auto it2 = graphe.mSommets.begin(); it2 != graphe.mSommets.end(); it2++)
        {
            if(existe(graphe.mAdjacente, it->first, it2->first))
                if(graphe.mAdjacente[it->first][it2->first].mValeur < 10)
                    cout << graphe.mAdjacente[it->first][it2->first].mValeur << "  ";
                else
                    cout << graphe.mAdjacente[it->first][it2->first].mValeur << " ";
            else
                cout << "X  ";
        }

        cout << endl;
    }
}

/**
 * @brief Affiche les arcs d'un graphe
 * @param Graphe& graphe : référence vers l'objet Graphe contenant la matrice d'incidene
 * @return void
*/

void afficherArcs(Graphe& graphe)
{
    for(auto it = graphe.mSommets.begin(); it != graphe.mSommets.end(); it++)
    {
        for(auto it2 = graphe.mSommets.begin(); it2 != graphe.mSommets.end(); it2++)
        {
            if(existe(graphe.mAdjacente, it->first, it2->first))
            {
                cout << it->first << " " << it2->first << " " << graphe.mAdjacente[it->first][it2->first].mValeur << endl;
            }
        }
    }
}

/**
 * @brief A partir de la matrice d'incidence, créé un deuxième graphe contenant la matrice de la fermeture transitive
 * @param Graphe& transitif : référence vers l'objet Graphe contenant la matrice d'incidence
 * @param bool afficherMat
 * @return void
*/

void creerGrapheTransitif(Graphe &transitif, bool afficherMat)
{
    //int i, j, k, n = transitif.mNSommets;

    for(auto it3 = transitif.mSommets.begin(); it3 != transitif.mSommets.end(); it3++)
    {
        for(auto it = transitif.mSommets.begin(); it != transitif.mSommets.end(); it++)
        {
            for(auto it2 = transitif.mSommets.begin(); it2 != transitif.mSommets.end(); it2++)
            {
                if(!existe(transitif.mAdjacente, it->first, it2->first) && (existe(transitif.mAdjacente, it->first, it3->first) && existe(transitif.mAdjacente, it3->first, it2->first)))
                {
                    Arc arc2(transitif.mAdjacente[it->first][it3->first].mValeur + transitif.mAdjacente[it3->first][it2->first].mValeur, it->first, it2->first);
                    transitif.mAdjacente[it->first][it2->first] = arc2;

                    if(afficherMat)
                    {
                        afficherMatrice(transitif);
                        cout << endl << "-------------------" << endl << endl;
                    }
                }
            }
        }
    }
}

/**
 * @brief Fonction de détection d'un circuit dans un graphe, elle se base sur la matrice représentant la fermeture transitive
 *        Si la matrice ne possède aucun 1 sur sa diagonale, alors le graphe ne possède aucun circuit
 * @param Graphe& graphe : référence vers l'objet Graphe contenant la matrice de la fermeture transitive
 * @return bool
*/

bool detectionCircuit(Graphe& graphe)
{
    int n = graphe.mNSommets;

    for(int i = 0; i < n; i++)
    {
        if(existe(graphe.mAdjacente, i, i))
        {
            graphe.mCircuits = true;
            return false;
        }
    }

    return true;
}

/**
 * @brief Calcul le rang de chaque sommet du graphe
 * @param Sommet& sommets : la liste des sommets qui sera modifiée
 * @param Sommet& sommetsOriginal : la liste des sommets originale
 * @param bool afficher : faut-il afficher les étapes de calcul
 * @param int rangSommet : rang actuel
 * @param int ordreSommet : ordre actuel
 * @return bool
*/

void rang(map<int, Sommet>& sommets, map<int, Sommet>& sommetsOriginal, bool afficher, int rangSommet, int ordreSommet)
{
    list<int> dejaTraite;

    if(afficher)
    {
        cout << "########## ITERATION " << rangSommet << endl;

        for(auto it = sommets.begin(); it != sommets.end(); it++)
        {
            cout << "sommet " << it->second.mValue << " a pour predecesseurs :" ;

            for(auto it2 = it->second.mPredecesseurs.begin(); it2 != it->second.mPredecesseurs.end(); it2++)
            {
                cout << *it2 << " ";
            }

            cout << endl;
        }

        cout << "-------------------" << endl;
    }

    //on parcourt la liste des sommets
    for(auto it = sommets.begin(); it != sommets.end(); it++)
    {
        if(!existe(dejaTraite, it->second.mValue))
        {
            //si le sommet courant n'a pas de successeurs, on met à jour son rang
            if(it->second.mPredecesseurs.size() == 0)
            {
                sommetsOriginal[it->second.mValue].mRang = rangSommet;
                sommetsOriginal[it->second.mValue].mOrdre = ordreSommet;
                ordreSommet++;

                dejaTraite.push_back(it->second.mValue);

                //On affiche le rang du sommet
                if(afficher)
                {
                    cout << "sommet " << it->second.mValue << " : " << endl;
                    cout << "    rang: " << sommetsOriginal[it->second.mValue].mRang << endl;
                    cout << "    ordre: " << sommetsOriginal[it->second.mValue].mOrdre << endl;
                }
            }
        }
    }

    //Une fois qu'on a parcouru tous les sommets, on peut supprimer ceux qui ont été traités
    for(auto it = dejaTraite.begin(); it != dejaTraite.end(); it++)
    {
        rangSupprimerPredecceseurs(sommets, *it);

        if(existe(dejaTraite, *it))
            sommets.erase(*it);
    }

    rangSommet++;

    //Si il reste des sommets qui ont des prédecesseurs, alors on relance la fonction
    if(sommets.size() > 0)
        rang(sommets, sommetsOriginal, afficher, rangSommet, ordreSommet);
}

/**
 * @brief Lorsque l'on calcule le rang, on supprime les sommets au fur et à mesure qu'ils ont été traités
 * @param map<int, Sommet>& sommets : la liste des sommets
 * @param int sommet : le numéro du sommet à supprimer
 * @return void
*/

void rangSupprimerPredecceseurs(map<int, Sommet>& sommets, int sommet)
{
    //on parcourt la liste des sommets
    for(auto it = sommets.begin(); it != sommets.end(); it++)
    {
        if(existe(it->second.mPredecesseurs, sommet))
        {
            it->second.mPredecesseurs.remove(sommet);
        }
    }
}

/**
 * @brief On vérifie que le graphe possède une entrée unique
 * @param Graphe& graph
 * @return bool
*/

bool verifierEntreeUnique(Graphe& graphe)
{
    int nombreEntree = 0;

    for(auto it = graphe.mSommets.begin(); it != graphe.mSommets.end(); it++)
    {
        if(it->second.mPredecesseurs.size() == 0)
            nombreEntree++;
    }

    return nombreEntree == 1 ? true : false;
}

/**
 * @brief On vérifie que le graphe possède une sortie unique
 * @param Graphe& graphe
 * @return bool
*/

bool verifierSortieUnique(Graphe& graphe)
{
    int nombreSortie = 0;

    for(auto it = graphe.mSommets.begin(); it != graphe.mSommets.end(); it++)
    {
        if(it->second.mSuccesseurs.size() == 0)
            nombreSortie++;
    }

    return nombreSortie == 1 ? true : false;
}

/**
 * @brief On vérifie que le graphe possède une sortie unique
 * @param Graphe& graphe
 * @param bool calculate : doit-on recalculer le rang, la date au plus tot etc. ?
 * @return bool
*/

bool verifierGraphe(Graphe& graphe, bool afficher)
{
    Graphe transitif = graphe;
    creerGrapheTransitif(transitif, false);
    bool valide = true;

    if(detectionCircuit(transitif) == false)
    {
        valide = false;
        cout << "Le graphe possede un circuit" << endl;
    }
    else
    {
        if(verifierEntreeUnique(graphe) == false)
        {
            valide = false;
            cout << "Le graphe possede plusieurs entrees" << endl;
        }

        if(verifierSortieUnique(graphe) == false)
        {
            valide = false;
            cout << "Le graphe possede plusieurs sorties" << endl;
        }

        if(verifierEntreeSommetSortiePossible(graphe) == false)
        {
            valide = false;
        }

        if(valide)
        {
            map<int, Sommet> sommetsTmp = graphe.mSommets;
            rang(sommetsTmp, graphe.mSommets, afficher);

            dateAuPlusTot(graphe, afficher);
            dateAuPlusTard(graphe, afficher);
        }

        if(graphe.mDateFinProjet != 0 && graphe.mDateFinProjet < graphe.mSommets[getSortie(graphe)].mDatePlusTot)
        {
            valide = false;
            cout << "Le projet dure trop longtemps" << endl;
        }
    }

    return valide;
}

/**
 * @brief On récupère le sommet demandé
 * @param Graphe& graphe
 * @return bool
*/

Sommet& getSommet(Graphe& graphe, int ordre)
{
    for(auto it = graphe.mSommets.begin(); it != graphe.mSommets.end(); it++)
    {
        if(it->second.mOrdre == ordre)
            return it->second;
    }

    return graphe.mSommets.begin()->second;
}

/**
 * @brief Calcul la date au plus tôt de chaque sommet
 * @param Graphe& graphe
 * @return bool
*/

void dateAuPlusTot(Graphe& graphe, bool afficher)
{
    for(int i = 0; i < graphe.mNSommets; i++)
    {
        Sommet& sommet = getSommet(graphe, i);

        //On parcourt les prédecesseurs pour pouvoir choisir la date au plus tard qui convient
        if(sommet.mPredecesseurs.size() == 0)
        {
            sommet.mDatePlusTot = 0;
        }
        else
        {
            //On parcout tous les prédecesseurs et on prend celui qui a la plus longue date au plus tôt
            int auPlusTot = 0;

            for(auto it = sommet.mPredecesseurs.begin(); it != sommet.mPredecesseurs.end(); it++)
            {
                if(graphe.mSommets[*it].mDatePlusTot + graphe.mAdjacente[*it][sommet.mValue].mValeur > auPlusTot)
                    auPlusTot = graphe.mSommets[*it].mDatePlusTot + graphe.mAdjacente[*it][sommet.mValue].mValeur;
            }

            sommet.mDatePlusTot = auPlusTot;
        }

        if(afficher)
            cout << "date au plus tot de " << sommet.mValue << " : " << sommet.mDatePlusTot << endl;
    }
}

/**
 * @brief Calcul la date au plus tard de chaque sommet
 * @param Graphe& graphe
 * @return bool
*/

void dateAuPlusTard(Graphe& graphe, bool afficher)
{
    for(int i = graphe.mNSommets-1; i >= 0; i--)
    {
        Sommet& sommet = getSommet(graphe, i);

        //On parcourt les successeurs
        if(sommet.mSuccesseurs.size() == 0)
        {
            sommet.mDatePlusTard = getSommet(graphe, graphe.mNSommets-1).mDatePlusTot;
        }
        else
        {
            //On parcout tous les successeurs et on regarde lequel a la date au plus tard la plus courte
            //(en ayant soustrait la durée de l'arc les reliant)
            int auPlusTard = getSommet(graphe, graphe.mNSommets-1).mDatePlusTot;

            for(auto it = sommet.mSuccesseurs.begin(); it != sommet.mSuccesseurs.end(); it++)
            {
                if(graphe.mSommets[*it].mDatePlusTard - graphe.mAdjacente[sommet.mValue][*it].mValeur < auPlusTard)
                    auPlusTard = graphe.mSommets[*it].mDatePlusTard - graphe.mAdjacente[sommet.mValue][*it].mValeur;
            }

            sommet.mDatePlusTard = auPlusTard;
        }

        if(afficher)
            cout << "date au plus tard de " << sommet.mValue << " : " << sommet.mDatePlusTard << endl;
    }
}

/**
 * @brief Vérification qu'un graphe classique chargé à l'étape 1 est un graphe d'ordonnancement
 * @param Graphe& graphe
 * @return bool
*/

bool verifierEntreeSommetSortiePossible(Graphe& graphe)
{
    Graphe transitif = graphe;
    creerGrapheTransitif(transitif, false);

    bool possible = true;

    //si les cases de la matrice commençant par 0 existent toutes, alors, tout sommet est accessible depuis l'entrée
    for(auto it = graphe.mSommets.begin(); it != graphe.mSommets.end(); it++)
    {
        if(it->first != getEntree(graphe) && !existe(transitif.mAdjacente, getEntree(graphe), it->first))
        {
            cout << "Le sommet " << it->first << " n'est pas accessible depuis l'entree" << endl;
            possible = false;
        }

        if(it->first != getSortie(graphe) && !existe(transitif.mAdjacente, it->first, getSortie(graphe)))
        {
            cout << "Le sommet " << it->first << " ne permet pas d'atteindre la sortie" << endl;
            possible = false;
        }
    }

    return possible;
}

/**
 * @brief Affichage du rang des sommets d'un graphe
 * @param Graphe& graphe
 * @return bool
*/

void afficherRang(Graphe& graphe)
{
    cout << endl << endl;

    for(auto it = graphe.mSommets.begin(); it != graphe.mSommets.end(); it++)
    {
        cout << "sommet " << it->second.mValue << " : " << endl;
        cout << "    rang: " << it->second.mRang << endl;
        cout << "    ordre: " << it->second.mOrdre << endl;
    }
}

void menuEditeurProjet(Graphe& graphe)
{
    bool editeur = true;

    while(editeur)
    {
        cout << "##########################" << endl;
        cout << "##### EDITEUR PROJET #####" << endl;
        cout << "##########################" << endl;
        cout << endl;
        cout << "1. Ajouter une tache" << endl;
        cout << "2. Supprimer une tache" << endl;
        cout << "3. Ajouter une contrainte" << endl;
        cout << "4. Supprimer une contrainte" << endl;
        cout << "5. Modifier la duree d'execution" << endl;
        cout << "6. Modifier la date de fin du projet" << endl;
        cout << "7. Quitter" << endl;
        cout << "8. Afficher le rang" << endl;
        cout << "9. Afficher la matrice d'adjacence" << endl;
        cout << "10.Afficher la liste des arcs" << endl;
        cout << "11.Afficher la matrice de la fermeture transitive" << endl;
        cout << endl;
        cout << "Votre choix: ";

        int choix = 0;
        cin >> choix;

        switch(choix)
        {
            case 1:
                ajouterTache(graphe);
            break;

            case 2:
                supprimerTache(graphe);
            break;

            case 3:
                ajouterContrainte(graphe);
            break;

            case 4:
                supprimerContrainte(graphe);
            break;

            case 5:
                modifierDureeExecution(graphe);
            break;

            case 6:
                modifierDateFinProjet(graphe);
            break;

            case 7:
                editeur = false;
            break;

            case 8:
                {
                    map<int, Sommet> sommetsTmp = graphe.mSommets;
                    rang(sommetsTmp, graphe.mSommets);
                    afficherRang(graphe);
                }
            break;

            case 9:
                afficherMatrice(graphe);
            break;

            case 10:
                afficherArcs(graphe);
            break;

            case 11:
                Graphe transitif = graphe;
                creerGrapheTransitif(transitif);
            break;
        }
    }
}

void ajouterTache(Graphe& graphe)
{
    Graphe tmp = graphe;

    int duree;
    int influe = 0;
    int dependDe = 0;
    list<int> influes;
    list<int> dependDes;

    cout << "########### Ajout d'une tache" << endl << endl;
    cout << "Duree de la tache:" ;
    cin >> duree;

    cout << "De quelles taches depend cette tache (-1 pour arreter): " << endl;

    do
    {
        cout << "tache: ";
        cin >> dependDe;

        //on verifie que la tache existe
        if(existe(tmp.mSommets, dependDe))
            dependDes.push_back(dependDe);
        else if(dependDe != -1)
            cout << "cette tache n'existe pas" << endl;
    }while(dependDe != -1);

    cout << "Quelles taches sont influencees par cette tache (-1 pour arreter): " << endl;

    do
    {
        cout << "tache: ";
        cin >> influe;

        //on verifie que la tache existe
        if(existe(tmp.mSommets, influe))
            influes.push_back(influe);
        else if(influe != -1)
            cout << "cette tache n'existe pas" << endl;
    }while(influe != -1);

    //On ajoute le sommet à la liste
    Sommet sommet;
    sommet.mValue = tmp.mNSommets;
    sommet.mPredecesseurs = dependDes;
    sommet.mSuccesseurs = influes;
    sommet.mDuree = duree;

    tmp.mSommets[sommet.mValue] = sommet;
    tmp.mNSommets++;

    //On créé les arcs à partir des listes de successeurs et prédecesseurs
    for(auto it = sommet.mPredecesseurs.begin(); it != sommet.mPredecesseurs.end(); it++)
    {
        Sommet predecesseur = tmp.mSommets[*it];
        Arc arc(predecesseur.mDuree, predecesseur.mValue, sommet.mValue);
        tmp.mAdjacente[predecesseur.mValue].insert(pair<int, Arc>(sommet.mValue, arc));
        tmp.mArcs++;
    }

    for(auto it = sommet.mSuccesseurs.begin(); it != sommet.mSuccesseurs.end(); it++)
    {
        Sommet successeur = tmp.mSommets[*it];
        Arc arc(sommet.mDuree, sommet.mValue, successeur.mValue);
        tmp.mAdjacente[sommet.mValue].insert(pair<int, Arc>(successeur.mValue, arc));
        tmp.mArcs++;
    }

    //On vérifie ensuite que le graphe remplie toutes les conditions pour être un graphe d'ordonnancement

    if(verifierGraphe(tmp, false))
    {
        cout << "Le graphe est un graphe d'ordonnancement, la tache a ete ajoutee" << endl;
        graphe = tmp;
    }
    else
    {
        cout << "Le graphe n'est pas un graphe d'ordonnancement, la tache n'a pas pu etre ajoutee" << endl;
    }
}

void supprimerTache(Graphe& graphe)
{
    Graphe tmp = graphe;
    int tache;

    cout << "########### Suppression d'une tache" << endl << endl;
    cout << "Quelle tache voulez-vous supprimer:" << endl;

    do
    {
        cout << "tache: ";
        cin >> tache;

        //on verifie que la tache existe
        if(!existe(tmp.mSommets, tache))
            cout << "cette tache n'existe pas" << endl;
    }while(!existe(tmp.mSommets, tache));

    //Si on a trouve une tache qui existe, alors, on supprime tous les arcs qui la concerne
    for(auto it = tmp.mAdjacente.begin(); it != tmp.mAdjacente.end(); it++)
    {
        if(it->first == tache)
        {
            cout << "SUPPRESSION !!!!!!!" << endl;
            int nbArcs = it->second.size();
            tmp.mAdjacente.erase(it);
            tmp.mArcs-=nbArcs;
        }
        else
        {
            for(auto it2 = it->second.begin(); it2 != it->second.end(); it2++)
            {
                if(it2->first == tache)
                {
                    cout << "SUPPRESSION !!!!!!!" << endl;

                    it->second.erase(it2);
                    tmp.mArcs--;
                }
            }
        }
    }

    //Ensuite, on supprime le sommet concerne
    tmp.mSommets.erase(tache);
    tmp.mNSommets--;

    //On verifie ensuite que le graphe fonctionne toujours
    if(verifierGraphe(tmp, false))
    {
        cout << "Le graphe est un graphe d'ordonnancement, la tache a bien ete supprimee" << endl;
        graphe = tmp;
    }
    else
    {
        cout << "Le graphe n'est pas un graphe d'ordonnancement, la tache n'a pas pu etre supprimmee" << endl;
    }
}

void ajouterContrainte(Graphe& graphe)
{
    Graphe tmp = graphe;
    int tache;
    int depend;

    cout << "########### Ajout d'une contrainte" << endl << endl;
    cout << "A quelle tache voulez-vous ajouter une contrainte:" << endl;

    //A quelle tâche doit-on ajouter une contrainte
    do
    {
        cout << "tache: ";
        cin >> tache;

        //on verifie que la tache existe
        if(!existe(tmp.mSommets, tache))
            cout << "cette tache n'existe pas" << endl;
    }while(!existe(tmp.mSommets, tache));

    cout << "De quelle tache doit dependre la tache " << tache << " : " << endl;

    //De quelle autre tâche va dépendre la tâche que l'on modifie
    do
    {
        cout << "tache: ";
        cin >> depend;

        //on verifie que la tache existe
        if(existe(tmp.mSommets, depend) && depend != tache && !existe(tmp.mAdjacente, depend, tache))
            break;
        else if(depend != -1)
            cout << "vous ne pouvez pas ajouter cette contrainte" << endl;
    }while(true);

    //On crée le nouvel arc
    Arc arc(tmp.mSommets[depend].mDuree, depend, tache);
    tmp.mAdjacente[depend].insert(pair<int, Arc>(tache, arc));
    tmp.mArcs++;

    //On vérifie qu'il s'agit toujours d'un arc d'ordonnancement
    //On verifie ensuite que le graphe fonctionne toujours
    if(verifierGraphe(tmp, false))
    {
        cout << "Le graphe est un graphe d'ordonnancement, la contrainte a bien ete ajoutee" << endl;
        graphe = tmp;
    }
    else
    {
        cout << "Le graphe n'est pas un graphe d'ordonnancement, la contrainte n'a pas pu etre ajoutee" << endl;
    }
}

void supprimerContrainte(Graphe& graphe)
{
    Graphe tmp = graphe;
    int tache;
    int depend;

    cout << "########### Suppression d'une contrainte" << endl << endl;
    cout << "A quelle tache voulez-vous supprimer une contrainte: " << endl;

    //A quelle tâche doit-on supprimer une contrainte
    do
    {
        cout << "tache: ";
        cin >> tache;

        //on verifie que la tache existe
        if(!existe(tmp.mSommets, tache))
            cout << "cette tache n'existe pas" << endl;
    }while(!existe(tmp.mSommets, tache));

    cout << "De quelle tache ne doit plus dependre la tache " << tache << " : " << endl;

    //De quelle autre tâche va dépendre la tâche que l'on modifie
    do
    {
        cout << "tache: ";
        cin >> depend;

        //on verifie que la tache existe
        if(existe(tmp.mSommets, depend) && existe(tmp.mAdjacente, depend, tache))
            break;
        else if(depend != -1)
            cout << "vous ne pouvez pas supprimer cette contrainte" << endl;
    }while(true);

    //On supprime l'arc concerné
    tmp.mArcs--;
    tmp.mAdjacente[depend].erase(tache);

    //On vérifie qu'il s'agit toujours d'un arc d'ordonnancement
    //On verifie ensuite que le graphe fonctionne toujours
    if(verifierGraphe(tmp, false))
    {
        cout << "Le graphe est un graphe d'ordonnancement, la contrainte a bien ete supprimee" << endl;
        graphe = tmp;
    }
    else
    {
        cout << "Le graphe n'est pas un graphe d'ordonnancement, la contrainte n'a pas pu etre supprimee" << endl;
    }
}

void modifierDureeExecution(Graphe& graphe)
{
    Graphe tmp = graphe;
    int tache;
    int duree;

    cout << "########### Modification de la duree d'execution d'une tache" << endl << endl;
    cout << "Quelle est la tache dont vous voulez modifier la duree d'execution: " << endl;

    //De quelle tache doit-on modifier la duree d'execution ?
    do
    {
        cout << "tache: ";
        cin >> tache;

        //on verifie que la tache existe
        if(!existe(tmp.mSommets, tache))
            cout << "cette tache n'existe pas" << endl;
    }while(!existe(tmp.mSommets, tache));

    cout << "Quelle est la duree d'execution de la tache " << tache << " : " << endl;

    //duree d'execution
    do
    {
        cout << "duree: ";
        cin >> duree;
    }while(duree < 0);

    //on modifie la duree du sommet X
    tmp.mSommets[tache].mDuree = duree;

    //Maintenant, il faut modifier la duree de tous les arcs pour lesquels la tache X est sommet initial
    for(auto it = tmp.mAdjacente[tache].begin(); it != tmp.mAdjacente[tache].end(); it++)
    {
        it->second.mValeur = duree;
    }

    //On vérifie qu'il s'agit toujours d'un arc d'ordonnancement
    //On verifie ensuite que le graphe fonctionne toujours
    if(verifierGraphe(tmp, false))
    {
        cout << "Le graphe est un graphe d'ordonnancement, la contrainte a bien ete supprimee" << endl;
        graphe = tmp;
    }
    else
    {
        cout << "Le graphe n'est pas un graphe d'ordonnancement, la contrainte n'a pas pu etre supprimee" << endl;
    }
}

void modifierDateFinProjet(Graphe& graphe)
{
    int date;

    cout << "########### Changer la date de rendu du projet" << endl << endl;
    cout << "Quelle a la date limite de rendu du projet:" << endl;

    //A quelle tâche doit-on supprimer une contrainte
    do
    {
        cout << "date: ";
        cin >> date;

        if(graphe.mSommets[getSortie(graphe)].mDuree <= date)
            break;
        else
            cout << "Cette date est trop tot" << endl;
    }while(true);

    graphe.mDateFinProjet = date;

    cout << "La date limite de rendu du projet a bien ete modifiee" << endl;
}

int getDateFinProjet(Graphe& graphe)
{
    int dateAuPlusTot = 0;

    //On récupère la date au plus tôt la plus grande
    for(auto it = graphe.mSommets.begin(); it != graphe.mSommets.end(); it++)
    {
        if(it->second.mDatePlusTot > dateAuPlusTot)
            dateAuPlusTot = it->second.mDatePlusTot;
    }

    return dateAuPlusTot;
}

int getEntree(Graphe& graphe)
{
    int entree = 0;

    for(auto it = graphe.mSommets.begin(); it != graphe.mSommets.end(); it++)
    {
        if(it->second.mPredecesseurs.size() == 0)
            entree = it->first;
    }

    return entree;
}

int getSortie(Graphe& graphe)
{
    int sortie = 0;

    for(auto it = graphe.mSommets.begin(); it != graphe.mSommets.end(); it++)
    {
        if(it->second.mSuccesseurs.size() == 0)
            sortie = it->first;
    }

    return sortie;
}
