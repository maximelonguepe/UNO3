#include <stdlib.h>
#include <string.h>
#include<stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <stdint.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <ctype.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "errno.h"
#define NBCOULEURS 4
#define NBPOSSIBILITES 15
#define NBBOUCLECARTESTANDARD 2
#define NBBOUCLEZERO 1
#define NBBOUCLECARTESJOKER 1
#define TAILLEPIOCHE 107
#define MAINDEPART 7
// Efface l'écran et replace le curseur en haut à gauche
#ifndef COULEURS
#define COULEURS
#define clrscr() printf("\033[H\033[2J")

// Sélection de couleurs
#define couleur(param) printf("\033[%sm",param)
/*   param devant être un const char *, vide (identique à "0") ou formé
     d'une où plusieurs valeurs séparées par des ; parmi
         0  réinitialisation         1  haute intensité (des caractères)
         5  clignotement             7  video inversé
         30, 31, 32, 33, 34, 35, 36, 37 couleur des caractères
         40, 41, 42, 43, 44, 45, 46, 47 couleur du fond
            les couleurs, suivant la logique RGB, étant respectivement
               noir, rouge, vert, jaune, bleu, magenta, cyan et blanc */


#endif
#define ROUGE couleur("31")
#define VERT couleur("32")
#define JAUNE couleur("33")
#define BLEU couleur("34")
#define REINIT couleur("0")
#define CLIGNOTE couleur("5")





typedef char couleur[2];
typedef char numero[4];

typedef struct {
    numero numero_carte;
    couleur couleur;

} t_carte;




typedef struct {
    t_carte pioche[117];
    int nombreCarteRestante;
} t_pioche;
void get_win_value(int *col, int *win);
void listeCouleurs(couleur * couleurs);
void listePossibilites(numero * possibilites);
void affecterCouleur(couleur c, t_carte *card);
void affecterNumero(numero n, t_carte *card);
void construireCarte(couleur couleur, numero num, t_carte *carte);
void afficherCarte(t_carte carte);
int Random(int _iMin, int _iMax);
void melangerPioche(t_pioche * pioche);
void afficherPioche(t_pioche pioche);
void ajouterALaPioche(t_carte carte, int indice,t_pioche * pioche);
void construirePioche(couleur * couleurs, numero * possibilite,t_pioche * pioche);
void print_droit(char* chaine);
void print_milieu(char* chaine);
void print_milieu_sans_retour(char* chaine);
void affichageCouleur(couleur couleur1);

