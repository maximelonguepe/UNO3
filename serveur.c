#include "lib_serveur.h"

#define NBJOUEUR 4

int id = 0;


/*
 * definition des types et var globales utiles par la suite
 */

couleur couleurs[NBCOULEURS] = {"r", "b", "j", "v"};
numero possibilites[NBPOSSIBILITES] = {"+2", "+4", "pa", "jo", "in", "0", "1", "2", "3", "4", "5", "6", "7",
                                       "8", "9"};
t_pioche pioche;
t_carte *cartesJoueurs;
volatile sig_atomic_t n_sigusr1 = 0;
volatile sig_atomic_t n_sigusr2 = 0;

static void sig_handler(int signum);

int main(int argc, char *argv[]) {
    key_t key;
    int shmid;
    t_partie *partie;
    t_tas *tas1 = malloc(sizeof(t_tas));

    if (signal(SIGUSR1, sig_handler) == SIG_ERR) {
        perror("signal");
        exit(EXIT_FAILURE);
    }
    if (signal(SIGUSR2, sig_handler) == SIG_ERR) {
        perror("signal");
        exit(EXIT_FAILURE);
    }

    //segment pour le tableau de joueurs
    key = ftok("partie.txt", 'R');
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
    partie->joueur[0].pid = getpid();
    strcpy(partie->joueur[0].nom, "serveur");

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

    key_t cle2;
    t_tas *tas;
    cle2 = genererCleTas();
    tas = recupererTasPartagee(cle2);
    initTas(tas);
    ajouterCarteTas(tas, *carte1);
    envoyerSignal1Joueurs(partie);
    //afficherCarte(tas->cartes[0]);
    //sendFifoAllPlayers(partie);
    sendFifoCartes(partie, cartesJoueurs);
    printf("---- taille pioche : %d\n", pioche.nombreCarteRestante);
    partie->jouant = partie->joueur[1];
    int inverse = 0;
    pthread_t threadPartie;
    pthread_create(&threadPartie, NULL, functionThreadPartieServer,NULL );
    n_sigusr1 = 0;
    sleep(10);
    void *ret;
    pthread_join(threadPartie, &ret);

}

static void sig_handler(int signum) {

    if (signum == SIGUSR1) {
        n_sigusr1++;
    } else {


        char *msg = "Reçu signal inattendu\n";
        write(STDERR_FILENO, msg, strlen(msg));
        _exit(EXIT_FAILURE);

    }
}

