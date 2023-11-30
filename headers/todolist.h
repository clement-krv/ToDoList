#ifndef TODOLIST_H
#define TODOLIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_TACHES 100

typedef enum {
    EN_ATTENTE,
    EN_COURS,
    TERMINE
} StatutTache;

typedef struct Tache {
    char *nom;
    time_t dateCreation;
    StatutTache statut;
    int jourPourTerminer;
    struct Tache *suivant;
} Tache;

typedef struct {
    Tache *tete;
    Tache *queue;
    int nombreDeTaches;
} ListeTaches;

ListeTaches *creerListeTaches();
void ajouterTache(ListeTaches *liste, Tache *tache, char *nomFichier);
void trierTachesParDate(ListeTaches *liste);
void lireTachesDepuisFichier(ListeTaches *liste, FILE *fichier, char *nomFichier);
void ajouterTacheSansEcrireDansFichier(ListeTaches *liste, Tache *tache);
void modifierTache(ListeTaches *liste, char *nom, StatutTache nouveauStatut, char *nomFichier);
int comparerTaches(const void *a, const void *b);
void retirerTachesTerminees(ListeTaches *liste, char *nomFichier);
void ecrireTachesDansFichier(ListeTaches *liste, FILE *fichier);
void mettreAJourTaches(ListeTaches *liste);
Tache *creerTache(char *nom, int joursPourTerminer);
void libererListeTaches(ListeTaches *liste);

#endif
