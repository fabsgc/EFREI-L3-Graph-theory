#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED

#include "fonctions.h"
#include "Arc.h"
#include "Sommet.h"

using namespace std;

class Graphe{
public:
    int mNSommets;
    int mArcs;
    int mDateFinProjet;
    bool mCircuits;
    map<int, map<int, Arc> > mAdjacente;
    map<int, Sommet> mSommets;

    Graphe();
};

#endif // GRAPH_H_INCLUDED
