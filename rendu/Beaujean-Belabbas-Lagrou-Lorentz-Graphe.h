#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED

#include "Beaujean-Belabbas-Lagrou-Lorentz-fonctions.h"
#include "Beaujean-Belabbas-Lagrou-Lorentz-Arc.h"
#include "Beaujean-Belabbas-Lagrou-Lorentz-Sommet.h"

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
