#include "headers/todolist.h"

ListeTaches* creerListeTaches() {
    ListeTaches* liste = (ListeTaches*)malloc(sizeof(ListeTaches));
    liste->taches = NULL;
    liste->nombreDeTaches = 0;
    return liste;
}

void ajouterTache(ListeTaches* liste, Tache* tache) {
    liste->taches = (Tache**)realloc(liste->taches, (liste->nombreDeTaches + 1) * sizeof(Tache*));
    liste->taches[liste->nombreDeTaches++] = tache;

    mettreAJourTaches(liste);
}

int comparerTaches(const void* a, const void* b) {
    Tache* tacheA = *(Tache**)a;
    Tache* tacheB = *(Tache**)b;
    return tacheA->jourPourTerminer - tacheB->jourPourTerminer;
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
            liste->taches[i] = liste->taches[--liste->nombreDeTaches];
        }
    }
}

void mettreAJourTaches(ListeTaches* liste) {
    // Obtenir la date actuelle
    time_t maintenant = time(NULL);

    // Trier les tâches par le nombre de jours pour terminer
    qsort(liste->taches, liste->nombreDeTaches, sizeof(Tache*), comparerTaches);

    // Mettre à jour le statut des tâches
    int tachesEnCours = 0;
    for (int i = 0; i < liste->nombreDeTaches; i++) {
        // Calculer le nombre de jours depuis la création de la tâche
        double joursDepuisCreation = difftime(maintenant, liste->taches[i]->dateCreation) / (60 * 60 * 24);

        // Mettre à jour le nombre de jours pour terminer
        liste->taches[i]->jourPourTerminer -= (int)joursDepuisCreation;

        // Mettre à jour le statut de la tâche
        if (liste->taches[i]->jourPourTerminer <= 0) {
            liste->taches[i]->statut = TERMINE;
        } else if (tachesEnCours < 5) {
            liste->taches[i]->statut = EN_COURS;
            tachesEnCours++;
        } else {
            liste->taches[i]->statut = EN_ATTENTE;
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

void libererListeTaches(ListeTaches* liste) {
    for (int i = 0; i < liste->nombreDeTaches; i++) {
        free(liste->taches[i]);
    }
    free(liste->taches);
    free(liste);
}