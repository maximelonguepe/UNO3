#include "cartes.h"

/*
 * definition des fonctions
 */

/*
 * liste les differentes couleurs possible
 */
void listeCouleurs(couleur *couleurs) {
    for (int i = 0; i < 4; ++i) {
        printf("%s\n", couleurs[i]);
    }
}

/*
 *  * liste les differentes possibilites / numeros possible

 */
void listePossibilites(numero *possibilites) {
    for (int i = 0; i < 15; ++i) {
        printf("%s\n", possibilites[i]);
    }
}

/*
 * permet d'affecter une couleur a une carte peut etre utile si on change par la suite le type de la couleur
 */
void affecterCouleur(couleur couleur, t_carte *card) {
    strcpy(card->couleur, couleur);
}

/*
 * permet d'affecter un numéro/possibilté a une carte peut etre utile si on change par la suite le type du numero
 */

void affecterNumero(numero n, t_carte *card) {
    strcpy(card->numero_carte, n);
}


/*
 * permet de construire une carte a partir d'une couleur et d'un numero
 */
void construireCarte(numero num, couleur c,  t_carte *carte) {

    //printf("%s\n",c);
    affecterCouleur(c, carte);
    affecterNumero(num, carte);

}
void affichageCouleur(couleur couleur1){
    switch (couleur1[0]) {
        case 'r':
            ROUGE;
            break;
        case 'j':
            JAUNE;
            break;
        case 'b':
            BLEU;
            break;
        case 'v':
            VERT;
            break;
        default:
            REINIT;
            break;
    }
}
/*
 * permet d'afficher une carte : va etre amélioré par la suite
 */
void afficherCarte(t_carte carte) {


    affichageCouleur(carte.couleur);
    printf("%s\n", carte.couleur);
    printf("%s\n", carte.numero_carte);
    printf("---\n");
    REINIT;
}

/*
 * permet d'afficher la pioche : je doute que cette fonctionnalité soit utile mais jen avais lutilite pour mes tests
 */
void afficherPioche(t_pioche pioche) {
    printf("Nombre carte pioche : %d\n", pioche.nombreCarteRestante);

    for (int i = 0; i <= pioche.nombreCarteRestante; i++) {
        afficherCarte(pioche.pioche[i]);
    }
}

int Random(int _iMin, int _iMax) {
    return (_iMin + (rand() % (_iMax - _iMin + 1)));
}

void melangerPioche(t_pioche *pioche) {
    int iRandom;
    int iRandom2;
    int i;
    t_carte temp;
    srand(time(NULL));
    for (i = 0; i <= TAILLEPIOCHE; i++) {
        iRandom = Random(0, TAILLEPIOCHE);
        iRandom2 = Random(0, TAILLEPIOCHE);
        temp = pioche->pioche[iRandom];
        pioche->pioche[iRandom] = pioche->pioche[iRandom2];
        pioche->pioche[iRandom2] = temp;

    }
    printf("%d cartes ont été mélangées\n", i);


}

/*
 * permet d'ajouter une carte à la pioche
 */
void ajouterALaPioche(t_carte carte, int indice, t_pioche *pioche) {
    pioche->pioche[indice] = carte;
}


void construirePioche(couleur *couleurs, numero *possibilites, t_pioche *pioche) {
    t_carte carteP;
    //setUpNombreCarte(&pioche);
    //printf("%d\n",pioche.nombreCarteRestante);
    int compteur = 0;

    for (int i = 0; i < NBCOULEURS; i++) {
        for (int j = 0; j < NBPOSSIBILITES; j++) {
            if (strcmp(possibilites[j], "0") == 0) {
                construireCarte(possibilites[j], couleurs[i], &carteP);
                for (int k = 0; k < NBBOUCLEZERO; k++) {
                    // afficherCarte(carteP);
                    ajouterALaPioche(carteP, compteur, pioche);
                    compteur++;
                }
            } else if (strcmp(possibilites[j], "0") != 0 && strcmp(possibilites[j], "jo") != 0 &&
                       strcmp(possibilites[j], "+4") != 0) {
                construireCarte(possibilites[j], couleurs[i], &carteP);
                //afficherCarte(carteP);
                for (int k = 0; k < NBBOUCLECARTESTANDARD; k++) {
                    // afficherCarte(carteP);
                    ajouterALaPioche(carteP, compteur, pioche);
                    compteur++;
                }
            } else if (strcmp(possibilites[j], "jo") == 0 || strcmp(possibilites[j], "+4") == 0) {
                construireCarte(possibilites[j], "", &carteP);
                //afficherCarte(carteP);
                for (int k = 0; k < NBBOUCLECARTESJOKER; k++) {
                    //afficherCarte(carteP);
                    ajouterALaPioche(carteP, compteur, pioche);
                    compteur++;
                }
            }
        }

    }
    printf("%d cartes ont été cree\n", compteur+1);

}

void get_win_value(int *col, int *win)
{
    struct winsize wins;

    ioctl(0, TIOCGWINSZ, &wins);
/* aussi
   ioctl(0, TIOCGSIZE, &win);
   C'est la meme chose
*/
    *col = wins.ws_col;
    *win = wins.ws_row;
}


void print_droit(char* chaine){
    int col,win;
    get_win_value(&col,&win);
    for (int i = 0; i < col-strlen(chaine); ++i)
    {
        printf(" ");
    }
    printf("%s\n",chaine );

}
void print_milieu(char* chaine){
    int col,win;
    get_win_value(&col,&win);
    for (int i = 0; i < (col-strlen(chaine))/2; ++i)
    {
        printf(" ");
    }
    printf("%s\n",chaine );

}
void print_milieu_sans_retour(char* chaine){
    int col,win;
    get_win_value(&col,&win);
    for (int i = 0; i < (col-strlen(chaine))/2; ++i)
    {
        printf(" ");
    }
    printf("%s",chaine );

}





