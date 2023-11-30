#include "headers/todolist.h"


//Fonction qui permet de créer une liste de tâches
ListeTaches* creerListeTaches() {
    ListeTaches* liste = (ListeTaches*)malloc(sizeof(ListeTaches));
    liste->taches = NULL;
    liste->nombreDeTaches = 0;

    // Ouvrir le fichier en mode lecture
    FILE *fichier = fopen("taches.txt", "r");
    if (fichier == NULL) {
        // Si le fichier n'existe pas, retourner la liste vide
        return liste;
    }

    // Lire les tâches du fichier
    lireTachesDepuisFichier(liste, fichier);

    // Fermer le fichier
    fclose(fichier);

    // Trier les tâches par date de création
    trierTachesParDate(liste);

    return liste;
}

void trierTachesParDate(ListeTaches *liste) {
    for (int i = 0; i < liste->nombreDeTaches - 1; i++) {
        for (int j = 0; j < liste->nombreDeTaches - i - 1; j++) {
            if (liste->taches[j]->dateCreation < liste->taches[j + 1]->dateCreation) {
                // Échanger les tâches
                Tache *temp = liste->taches[j];
                liste->taches[j] = liste->taches[j + 1];
                liste->taches[j + 1] = temp;
            }
        }
    }
}

void lireTachesDepuisFichier(ListeTaches *liste, FILE *fichier) {
    char nom[100];
    long dateCreation;
    int statut;
    int jourPourTerminer;

    // Lire chaque tâche du fichier
    while (fscanf(fichier, "%s\n%ld\n%d\n%d\n", nom, &dateCreation, &statut, &jourPourTerminer) == 4) {
        // Créer une nouvelle tâche avec les données lues
        Tache *tache = (Tache*)malloc(sizeof(Tache));
        tache->nom = strdup(nom);
        tache->dateCreation = (time_t)dateCreation;
        tache->statut = (StatutTache)statut;
        tache->jourPourTerminer = jourPourTerminer;

        // Ajouter la tâche à la liste
        ajouterTache(liste, tache);
    }
}

//Fonction qui permet d'ajouter une tâche à la liste de tâches
void ajouterTache(ListeTaches* liste, Tache* tache) {
    liste->taches = (Tache**)realloc(liste->taches, (liste->nombreDeTaches + 1) * sizeof(Tache*));

    // Trouver la position d'insertion
    int pos = 0;
    while (pos < liste->nombreDeTaches && liste->taches[pos]->dateCreation > tache->dateCreation) {
        pos++;
    }

    // Déplacer les tâches suivantes vers le bas de la liste
    for (int i = liste->nombreDeTaches; i > pos; i--) {
        liste->taches[i] = liste->taches[i - 1];
    }

    // Insérer la nouvelle tâche à la bonne position
    liste->taches[pos] = tache;
    liste->nombreDeTaches++;

    // Ouvrir le fichier en mode écriture (ce qui efface le contenu existant)
    FILE *fichier = fopen("taches.txt", "w");
    if (fichier == NULL) {
        printf("Erreur d'ouverture du fichier\n");
        return;
    }

    // Écrire toutes les tâches dans le fichier
    ecrireTachesDansFichier(liste, fichier);

    // Fermer le fichier
    fclose(fichier);
}

void modifierTache(ListeTaches* liste, char* nom, StatutTache nouveauStatut) {
    // Trouver la tâche avec le nom donné
    int pos;
    for (pos = 0; pos < liste->nombreDeTaches; pos++) {
        if (strcmp(liste->taches[pos]->nom, nom) == 0) {
            break;
        }
    }

    if (pos == liste->nombreDeTaches) {
        printf("Tâche non trouvée\n");
        return;
    }

    // Retirer la tâche de la liste
    Tache* tache = liste->taches[pos];
    for (int i = pos; i < liste->nombreDeTaches - 1; i++) {
        liste->taches[i] = liste->taches[i + 1];
    }
    liste->nombreDeTaches--;

    // Modifier le statut de la tâche
    tache->statut = nouveauStatut;

    // Réinsérer la tâche à la bonne position
    ajouterTache(liste, tache);
}

//Fonction qui compare les tâches par rapport a leur nombre de jours pour terminer et les trie en ordre croissant
int comparerTaches(const void* a, const void* b) {
    Tache* tacheA = *(Tache**)a;
    Tache* tacheB = *(Tache**)b;
    return tacheA->jourPourTerminer - tacheB->jourPourTerminer;
}

//Fonction qui affiche les tâches
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

//Fonction qui permet de retirer une tâche de la liste de tâches quand son statut est TERMINE
void retirerTachesTerminees(ListeTaches* liste) {
    int i = 0;
    while (i < liste->nombreDeTaches) {
        if (liste->taches[i]->statut == TERMINE) {
            // Libérer la mémoire de la tâche
            free(liste->taches[i]->nom);
            free(liste->taches[i]);

            // Déplacer les tâches restantes vers le haut de la liste
            for (int j = i; j < liste->nombreDeTaches - 1; j++) {
                liste->taches[j] = liste->taches[j + 1];
            }

            // Réduire le nombre de tâches
            liste->nombreDeTaches--;
        } else {
            i++;
        }
    }

    // Ouvrir le fichier en mode écriture (ce qui efface le contenu existant)
    FILE *fichier = fopen("taches.txt", "w");
    if (fichier == NULL) {
        printf("Erreur d'ouverture du fichier\n");
        return;
    }

    // Réécrire toutes les tâches restantes dans le fichier
    ecrireTachesDansFichier(liste, fichier);

    // Fermer le fichier
    fclose(fichier);
}

void ecrireTachesDansFichier(ListeTaches* liste, FILE* fichier) {
    qsort(liste->taches, liste->nombreDeTaches, sizeof(Tache*), comparerTaches);

    for (int i = 0; i < liste->nombreDeTaches; i++) {
        Tache* tache = liste->taches[i];
        fprintf(fichier, "{\n"); // Ajoutez cette ligne
        fprintf(fichier, "Nom: %s\n", tache->nom);
        
        char buffer[80];
        struct tm * timeinfo;
        timeinfo = localtime(&(tache->dateCreation));
        strftime(buffer, 80, "%c", timeinfo); // Utilisez %c pour le format de date/heure local
        fprintf(fichier, "Date de creation: %s\n", buffer);

        fprintf(fichier, "Statut: %s\n", tache->statut == EN_ATTENTE ? "En attente" : tache->statut == EN_COURS ? "En cours" : "Terminee");
        fprintf(fichier, "Jours pour terminer: %d\n", tache->jourPourTerminer);
        fprintf(fichier, "}\n\n"); // Ajoutez cette ligne
    }
}

//Fonction qui permet de mettre à jour les tâches
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

//Fonction qui permet de créer une tâche
Tache* creerTache(char* nom, int joursPourTerminer) {
    Tache* tache = (Tache*)malloc(sizeof(Tache));
    tache->nom = strdup(nom);
    tache->dateCreation = time(NULL);
    tache->statut = EN_ATTENTE;
    tache->jourPourTerminer = joursPourTerminer;
    return tache;
}

//Fonction qui permet de libérer la mémoire de la liste de tâches
void libererListeTaches(ListeTaches* liste) {
    for (int i = 0; i < liste->nombreDeTaches; i++) {
        free(liste->taches[i]->nom);
        free(liste->taches[i]);
    }
    free(liste->taches);
    free(liste);
}