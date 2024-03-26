#include "deck.h"
#include "dealer.h"
#include <limits>
void vider_cin();
#include <iostream>
#include <string>
using namespace std;

/*-------------------------- Librairies externes ----------------------------*/
#include "include/serial/SerialPort.hpp"
#include "include/json.hpp"
using json = nlohmann::json;

/*------------------------------ Constantes ---------------------------------*/
#define BAUD 96000           // Frequence de transmission serielle
#define MSG_MAX_SIZE 1024   // Longueur maximale d'un message

/*------------------------- Prototypes de fonctions -------------------------*/
bool SendToSerial(SerialPort *arduino, json* j_msg_send,int gagner);
bool RcvFromSerial(SerialPort *arduino, string &msg);
int etat_bouton(json* j_msg_rcv); //(adresse j_msg_rcv)

/*---------------------------- Variables globales ---------------------------*/
SerialPort *arduino; //doit etre un objet global!
int main()
{
    int nombre=0;
    int x=0;
    int previous=1;
    int jouer=1;
    int mise =0;
    int remettre_argent=0;
    bool premier_tour = true;
    int argent =0;
    int button = 1;//envoie de 1 par défaut à 5V
    int valeur_joystick;
    json j_msg_send, j_msg_rcv;//création objet JSON
    int gagner=0;//initialise gagner pour envoyer un message à l'arduino pour le LCD
    int temps=300;//temps en milli secondes

//INITIALISATION DE LA COMMUNICATION SERIE AVEC ARDUINO
    string raw_msg;
    string com;
    std::cout << "Entrer le port de communication du Arduino: ";
    cin >> com;
    arduino = new SerialPort(com.c_str(), BAUD);    
    if(!arduino->isConnected()){
        cerr << "Impossible de se connecter au port "<< string(com) <<". Fermeture du programme!" <<endl;
        exit(1);
    }
//FIN DE L'INITIALISATION DE LA COMMUNICATION AVEC ARDUINO
        std::cout << "Avec combien de paquets voulez-vous jouer? ";
        std::cout<<endl<<"Joystick vers le haut egal +1 paquet et vers le bas egal -1 paquet. Appuyez sur hit quand vous etes pret a jouer.";
    do 
    {
        if (cin.fail()) 
            vider_cin();
        //cin >> nombre;
        valeur_joystick=etat_bouton(&j_msg_rcv);

        //std::cout<<endl<<valeur_joystick;
        if(valeur_joystick>=45&&valeur_joystick<=135)
        {   
            nombre++;
            std::cout<<endl<<"Vous etes rendu a "<<nombre<<"paquets";
        }
        else if(valeur_joystick<=315&&valeur_joystick>=225)
        {   
            nombre--;
            std::cout<<endl<<"Vous etes rendu a "<<nombre<<"paquets";
        }
        if (nombre > 8&&valeur_joystick==1) {
            std::cout << endl << "Entrez une valeur inferieure à 8. ";
        }
        if (nombre < 0&&valeur_joystick==1) {
            std::cout << endl << "Entrez une valeur superieure à 0. ";
        }
        Sleep(temps);
    } while(valeur_joystick!=1|| nombre < 0 || nombre > 8) ;//(cin.fail() || nombre < 0 || nombre > 8)
    deck paquet(nombre);
    joueur joueur1(paquet);
    dealer dealer1(paquet);
    valeur_joystick=0;
    std::cout <<endl<< "Combien d'argent voulez-vous mettre dans le solde? : ";
    std::cout << "Joystick vers le haut egal +100$ et vers le bas egal -100$. Appuyez sur hit quand vous etes pret a jouer.";
    do
    {
        if (cin.fail()) 
            vider_cin();
        //cin>>argent;
        valeur_joystick=etat_bouton(&j_msg_rcv);
        if(valeur_joystick>=45&&valeur_joystick<=135)
        {   
            argent+=100;
            std::cout<<endl<<"Vous etes rendu a "<<argent<<"$";
        }
        else if(valeur_joystick<=315&&valeur_joystick>=225)
        {   
            argent-=100;
            std::cout<<endl<<"Vous etes rendu a "<<argent<<"$";
        }
        if(argent<=0&&valeur_joystick==1)
            std::cout<<"Entrez une valeur superieur à 0";
        Sleep(temps);
    }while (argent<=0||valeur_joystick!=1); 
    joueur1.set_argent(argent);
    std::cout<<endl<<"Nous avons un jeu de carte à "<<paquet.get_nombre_deck()<<" paquets"<<endl;

    //std::cout<<"voici le paquet"<<endl;

    //paquet.afficher_deck();
    while(jouer==1)
    {
        jouer=-1;//don't remove
        bool blackjack=true;
        joueur1.nouvelle_main();
        dealer1.nouvelle_main();
        valeur_joystick=0;
        std::cout<<endl<<"Votre solde est de :"<<joueur1.get_argent()<<"$. Combien voulez-vous miser? :" ;
        int dixieme_argent = 20;
        std::cout << "Joystick vers le haut egal +"<<dixieme_argent<<"$ et vers le bas egal -"<<dixieme_argent<<"$. Appuyez sur hit quand vous etes pret a jouer.";
        do
            {
                //ider_cin();
                if((mise<=0||mise>joueur1.get_argent())&&valeur_joystick==1)
                    std::cout<<endl<< "Entrez une valeur superieure à 0 et inferieure ou egale au solde" ;
                //cin>>mise;
                valeur_joystick=etat_bouton(&j_msg_rcv);
                if(valeur_joystick>=45&&valeur_joystick<=135)
                {   
                    mise+=dixieme_argent;
                    std::cout<<endl<<"La mise est de "<<mise;
                }
                else if(valeur_joystick<=315&&valeur_joystick>=225)
                {   
                    mise-=dixieme_argent;
                    std::cout<<endl<<"La mise est de "<<mise;
                }
                Sleep(temps);
            }while(cin.fail()||mise<=0||valeur_joystick!=1|| mise>joueur1.get_argent());
        joueur1.miser(mise);
        std::cout<<endl<<"La main du joueur"<<endl;
        joueur1.hit();
        dealer1.hit();
        sleep(1);
        std::cout<<endl<<"La main du dealer"<<endl;
        dealer1.afficher_main(false);
        sleep(1);
        std::cout<<endl<<"La main du joueur"<<endl;
        joueur1.hit();
        dealer1.hit();
        std::cout<<endl<<"la somme de la main du joueur est : "<<joueur1.somme_main()<<endl;
        while(joueur1.somme_main()<21)
        {
            blackjack=false;
                if (premier_tour)
                    std::cout<<"1 pour hit, 2 pour stand, 3 pour doubler :";
                else
                    std::cout<<"1 pour hit, 2 pour stand :" ;
            do 
            {
            //cin >> x;
                    // Effacer le message précédent
                    
                    /*if (!j_msg_rcv.empty())
                        j_msg_rcv.clear();

                    // Recevoir le message depuis l'Arduino
                        if (!RcvFromSerial(arduino, raw_msg)) 
                            cerr << "Erreur lors de la reception du message." << endl;
                        // Si un message a été reçu
                        if (raw_msg.size() > 0) {
                            j_msg_rcv = json::parse(raw_msg);
                            button = j_msg_rcv["button"];
                            if (button == 0){  // Modifier la variable x en fonction de l'état du bouton
                                x = 1;
                                std::cout<<j_msg_rcv;
                            }
                        }*/
            
            if(x!=0&&x!=previous)
            {
                std::cout<<endl<<"Lache le pot et fais ton choix!";
                previous=x;
            }
            x = etat_bouton(&j_msg_rcv);
            //std::cout<<x;
                
                //std::cout<<"La valeur de bouton est :"<<button;
            
            if (cin.fail() || x < 1 || x > 3 || (x == 3 && joueur1.get_argent()-mise < 0)) {
                if(x == 3 && joueur1.get_argent()-mise<0&&premier_tour)
                        std::cout<<endl<<"Fonds insuffisant pour doubler. Veuillez saisir à nouveau : " ;
                /*else
                {
                std::cout << endl << "La valeur est hors limite. Veuillez saisir à nouveau : ";
                vider_cin();
                }*/
            }
            Sleep(temps);    
            } while (x < 1 || x > 3|| (x == 3 && joueur1.get_argent()-mise < 0)); 
            button=1;
            switch(x)
            {
                case 1:
                {
                    joueur1.hit();
                    std::cout<<endl<<"la somme de la main du joueur est : "<<joueur1.somme_main()<<endl;
                }
                case 2:
                {
                    break;
                }
                case 3:
                {   
                    if (!premier_tour)
                        break;
                    if(!joueur1.doubler())
                        std::cout<<endl<<"Fonds insuffisant pour doubler" ; 
                }
            }
            if(x==2 || x==3)
            {
                if(!premier_tour && x==3)
                    continue;
                else 
                    break;
            }
            premier_tour=false;
        }
        std::cout<<endl<<endl<<"La main du dealer"<<endl;
        dealer1.afficher_main(true);
        std::cout<<endl<<"la somme de la main du dealer est : "<<dealer1.somme_main()<<endl;
        gagner = joueur1.gagner(joueur1.somme_main(),dealer1.somme_main(),blackjack);
        SendToSerial(arduino,&j_msg_send,gagner);
        std::cout<<endl<<"Vous etes rendu à "<<joueur1.get_argent()<<"$" ;
        std::cout<<endl<<"Voulez vous rejouer ? hit pour oui stand pour non :" ;
        do
        {   
            jouer = etat_bouton(&j_msg_rcv);
            /*if (cin.fail())
                    vider_cin();
            cin>>jouer;*/
            Sleep(temps);
        }while(cin.fail()||(jouer!=2&&jouer!=1));
        if(joueur1.get_argent()==0&&jouer==1)
        {
            std::cout<<endl<<"Vous n'avez plus d'argent desole. Voulez vous remettre de l'argent dans votre solde? hit pour oui stand pour non : " ;
            do
            {   
            remettre_argent = etat_bouton(&j_msg_rcv);
                /*if (cin.fail())
                    vider_cin();
                cin>>remettre_argent;*/
                Sleep(temps);
            }while(cin.fail()||(remettre_argent!=2&&remettre_argent!=1));
            if(remettre_argent==1)
            {
                std::cout<<endl<<"Combien d'argent voulez vous remettre? :" ;
                cin>>mise;
                joueur1.set_argent(mise);
            }
            else
            {
                std::cout<<endl<<"Merci d'avoir joue.";
                jouer=0;
            }
        }
        premier_tour=true;
        button=1;
        x=0;
    }
    return 0;
}

void vider_cin()
{
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignorer les caractères restants dans le buffer
}

bool SendToSerial(SerialPort *arduino, json* j_msg_send,int gagner){
    (*j_msg_send)["bar"] = gagner;
    std::cout<<*j_msg_send;
    string msg = j_msg_send->dump();
    bool ret = arduino->writeSerialPort(msg.c_str(), msg.length());
    return ret;
}

bool RcvFromSerial(SerialPort *arduino, string &msg){
    string str_buffer;
    char char_buffer[MSG_MAX_SIZE];
    int buffer_size;
    msg.clear(); // clear string

    buffer_size = arduino->readSerialPort(char_buffer, MSG_MAX_SIZE);
    str_buffer.assign(char_buffer, buffer_size);
    msg.append(str_buffer);//msg contient un string de l'objet JSON
    //std::cout<<endl<<"Le message est : "<<msg;
    return true;
}
int etat_bouton(json* j_msg_rcv)
{
    int valeur_cle;
    string raw_msg;
    string cle;
    string response;
    //if (!(*j_msg_rcv).empty())
      //   (*j_msg_rcv).clear();
// Recevoir le message depuis l'Arduino
    if (!RcvFromSerial(arduino, raw_msg)) 
        cerr << "Erreur lors de la reception du message." << endl;
    // Si un message a été reçu
    if (raw_msg.size() > 0) {
        try{
            *j_msg_rcv = json::parse(raw_msg);
            for (auto i = (*j_msg_rcv).begin(); i != (*j_msg_rcv).end(); i++) {
                cle = i.key(); // Obtenir la clé actuelle
                valeur_cle = i.value(); // Obtenir la valeur actuelle
                if(valeur_cle==1 && cle!="pot")//un bouton a été pesé
                    break;
                else if(cle=="pot"&&valeur_cle!=0)//le pot a été tourné 
                    break;
                else if(cle=="test")
                    break;
            }
            //Sleep(100);
            std::cout<<*j_msg_rcv;
        } catch (const json::parse_error& e) {}
            //{cerr << "Erreur de parsing JSON : " << e.what() << endl;} Aucun affichage supplémentaire dans le terminal en cas d'échec

        if(cle=="hit" && valeur_cle==1)
            return 1;
        else if(cle == "stand"&& valeur_cle==1)
            return 2;
        else if (cle == "pot" && valeur_cle!=0)
            return valeur_cle;   
        else if(cle == "double"&& valeur_cle==1)
            return 3;
        else if(cle=="test")
            return 4;
    }
return 0;
}
