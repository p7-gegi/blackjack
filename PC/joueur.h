#ifndef JOUEUR_H
#define JOUEUR_H
#include "deck.h"
#define CAPACITE 15 //capacité maximale de la main du joueur
class joueur
{
public:
    joueur(deck &paquet);
    ~joueur();
    void hit();
    void stay();
    int doubler();
    void miser(int valeur);
    void set_argent(int valeur);
    int somme_main();
    void afficher_main();
    int gagner(int valeur_joueur,int valeur_dealer,bool blackjack);
    int get_argent();
    void nouvelle_main();
protected:
    carte *main;//maximum de 10 ou on peut l'allouer dynamiquement
    deck &jeu;
    int nombre_de_carte;
    int somme;
    int argent;
    int mise;
};


#endif 