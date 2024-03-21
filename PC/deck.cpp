#include "deck.h"


deck::deck(int nombre)
{
    if (nombre<0)
        nombre = 1;
    else if(nombre>=8)
        nombre=8;
    nombre_de_deck=nombre;
    nb_carte_tire=0;
    set_deck(nombre);
}

deck::~deck()
{
    if (deck_total != nullptr) 
    {
        delete[] deck_total;
        deck_total = nullptr;
    }
}

void deck::set_deck(int nombre)
{
    if (nombre<0)
        nombre = 1;
   nombre_de_deck = nombre;
   initialise_deck();
}

int deck::get_nombre_deck()
{
    return nombre_de_deck;
}

void deck::initialise_deck()
{
deck_total = new carte[52 * nombre_de_deck];
int valeur=0;
string sorte = "0";
for(int i=0;i<4*nombre_de_deck;i++)
{
    if(i%4==1)
        sorte=u8"\u2665";//♥
    else if(i%4==2)
        sorte=u8"\u2660";//♠
    else if(i%4==3)
        sorte=u8"\u2666";//♦
    else if(i%4==0)
        sorte=u8"\u2663";//♣
    for(int j=0;j<13;j++)
    {   valeur = j+1;
        deck_total[i*13+j] = carte(valeur,sorte);
    }       
}
shuffle();
}


void deck::afficher_deck()
{
    carte card;
    for (int i=0;i<4*nombre_de_deck;i++)
    {
        cout << endl;
        for(int j =0;j<13;j++)
        {
            card = deck_total[i*13+j];
            cout<<card;
        }
    }
    cout<<endl;
}

void deck::shuffle()
{
srand(time(NULL));
//int Random_number = rand() % 52; //Generate random numbers between 0 and 51
int cards_total = 52 * nombre_de_deck;

for (int i = 0; i < cards_total; i++) 
{
        // Générer un index aléatoire entre i et cards_total - 1 inclusivement
        int j = i + rand() % (cards_total - i);

        // Échanger les cartes à l'index i et j
        carte temp = deck_total[i];
        deck_total[i] = deck_total[j];
        deck_total[j] = temp;
    }

}

carte deck::tirer_carte()
{
carte card;  
if (nb_carte_tire<52*nombre_de_deck)
    card = deck_total[nb_carte_tire];
else
    initialise_deck();
nb_carte_tire++;
return card;
}

int deck::get_carte_tire()
{
    return nb_carte_tire;
}