#include "lib_joueur.h"

int id;
volatile sig_atomic_t n_sigusr1 = 0;
volatile sig_atomic_t n_sigusr2 = 0;

static void sig_handler(int);

int main(int argc, char *argv[]) {

    key_t key;
    int shmid;
    t_partie *partie;
    t_joueur joueur;
    t_tas * tas;
    key_t cle;

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
    affichageJoueursClient(partie);


    n_sigusr1=0;
    while(n_sigusr1==0){

    }

    cle2=genererCleTas();
    tas=recupererTasPartagee(cle2);
    affichageDerniereCarteTas(tas);
    n_sigusr1=0;
   // sleep(1);
    //while(n_sigusr1==0);
    recupererMain(partie->joueur[id]);

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


