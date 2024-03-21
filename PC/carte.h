#include <string>
#include <iostream>
using namespace std;
class carte
{
public:
    carte(int nombre,string symbole);
    carte();
    int get_valeur();
    string get_sorte();
    friend ostream& operator<< (ostream &os, carte &card);
private:
    string sorte;//coeur,pique,carreau,trefle
    int valeur;//0 à 13
};
