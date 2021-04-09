#include "cartes.h"

#define MAXPSEUDO 7
#define MAXJOUEURS 10
#define TAILLE_SHM 1024
#define MAXCARTES 20

typedef struct {
    int id;
    int pid;
    int nombreCartes;
    char nom[MAXPSEUDO];
} t_joueur;

typedef struct {
    int nombreJoueurs;
    t_joueur joueur[MAXJOUEURS];

} t_partie;

typedef struct {

    t_carte cartes[108];
    int taille;
} t_tas;

key_t genererCleTas();


void affichageJoueur(t_joueur joueur);

void affectationNom(t_joueur *joueur, char *nom);

void listageJoueurs(t_joueur *joueur, int nbJoueurs);

void affichageJoueursClient(t_partie *partie);

void affichageCarteMilieu(t_carte carte);

void augmenterTailleTas(t_tas *tas);

void affichageNumeroMain(t_carte *cartes, int nombreCartes);

void ajouterCarteTas(t_tas *tas, t_carte carte);

t_tas *recupererTasPartagee(key_t cle2);

void affichageDerniereCarteTas(t_tas *tas);

void affichageCouleurMain(t_carte *cartes, int nombreCartes);

void affichageHautCartes(t_carte *cartes, int nombreCartes);

t_carte *recupererMain(t_joueur joueur, t_carte *mainJoueur);

void affichageMain2(t_carte *cartes, t_joueur joueur);

int partieTerminee(t_partie *partie);

void affichageClient(t_partie *partie, t_tas *tas, t_carte *mainDepart, int id);

void sendSigusr1Server(t_partie * partie);






