#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>
#include <unistd.h>
#include <err.h>

// Structure pour stocker les statistiques sur une ville dans l'AVL
typedef struct {
    char NomVille[100];
    int NbTrajets;
    int NbDeparts;
    int *routeIDs;  // Tableau dynamique d'entiers pour stocker les RouteID
    int tailleTabRoute;
} StatsVille;


// Structure pour les nœuds de l'AVL
typedef struct NoeudAVL {
    StatsVille stats;
    struct NoeudAVL *gauche;
    struct NoeudAVL *droite;
    int taille;
} NoeudAVL;




// Fonction pour calculer la hauteur d'un nœud
int taille(NoeudAVL *noeud) {
    if (noeud == NULL) return 0;
    return noeud->taille;
}

// Fonction pour obtenir le maximum de deux nombres
int max(int a, int b) {
    return (a > b) ? a : b;
}

// Fonction pour effectuer une rotation à droite
NoeudAVL *rotationdroite(NoeudAVL *y) {
    NoeudAVL *x = y->gauche;
    NoeudAVL *T2 = x->droite;

    // Effectuer la rotation
    x->droite = y;
    y->gauche = T2;

    // Mettre à jour les hauteurs
    y->taille = max(taille(y->gauche), taille(y->droite)) + 1;
    x->taille = max(taille(x->gauche), taille(x->droite)) + 1;

    // Retourner le nouveau nœud racine
    return x;
}

// Fonction pour effectuer une rotation à gauche
NoeudAVL *rotationgauche(NoeudAVL *x) {
    NoeudAVL *y = x->droite;
    NoeudAVL *T2 = y->gauche;

    // Effectuer la rotation
    y->gauche = x;
    x->droite = T2;

    // Mettre à jour les hauteurs
    x->taille = max(taille(x->gauche), taille(x->droite)) + 1;
    y->taille = max(taille(y->gauche), taille(y->droite)) + 1;

    // Retourner le nouveau nœud racine
    return y;
}



// Fonction pour obtenir le facteur d'équilibre d'un nœud
int Equilibre(NoeudAVL *noeud) {
    if (noeud == NULL) return 0;
    return taille(noeud->gauche) - taille(noeud->droite);
}

// Fonction pour insérer un élément dans un AVL
NoeudAVL *insererAVLDeparts(NoeudAVL *noeud, const char *NomVille) {
    // Si l'arbre est vide, créer un nouveau nœud
    if (noeud == NULL) {
        NoeudAVL *nouveaunoeud = (NoeudAVL *)malloc(sizeof(NoeudAVL));
        if (nouveaunoeud == NULL) {
            fprintf(stderr, "Erreur d'allocation de mémoire.\n");
            exit(EXIT_FAILURE);
        }
        strcpy(nouveaunoeud->stats.NomVille, NomVille);
        nouveaunoeud->stats.NbDeparts = 1;
        nouveaunoeud->taille = 1;
        nouveaunoeud->gauche = nouveaunoeud->droite = NULL;
        return nouveaunoeud;
    }

    // Sinon, comparer et insérer récursivement
    int compare = strcmp(NomVille, noeud->stats.NomVille);

    if (compare < 0) {
        noeud->gauche = insererAVLDeparts(noeud->gauche, NomVille);
    } else if (compare > 0) {
        noeud->droite = insererAVLDeparts(noeud->droite, NomVille);
    } else {
        // La ville existe déjà, mettez à jour le compteur ou effectuez une autre action
        printf("Ville déjà existante : %s, mise à jour du compteur.\n", NomVille);
        (noeud->stats.NbDeparts)++;
        return noeud;
    }

    // Mettre à jour la hauteur du nœud actuel
    noeud->taille = 1 + max(taille(noeud->gauche), taille(noeud->droite));

    // Équilibrer l'arbre après l'insertion
    int eq = Equilibre(noeud);

    // Cas Gauche-Gauche
    if (eq > 1 && strcmp(NomVille, noeud->gauche->stats.NomVille) < 0) {
        return rotationdroite(noeud);
    }

    // Cas Droit-Droit
    if (eq < -1 && strcmp(NomVille, noeud->droite->stats.NomVille) > 0) {
        return rotationgauche(noeud);
    }

    // Cas Gauche-Droite
    if (eq > 1 && strcmp(NomVille, noeud->gauche->stats.NomVille) > 0) {
        noeud->gauche = rotationgauche(noeud->gauche);
        return rotationdroite(noeud);
    }

    // Cas Droit-Gauche
    if (eq < -1 && strcmp(NomVille, noeud->droite->stats.NomVille) < 0) {
        noeud->droite = rotationdroite(noeud->droite);
        return rotationgauche(noeud);
    }

    return noeud;
}

// Fonction pour insérer un élément dans un AVL
NoeudAVL *insererAVLTrajets(NoeudAVL *noeud, const char *NomVille, int compteur, int routeID) {
    // Si l'arbre est vide, créer un nouveau nœud
    if (noeud == NULL) {
        NoeudAVL *nouveaunoeud = (NoeudAVL *)malloc(sizeof(NoeudAVL));
        if (nouveaunoeud == NULL) {
            fprintf(stderr, "Erreur d'allocation de mémoire.\n");
            exit(EXIT_FAILURE);
        }
        strcpy(nouveaunoeud->stats.NomVille, NomVille);
        nouveaunoeud->stats.NbTrajets = compteur;
        nouveaunoeud->stats.tailleTabRoute = 1;  // Initialiser le compteur à un
        nouveaunoeud->stats.routeIDs = (int *)malloc(sizeof(int));  // Allouer mémoire pour le tableau
        if (nouveaunoeud->stats.routeIDs == NULL) {
            fprintf(stderr, "Erreur d'allocation de mémoire pour routeIDs.\n");
            exit(EXIT_FAILURE);
        }
        nouveaunoeud->stats.routeIDs[0] = routeID;  // Ajouter le RouteID au tableau

        nouveaunoeud->taille = 1;
        nouveaunoeud->gauche = nouveaunoeud->droite = NULL;
        return nouveaunoeud;
    }

    // Sinon, comparer et insérer récursivement
    int compare = strcmp(NomVille, noeud->stats.NomVille);

    if (compare < 0) {
        noeud->gauche = insererAVLTrajets(noeud->gauche, NomVille, compteur, routeID);
    } else if (compare > 0) {
        noeud->droite = insererAVLTrajets(noeud->droite, NomVille, compteur, routeID);
    } else {
        // La ville existe déjà, mettez à jour le compteur ou effectuez une autre action
        printf("Ville déjà existante : %s, mise à jour du compteur.\n", NomVille);
        (noeud->stats.NbTrajets)++;
        noeud->stats.tailleTabRoute++;
        noeud->stats.routeIDs = (int *)realloc(noeud->stats.routeIDs, sizeof(int) * noeud->stats.tailleTabRoute);
        if (noeud->stats.routeIDs == NULL) {
            fprintf(stderr, "Erreur d'allocation dynamique de mémoire pour routeIDs.\n");
            exit(EXIT_FAILURE);
        }
        noeud->stats.routeIDs[noeud->stats.tailleTabRoute - 1] = routeID;
        return noeud;
    }

    // Mettre à jour la hauteur du nœud actuel
    noeud->taille = 1 + max(taille(noeud->gauche), taille(noeud->droite));

    // Équilibrer l'arbre après l'insertion
    int eq = Equilibre(noeud);

    // Cas Gauche-Gauche
    if (eq > 1 && strcmp(NomVille, noeud->gauche->stats.NomVille) < 0) {
        return rotationdroite(noeud);
    }

    // Cas Droit-Droit
    if (eq < -1 && strcmp(NomVille, noeud->droite->stats.NomVille) > 0) {
        return rotationgauche(noeud);
    }

    // Cas Gauche-Droite
    if (eq > 1 && strcmp(NomVille, noeud->gauche->stats.NomVille) > 0) {
        noeud->gauche = rotationgauche(noeud->gauche);
        return rotationdroite(noeud);
    }

    // Cas Droit-Gauche
    if (eq < -1 && strcmp(NomVille, noeud->droite->stats.NomVille) < 0) {
        noeud->droite = rotationdroite(noeud->droite);
        return rotationgauche(noeud);
    }

    return noeud;
}

// Fonction de recherche dans un AVL
NoeudAVL *rechercheAVL(NoeudAVL *noeud, const char *NomVille) {
    // Parcours récursif de l'arbre
    if (noeud == NULL){
        return NULL;
    }
  
    if (strcmp(NomVille, noeud->stats.NomVille) == 0) {
        return noeud;
    }
  
    if (strcmp(NomVille, noeud->stats.NomVille) < 0) {
        return rechercheAVL(noeud->gauche, NomVille);
    } else {
        return rechercheAVL(noeud->droite, NomVille);
    }
}

int RouteIDTrouve(int *tab, int routeID, int tailleTabRoute){
    for (int i = 0; i < tailleTabRoute; i++) {
        if (tab[i] == routeID) {
            return 1;  // RouteID trouvé
        }
    }
    return 0;
}
// Fonction pour libérer récursivement la mémoire d'un arbre AVL
void libererAVL(NoeudAVL *noeud) {
    if (noeud == NULL) {
        return;
    }

    // Libérer les sous-arbres gauche et droit
    libererAVL(noeud->gauche);
    libererAVL(noeud->droite);

    // Libérer la mémoire du nœud courant, y compris routeIDs
    free(noeud->stats.routeIDs);
    free(noeud);
}

// Fonction pour libérer toute la structure AVL, y compris la racine
void libererAVLArbre(NoeudAVL **noeud) {
    if (*noeud != NULL) {
        libererAVL(*noeud);  // Libérer la mémoire des sous-arbres gauche et droit
        *noeud = NULL;    // Définir la racine sur NULL après la libération de la sous-structure AVL
    }
}

void afficherTopVilles(StatsVille TopVilles[], int compteur) {
    printf("Top 10 des villes avec le nombre total de Trajets et Départs :\n");
    for (int i = 0; i < compteur; i++) {
        printf("%s : %d trajets,  %d départs\n", TopVilles[i].NomVille, TopVilles[i].NbTrajets, TopVilles[i].NbDeparts);
    }
    printf("\n");
}

void Departs(NoeudAVL **avl, const char *ligne) {
    int stepID;
    char villeA[256];

    if (sscanf(ligne, "%*d;%d;%255[^;];%*[^;];%*f;%*[^;\n]", &stepID, villeA) == 2) {
        if (stepID==1){
            NoeudAVL *existenoeud = rechercheAVL(*avl, villeA);

            if (existenoeud == NULL) {
                *avl = insererAVLDeparts(*avl, villeA);
            } else {
                // Ville existante, incrémenter le compteur de départs
                existenoeud->stats.NbDeparts++;
            }
        }
     
    } else {
        fprintf(stderr, "Erreur de lecture de la ligne du fichier.\n");
        exit(EXIT_FAILURE);
    }
}

void Trajets(NoeudAVL **avl, const char *ligne) {
    int routeID;
    char villeA[256], villeB[256];

    if (sscanf(ligne, "%d;%*d;%255[^;];%255[^;];%*f;%*[^;\n]", &routeID, villeA, villeB) == 3) {
        NoeudAVL *existenoeudA = rechercheAVL(*avl, villeA);
        if (existenoeudA == NULL) {
            *avl = insererAVLTrajets(*avl, villeA, 1, routeID);
        } else {
            int existeRouteID = RouteIDTrouve(existenoeudA->stats.routeIDs, routeID, existenoeudA->stats.tailleTabRoute);
        
            if(existeRouteID==0){
                existenoeudA->stats.NbTrajets++;
                existenoeudA->stats.tailleTabRoute++;
                existenoeudA->stats.routeIDs = (int *)realloc(existenoeudA->stats.routeIDs,
                                                            sizeof(int) * existenoeudA->stats.tailleTabRoute);
                if (existenoeudA->stats.routeIDs == NULL) {
                    fprintf(stderr, "Erreur d'allocation dynamique de mémoire pour routeIDs.\n");
                    exit(EXIT_FAILURE);
                }
                existenoeudA->stats.routeIDs[existenoeudA->stats.tailleTabRoute - 1] = routeID;
            } 

        }      

        NoeudAVL *existenoeudB = rechercheAVL(*avl, villeB);
        if (existenoeudB == NULL) {
            *avl = insererAVLTrajets(*avl, villeB, 1, routeID);
        } else {
            int existeRouteID = RouteIDTrouve(existenoeudB->stats.routeIDs, routeID, existenoeudB->stats.tailleTabRoute);
            

            if(existeRouteID==0){
                existenoeudB->stats.NbTrajets++;
                existenoeudB->stats.tailleTabRoute++;
                existenoeudB->stats.routeIDs = (int *)realloc(existenoeudB->stats.routeIDs,
                                                            sizeof(int) * existenoeudB->stats.tailleTabRoute);
                if (existenoeudB->stats.routeIDs == NULL) {
                    fprintf(stderr, "Erreur d'allocation dynamique de mémoire pour routeIDs.\n");
                    exit(EXIT_FAILURE);
                }
                existenoeudB->stats.routeIDs[existenoeudB->stats.tailleTabRoute - 1] = routeID;
            }

        }    
    } else {
        fprintf(stderr, "Erreur de lecture de la ligne du fichier.\n");
        exit(EXIT_FAILURE);
    }
}

void procedure(FILE *fichier, NoeudAVL **avl) {
    setlocale(LC_ALL, "");

    char tab[256];
    if (fgets(tab, sizeof(tab), fichier) == NULL) {
        fprintf(stderr, "Erreur lors de la lecture de la première ligne du fichier.\n");
        exit(EXIT_FAILURE);
    }

    char ligne[256];
    while (fgets(ligne, sizeof(ligne), fichier) != NULL) {
        Trajets(avl, ligne);
        Departs(avl, ligne);
        
        
    }
}

// Fonction pour récupérer les 10 plus grandes valeurs de compteur dans un AVL
void Top10compteurs(const NoeudAVL *noeud, StatsVille *resultat, int *index) {
    if (noeud != NULL) {
        // Parcours récursif de l'arbre
        Top10compteurs(noeud->gauche, resultat, index);

        // Traitement du nœud courant
        if (*index < 10 || noeud->stats.NbTrajets > resultat[9].NbTrajets ||
            (noeud->stats.NbTrajets == resultat[9].NbTrajets && strcmp(noeud->stats.NomVille, resultat[9].NomVille) < 0)) {
            // Si le tableau n'est pas plein, ou la valeur est plus grande que la plus petite du tableau,
            // ou la valeur est égale mais le nom de ville est plus petit alphabétiquement
            if (*index == 10) {
                // Si le tableau est plein, enlever la dernière valeur
                (*index)--;
            }

            // Trouver la bonne position pour insérer le nœud courant dans le tableau
            int i = *index - 1;
            while (i >= 0 && (noeud->stats.NbTrajets > resultat[i].NbTrajets ||
                              (noeud->stats.NbTrajets == resultat[i].NbTrajets && strcmp(noeud->stats.NomVille, resultat[i].NomVille) < 0))) {
                resultat[i + 1] = resultat[i];
                i--;
            }

            // Insérer le nœud courant dans le tableau à la bonne position
            resultat[i + 1] = noeud->stats;
            (*index)++;
        }

        Top10compteurs(noeud->droite, resultat, index);
    }
}

// Fonction de comparaison insensible à la casse pour qsort
int compareNomVilles(const void *a, const void *b) {
    return strcasecmp(((StatsVille *)a)->NomVille, ((StatsVille *)b)->NomVille);
}

void SauvegarderResultats(const char *fichier, const StatsVille *TopVilles, int TopVillescompteur) {
    FILE *f = fopen(fichier, "wb");
    if (f == NULL) {
        fprintf(stderr, "Impossible d'ouvrir le fichier de sauvegarde.\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < TopVillescompteur; i++) {
        fprintf(f, "%s;%d;%d\n", TopVilles[i].NomVille, TopVilles[i].NbTrajets, TopVilles[i].NbDeparts);
    }

    fclose(f);
}

int main() {
    // Revenir en arrière d'un dossier (aller dans le dossier "projet")
    int err = chdir("..");
    if (err)
        errx(1, "main: chdir error");

    // Ouvrir le fichier CSV en mode lecture
    const char *fichier = "stats/stats.csv";
    FILE *fichier = fopen(fichier, "r");

    if (fichier == NULL) {
        fprintf(stderr, "Impossible d'ouvrir le fichier CSV.\n");
        return EXIT_FAILURE;
    }
    
    NoeudAVL *avl = NULL;

    procedure(fichier, &avl);

    StatsVille TopVilles[10];
    int TopVillescompteur = 0;

    Top10compteurs(avl, TopVilles, &TopVillescompteur);

    qsort(TopVilles, TopVillescompteur, sizeof(StatsVille), compareNomVilles);
    // Affichage du résultat
    afficherTopVilles(TopVilles,TopVillescompteur);
    SauvegarderResultats("temp/stats_t.dat", TopVilles, TopVillescompteur);
    // Fermer le fichier
    fclose(fichier);
    
    // Libérer la mémoire de l'arbre AVL des départs
    libererAVLArbre(&avl);

    return 0; // Terminer le programme avec succès
}
