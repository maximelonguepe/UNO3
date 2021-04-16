#include "lib_serveur.h"

// TODO : gerer le cas ou la premiere carte est un joker
t_carte *cartes;
int tailleCarte;
int inverse = 0;

/**
 * retourne la taille de la main globale des joueurs
 * @param partie
 * @return
 */
int tailleMainPartagee(t_partie *partie) {
    int nbCartes = 0;
    for (int i = 1; i <= partie->nombreJoueurs; ++i) {
        nbCartes += partie->joueur[i].nombreCartes;
    }
    return nbCartes;
}

/**
 * recupere la derniere carte de la pioche et decremente la pioche
 * @param pioche
 * @return
 */
t_carte recupererDerniereCartePioche(t_pioche *pioche) {
    t_carte carte = pioche->pioche[pioche->nombreCarteRestante];
    pioche->nombreCarteRestante--;
    return carte;
}

/**
 * Distribue la main de départ dans la main globale
 * @param pioche
 * @param carteJoueurs
 * @param partie
 */
void distributionMainDepart(t_pioche *pioche, t_carte *carteJoueurs, t_partie *partie) {
    int j = 0;
    for (int i = 1; i <= partie->nombreJoueurs; ++i) {
        for (int k = 0; k < MAINDEPART; ++k) {
            carteJoueurs[j] = recupererDerniereCartePioche(pioche);
            j++;
        }
        partie->joueur[i].nombreCartes = MAINDEPART;
    }

}

/**
 * reto
 * @param partie
 * @param joueur
 * @return
 */
int nombreDebut(t_partie *partie, t_joueur joueur) {
    int nombre = 0;
    for (int i = 1; i < joueur.id; ++i) {

        nombre += partie->joueur[i].nombreCartes;
    }
    return nombre;

}

/**
 * retourne l'index de la carte demandée dans l'ensemble de cartes de tous les joueurs
 * @param partie
 * @param joueur
 * @param carte
 * @return
 */
int numeroCarte(t_partie *partie, t_joueur joueur, t_carte carte) {


    int debut = nombreDebut(partie, joueur);
    int fin = positionFinMainTableauMain(joueur, debut);
    for (int i = debut; i < fin; ++i) {

        if (strcmp(cartes[i].numero_carte, carte.numero_carte) == 0) {
            if (strcmp(cartes[i].couleur, carte.couleur) == 0) {
                return i;
            } else if (estCarteSpeciale(carte)) {
                return i;
            }
        }
    }
}

/**
 * permet de decaler la main vers la gauche lorsqu'on veut ajouter une carte au tas
 * @param partie
 * @param debut
 * @param joueur
 */
void decalage(t_partie *partie, int debut, t_joueur *joueur) {
    for (int i = debut; i < tailleMainPartagee(partie); ++i) {
        cartes[i] = cartes[i + 1];
    }
    joueur->nombreCartes--;
    tailleCarte = tailleMainPartagee(partie);
    cartes = realloc(cartes, tailleCarte * sizeof(t_carte));
}

/**
 * 
 * @param partie
 */
void decalagePioche(t_partie *partie) {
    partie->joueur[partie->jouant.id].nombreCartes++;
    cartes = (t_carte *) realloc(cartes, (tailleMainPartagee(partie) + 1) * sizeof(t_carte));
    int debut = nombreDebut(partie, partie->joueur[partie->jouant.id]);
    int fin = debut + partie->joueur[partie->jouant.id].nombreCartes;
    for (int i = tailleMainPartagee(partie); i >= fin; --i) {
        cartes[i] = cartes[i - 1];
    }
    cartes[fin - 1] = recupererDerniereCartePioche(&pioche);

}

int joueurSuivant(t_partie *partie, t_joueur joueur, int inverse) {
    switch (inverse) {
        case 0:
            if (joueur.id == partie->nombreJoueurs) return 1;
            else return joueur.id + 1;

        case 1:
            if (joueur.id == 1)return partie->nombreJoueurs;
            else return joueur.id - 1;
    }
}

void *functionThreadPartieServer(void *pVoid) {
    sleep(1);
    key_t clePartie;
    clePartie = genererClePartie();
    t_partie *partie = recupererPartiePartagee(clePartie);
    cartes = (t_carte *) calloc((partie->nombreJoueurs), sizeof(t_carte));
    cartes = (t_carte *) pVoid;
    struct sigaction newact;
    envoyerSignal1Joueur(partie->jouant);
    envoyerSignal2TousJoueursSauf1(*partie, partie->jouant);
    newact.sa_handler = MONSIGServer;
    sigemptyset(&newact.sa_mask);
    sigaction(SIGUSR1, &newact, NULL);
    sigaction(SIGALRM, &newact, NULL);
    sigaction(SIGUSR2, &newact, NULL);

    while (!partieTerminee(partie)) {
        sleep(2);
    }

    pthread_exit(0);
}


void MONSIGServer(int num) {

    struct data_t *memoryShared;
    int retour_jouer_carte;
    key_t key;
    key = genererClePartie();
    recupererPartiePartagee(key);
    t_partie *partie;
    key = genererClePartie();
    partie = recupererPartiePartagee(key);
    t_tas *tas;
    key_t cleTas = genererCleTas();
    tas = recupererTasPartagee(cleTas);
    switch (num) {
        case SIGUSR1:

            partie = recupererPartiePartagee(key);

            int numero = numeroCarte(partie, partie->jouant, recupererDerniereCarteTas(tas));
            decalage(partie, numero, &partie->joueur[partie->jouant.id]);
            if (strcmp(recupererDerniereCarteTas(tas).numero_carte, "in") == 0) {
                switch (inverse) {
                    case 0:
                        inverse = 1;
                        break;
                    case 1:
                        inverse = 0;
                        break;

                }
            }
            if (!partieTerminee(partie)) {
                partie->jouant = partie->joueur[joueurSuivant(partie, partie->jouant, inverse)];
                if (estPasse(recupererDerniereCarteTas(tas))) {
                    partie->jouant = partie->joueur[joueurSuivant(partie, partie->jouant, inverse)];
                } else if (estPlus2(recupererDerniereCarteTas(tas))) {
                    decalagePioche(partie);
                    decalagePioche(partie);
                } else if (estPlus4(recupererDerniereCarteTas(tas))) {
                    decalagePioche(partie);
                    decalagePioche(partie);
                    decalagePioche(partie);
                    decalagePioche(partie);
                }

                sendFifoCartes(partie, cartes, 0);
                envoyerSignal1Joueur(partie->jouant);

                envoyerSignal2TousJoueursSauf1(*partie, partie->jouant);

            } else envoyerSignal2TousJoueurs(partie);
            break;
        case SIGUSR2:
            decalagePioche(partie);
            partie->jouant = partie->joueur[joueurSuivant(partie, partie->jouant, inverse)];
            sendFifoCartes(partie, cartes, 0);
            envoyerSignal1Joueur(partie->jouant);
            envoyerSignal2TousJoueursSauf1(*partie, partie->jouant);
            break;

        case SIGALRM:
            //printf("sig recu sigalarm\n");

            break;

        default:
            break;
    }
}

void envoyerSignal1Joueurs(t_partie *partie) {
    for (int i = 1; i <= partie->nombreJoueurs; ++i) {
        envoyerSignal1Joueur(partie->joueur[i]);
    }
}

void lancerPartie(t_partie *partie) {
    envoyerSignal1Joueurs(partie);
}


void initTas(t_tas *tas) {
    tas->taille = 0;

}


void sendFifo2(t_joueur joueur, t_carte *carte, int signal) {
    ROUGE;
    REINIT;
    t_carte *main;
    key_t cle;
    cle = genererCleClient(joueur);
    main = recupererMainPartagee(cle, joueur);
    copie(main, carte, joueur.nombreCartes);
    if (signal == 1)
        envoyerSignal1Joueur(joueur);


}

void sendFifo3(t_joueur joueur, t_carte *carte) {
    t_carte *main;
    key_t cle;
    cle = genererCleClient(joueur);
    main = calloc(joueur.nombreCartes * 30, sizeof(t_carte));
    main = recupererMainPartagee(cle, joueur);
    copie(main, carte, joueur.nombreCartes);

}

int positionFinMainTableauMain(t_joueur joueur, int positionActuelle) {
    return joueur.nombreCartes + positionActuelle;
}

void selectionneMain(int debut, int fin, t_carte *mains, t_carte *destination) {//ICI2
    t_carte tab[fin - debut];
    int j = 0;
    for (int i = debut; i < fin; ++i) {
        afficherCarte(mains[i]);
        destination[j] = mains[i];
        j++;
    }

}

void envoyerSignal2Joueur(t_joueur joueur) {
    kill(joueur.pid, SIGUSR2);
}

void envoyerSignal2TousJoueursSauf1(t_partie partie, t_joueur joueur) {
    for (int i = 1; i <= partie.nombreJoueurs; ++i) {
        if (partie.joueur[i].id != joueur.id) {
            envoyerSignal2Joueur(partie.joueur[i]);
        }
    }
}

void envoyerSignal2TousJoueurs(t_partie *partie) {
    for (int i = 1; i <= partie->nombreJoueurs; ++i) {
        envoyerSignal2Joueur(partie->joueur[i]);
    }
}

void creerFichierTxt(t_joueur joueur) {
    char chaine[10];
    genererNomFichier(joueur, chaine);
    FILE *fichier = NULL;
    fichier = fopen(chaine, "w");
    if (fichier != NULL) {

        fclose(fichier); // On ferme le fichier qui a été ouvert
    }

}


void sendFifoCartes(t_partie *partie, t_carte *mains, int signal) {
    int positionActuelle = 0;
    int positionFinale = 0;
    for (int i = 1; i <= partie->nombreJoueurs; ++i) {
        printf("Joueur %d\n", i);
        t_carte cartes[partie->joueur[i].nombreCartes];
        positionActuelle = positionFinale;
        positionFinale = positionFinMainTableauMain(partie->joueur[i], positionActuelle);
        selectionneMain(positionActuelle, positionFinale, mains, cartes);
        creerFichierTxt(partie->joueur[i]);
        sendFifo2(partie->joueur[i], cartes, signal);
    }
}




