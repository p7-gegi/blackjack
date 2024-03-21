#include "dealer.h"

dealer::dealer(deck &paquet) : joueur(paquet), jeu(paquet)
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

void dealer::afficher_main(bool dernier)
{
    cout<<endl;
    carte card;
    for(int i=0;i<nombre_de_carte;i++)
    {
            card = main[i];
            sleep(1.4);
            cout<<card;
            if(somme_main()<17&&dernier) //dernier tour donc hit et affiche la main du dealer
            {
                hit();
            }
    }
        cout<<endl;
}

void dealer::hit()
{
    main[nombre_de_carte] = jeu.tirer_carte();
    nombre_de_carte++;
    somme_main();
}

