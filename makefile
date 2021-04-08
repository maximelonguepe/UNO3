all:  clean client serveur
debug : client serveur	
client : lib_joueur.o cartes.o
	gcc lib_joueur.o client.c cartes.o -o client -lpthread
serveur: lib_joueur.o lib_serveur.o cartes.o
	gcc lib_joueur.o lib_serveur.o cartes.o serveur.c -o serveur -lpthread
lib_joueur.o:
	gcc lib_joueur.c -c
lib_serveur.o:
	gcc lib_serveur.c -c

cartes.o:
	gcc cartes.c -c


clean :
	rm *.o client serveur
