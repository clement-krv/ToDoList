#include "headers/todolist.h"

// Fonction qui permet de créer une liste de tâches
ListeTaches *creerListeTaches()
{
    ListeTaches *liste = (ListeTaches *)malloc(sizeof(ListeTaches));
    liste->tete = NULL;
    liste->queue = NULL;
    liste->nombreDeTaches = 0;

    return liste;
}

// Fonction qui permet d'ajouter une tâche à une liste de tâches
void ajouterTache(ListeTaches *liste, Tache *tache, char *nomFichier)
{
    if (liste == NULL || tache == NULL) {
        printf("Liste ou tache non valide.\n");
        return;
    }

    if (liste->tete == NULL) {
        liste->tete = tache;
        liste->queue = tache;
    } else {
        liste->queue->suivant = tache;
        liste->queue = tache;
    }
    tache->suivant = NULL;

    liste->nombreDeTaches++;

    FILE *fichier = fopen(nomFichier, "a");
    if (fichier != NULL) {
        // Écrire la tâche dans le fichier
        fprintf(fichier, "{\n");
        fprintf(fichier, "Nom: %s\n", tache->nom);

        char buffer[80];
        struct tm *timeinfo;
        timeinfo = localtime(&(tache->dateCreation));
        strftime(buffer, 80, "%c", timeinfo);
        fprintf(fichier, "Date de creation: %s\n", buffer);

        // Utilisez la même logique pour le statut de la tâche
        if (tache->statut == TERMINE) {
            fprintf(fichier, "Statut: Terminee\n");
        } else if (tache->statut == EN_COURS) {
            fprintf(fichier, "Statut: En cours\n");
        } else {
            fprintf(fichier, "Statut: En attente\n");
        }
        fprintf(fichier, "Jours pour terminer: %d\n", tache->jourPourTerminer);
        fprintf(fichier, "}\n\n");

        fclose(fichier);
    } else {
        printf("Erreur lors de l'ouverture du fichier %s\n", nomFichier);
    }
}

void trierTachesParDate(ListeTaches *liste)
{
    if (liste == NULL || liste->tete == NULL || liste->tete->suivant == NULL) {
        return; // Rien à trier si la liste est vide ou a un seul élément
    }

    int swapped;
    Tache *ptr;
    Tache *suivant = NULL;

    do {
        swapped = 0;
        ptr = liste->tete;

        while (ptr->suivant != suivant) {
            if (difftime(ptr->dateCreation, ptr->suivant->dateCreation) < 0) {
                // Échanger les tâches
                Tache *temp = ptr;
                ptr = ptr->suivant;
                ptr->suivant = temp;
                swapped = 1;
            }
            ptr = ptr->suivant;
        }
        suivant = ptr;

    } while (swapped);
}

void lireTachesDepuisFichier(ListeTaches *liste, FILE *fichier, char *nomFichier)
{
    if (fichier == NULL) {
        printf("Erreur: Impossible d'ouvrir le fichier %s\n", nomFichier);
        return;
    }

    char nom[100];
    long dateCreation;
    int statut;
    int jourPourTerminer;

    while (fscanf(fichier, "{\nNom: %99[^\n]\nDate de creation: %ld\nStatut: %*s\nJours pour terminer: %d\n}\n\n",
        nom, &dateCreation, &jourPourTerminer) == 3) {

            Tache *nouvelleTache = creerTache(nom, jourPourTerminer);
            nouvelleTache->dateCreation = (time_t)dateCreation;

            ajouterTache(liste, nouvelleTache, nomFichier);
        }

    fclose(fichier);
}

void ajouterTacheSansEcrireDansFichier(ListeTaches *liste, Tache *tache)
{
    if (liste == NULL || tache == NULL) {
        printf("Liste ou tache non valide.\n");
        return;
    }

    if (liste->tete == NULL) {
        liste->tete = tache;
        liste->queue = tache;
    } else {
        liste->queue->suivant = tache;
        liste->queue = tache;
    }
    tache->suivant = NULL;

    liste->nombreDeTaches++;
}

void modifierTache(ListeTaches *liste, char *nom, StatutTache nouveauStatut, char *nomFichier)
{
    if (liste == NULL || liste->tete == NULL) {
        printf("Liste vide ou non valide.\n");
        return;
    }

    Tache *courante = liste->tete;

    while (courante != NULL) {
        if (strcmp(courante->nom, nom) == 0) {
            courante->statut = nouveauStatut;
            return;
        }
        courante = courante->suivant;
    }

    printf("Tâche non trouvée.\n");
}

int comparerTaches(const void *a, const void *b)
{
    const Tache *tacheA = *((const Tache **)a);
    const Tache *tacheB = *((const Tache **)b);

    if (tacheA->dateCreation < tacheB->dateCreation) {
        return 1;
    } else if (tacheA->dateCreation > tacheB->dateCreation) {
        return -1;
    }

    return 0;
}

void retirerTachesTerminees(ListeTaches *liste, char *nomFichier)
{
    if (liste == NULL || liste->tete == NULL) {
        printf("Liste vide ou non valide.\n");
        return;
    }

    Tache *courante = liste->tete;
    Tache *precedente = NULL;

    while (courante != NULL) {
        if (courante->statut == TERMINE) {
            if (precedente == NULL) {
                // La tâche à supprimer est la tête de la liste
                liste->tete = courante->suivant;
            } else {
                // La tâche à supprimer n'est pas la tête de la liste
                precedente->suivant = courante->suivant;
            }

            Tache *aSupprimer = courante;
            courante = courante->suivant;

            free(aSupprimer->nom);
            free(aSupprimer);

            liste->nombreDeTaches--;
        } else {
            precedente = courante;
            courante = courante->suivant;
        }
    }

    // Réécrire la liste mise à jour dans le fichier
    FILE *fichier = fopen(nomFichier, "w");
    if (fichier != NULL) {
        ecrireTachesDansFichier(liste, fichier);
        fclose(fichier);
    } else {
        printf("Erreur lors de l'ouverture du fichier %s\n", nomFichier);
    }
}

void ecrireTachesDansFichier(ListeTaches *liste, FILE *fichier)
{
    if (liste == NULL || fichier == NULL) {
        printf("Liste ou fichier non valide.\n");
        return;
    }

    Tache *courante = liste->tete;

    while (courante != NULL) {
        fprintf(fichier, "{\n");
        fprintf(fichier, "Nom: %s\n", courante->nom);

        char buffer[80];
        struct tm *timeinfo;
        timeinfo = localtime(&(courante->dateCreation));
        strftime(buffer, 80, "%c", timeinfo);
        fprintf(fichier, "Date de creation: %s\n", buffer);

        // Utilisez la même logique pour le statut de la tâche
        if (courante->statut == TERMINE) {
            fprintf(fichier, "Statut: Terminee\n");
        } else if (courante->statut == EN_COURS) {
            fprintf(fichier, "Statut: En cours\n");
        } else {
            fprintf(fichier, "Statut: En attente\n");
        }
        fprintf(fichier, "Jours pour terminer: %d\n", courante->jourPourTerminer);
        fprintf(fichier, "}\n\n");

        courante = courante->suivant;
    }
}

void mettreAJourTaches(ListeTaches *liste)
{
    if (liste == NULL || liste->tete == NULL) {
        printf("Liste vide ou non valide.\n");
        return;
    }

    time_t maintenant = time(NULL);
    Tache *courante = liste->tete;

    while (courante != NULL) {
        double joursDepuisCreation = difftime(maintenant, courante->dateCreation) / (60 * 60 * 24);
        courante->jourPourTerminer -= (int)joursDepuisCreation;

        if (courante->jourPourTerminer <= 0) {
            courante->statut = TERMINE;
        } else if (courante->statut == EN_ATTENTE && courante->jourPourTerminer > 0) {
            courante->statut = EN_COURS;
        } else if (courante->statut == EN_COURS && courante->jourPourTerminer > 0) {
            courante->statut = EN_ATTENTE;
        }

        courante->dateCreation = maintenant;
        courante = courante->suivant;
    }
}

Tache *creerTache(char *nom, int joursPourTerminer)
{
    Tache *nouvelleTache = (Tache *)malloc(sizeof(Tache));
    if (nouvelleTache == NULL) {
        printf("Erreur d'allocation de mémoire.\n");
        return NULL;
    }

    nouvelleTache->nom = strdup(nom);
    nouvelleTache->dateCreation = time(NULL);
    nouvelleTache->jourPourTerminer = joursPourTerminer;

    if (joursPourTerminer <= 0) {
        nouvelleTache->statut = TERMINE;
    } else {
        nouvelleTache->statut = EN_ATTENTE;
    }

    nouvelleTache->suivant = NULL;

    return nouvelleTache;
}

void libererListeTaches(ListeTaches *liste)
{
    if (liste == NULL) {
        printf("Liste vide ou non valide.\n");
        return;
    }

    Tache *courante = liste->tete;
    Tache *suivante = NULL;

    while (courante != NULL) {
        suivante = courante->suivant;
        free(courante->nom);
        free(courante);
        courante = suivante;
    }

    liste->tete = NULL;
    liste->queue = NULL;
    liste->nombreDeTaches = 0;

    free(liste);
}
