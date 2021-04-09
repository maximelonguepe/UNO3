#include "lib_joueur.h"

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


int partieTerminee(t_partie * partie){

    for (int i = 1; i <=partie->nombreJoueurs ; ++i) {
        if(partie->joueur[i].nombreCartes==0)return 1;
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
    //printf("clee generee %d\n",cle2);
    return cle2;
}
key_t genererClePartie() {
    return genererCle("partie.txt");
}

t_partie * recupererPartiePartagee(key_t key){
    int shmid;
    t_partie *partie;
    shmid = shmget(key, TAILLE_SHM, 0644 | IPC_CREAT);
    partie = shmat(shmid, (void *) 0, 0);
    return partie;
}

key_t genererCleTas() {
    return genererCle("tas.txt");
}

t_tas *recupererTasPartagee(key_t cle2) {
    t_tas *tas;
    int media2 = shmget(cle2, sizeof(t_tas), 0777 | IPC_CREAT);
    //printf("media 2 : %d\n",media2);
    switch errno {
        case EACCES :
            printf("----1\n");
            break;
        case EEXIST:
            printf("----2\n");
            break;
        case EINVAL:
            printf("----3\n");
            break;

        case ENFILE :
            printf("----4\n");
            break;
        case ENOENT:
            printf("----5\n");
            break;
        case ENOMEM:
            printf("----6\n");
            break;

        case ENOSPC :
            printf("----7\n");
            break;
        case EPERM:
            printf("----8\n");
            break;

    }
    tas = shmat(media2, (void *) 0, 0);
    return tas;
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

int nombreDeCarteSuperieurLigne(int nombreCartes){
    int tailletxt=0;
    int col;
    int lignes;
    for (int i = 0; i < nombreCartes; ++i) {
        tailletxt+=10;
    }
    get_win_value(&col,&lignes);

    if (col<tailletxt){
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

void affichageClient(t_partie * partie,t_tas * tas, t_carte * mainDepart,int id){
    affichageJoueursClient(partie);
    affichageDerniereCarteTas(tas);
    affichageMain2(mainDepart,partie->joueur[id]);

}

t_carte * recupererMain(t_joueur joueur,t_carte * mainJoueur) {
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
    close(sortieTube);
    //return cartes;
}


void sendSigusr1Server(int pid){
    kill(pid,SIGUSR1);
}
void envoyerSignal1Joueur(t_joueur tJoueur){
    printf("envoie message \n");
    kill(tJoueur.pid,SIGUSR1);
    printf("message envoyé\n");
}
void * functionThreadPartie(void *pVoid){

    struct sigaction newact;
    newact.sa_handler=MONSIG;
    sigemptyset(&newact.sa_mask);
    sigaction(SIGUSR1,&newact,NULL);
    sigaction(SIGALRM,&newact,NULL);
    sigaction(SIGUSR2,&newact,NULL);
    //printf("INFO : Attente des informations du serveur ;\n");

    while (1){
        sleep(20000);
    }

    pthread_exit(0);
}


void refreshPartie(t_partie * partie){
    key_t key;
    int shmid;
    key = ftok("partie.txt", 'R');
    shmid = shmget(key, TAILLE_SHM, 0644 | IPC_CREAT);
    partie = shmat(shmid, (void *) 0, 0);
}







void MONSIG(int num){

    struct data_t *memoryShared;
    int retour_jouer_carte;
    char reponse[5];
    switch(num){

        case SIGUSR1:
            //sendSigusr1Server(pidServer);
            //printf("signal recu sigusr1 \n");
            printf("Veuillez saisir la carte que vous souhaitez jouer \n");
            scanf("%s",reponse);
            sendSigusr1Server(pidServer);
            break;
        case SIGUSR2:
            printf("sig recu sigusr2\n");
            break;

        case SIGALRM:
            printf("sig recu sigalarm\n");

            break;

        default:
            break;
    }
}



