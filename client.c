#include "lib_joueur.h"

int id;


volatile sig_atomic_t n_sigusr1 = 0;
volatile sig_atomic_t n_sigusr2 = 0;

static void sig_handler(int);

/*TODO : pour jouer une carte :
 * vérifier que la carte est presente dans la main du joueur
 * vérifier que la carte est possible a jouer
 * ajouter la carte au tas si elle est jouable
 * diminuer le nombre de cartes du joueur
 * reafficher propremment laffichage global des joueurs
 * */



int main(int argc, char *argv[]) {
    t_envoi * envoi;
    envoi=malloc(sizeof(t_envoi));
    key_t key;
    int shmid;
    t_partie *partie;
    t_joueur joueur;
    t_tas *tas;
    key_t cle;
    t_carte * mainDepart;

    key = genererClePartie();


    shmid = shmget(key, TAILLE_SHM, 0644 | IPC_CREAT);
    partie = shmat(shmid, (void *) 0, 0);

    printf("Bonjour veuillez saisir votre pseudo : \n");
    scanf("%s", joueur.nom);
    printf("écriture de \"%s\" en mémoire partagée\n", joueur.nom);
    (partie->nombreJoueurs)++;
    id = partie->nombreJoueurs;
    partie->joueur[partie->nombreJoueurs].id = id;
    partie->joueur[partie->nombreJoueurs].pid = getpid();
    strncpy(partie->joueur[partie->nombreJoueurs].nom, joueur.nom, TAILLE_SHM);

    envoi->idClient=id;
    key_t cle2;


    printf("Nombre joueurs : %d \n", partie->nombreJoueurs);


    if (shmdt(partie) == -1) {
        perror("shmdt");
        exit(1);
    }

    if (signal(SIGUSR1, sig_handler) == SIG_ERR) {
        perror("signal");
        exit(EXIT_FAILURE);
    }
    if (signal(SIGUSR2, sig_handler) == SIG_ERR) {
        perror("signal");
        exit(EXIT_FAILURE);
    }
    CLIGNOTE;
    ROUGE;
    printf("en attente de joueurs");
    while ((n_sigusr1) == 0) {

    }
    clrscr();
    REINIT;
    printf("partie commencee tous les joueurs connectes\n");
    partie = shmat(shmid, (void *) 0, 0);

    n_sigusr1 = 0;
    while (n_sigusr1 == 0) {

    }
    cle2 = genererCleTas();
    tas = recupererTasPartagee(cle2);
    //recupererMain(partie->joueur[id], mainDepart);
    key_t cleMain;
    cleMain=genererCleClient(partie->joueur[id]);
    n_sigusr1 = 0;
    while (n_sigusr1 == 0) {

        mainDepart = malloc(sizeof(t_carte) * partie->joueur[id].nombreCartes);
        mainDepart = recupererMainPartagee(cleMain, partie->joueur[id]);
        affichageClient(partie, tas, mainDepart, id);
    }
    n_sigusr1 = 0;
    n_sigusr2 = 0;
    char reponse[5];
    //pidServer=partie->joueur[0].pid;
    n_sigusr1 = 0;
    envoi->main=malloc(sizeof(t_carte)*partie->joueur[id].nombreCartes);
    copie(envoi->main,mainDepart,partie->joueur[id].nombreCartes);
    pthread_t threadPartie;
    pthread_create(&threadPartie, NULL, functionThreadPartie, (void *)envoi );

    void *ret;
    pthread_join(threadPartie, &ret);
    partie=recupererPartiePartagee(key);
    print_milieu("fin de partie\n");
    char chaine[100];
    strcpy(chaine,"Joueur ayant gagné : ");
    strcat(chaine,partie->jouant.nom);
    print_milieu(chaine);

    return 0;

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


