#include "carte.h"

carte::carte(int nombre, string symbole)
{
    if(13>=nombre>=1)
        valeur = nombre;
    else
        valeur=0;
    if (symbole!="0")
        sorte = symbole;
    else
        sorte = "0";
}
carte::carte()
{
    valeur=0;
    sorte="0";
}

int carte::get_valeur()
{
    return valeur;
}

string carte::get_sorte()
{
    return sorte;
}
ostream& operator<< (ostream &os, carte &card)
{
    //os<<card.valeur<<card.sorte<<" " ;
os << "╔═════════╗" << endl;
os << "║ ";
if(card.valeur == 1)
    os << "A" << "       ";
else if (card.valeur < 10 && card.valeur!=1)
    os << card.valeur << "       ";
else if(card.valeur ==10)
    os<< card.valeur << "      ";
else if (card.valeur == 11) {
    os << "J" << "       ";
} else if (card.valeur == 12) {
    os << "Q" << "       ";
} else if (card.valeur == 13) {
    os << "K" << "       ";
}
os << "║" << endl;
os << "║         ║" << endl;
os << "║    " << card.sorte << "    ║" << endl;
os << "║         ║" << endl;
os << "║       ";
if(card.valeur == 1)
    os << "A" << " ";        
else if (card.valeur < 10 && card.valeur!=1) 
    os << card.valeur << " ";
else if (card.valeur==10)
    os<< card.valeur;
else if (card.valeur == 11) 
    os << "J" << " ";
else if (card.valeur == 12) 
    os << "Q" << " ";
else if (card.valeur == 13) 
    os << "K" << " ";
os << "║" << endl;
os << "╚═════════╝" << endl;
return os;
}