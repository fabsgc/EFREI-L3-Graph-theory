#ifndef FONCTIONS_H_INCLUDED
#define FONCTIONS_H_INCLUDED

#include <map>
#include <string>
#include <list>
#include <sstream>
#include <string>
#include <fstream>
#include <algorithm>

using namespace std;

/**
 * @brief Permet de savoir si un arc existe dans un graphe
 * @param map<int, map<int, T>> mymap
 * @param T x
 * @param T y
 * @return bool
*/
template<typename T>
bool existe(map<int, map<int, T> > mymap, int x, int y){
    if (mymap.find(x) != mymap.end() && mymap[x].find(y) != mymap[x].end())
        return true;

    return false;
}

/**
 * @brief Permet de savoir si un arc existe dans un graphe
 * @param map<int, T> mymap
 * @param int x
 * @return bool
*/
template<typename T>
bool existe(map<int, T> mymap, int x){
    if (mymap.find(x) != mymap.end())
        return true;

    return false;
}

/**
 * @brief Permet de savoir si une valeur existe dans une liste
 * @param list<int> mylist
 * @param int x
 * @return bool
*/
bool existe(list<int> mylist, int x);

/**
 * @brief Convertie le type donné en paramètre en string
 * @param T x
 * @return string
*/
template<typename T>
std::string var2Str(T x)
{
    stringstream type;
    type << x;
    return type.str();
}

#endif // FONCTIONS_H_INCLUDED
