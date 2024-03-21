#include "joueur.h"


joueur::joueur(deck &paquet) : jeu(paquet)
{
    carte card;
    main = new carte[CAPACITE];
    for(int i=0;i<CAPACITE;i++)
        main[i]=card;
    nombre_de_carte = 0;
    argent = 0;
    somme = 0;
    mise =0;
}
joueur::~joueur()
{
    if (main!=nullptr)
    {
        delete[] main;
        main = nullptr;
    }
}
void joueur::hit()
{
    if (somme_main()<21)
    {   
        main[nombre_de_carte] = jeu.tirer_carte();
        nombre_de_carte++;
    }
    afficher_main();
}
void joueur::stay()
{
    
}
//return 1 doubler ok else return 0
int joueur::doubler()
{
    if(argent-mise>=0)
    {   
        argent-=mise;
        mise = mise*2;
        hit();
        return 1;
    }
    else 
        return 0;
}
void joueur::miser(int valeur)
{
    mise = valeur;
    argent -= mise;
}
void joueur::set_argent(int valeur)
{
    if(valeur<0)
        valeur = 0;
    argent = valeur;
}
int joueur::somme_main()
{
    somme = 0;
    int nb_as = 0; // Compter le nombre d'As dans la main
    carte card;
    for(int i = 0; i < nombre_de_carte; i++) {
        card = main[i];
        int valeur = card.get_valeur();
        if(valeur >= 10) {
            somme += 10; // Figures valent 10
        } else if (valeur == 1) {
            nb_as++; // Compter les As
            somme += 1; // Ajouter la valeur 1 pour l'instant
        } else {
            somme += valeur;
        }
    }
    // Traiter les As en ajoutant 10 si possible
    while (nb_as > 0 && somme + 10 <= 21) {
        somme += 10;
        nb_as--;
    }
    return somme;
}
void joueur::afficher_main()
{
    carte i = main[0];
    int index = 0;
    cout<<endl;
    carte card;
    carte temp;
    while(i.get_valeur() != temp.get_valeur())
    {
        sleep(1);
        card = main[index];
        //cout<<main[index]<<" ";
        cout<<card;
        index++;
        i = main[index];
    }
    cout<<endl;
}
//return 0 perdu, 1 égal, 2 gagné, 3 blackjack
int joueur::gagner(int valeur_joueur,int valeur_dealer, bool blackjack)
{
    if(valeur_joueur<=21)
    {
        if (blackjack)
        {
            argent += int(2.5*mise);
            cout<<endl<<"Gagnant, Gagnant Buffet Géant";
            return 3;
        }
        else if (valeur_joueur==valeur_dealer)
        {
            argent+=mise;
            cout<<endl<<"Push, mieux que rien";
            return 1;
        }
        else if (valeur_joueur>valeur_dealer||valeur_dealer>21)
        {
            argent+=2*mise;
            cout<<endl<<"Bravo, vous avez gagné";
            return 2;
        }
        else{
            cout<<endl<<"Meilleure chance la prochaine fois"; 
            return 0;
        }
    }
    else{
        cout<<endl<<"Meilleure chance la prochaine fois";
        return 0;
    }
}
int joueur::get_argent()
{
    return argent;
}
void joueur::nouvelle_main()
{
if (main!=nullptr)
    {
        delete[] main;
        main = nullptr;
    }
carte card;
main = new carte[CAPACITE];
for(int i=0;i<CAPACITE;i++)
    main[i]=card;
nombre_de_carte=0;
}


