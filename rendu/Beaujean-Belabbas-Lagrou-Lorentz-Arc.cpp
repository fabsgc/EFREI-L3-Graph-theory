#include "Beaujean-Belabbas-Lagrou-Lorentz-Arc.h"

Arc::Arc(int valeur, int initial, int terminal){
    mValeur = valeur;
    mInitial = initial;
    mTerminal = terminal;
}

Arc::Arc(const Arc & arc){
    mValeur = arc.mValeur;
    mInitial = arc.mInitial;
    mTerminal = arc.mTerminal;
}
