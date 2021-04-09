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

    key_t key;
    int shmid;
    t_partie *partie;
    t_joueur joueur;
    t_tas *tas;
    key_t cle;
    t_carte mainDepart[MAINDEPART];
    key = ftok("partie.txt", 'R');
    shmid = shmget(key, TAILLE_SHM, 0644 | IPC_CREAT);
    partie = shmat(shmid, (void *) 0, 0);
    printf("Bonjour veuillez saisir votre pseudo : \n");
    scanf("%s", joueur.nom);
    printf("écriture de \"%s\" en mémoire partagée\n", joueur.nom);
    (partie->nombreJoueurs)++;//ici
    id = partie->nombreJoueurs;
    partie->joueur[partie->nombreJoueurs].id = id;
    partie->joueur[partie->nombreJoueurs].pid = getpid();
    strncpy(partie->joueur[partie->nombreJoueurs].nom, joueur.nom, TAILLE_SHM);

    key_t cle2;


    printf("Nombre joueurs : %d \n", partie->nombreJoueurs);
    printf("Vous avez l'id : %d \n", id);
    printf("Vous avez le pid : %d\n", getpid());

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
    //attente du debut de partie
    CLIGNOTE;
    ROUGE;
    printf("en attente de joueurs");
    while ((n_sigusr1) == 0) {

    }
    clrscr();
    REINIT;
    printf("partie commencee tous les joueurs connectes\n");
    partie = shmat(shmid, (void *) 0, 0);
    //affichageJoueursClient(partie);


    n_sigusr1 = 0;
    while (n_sigusr1 == 0) {

    }

    cle2 = genererCleTas();
    tas = recupererTasPartagee(cle2);
    // affichageDerniereCarteTas(tas);
    n_sigusr1 = 0;
    // sleep(1);
    //while(n_sigusr1==0);
    recupererMain(partie->joueur[id], mainDepart);
    //affichageMain2(mainDepart,partie->joueur[id]);
    affichageClient(partie, tas, mainDepart, id);
    n_sigusr1 = 0;
    n_sigusr2 = 0;
    char reponse[5];
    pidServer=partie->joueur[0].pid;
    n_sigusr1 = 0;
    pthread_t threadPartie;
    pthread_create(&threadPartie, NULL, functionThreadPartie, NULL);
    //printf("C'est a votre tour quelle carte voulez vous jouer ? \n");
    //scanf("%s",reponse);

    void *ret;
    pthread_join(threadPartie, &ret);
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


