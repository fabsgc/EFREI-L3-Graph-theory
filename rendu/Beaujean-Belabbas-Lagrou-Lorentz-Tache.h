#ifndef TACHE_H_INCLUDED
#define TACHE_H_INCLUDED

#include "Beaujean-Belabbas-Lagrou-Lorentz-fonctions.h"

using namespace std;

class Tache{
public:
    int mNom;
    int mDuree;
    list<int> mContraintes;
};

#endif // TACHE_H_INCLUDED
