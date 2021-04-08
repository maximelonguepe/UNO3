#include "lib_serveur.h"

#define NBJOUEUR 2

int id = 0;


/*
 * definition des types et var globales utiles par la suite
 */
//TODO : envoyer les tubes aux differents client en utilisant leurs id
couleur couleurs[NBCOULEURS] = {"r", "b", "j", "v"};
numero possibilites[NBPOSSIBILITES] = {"+2", "+4", "pa", "jo", "in", "0", "1", "2", "3", "4", "5", "6", "7",
                                       "8", "9"};
t_pioche pioche;
t_carte *cartesJoueurs;


int main(int argc, char *argv[]) {
    key_t key;
    int shmid;
    t_partie *partie;
    t_tas *tas1 = malloc(sizeof(t_tas));

    //segment pour le tableau de joueurs
    key = ftok("serveur.c", 'R');
    shmid = shmget(key, TAILLE_SHM, 0644 | IPC_CREAT);
    partie = shmat(shmid, (void *) 0, 0);
    partie->nombreJoueurs = 0;
    printf("nombre joueur initialisé : %d\n", partie->nombreJoueurs);

    key_t cle;
    int media;
    t_carte *carte1;
    cle = ftok("carte.txt", 'R');
    media = shmget(cle, sizeof(carte1), 0644 | IPC_CREAT);
    carte1 = shmat(media, (void *) 0, 0);


    //tas = malloc(sizeof(t_tas));

    while (partie->nombreJoueurs < NBJOUEUR) {
        partie = shmat(shmid, (void *) 0, 0);

        sleep(2);
    }
    printf("%d joueurs connectés\n", partie->nombreJoueurs);
    for (int i = 1; i <= partie->nombreJoueurs; ++i) {
        affichageJoueur(partie->joueur[i]);
    }

    construirePioche(couleurs, possibilites, &pioche);
    pioche.nombreCarteRestante = 107;
    melangerPioche(&pioche);
    //afficherPioche(pioche);

    cartesJoueurs = (t_carte *) calloc(partie->nombreJoueurs * MAINDEPART, sizeof(t_carte));
    distributionMainDepart(&pioche, cartesJoueurs, partie);
    printf("Il y a %d cartes dans la main globale\n", partie->nombreJoueurs * MAINDEPART);
    printf("Il y a %d cartes dans la main du joueur\n", partie->joueur[2].nombreCartes);
    afficherPioche(pioche);
    lancerPartie(partie);
    REINIT;
    afficherPioche(pioche);
    // recupération de la derniere carte et ajout au tas.
    *carte1 = pioche.pioche[pioche.nombreCarteRestante];
    pioche.nombreCarteRestante--;
    clrscr();
    initTas(tas1);
    ajouterCarteTas(tas1, *carte1);
    afficherCarte(tas1->cartes[0]);

    //segment pour le tas de carte

    key_t cle2;
    t_tas *tas;
    cle2 = genererCleTas();
    tas = recupererTasPartagee(cle2);
    initTas(tas);
    ajouterCarteTas(tas, *carte1);
    envoyerSignal1Joueurs(partie);
    //afficherCarte(tas->cartes[0]);
    //sendFifoAllPlayers(partie);
    sendFifoCartes(partie,cartesJoueurs);


}

