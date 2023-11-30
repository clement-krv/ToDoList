//Cree par Alexis BRETON et Clément KERVICHE

#ifndef TODOLIST_H
#define TODOLIST_H

// Bibliothèques
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Constantes

#define MAX_TACHES 100

// Structures

typedef enum {
    EN_ATTENTE,
    EN_COURS,
    TERMINE
} StatutTache;

typedef struct {
    char* nom;
    time_t dateCreation;
    StatutTache statut;
    int jourPourTerminer;
} Tache;

typedef struct{
    Tache** taches;
    int nombreDeTaches;
} ListeTaches;

// Prototypes

ListeTaches* creerListeTaches();
void ajouterTache(ListeTaches *liste, Tache *tache, char *nomFichier);
int comparerTaches(const void *a, const void *b);
void retirerTachesTerminees(ListeTaches *liste, char *nomFichier);
void mettreAJourTaches(ListeTaches *liste);
void libererListeTaches(ListeTaches *liste);

void ecrireTachesDansFichier(ListeTaches *liste, FILE *fichier);
void trierTachesParDate(ListeTaches *liste);
void modifierTache(ListeTaches* liste, char* nom, StatutTache nouveauStatut, char *nomFichier);
void lireTachesDepuisFichier(ListeTaches *liste, FILE *fichier, char *nomFichier);
void ajouterTacheSansEcrireDansFichier(ListeTaches *liste, Tache *tache);
Tache *creerTache(char *nom, int joursPourTerminer);

#endif