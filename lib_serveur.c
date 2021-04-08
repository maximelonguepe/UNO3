#include "lib_serveur.h"

void distribuerNCartesJoueur(int idJoueur, int nombreCarte, t_pioche pioche) {

}

void distributionMainDepart(t_pioche *pioche, t_carte *carteJoueurs, t_partie *partie) {
    int j = 0;
    for (int i = 1; i <= partie->nombreJoueurs; ++i) {
        for (int k = 0; k < MAINDEPART; ++k) {
            carteJoueurs[j] = pioche->pioche[pioche->nombreCarteRestante];
            pioche->nombreCarteRestante--;
            j++;
        }
        partie->joueur[i].nombreCartes = MAINDEPART;
    }

}

int nombreDebut(t_partie *partie, t_joueur joueur) {
    int nombre = 0;
    for (int i = 1; i < joueur.id; ++i) {

        nombre += partie->joueur[i].nombreCartes;
    }
    return nombre;

}

void affichageMain(t_partie *partie, t_carte *mains) {

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

void envoyerSignal1Joueurs(t_partie *partie) {
    for (int i = 1; i <= partie->nombreJoueurs; ++i) {
        kill(partie->joueur[i].pid, SIGUSR1);
    }
}

void lancerPartie(t_partie *partie) {
    envoyerSignal1Joueurs(partie);
}


void sendFifo(t_partie *partie, t_carte *carte) {
    printf("toto\n");
    for (int i = 1; i <= partie->nombreJoueurs; ++i) {
        char str[3];
        sprintf(str, "%d", i);

        int fd;

        char myfifo[9];
        strcpy(myfifo, "/tmp/");
        strcat(myfifo, str);
        printf("envoi tube a  : %s\n", myfifo);
        /* create the FIFO (named pipe) */
        mkfifo(myfifo, 0666);
        /* write "Hi" to the FIFO */
        fd = open(myfifo, O_WRONLY);
        printf("Taille carte : %ld\n", sizeof(carte) + 2);
        write(fd, carte, sizeof(carte) + 2);
        close(fd);

        /* remove the FIFO */
        //unlink(myfifo);



    }


}

//t_carte *  affecterPremiereCarteTas(t_pioche * pioche, )

void initTas(t_tas *tas) {
    tas->taille = 0;

}


void sendFifo2(t_joueur joueur) {
    int i = joueur.id;
    char str[3];
    sprintf(str, "%d", i);
    char myfifo[9];
    strcpy(myfifo, "");
    strcat(myfifo, str);
    strcat(myfifo, ".fifo");
    int entreeTube;
    printf("creation fifo : %s\n", myfifo);


    t_carte carte;
    strcpy(carte.couleur, "r");
    strcpy(carte.numero_carte, str);
    if ((entreeTube = open(myfifo, O_WRONLY)) == -1) {
        printf("CLIENT - Impossible d'ouvrir l'entree du FIFO \n");
        exit(EXIT_FAILURE);
    }
    write(entreeTube, &carte, sizeof(carte));


}


void sendFifoAllPlayers(t_partie *partie) {
    for (int i = 1; i <= partie->nombreJoueurs; ++i) {
        sendFifo2(partie->joueur[i]);
    }
}

int positionFinMainTableauMain(t_joueur joueur,int positionActuelle) {
    return joueur.nombreCartes+positionActuelle;
}

void selectionneMain(int debut, int fin, t_carte * mains,t_carte * destination){
    t_carte tab[fin-debut];
    int j=0;
    for (int i = debut; i < fin; ++i) {
        afficherCarte(mains[i]);
        destination[j]=mains[i];
        j++;
    }
   // copie(destination,mains,fin-debut);
    //return tab;

}

void copie(t_carte * mains, t_carte * section,int taille){
    for (int i = 0; i <taille ; ++i) {
        mains[i]=section[i];
    }
}


void sendFifoCartes(t_partie *  partie, t_carte *mains) {
    int positionActuelle=0;
    int positionFinale=0;
    for (int i = 1; i <= partie->nombreJoueurs; ++i) {
        printf("Joueur %d\n",i);
        t_carte cartes[partie->joueur[i].nombreCartes];
        positionActuelle=positionFinale;
        positionFinale=positionFinMainTableauMain(partie->joueur[i],positionActuelle);
        //printf("Plage du joueur : %d à %d \n",positionActuelle,positionFinale);
        //copie(cartes,selectionneMain(positionActuelle,positionFinale,mains),partie->joueur[i].nombreCartes);
        selectionneMain(positionActuelle,positionFinale,mains,cartes);
        printf("--->\n");

    }
}
