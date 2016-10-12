#ifndef ARC_H_INCLUDED
#define ARC_H_INCLUDED

#include "Beaujean-Belabbas-Lagrou-Lorentz-fonctions.h"

using namespace std;

class Arc{
public:
    Arc(){}
    Arc(int valeur, int initial, int terminal);
    Arc(const Arc & arc);

public:
    int mValeur;
    int mInitial;
    int mTerminal;
};

#endif // ARC_H_INCLUDED
