#include "lib_joueur.h"

int id;
volatile sig_atomic_t n_sigusr1 = 0;
volatile sig_atomic_t n_sigusr2 = 0;

static void sig_handler(int);

int main(int argc, char *argv[]) {

    key_t key;
    int shmid;
    t_partie *data;
    t_joueur joueur;
    t_tas * tas;
    key_t cle;
    int media;
    t_carte * carte1;
    cle=ftok("carte.txt",'R');
    media = shmget(cle, sizeof(carte1),0644|IPC_CREAT);
    key = ftok("serveur.c", 'R');
    shmid = shmget(key, TAILLE_SHM, 0644 | IPC_CREAT);
    data = shmat(shmid, (void *) 0, 0);
    printf("Bonjour veuillez saisir votre pseudo : \n");
    scanf("%s", joueur.nom);
    printf("écriture de \"%s\" en mémoire partagée\n", joueur.nom);
    (data->nombreJoueurs)++;//ici
    id = data->nombreJoueurs;
    data->joueur[data->nombreJoueurs].id = id;
    data->joueur[data->nombreJoueurs].pid = getpid();
    strncpy(data->joueur[data->nombreJoueurs].nom, joueur.nom, TAILLE_SHM);

    key_t cle2;


    printf("Nombre joueurs : %d \n", data->nombreJoueurs);
    printf("Vous avez l'id : %d \n", id);
    printf("Vous avez le pid : %d\n", getpid());

    if (shmdt(data) == -1) {
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
    data = shmat(shmid, (void *) 0, 0);
    affichageJoueursClient(data);
//    return (EXIT_SUCCESS);

    char str[3];
    sprintf(str, "%d", id);

    int fd;

    char myfifo[9];
    strcpy(myfifo, "/tmp/");
    strcat(myfifo, str);
    t_carte *carte;
    /* open, read, and display the message from the FIFO */
    /* printf("tentative lecture tube : %s\n",myfifo);
     fd = open(myfifo, O_RDONLY);
     printf("Taille carte : %ld\n", sizeof(carte)+5);
     read(fd, carte, sizeof(carte));
     afficherCarte(*carte);
     close(fd);*/

    // unlink(myfifo);


    n_sigusr1=0;
    while(n_sigusr1==0){

    }

    cle2=genererCleTas();
    tas=recupererTasPartagee(cle2);
    //printf("-----%d \n",tas->taille);
    //afficherCarte(tas->cartes[0]);
    affichageDerniereCarteTas(tas);

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