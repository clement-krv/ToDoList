#include "headers/todolist.h"

ListeTaches* creerListeTaches() {
    ListeTaches* liste = (ListeTaches*)malloc(sizeof(ListeTaches));
    liste->taches = NULL;
    liste->nombreDeTaches = 0;
    return liste;
}

void ajouterTache(ListeTaches* liste, Tache* tache) {
    liste->taches = (Tache**)realloc(liste->taches, (liste->nombreDeTaches + 1) * sizeof(Tache*));
    liste->taches[liste->nombreDeTaches] = tache;
    liste->nombreDeTaches++;
}

int comparerTaches(const void* a, const void* b) {
    Tache* tacheA = *(Tache**)a;
    Tache* tacheB = *(Tache**)b;
    return tacheA->jourPourTerminer - tacheB->jourPourTerminer;
}

int compterTachesEnCours(ListeTaches* liste) {
    int compteur = 0;
    for (int i = 0; i < liste->nombreDeTaches; i++) {
        if (liste->taches[i]->statut == EN_COURS) {
            compteur++;
        }
    }
    return compteur;
}

void afficherTaches(ListeTaches* liste) {
    qsort(liste->taches, liste->nombreDeTaches, sizeof(Tache*), comparerTaches);

    for (int i = 0; i < liste->nombreDeTaches; i++) {
        Tache* tache = liste->taches[i];
        printf("Nom: %s\n", tache->nom);
        printf("Date de creation: %s", ctime(&(tache->dateCreation)));
        printf("Statut: %s\n", tache->statut == EN_ATTENTE ? "En attente" : tache->statut == EN_COURS ? "En cours" : "Terminee");
        printf("Jours pour terminer: %d\n", tache->jourPourTerminer);
        printf("\n");
    }
}

void retirerTachesTerminees(ListeTaches* liste) {
    for (int i = 0; i < liste->nombreDeTaches; i++) {
        if (liste->taches[i]->statut == TERMINE) {
            free(liste->taches[i]);
            for (int j = i; j < liste->nombreDeTaches - 1; j++) {
                liste->taches[j] = liste->taches[j + 1];
            }
            liste->nombreDeTaches--;
            i--;         }
    }
}
void mettreAJourTaches(ListeTaches* liste) {
    for (int i = 0; i < liste->nombreDeTaches; i++) {
        if (liste->taches[i]->statut == EN_COURS) {
            liste->taches[i]->jourPourTerminer--;

            if (liste->taches[i]->jourPourTerminer == 0) {
                liste->taches[i]->statut = TERMINE;
            }
        }
    }
}

void commencerTachesEnAttente(ListeTaches* liste) {
    int nombreTachesEnCours = 0;

    for (int i = 0; i < liste->nombreDeTaches; i++) {
        if (liste->taches[i]->statut == EN_COURS) {
            nombreTachesEnCours++;
        }
    }
    if (nombreTachesEnCours < 5) {
        for (int i = 0; i < liste->nombreDeTaches && nombreTachesEnCours < 5; i++) {
            if (liste->taches[i]->statut == EN_ATTENTE) {
                liste->taches[i]->statut = EN_COURS;
                nombreTachesEnCours++;
            }
        }
    }
}

Tache* creerTache(char* nom, int joursPourTerminer) {
    Tache* tache = (Tache*)malloc(sizeof(Tache));
    tache->nom = strdup(nom);
    tache->dateCreation = time(NULL);
    tache->statut = EN_ATTENTE;
    tache->jourPourTerminer = joursPourTerminer;
    return tache;
}