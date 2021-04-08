#include <unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include<string.h>

#define CHECK(sts, msg)if((sts)==-1){perror(msg);exit(-1);}


void signalCallback(int signal) {
    if (signal) {
        switch (signal) {
            case SIGINT:
                printf("Ctrl+C annulé\n");
                exit(11);
                break;
            case SIGUSR1:
                printf("Bravo vous avez réussi a vous connecter\n");
                break;
            case SIGUSR2:
                printf("Echec vous ne connaitras jamais le code !!! :)\n");
                break;

            case SIGALRM:
                printf("Fin du temps imparti :( \n");
                exit(4);
                break;
        }
    }
}

int main() {


    int a;
    int nbcoups = 3;
    char mdp[250];
    sigset_t intmask;
    struct sigaction newmask;
    int sts;
    char c;
    printf("Bonjour, je suis le processus %d\n", getpid());
    /* Initialise et affecte le mask */
    sts = sigemptyset(&intmask);
    CHECK(sts, "Appel de sigemptyset");

    /* Ajout du signal SIGINT au masque que l'on vient de créer */
    sts = sigaddset(&intmask, SIGINT);
    CHECK(sts, "Appel de sigaddset");

    /* Mise en oeuvre du masque */
    /* SIG_BLOCK : ajoute les signaux du masque au masque courant */
    sts = sigprocmask(SIG_BLOCK, &intmask, NULL);
    CHECK(sts, "appel de sigprocmask");
    CHECK(a = fork(), "Ne peut pas créer un nouveau processus");

    if (a == 0) {// si fils
        printf("je suis le fils\n");
        struct sigaction maskk;
        maskk.sa_handler = signalCallback;
        sts = sigaction(SIGINT, &maskk, NULL);
        sts = sigaction(SIGALRM, &maskk, NULL);
        while (nbcoups != 0) {
            alarm(20);
            printf("Veuillez saisir votre mot de passe (vous avez 20 secondes)\n");
            scanf("%s", mdp);
            if (strcmp(mdp, "toto") != 0)nbcoups--;
            else {

                printf("reussi\n");
                kill(getppid(), SIGUSR1);
                exit(1);


            }

        }

        kill(getppid(), SIGUSR2);
        exit(3);



    } else {
        printf("je suis le pere\n");
        newmask.sa_handler = signalCallback;
        sts = sigaction(SIGINT, &newmask, NULL);
        sts = sigaction(SIGUSR1, &newmask, NULL);
        sts = sigaction(SIGUSR2, &newmask, NULL);

        wait(&a);


    }


    return (0);
}