#include "lib_joueur.h"

int *idClient;
t_envoi *envoi;
couleur coul[NBCOULEURS] = {"r", "b", "j", "v"};
numero possi[NBPOSSIBILITES] = {"+2", "+4", "pa", "jo", "in", "0", "1", "2", "3", "4", "5", "6", "7",
                                "8", "9"};

/**
 * Affichage du joueur en train de jouer
 * @param partie
 */
void affichageJoueurJouant(t_partie *partie) {
    printf("Joueur en train de jouer : %s\n", partie->joueur[partie->jouant.id].nom);
}

/**
 * Affiche le nom du joueur
 * @param joueur
 */
void affichageJoueur(t_joueur joueur) {
    printf("%s\n", joueur.nom);
}

/**
 * affecte a un joueur un nom
 * @param joueur
 * @param nom
 */
void affectationNom(t_joueur *joueur, char *nom) {
    strcpy(joueur->nom, nom);
}

/**
 * liste les joueurs connectés a la partie
 * @param joueur
 * @param nbJoueurs
 */
void listageJoueurs(t_joueur *joueur, int nbJoueurs) {
    for (int i = 1; i <= nbJoueurs; ++i) {
        affichageJoueur(joueur[i]);
    }
}

/**
 * si la chaine est une couleur retourne 1 sinon retourne 0
 * @param couleur
 * @return
 */
int estCouleur(char *couleur) {
    for (int i = 0; i < NBCOULEURS; ++i) {
        if (strcmp(couleur, coul[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

/**
 * si la chaine est une possibilité (carte normale ou joker etc...)
 * @param possiblitess
 * @return
 */
int estpossibilite(char *possiblitess) {
    for (int i = 0; i < NBPOSSIBILITES; ++i) {
        if (strcmp(possiblitess, possi[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

/**
 * si la chaine correspond à une carte : par exemple j7 retourne 1
 * sinon si la chaine ne correspond pas par exmple : a8 retourne 0
 * cas spéciaux : les jokers : +4 ,jo retourne 1
 * cependant si on saisit la carte rjo celle si va retourner 0
 * @param chaine
 * @return
 */
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

/**
 * si un des joueurs a sa main vide alors la partie se termine
 * @param partie
 * @return
 */
int partieTerminee(t_partie *partie) {

    for (int i = 1; i <= partie->nombreJoueurs; ++i) {
        if (partie->joueur[i].nombreCartes == 0)return 1;
    }
    return 0;
}

/**
 * affichage coté client de la liste des joueurs : leur nom et leur nombre de cartes
 * @param partie
 */
void affichageJoueursClient(t_partie *partie) {
    char str[3];
    for (int i = 1; i <= partie->nombreJoueurs; ++i) {
        print_droit(partie->joueur[i].nom);
        sprintf(str, "%d", partie->joueur[i].nombreCartes);
        print_droit(str);
        print_droit("-----\n\n");
    }
}

/**
 * affichage d'une carte au milieu
 * sert pour afficher la derniere carte du tas
 * @param carte
 */
void affichageCarteMilieu(t_carte carte) {
    affichageCouleur(carte.couleur);
    char couleur[25];
    char numero[25];
    strcpy(couleur, "- ");
    strcat(couleur, carte.couleur);
    if (strlen(carte.numero_carte) == 1)strcat(couleur, " -");
    else {
        if (strcmp(carte.couleur, "") != 0) strcat(couleur, "  -");
        else strcat(couleur, "   -");
    }
    strcpy(numero, "- ");
    strcat(numero, carte.numero_carte);
    strcat(numero, " -");

    if (strlen(carte.numero_carte) == 1) print_milieu("-----");
    else print_milieu("------");
    print_milieu(couleur);
    print_milieu(numero);
    if (strlen(carte.numero_carte) == 1) print_milieu("-----");
    else print_milieu("------");
    REINIT;
}

/**
 * augmenter la taille du tas
 * principalement utilisé quand un joueur joue une carte
 * @param tas
 */

void augmenterTailleTas(t_tas *tas) {
    tas->taille++;
}

/**
 * ajoute une carte au tas
 * @param tas
 * @param carte
 */
void ajouterCarteTas(t_tas *tas, t_carte carte) {
    //tas->cartes = realloc(tas->cartes, tas->taille + 1 * sizeof(t_carte));
    tas->cartes[tas->taille] = carte;
    augmenterTailleTas(tas);

}

/**
 * genere avec une chaine relative a un nom de fichier une clé
 * @param chaine
 * @return
 */
key_t genererCle(char *chaine) {
    key_t cle2 = ftok(chaine, 'R');
    return cle2;
}

/**
 * renvoie la clé relative a chaque client
 * @param joueur
 * @return
 */
key_t genererCleClient(t_joueur joueur) {
    char chaine[10];
    genererCle(genererNomFichier(joueur, chaine));
}

/**
 * genere une chaine de caractere relative a l'id du client
 * @param joueur
 * @param chaine
 * @return
 */
char *genererNomFichier(t_joueur joueur, char *chaine) {
    char str[3];
    sprintf(str, "%d", joueur.id);
    strcpy(chaine, "");
    strcat(chaine, str);
    strcat(chaine, ".txt");
    return chaine;
}

/**
 * genere une cle de partie
 * @return
 */
key_t genererClePartie() {
    return genererCle("partie.txt");
}

/**
 * recupère une partie en passant la clé de la partie en parametre
 * @param key
 * @return
 */
t_partie *recupererPartiePartagee(key_t key) {
    int shmid;
    t_partie *partie;
    shmid = shmget(key, TAILLE_SHM, 0644 | IPC_CREAT);
    partie = shmat(shmid, (void *) 0, 0);
    return partie;
}

/**
 *  recupère la main en passant la clé de la main en parametre
 * @param key
 * @param joueur
 * @return
 */
t_carte *recupererMainPartagee(key_t key, t_joueur joueur) {
    int shmid;
    t_carte *carte;
    shmid = shmget(key, joueur.nombreCartes, 0644 | IPC_CREAT);
    carte = shmat(shmid, (void *) 0, 0);
    return carte;
}

/**
 * genere une cle relative au tas dans la memoire partagee
 * @return
 */

key_t genererCleTas() {
    return genererCle("tas.txt");
}

/**
 * recupere le tas dans la memoire partagee
 * @param cle2
 * @return
 */
t_tas *recupererTasPartagee(key_t cle2) {
    t_tas *tas;
    int media2 = shmget(cle2, sizeof(t_tas), 0777 | IPC_CREAT);
    tas = shmat(media2, (void *) 0, 0);
    return tas;
}

/**
 * recupere la derniere carte du tas
 * @param tas
 * @return
 */
t_carte recupererDerniereCarteTas(t_tas *tas) {
    return tas->cartes[tas->taille - 1];
}

/**
 * affiche la derniere carte du tas au milieu de lecra du joueur
 * @param tas
 */
void affichageDerniereCarteTas(t_tas *tas) {
    affichageCarteMilieu(tas->cartes[tas->taille - 1]);
}

/**
 * affiche le haut des cartes ( le ----)
 * @param cartes
 * @param nombreCartes
 */
void affichageHautCartes(t_carte *cartes, int nombreCartes) {
    char chaine[10000];
    strcpy(chaine, "");
    for (int i = 0; i < nombreCartes; ++i) {

        //strcat(chaine,"-----\t");
        affichageCouleur(cartes[i].couleur);
        if (strlen(cartes[i].numero_carte) == 1) {
            printf("-----\t");

        } else {
            printf("------\t");
        }
        REINIT;
    }
    printf("\n");
}

/**
 * affiche les couleurs sur la meme ligne
 * @param cartes
 * @param nombreCartes
 */
void affichageCouleurMain(t_carte *cartes, int nombreCartes) {
    for (int i = 0; i < nombreCartes; ++i) {
        affichageCouleur(cartes[i].couleur);
        printf("- ");
        printf("%s", cartes[i].couleur);
        if (strlen(cartes[i].numero_carte) == 1) {
            printf(" -\t");
        } else {
            if (strcmp(cartes[i].couleur, "") != 0) printf("  -\t");
            else printf("   -\t");


        }
        REINIT;
    }
    printf("\n");
}

/**
 * affiche le numero de la carte
 * @param cartes
 * @param nombreCartes
 */
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
    if ((strcmp(chaine, "jo") == 0) || strcmp(chaine, "+4") == 0) {
        printf("est carte sans couleur \n");
        strcpy(chainePossibilite, chaine);
        strcpy(chaineCouleur, "");
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
    char chaineCouleur[3];
    char chainePossibilite[3];
    char couleur[1];
    char possibilite[3];
    t_carte carte;
    separerChaine(chaine, chaineCouleur, chainePossibilite);
    for (int i = 0; i < joueur.nombreCartes; ++i) {
        carte = main[i];
        if (strcmp(chaineCouleur, "") == 0) {
            if (strcmp(chainePossibilite, "jo") == 0 || strcmp(chainePossibilite, "+4") == 0) {
                return 1;
            } else return 0;
        } else {
            if (strcmp(carte.numero_carte, chainePossibilite) == 0 && strcmp(carte.couleur, chaineCouleur) == 0) {
                return 1;
            }
        }
    }
    return 0;

}

t_carte containTest(char *chaine, t_joueur joueur, t_carte *main) {
    printf("Recherche carte \n");
    char chaineCouleur[3];
    char chainePossibilite[4];

    separerChaine(chaine, chaineCouleur, chainePossibilite);
    for (int i = 0; i < joueur.nombreCartes; ++i) {
        if (strcmp(main[i].numero_carte, chainePossibilite) == 0 && strcmp(main[i].couleur, chaineCouleur) == 0) {
            return main[i];
        }
    }
    //return 0;
}


int estJouable(t_tas *tas, char *chaine) {
    char chaineCouleur[3];
    char chainePossibilite[4];

    separerChaine(chaine, chaineCouleur, chainePossibilite);
    if (strcmp(chaineCouleur, "") == 0) {
        if (strcmp(chainePossibilite, "jo") == 0 || strcmp(chainePossibilite, "+4") == 0) {
            return 1;
        } else return 0;
    } else {
        if (strcmp(recupererDerniereCarteTas(tas).numero_carte, chainePossibilite) == 0 ||
            strcmp(recupererDerniereCarteTas(tas).couleur, chaineCouleur) == 0) {
            return 1;
        } else return 0;
    }
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
    int cartePresente = 0;
    int carteNonPresente = 0;
    int jouable = 0;
    int nonJouable = 0;
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
    t_carte *carte;
    switch (num) {

        case SIGUSR1:

            //printf("Cle : %d\n",genererCleClient(partie->joueur[envoi->idClient]));
            main = recupererMainPartagee(cleMain, partie->joueur[envoi->idClient]);
            affichageClientPartieCommencee(partie, tas, main, envoi->idClient);
            while ((existanceCarte == 0) || cartePresente == 0 || jouable == 0) {
                if (erreurSaisie) {
                    ROUGE;
                    printf("Tapez une carte existante ! \n");
                    REINIT;
                }
                if (carteNonPresente) {
                    ROUGE;
                    printf("Vous n'avez pas cette carte dans votre main \n");
                    REINIT;
                }
                if (nonJouable) {
                    ROUGE;
                    printf("Vous n'avez pas le droit de jouer cette carte \n");
                    REINIT;
                }
                printf("Veuillez saisir la carte que vous souhaitez jouer \n");
                scanf("%s", reponse);
                existanceCarte = existe(reponse);
                if (!existanceCarte) erreurSaisie = 1;
                cartePresente = contains(reponse, partie->joueur[envoi->idClient], main);
                if (!cartePresente)carteNonPresente = 1;
                jouable = estJouable(tas, reponse);
                if (!jouable)nonJouable = 1;

            }
            erreurSaisie = 0;
            carteNonPresente = 0;
            if (strcmp(reponse, "pioche") != 0) {
                sendSigusr1Server(partie);

                ajouterCarteTas(tas, containTest(reponse, partie->joueur[envoi->idClient], main));
            } else {

            }
            break;
        case SIGUSR2:
            main = recupererMainPartagee(cleMain, partie->joueur[envoi->idClient]);
            affichageClientPartieCommencee(partie, tas, main, envoi->idClient);
            break;

        case SIGALRM:
            printf("sig recu sigalarm\n");

            break;

        default:
            break;
    }
}



