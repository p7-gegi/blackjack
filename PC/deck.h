#ifndef DECK_H
#define DECK_H
#include "carte.h"
#include <iostream>
#include <time.h>
using namespace std;
#include <unistd.h>

class deck
{
public :
    deck(int nombre);
    ~deck();
    void initialise_deck();
    void shuffle();
    void afficher_deck();
    void set_deck(int nombre);
    int get_nombre_deck();
    carte tirer_carte();
    int get_carte_tire();
private:
    carte *deck_total;
    int nombre_de_deck;
    int nb_carte_tire;
};

#endif