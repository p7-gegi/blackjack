#ifndef DEALER_H
#define DEALER_H
#include "joueur.h"

class dealer: public joueur
{
public:
    dealer(deck &paquet);
    void afficher_main(bool dernier);
    void hit();
protected:
    deck &jeu;
    int somme;
};
#endif