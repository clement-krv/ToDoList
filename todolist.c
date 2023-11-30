#include "headers/todolist.h"

// Fonction qui permet de créer une liste de tâches
ListeTaches *creerListeTaches(char *nomFichier)
{
    ListeTaches *liste = (ListeTaches *)malloc(sizeof(ListeTaches));
    liste->taches = NULL;
    liste->nombreDeTaches = 0;

    // Ouvrir le fichier en mode lecture
    FILE *fichier = fopen(nomFichier, "r");
    if (fichier == NULL)
    {
        // Si le fichier n'existe pas, retourner la liste vide
        return liste;
    }

    // Lire les tâches du fichier
    lireTachesDepuisFichier(liste, fichier, nomFichier);

    // Fermer le fichier
    fclose(fichier);

    // Trier les tâches par date de création
    trierTachesParDate(liste);

    return liste;
}

void trierTachesParDate(ListeTaches *liste)
{
    for (int i = 0; i < liste->nombreDeTaches - 1; i++)
    {
        for (int j = 0; j < liste->nombreDeTaches - i - 1; j++)
        {
            if (liste->taches[j]->dateCreation < liste->taches[j + 1]->dateCreation)
            {
                // Échanger les tâches
                Tache *temp = liste->taches[j];
                liste->taches[j] = liste->taches[j + 1];
                liste->taches[j + 1] = temp;
            }
        }
    }
}

void lireTachesDepuisFichier(ListeTaches *liste, FILE *fichier, char *nomFichier)
{
    char nom[100];
    long dateCreation;
    int statut;
    int jourPourTerminer;

    // Lire chaque tâche du fichier
    while (fgets(nom, sizeof(nom), fichier) != NULL && fscanf(fichier, "%ld\n%d\n%d\n", &dateCreation, &statut, &jourPourTerminer) == 3)
    {
        nom[strcspn(nom, "\n")] = 0; // Supprimer le caractère de nouvelle ligne à la fin

        // Créer une nouvelle tâche avec les données lues
        Tache *tache = (Tache *)malloc(sizeof(Tache));
        tache->nom = strdup(nom);
        tache->dateCreation = (time_t)dateCreation;
        tache->statut = (StatutTache)statut;
        tache->jourPourTerminer = jourPourTerminer;

        // Ajouter la tâche à la liste
        ajouterTacheSansEcrireDansFichier(liste, tache);
    }
}

void ajouterTacheSansEcrireDansFichier(ListeTaches *liste, Tache *tache)
{
    // Si jourPourTerminer est 0, définir le statut sur TERMINE
    if (tache->jourPourTerminer == 0)
    {
        tache->statut = TERMINE;
    }
    // Si le statut est 1, définir le statut sur EN_COURS
    else if (tache->statut == 1)
    {
        tache->statut = EN_COURS;
    }

    // Ajouter la tâche à la liste
    liste->taches = (Tache **)realloc(liste->taches, sizeof(Tache *) * (liste->nombreDeTaches + 1));
    liste->taches[liste->nombreDeTaches] = tache;
    liste->nombreDeTaches++;
}

// Fonction qui permet d'ajouter une tâche à la liste de tâches
void ajouterTache(ListeTaches *liste, Tache *tache, char *nomFichier)
{
    // Si jourPourTerminer est 0, définir le statut sur TERMINE
    if (tache->jourPourTerminer == 0)
    {
        tache->statut = TERMINE;
    }
    else if (tache->statut == 1)
    {
        tache->statut = EN_COURS;
    }

    // Ajouter la tâche à la liste
    liste->taches = (Tache **)realloc(liste->taches, sizeof(Tache *) * (liste->nombreDeTaches + 1));
    liste->taches[liste->nombreDeTaches] = tache;
    liste->nombreDeTaches++;

    // Ouvrir le fichier en mode écriture
    FILE *fichier = fopen(nomFichier, "w");
    if (fichier != NULL)
    {
        // Écrire les tâches dans le fichier
        ecrireTachesDansFichier(liste, fichier);

        // Fermer le fichier
        fclose(fichier);
    }
    else
    {
        printf("Erreur lors de l'ouverture du fichier %s\n", nomFichier);
    }
}

void modifierTache(ListeTaches *liste, char *nom, StatutTache nouveauStatut, char *nomFichier)
{
    // Trouver la tâche avec le nom donné
    int pos;
    for (pos = 0; pos < liste->nombreDeTaches; pos++)
    {
        if (strcmp(liste->taches[pos]->nom, nom) == 0)
        {
            break;
        }
    }

    if (pos == liste->nombreDeTaches)
    {
        printf("Tâche non trouvée\n");
        return;
    }

    // Retirer la tâche de la liste
    Tache *tache = liste->taches[pos];
    for (int i = pos; i < liste->nombreDeTaches - 1; i++)
    {
        liste->taches[i] = liste->taches[i + 1];
    }
    liste->nombreDeTaches--;

    // Modifier le statut de la tâche
    tache->statut = nouveauStatut;

    // Réinsérer la tâche à la bonne position
    ajouterTache(liste, tache, nomFichier);
}

// Fonction qui compare les tâches par rapport a leur nombre de jours pour terminer et les trie en ordre croissant
int comparerTaches(const void *a, const void *b)
{
    Tache *tacheA = *(Tache **)a;
    Tache *tacheB = *(Tache **)b;
    return tacheA->jourPourTerminer - tacheB->jourPourTerminer;
}



// Fonction qui permet de retirer une tâche de la liste de tâches quand son statut est TERMINE
void retirerTachesTerminees(ListeTaches *liste, char *nomFichier)
{
    int i = 0;
    while (i < liste->nombreDeTaches)
    {
        if (liste->taches[i]->statut == TERMINE)
        {
            // Retirer la tâche de la liste
            for (int j = i; j < liste->nombreDeTaches - 1; j++)
            {
                liste->taches[j] = liste->taches[j + 1];
            }
            liste->nombreDeTaches--;
        }
        else
        {
            i++;
        }
    }

    // Ouvrir le fichier en mode écriture
    FILE *fichier = fopen(nomFichier, "w");
    if (fichier != NULL)
    {
        // Écrire les tâches dans le fichier
        ecrireTachesDansFichier(liste, fichier);

        // Fermer le fichier
        fclose(fichier);
    }
    else
    {
        printf("Erreur lors de l'ouverture du fichier %s\n", nomFichier);
    }
}

void ecrireTachesDansFichier(ListeTaches *liste, FILE *fichier)
{
    qsort(liste->taches, liste->nombreDeTaches, sizeof(Tache *), comparerTaches);

    int tachesEnCours = 0;
    for (int i = 0; i < liste->nombreDeTaches; i++)
    {
        Tache *tache = liste->taches[i];
        fprintf(fichier, "{\n");
        fprintf(fichier, "Nom: %s\n", tache->nom);

        char buffer[80];
        struct tm *timeinfo;
        timeinfo = localtime(&(tache->dateCreation));
        strftime(buffer, 80, "%c", timeinfo);
        fprintf(fichier, "Date de creation: %s\n", buffer);

        // Utilisez la même logique pour le statut de la tâche
        if (tache->statut == TERMINE)
        {
            fprintf(fichier, "Statut: Terminee\n");
        }
        else if (tachesEnCours < 5)
        {
            fprintf(fichier, "Statut: En cours\n");
            tachesEnCours++;
        }
        else
        {
            fprintf(fichier, "Statut: En attente\n");
        }
        fprintf(fichier, "Jours pour terminer: %d\n", tache->jourPourTerminer);
        fprintf(fichier, "}\n\n");
    }
}

// Fonction qui permet de mettre à jour les tâches
void mettreAJourTaches(ListeTaches *liste)
{
    // Obtenir la date actuelle
    time_t maintenant = time(NULL);

    // Trier les tâches par le nombre de jours pour terminer
    qsort(liste->taches, liste->nombreDeTaches, sizeof(Tache *), comparerTaches);

    // Mettre à jour le statut des tâches
    int tachesEnCours = 0;
    for (int i = 0; i < liste->nombreDeTaches; i++)
    {
        // Calculer le nombre de jours depuis la création de la tâche
        double joursDepuisCreation = difftime(maintenant, liste->taches[i]->dateCreation) / (60 * 60 * 24);

        // Mettre à jour le nombre de jours pour terminer
        liste->taches[i]->jourPourTerminer -= (int)joursDepuisCreation;

        // Mettre à jour le statut de la tâche
        if (liste->taches[i]->jourPourTerminer <= 0)
        {
            liste->taches[i]->statut = TERMINE;
        }
        else if (tachesEnCours < 5)
        {
            liste->taches[i]->statut = EN_COURS;
            tachesEnCours++;
        }
        else
        {
            liste->taches[i]->statut = EN_ATTENTE;
        }

        // Mettre à jour la date de création de la tâche
        liste->taches[i]->dateCreation = maintenant;
    }
}

// Fonction qui permet de créer une tâche
Tache *creerTache(char *nom, int joursPourTerminer)
{
    Tache *tache = (Tache *)malloc(sizeof(Tache));
    tache->nom = strdup(nom);
    tache->dateCreation = time(NULL);
    if (joursPourTerminer < 0)
        tache->statut = TERMINE;
    else
        tache->statut = EN_ATTENTE;
    tache->jourPourTerminer = joursPourTerminer;
    return tache;
}

// Fonction qui permet de libérer la mémoire de la liste de tâches
void libererListeTaches(ListeTaches *liste)
{
    for (int i = 0; i < liste->nombreDeTaches; i++)
    {
        free(liste->taches[i]->nom);
        free(liste->taches[i]);
    }
    free(liste->taches);
    free(liste);
}