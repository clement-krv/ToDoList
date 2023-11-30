# Projet Todo List

Ce projet a été réalisé en binôme dans le cadre de nos études à l'[ESGI](https://www.esgi.fr) (École Supérieure de Génie Informatique). Il s'agit d'une application de gestion de tâches (Todo List) écrite en C.

## Fonctionnalités

- Ajouter une tâche
- Supprimer une tâche
- Met les 5 tâches les plus urgentes en haut de la liste et en statut "en cours"
- Décrémente le nombre de jour pour terminer par rapport a la date du jour et de création de la tâche
- Pour voir les tâche veuillez aller dans le dossier *"taches"* et ouvrir le fichier qui porte le nom d'utilisateur, les taches sont enregistrer dans ce fichier.

## Prérequis

Pour compiler et exécuter ce projet, vous aurez besoin d'un compilateur C. Si vous n'en avez pas déjà un, vous pouvez en télécharger un en ligne.

## Explication pour quelques exemples de fonctions 

**POUR RETIRER UNE TACHES**

Elle initialise un compteur i à 0.
Elle entre dans une boucle while qui continue tant que i est inférieur au nombre de tâches dans la liste.
Dans cette boucle, elle vérifie si le statut de la tâche à l'index i est TERMINE.
Si c'est le cas, elle entre dans une boucle for qui déplace toutes les tâches suivantes une position en arrière dans le tableau, écrasant ainsi la tâche terminée.
Elle décrémente ensuite le nombre total de tâches dans la liste.
Si le statut de la tâche n'est pas TERMINE, elle incrémente simplement i pour passer à la prochaine tâche.
La boucle while continue jusqu'à ce que toutes les tâches aient été vérifiées.

**COMPARATIF DES TACHES EN FONCTIONS DES JOUR - TRIER**

Elle prend deux pointeurs génériques void *a et void *b comme arguments. Ces pointeurs représentent les tâches à comparer.
Elle convertit ces pointeurs génériques en pointeurs de type Tache pour pouvoir accéder aux attributs de la tâche.
Elle retourne la différence entre le nombre de jours pour terminer la tâche A et le nombre de jours pour terminer la tâche B. Si la valeur retournée est négative, cela signifie que la tâche A a moins de jours pour terminer que la tâche B. Si la valeur est positive, la tâche A a plus de jours pour terminer que la tâche B. Si la valeur est zéro, cela signifie que les deux tâches ont le même nombre de jours pour terminer.

**MAJ DU STATUT DES TACHES**

Il vérifie si le nombre de jours pour terminer la tâche courante (à l'index i) est inférieur ou égal à 0. Si c'est le cas, il change le statut de la tâche à TERMINE.
Si le nombre de jours pour terminer n'est pas inférieur ou égal à 0, il vérifie ensuite si le nombre de tâches en cours est inférieur à 5. Si c'est le cas, il change le statut de la tâche à EN_COURS et incrémente le compteur tachesEnCours.
Si aucune des conditions précédentes n'est remplie, cela signifie que le nombre de jours pour terminer est supérieur à 0 et que le nombre de tâches en cours est égal ou supérieur à 5. Dans ce 

## Compilation

Une fois que vous avez un compilateur C installé, vous pouvez compiler le projet en utilisant la commande suivante :

```bash
gcc -o main.exe .\main.c .\todolist.c
```

## Exécution

Une fois que vous avez compilé le projet, vous pouvez l'exécuter en utilisant la commande suivante :

```bash
.\main.exe
```

## Analyse

Pour l'utilisation des différentes fonction présente dans ce projet, nous nous sommes aider d'internet , [StackOverflow](https://stackoverflow.com/) ainsi que des IA comme [ChatGPT](https://chat.openai.com) et [Github Copilot](https://copilot.github.com/). En comulant ces différentes sources ainsi que nos connaissances, nous avons pu réaliser ce projet.

## Conclusion

Ce projet nous a permis de mettre en pratique nos connaissances en C et de nous familiariser avec les différents outils de développement. Nous avons également pu découvrir le fonctionnement d'un projet en binôme et les différentes méthodes de travail. 

## Auteurs

Ce projet a été réalisé par : 

- **Alexis BRETON** - [GitHub](https://github.com/alexisb31) - [LinkedIn](https://www.linkedin.com/in/alexis-breton-84b66328b/)
- **Clément Kerviche** - [GitHub](https://github.com/clement-krv) - [LinkedIn](https://www.linkedin.com/in/clément-kerviche-6b7a44262/cl%C3%A9ment-kerviche-8b1b3b1b8/)
