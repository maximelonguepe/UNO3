#include <stdlib.h> /* "standard libraries" */
#include <string.h> /* fonctions standards sur les chaînes *//* Les trois en-tetesde librariessuivants sont nécessaires pour les opérations système. En particulier, "shm" veut dire "sharedmemory". */
#include<stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>/* On va seulement utiliser 1024 octets (1K) pour le segment partagé : on définit une constante de taille. */
#define TAILLE_SHM 1024/* la fonction principale prend des arguments : argccontient le nombre d'arguments, et argvest un pointeur vers les arguments */
int main(int argc, char *argv[]){
key_t key;
int shmid;
char *data;

key = ftok("toto.c", 'R');/* allocation du segment de mémoire partagée */
shmid= shmget(key, TAILLE_SHM, 0644 | IPC_CREAT);/* récupération d'un pointeur vers ce segment de mémoire partagée */
data = shmat(shmid, (void *)0, 0);/* suivant le nombre d'arguments (0 ou 1), on lit le contenu du segment ou on écrit l'argument dessus. */
if (argc== 2) {printf("écriture de \"%s\" en mémoire partagée\n", argv[1]);
strncpy(data, argv[1], TAILLE_SHM); // copie de l'argument
} 
else{printf("contenu de la mémoire partagée : \"%s\"\n", data);}/* avant de terminer, on détache le segment de mémoire partagée. */
if (shmdt(data) == -1) 
{perror("shmdt");
exit(1);
}
return 0;}
