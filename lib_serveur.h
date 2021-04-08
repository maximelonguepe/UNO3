#include "lib_joueur.h"

typedef struct {
    char nom[MAXPSEUDO] ;
    int expediteur;
} t_message;
void lancerPartie(t_partie *partie);
void distributionMainDepart(t_pioche *pioche, t_carte *carteJoueurs, t_partie * partie);
int nombreDebut(t_partie *partie, t_joueur joueur);
int joueurSuivant(t_partie* partie,t_joueur joueur, int inverse);
void lancerPartie(t_partie * partie);
void sendFifo(t_partie * partie,t_carte * carte);
void initTas(t_tas * tas);
void envoyerSignal1Joueurs(t_partie *partie);
void sendFifo2();
