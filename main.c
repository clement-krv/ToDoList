#include "headers/todolist.h"

// Fonction main qui prend en compte une interface utilisateur pour ajouter des taches, les afficher et les retirer
int main()
{
    char nomUtilisateur[100];
    printf("Entrez votre nom: ");
    scanf("%s", nomUtilisateur);

    char nomFichier[150];
    snprintf(nomFichier, sizeof(nomFichier), "taches/%s.txt", nomUtilisateur);

    ListeTaches *liste = creerListeTaches(nomFichier);

    while (1)
    {
        printf("\n===== Menu =====\n");
        printf("1. Ajouter une tache\n");
        printf("2. Afficher les taches\n");
        printf("3. Retirer les taches terminees\n");
        printf("4. Modifier le statut d'une tache\n");
        printf("5. Quitter\n");
        printf("Choisissez une option: ");

        int choix;
        scanf("%d", &choix);

        switch (choix)
        {
        case 1:
        {
            char nom[100];
            int joursPourTerminer;
            printf("\n===== Ajouter une tache =====\n");
            printf("Entrez le nom de la tache: ");
            scanf("%s", nom);
            printf("Entrez le nombre de jours pour terminer la tache: ");
            scanf("%d", &joursPourTerminer);
            Tache *tache = creerTache(nom, joursPourTerminer);
            ajouterTache(liste, tache, nomFichier);
            break;
        }
        case 2:
            printf("\n===== Afficher les taches =====\n");
            afficherTaches(liste);
            break;
        case 3:
            printf("\n===== Taches terminees retiree =====\n");
            retirerTachesTerminees(liste, nomFichier);
            break;
        case 4:
        {
            char nom[100];
            int nouveauStatut;
            printf("\n===== Modifier le statut d'une tache =====\n");
            printf("Entrez le nom de la tache: ");
            scanf("%s", nom);
            printf("Entrez le nouveau statut de la tache (0 = en attente, 1 = en cours, 2 = terminee): ");
            scanf("%d", &nouveauStatut);
            modifierTache(liste, nom, (StatutTache)nouveauStatut, nomFichier);
            break;
        }
        case 5:
            libererListeTaches(liste);
            return 0;
        default:
            printf("Option non valide. Veuillez choisir une option entre 1 et 5.\n");
        }
    }

    return 0;
}