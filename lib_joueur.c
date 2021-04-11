#include "lib_joueur.h"

int *idClient;
t_envoi *envoi;
couleur coul[NBCOULEURS] = {"r", "b", "j", "v"};
numero possi[NBPOSSIBILITES] = {"+2", "+4", "pa", "jo", "in", "0", "1", "2", "3", "4", "5", "6", "7",
                                "8", "9"};

void affichageJoueurJouant(t_partie *partie) {
    printf("Joueur en train de jouer : %s\n", partie->joueur[partie->jouant.id].nom);
}

void affichageJoueur(t_joueur joueur) {
    printf("%s\n", joueur.nom);
}

void affectationNom(t_joueur *joueur, char *nom) {
    strcpy(joueur->nom, nom);
}

void listageJoueurs(t_joueur *joueur, int nbJoueurs) {
    for (int i = 1; i <= nbJoueurs; ++i) {
        affichageJoueur(joueur[i]);
    }
}

int estCouleur(char *couleur) {
    for (int i = 0; i < NBCOULEURS; ++i) {
        if (strcmp(couleur, coul[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

int estpossibilite(char *possiblitess) {
    for (int i = 0; i < NBPOSSIBILITES; ++i) {
        if (strcmp(possiblitess, possi[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

int existe(char *chaine) {
    if (strcmp(chaine, "pioche") == 0) {
        return 1;
    }
    if (strlen(chaine) < 1 || strlen(chaine) > 3)return 0;
    int couleurs = 0;
    char chainechouleur[5];
    char chainepossibilites[5];
    if (strcmp("jo", chaine) == 0 || strcmp("+4", chaine) == 0) {
        return 1;
    } else {
        chainechouleur[0] = chaine[0];
        chainechouleur[1] = '\0';
        if (!estCouleur(chainechouleur)) {
            return 0;
        } else {

            chainepossibilites[0] = chaine[1];
            if (strlen(chaine) == 2) {
                chainepossibilites[1] = '\0';

            } else {
                chainepossibilites[1] = chaine[2];
                chainepossibilites[2] = '\0';
            }
            return estpossibilite(chainepossibilites);

        }

    }


}


int partieTerminee(t_partie *partie) {

    for (int i = 1; i <= partie->nombreJoueurs; ++i) {
        if (partie->joueur[i].nombreCartes == 0)return 1;
    }
    return 0;
}

void affichageJoueursClient(t_partie *partie) {
    char str[3];
    for (int i = 1; i <= partie->nombreJoueurs; ++i) {
        print_droit(partie->joueur[i].nom);
        sprintf(str, "%d", partie->joueur[i].nombreCartes);
        print_droit(str);
        print_droit("-----\n\n");
    }
}

void affichageCarteMilieu(t_carte carte) {
    affichageCouleur(carte.couleur);
    char couleur[25];
    char numero[25];
    strcpy(couleur, "- ");
    strcat(couleur, carte.couleur);
    strcat(couleur, " -");
    strcpy(numero, "- ");
    strcat(numero, carte.numero_carte);
    strcat(numero, " -");

    print_milieu("-----");
    print_milieu(couleur);
    print_milieu(numero);

    print_milieu("-----");
    REINIT;
}


void augmenterTailleTas(t_tas *tas) {
    tas->taille++;
}

void ajouterCarteTas(t_tas *tas, t_carte carte) {
    //tas->cartes = realloc(tas->cartes, tas->taille + 1 * sizeof(t_carte));
    tas->cartes[tas->taille] = carte;
    augmenterTailleTas(tas);

}

key_t genererCle(char *chaine) {
    key_t cle2 = ftok(chaine, 'R');
    return cle2;
}

key_t genererCleClient(t_joueur joueur) {
    char chaine[10];
    genererCle(genererNomFichier(joueur, chaine));
}


char *genererNomFichier(t_joueur joueur, char *chaine) {
    char str[3];
    sprintf(str, "%d", joueur.id);
    strcpy(chaine, "");
    strcat(chaine, str);
    strcat(chaine, ".txt");
    return chaine;
}


key_t genererClePartie() {
    return genererCle("partie.txt");
}

t_partie *recupererPartiePartagee(key_t key) {
    int shmid;
    t_partie *partie;
    shmid = shmget(key, TAILLE_SHM, 0644 | IPC_CREAT);
    partie = shmat(shmid, (void *) 0, 0);
    return partie;
}

t_carte *recupererMainPartagee(key_t key, t_joueur joueur) {
    int shmid;
    t_carte *carte;
    shmid = shmget(key, joueur.nombreCartes, 0644 | IPC_CREAT);
    carte = shmat(shmid, (void *) 0, 0);
    return carte;
}


key_t genererCleTas() {
    return genererCle("tas.txt");
}

t_tas *recupererTasPartagee(key_t cle2) {
    t_tas *tas;
    int media2 = shmget(cle2, sizeof(t_tas), 0777 | IPC_CREAT);
    tas = shmat(media2, (void *) 0, 0);
    return tas;
}

t_carte recupererDerniereCarteTas(t_tas *tas) {
    return tas->cartes[tas->taille - 1];
}


void affichageDerniereCarteTas(t_tas *tas) {
    affichageCarteMilieu(tas->cartes[tas->taille - 1]);
}

void affichageHautCartes(t_carte *cartes, int nombreCartes) {
    char chaine[10000];
    strcpy(chaine, "");
    for (int i = 0; i < nombreCartes; ++i) {

        //strcat(chaine,"-----\t");
        affichageCouleur(cartes[i].couleur);
        printf("-----\t");
        REINIT;
    }
    printf("\n");
}

void affichageCouleurMain(t_carte *cartes, int nombreCartes) {
    for (int i = 0; i < nombreCartes; ++i) {
        affichageCouleur(cartes[i].couleur);
        printf("- ");
        printf("%s", cartes[i].couleur);
        printf(" -\t");
        REINIT;
    }
    printf("\n");
}

void affichageNumeroMain(t_carte *cartes, int nombreCartes) {
    for (int i = 0; i < nombreCartes; ++i) {
        affichageCouleur(cartes[i].couleur);
        printf("- ");
        printf("%s", cartes[i].numero_carte);
        printf(" -\t");
        REINIT;
    }
    printf("\n");

}

int nombreDeCarteSuperieurLigne(int nombreCartes) {
    int tailletxt = 0;
    int col;
    int lignes;
    for (int i = 0; i < nombreCartes; ++i) {
        tailletxt += 10;
    }
    get_win_value(&col, &lignes);

    if (col < tailletxt) {
        return 1;
    }
    return 0;
}


void affichageMain2(t_carte *cartes, t_joueur joueur) {


    affichageHautCartes(cartes, joueur.nombreCartes);
    affichageCouleurMain(cartes, joueur.nombreCartes);
    affichageNumeroMain(cartes, joueur.nombreCartes);
    affichageHautCartes(cartes, joueur.nombreCartes);

}

void affichageClient(t_partie *partie, t_tas *tas, t_carte *mainDepart, int id) {
    affichageJoueursClient(partie);
    affichageDerniereCarteTas(tas);
    affichageMain2(mainDepart, partie->joueur[id]);

}

void affichageClientPartieCommencee(t_partie *partie, t_tas *tas, t_carte *mainDepart, int id) {
    clrscr();
    affichageJoueurJouant(partie);
    affichageClient(partie, tas, mainDepart, id);
}


t_carte *recupererMain(t_joueur joueur, t_carte *mainJoueur) {
    int sortieTube;
    char str[3];
    sprintf(str, "%d", joueur.id);
    char myfifo[9];
    strcpy(myfifo, "");
    strcat(myfifo, str);
    strcat(myfifo, ".fifo");
    t_carte carte;
    t_carte cartes[joueur.nombreCartes];
    //printf("ouverture de : %s\n", myfifo);
    if (mkfifo(myfifo, 0777) != 0) {
        /*printf("SERVEUR - Impossible de créer le tube nommé \n");*/
        if (errno == EEXIST) {
            // printf("SERVEUR - le tube nommé existe déjà \n");
        } else {
            exit(EXIT_FAILURE);
        }
    }
    if ((sortieTube = open(myfifo, O_RDONLY)) == -1) {
        printf("SERVEUR - Impossible d'ouvrir la sortie du FIFO \n");
        exit(EXIT_FAILURE);
    }

    read(sortieTube, mainJoueur, sizeof(t_carte) * joueur.nombreCartes);
    //afficherCarte(cartes[6]);
    //affichageMain2(mainJoueur, joueur);
    //close(sortieTube);
    //return cartes;
}


void sendSigusr1Server(t_partie *partie) {
    kill(partie->joueur[0].pid, SIGUSR1);
}

void sendSigusr2Server(t_partie *partie) {
    kill(partie->joueur[0].pid, SIGUSR2);
}

void envoyerSignal1Joueur(t_joueur tJoueur) {
    // printf("envoie message \n");
    kill(tJoueur.pid, SIGUSR1);
    //printf("message envoyé\n");
}

void copie(t_carte *mains, t_carte *section, int taille) {
    for (int i = 0; i < taille; ++i) {
        mains[i] = section[i];
    }
}

void *functionThreadPartie(void *pVoid) {

    envoi = (t_envoi *) pVoid;

    struct sigaction newact;

    newact.sa_handler = MONSIG;
    sigemptyset(&newact.sa_mask);
    sigaction(SIGUSR1, &newact, NULL);
    sigaction(SIGALRM, &newact, NULL);
    sigaction(SIGUSR2, &newact, NULL);
    //printf("INFO : Attente des informations du serveur ;\n");

    while (1) {
        sleep(20000);
    }

    pthread_exit(0);
}


void refreshPartie(t_partie *partie) {
    key_t key;
    int shmid;
    key = ftok("partie.txt", 'R');
    shmid = shmget(key, TAILLE_SHM, 0644 | IPC_CREAT);
    partie = shmat(shmid, (void *) 0, 0);
}

void separerChaine(char *chaine, char *chaineCouleur, char *chainePossibilite) {
    if ((strcmp(chaine, "jo") == 0) || strcmp(chaine, "+4")) {
        strcpy(chainePossibilite, chaine);
    } else {
        chaineCouleur[0] = chaine[0];
        chaineCouleur[1] = '\0';
        chainePossibilite[0] = chaine[1];
        if (strlen(chaine) == 2) {
            chainePossibilite[1] = '\0';
        } else {
            chainePossibilite[1] = chaine[2];
            chainePossibilite[2] = '\0';
        }
    }
}

int contains(char *chaine, t_joueur joueur, t_carte *main) {
    char couleur[1];
    char possibilite[3];


}


void MONSIG(int num) {

    int retour_jouer_carte;
    char reponse[6];
    int conforme = 0;
    int erreurSaisie = 0;
    key_t cleTas;
    key_t clePartie;
    t_tas *tas;
    t_partie *partie;
    int existanceCarte = 0;
    cleTas = genererCleTas();
    tas = recupererTasPartagee(cleTas);
    t_carte derniereCarteTas;
    derniereCarteTas = recupererDerniereCarteTas(tas);
    printf("Vous avez l'id : %d\n", envoi->idClient);
    clePartie = genererClePartie();
    partie = recupererPartiePartagee(clePartie);
    key_t cleMain;
    cleMain = genererCleClient(partie->joueur[envoi->idClient]);
    t_carte *main;
    main = recupererMainPartagee(cleMain, partie->joueur[envoi->idClient]);

    switch (num) {

        case SIGUSR1:

            //printf("Cle : %d\n",genererCleClient(partie->joueur[envoi->idClient]));
            affichageClientPartieCommencee(partie, tas, main, envoi->idClient);
            while ((existanceCarte == 0)) {
                if (erreurSaisie) {
                    ROUGE;
                    printf("Tapez une carte existante ! \n");
                    REINIT;
                }
                printf("Veuillez saisir la carte que vous souhaitez jouer \n");
                scanf("%s", reponse);
                existanceCarte = existe(reponse);
                erreurSaisie = 1;


            }
            erreurSaisie = 0;
            if (strcmp(reponse, "pioche") != 0) {
                sendSigusr1Server(partie);
            } else {

            }
            break;
        case SIGUSR2:
            affichageClientPartieCommencee(partie, tas, envoi->main, envoi->idClient);
            break;

        case SIGALRM:
            printf("sig recu sigalarm\n");

            break;

        default:
            break;
    }
}



