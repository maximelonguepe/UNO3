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

void ajouterCarteTas(t_tas *tas, t_carte carte);

t_tas *recupererTasPartagee(key_t cle2);

key_t genererCleTas();

void affichageDerniereCarteTas(t_tas *tas);



