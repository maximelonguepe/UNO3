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

key_t genererCleTas() {
    return genererCle("tas.txt");
}

t_tas *recupererTasPartagee(key_t cle2) {
    t_tas *tas;
    int media2 = shmget(cle2, sizeof(t_tas), 0644);
    tas = shmat(media2, (void *) 0, 0);
    return tas;
}

void affichageDerniereCarteTas(t_tas *tas) {
    affichageCarteMilieu(tas->cartes[tas->taille - 1]);
}



