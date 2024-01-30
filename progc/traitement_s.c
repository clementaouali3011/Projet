#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>
#include <math.h>
#include <unistd.h>



// Structure pour stocker les statistiques sur une ville dans l'AVL
typedef struct {
    int routeID;
    double *Distances;
    int tailleTabDistances;
    double DistanceMin;
    double DistanceMax;
    double DistanceMoy;
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
    if (y == NULL || y->gauche == NULL) {
        return y;  // Rien à faire s'il n'y a pas de sous-arbre droit
    }
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
    if (x == NULL || x->droite == NULL) {
        return x;  // Rien à faire s'il n'y a pas de sous-arbre droit
    }
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


//fonction qui assigne la valeur maximum du tab Distances à la variable DistanceMax
void MajDistanceMAX (NoeudAVL *noeud){
    if (noeud == NULL) {
        return;  // Cas de base : nœud NULL
    }
    noeud->stats.DistanceMax = noeud->stats.Distances[noeud->stats.tailleTabDistances - 1];  // Utiliser la dernière valeur

    for (int i=0;i<noeud->stats.tailleTabDistances; i++){
        if (noeud->stats.Distances[i]>noeud->stats.DistanceMax){
            noeud->stats.DistanceMax = noeud->stats.Distances[i];
        }
    }
}    

//fonction qui assigne la valeur minimum du tab Distances à la variable DistanceMin
void MajDistanceMIN (NoeudAVL *noeud){
    if (noeud == NULL) {
        return;  // Cas de base : nœud NULL
    }
    noeud->stats.DistanceMin = noeud->stats.Distances[noeud->stats.tailleTabDistances - 1];  // Utiliser la dernière valeur

    for (int i=0;i < noeud->stats.tailleTabDistances; i++){
        if (noeud->stats.Distances[i]<noeud->stats.DistanceMin){
            noeud->stats.DistanceMin = noeud->stats.Distances[i];
        }
    }    
}



void MajDistanceMOY (NoeudAVL *noeud){
    if (noeud == NULL) {
        return;  // Cas de base : nœud NULL
    }

    if (noeud->stats.tailleTabDistances == 0) {
        noeud->stats.DistanceMoy = 0;  // Cas de base : tableau vide
    } else {
        double s = 0;
        for (int i = 0; i < noeud->stats.tailleTabDistances; i++) {
            s += noeud->stats.Distances[i];
        }
        noeud->stats.DistanceMoy = s / noeud->stats.tailleTabDistances;
    }
}

// Fonction pour insérer un élément dans un AVL
NoeudAVL *insererAVL(NoeudAVL *noeud, int routeID, double distance) {
    // Si l'arbre est vide, créer un nouveau nœud
    if (noeud == NULL) {
        NoeudAVL *nouveaunoeud = (NoeudAVL *)malloc(sizeof(NoeudAVL));
        if (nouveaunoeud == NULL) {
            fprintf( "Erreur d'allocation de mémoire.\n");
            exit(EXIT_FAILURE);
        }
        nouveaunoeud->stats.routeID = routeID;
        nouveaunoeud->stats.Distances = malloc(sizeof(double));
        if (nouveaunoeud->stats.Distances == NULL) {
            fprintf( "Erreur d'allocation de mémoire pour routeIDs.\n");
            exit(EXIT_FAILURE);
        }
        nouveaunoeud->stats.tailleTabDistances = 1;  // Initialize tailleTabDistances to 1
        nouveaunoeud->stats.Distances[0] = distance;        
        nouveaunoeud->stats.DistanceMax = distance;
        nouveaunoeud->stats.DistanceMin = distance;
        nouveaunoeud->stats.DistanceMoy = distance;
        nouveaunoeud->taille = 1;
        nouveaunoeud->gauche = nouveaunoeud->droite = NULL;
        return nouveaunoeud;
    }

    // Sinon, comparer et insérer récursivement

    if (routeID < noeud->stats.routeID) {
        noeud->gauche = insererAVL(noeud->gauche, routeID, distance);
    } else {
        noeud->droite = insererAVL(noeud->droite, routeID, distance);
    }
    // Mettre à jour la hauteur du nœud actuel
    noeud->taille = 1 + max(taille(noeud->gauche), taille(noeud->droite));

    // Équilibrer l'arbre après l'insererion
    int eq = Equilibre(noeud);

    // Cas Gauche-Gauche
    if (eq > 1 && routeID < noeud->gauche->stats.routeID) {
        return rotationdroite(noeud);
    }

    // Cas Droit-Droit
    if (eq < -1 && routeID > noeud->droite->stats.routeID) {
        return rotationgauche(noeud);
    }

    // Cas Gauche-Droite
    if (eq > 1 && routeID > noeud->gauche->stats.routeID) {
        noeud->gauche = rotationgauche(noeud->gauche);
        return rotationdroite(noeud);
    }

    // Cas Droit-Gauche
    if (eq < -1 && routeID < noeud->droite->stats.routeID) {
        noeud->droite = rotationdroite(noeud->droite);
        return rotationgauche(noeud);
    }

    return noeud;
}


// Fonction de recherche dans un AVL
NoeudAVL *rechercheAVL(NoeudAVL *noeud, int routeID) {
    // Parcours récursif de l'arbre
    if (noeud == NULL){
        return NULL;
    }
    
    if (routeID == noeud->stats.routeID) {
        return noeud;
    }
    if (routeID < noeud->stats.routeID){
        return rechercheAVL(noeud->gauche, routeID);
    } else {
        return rechercheAVL(noeud->droite, routeID);
    }
}

void Distance (NoeudAVL **avl, const char *ligne){
    int routeID;
    double distance;

    // Utiliser le point comme séparateur décimal
    setlocale(LC_NUMERIC, "C");
    
    if (sscanf(ligne, "%d;%*d;%*255[^;];%*255[^;];%lf;%*[^;\n]", &routeID, &distance) == 2) {
        NoeudAVL *existenoeud = rechercheAVL(*avl, routeID);
        if (existenoeud == NULL) {
            *avl = insererAVL(*avl, routeID, distance);
        } else {
            
            existenoeud->stats.Distances = (double *)realloc(existenoeud->stats.Distances, sizeof(double) * (existenoeud->stats.tailleTabDistances + 1));

            if (existenoeud->stats.Distances == NULL) {
                fprintf( "Erreur d'allocation dynamique de mémoire pour routeIDs.\n");
                exit(EXIT_FAILURE);
            }
            existenoeud->stats.Distances[existenoeud->stats.tailleTabDistances] = distance;
            existenoeud->stats.tailleTabDistances++; // Increment tailleTabDistances
            MajDistanceMAX (existenoeud);
            MajDistanceMIN (existenoeud);
            MajDistanceMOY (existenoeud);
        }
    } else {
        fprintf( "Erreur de lecture de la ligne du fichier.\n");
        exit(EXIT_FAILURE);
    }
}

void procedure(FILE *fichier, NoeudAVL **avl) {
    setlocale(LC_ALL, "");

    char tab[256];
    if (fgets(tab, sizeof(tab), fichier) == NULL) {
        fprintf( "Erreur lors de la lecture de la première ligne du fichier.\n");
        exit(EXIT_FAILURE);
    }

    char ligne[256];
    while (fgets(ligne, sizeof(ligne), fichier) != NULL) {
        Distance (avl, ligne);
    }
}


void Top50compteurs(const NoeudAVL *noeud, StatsVille *result, int *index) {
    if (noeud != NULL) {
        Top50compteurs(noeud->gauche, result, index);

        if (*index < 50 || (noeud->stats.DistanceMax - noeud->stats.DistanceMin) > (result[49].DistanceMax - result[49].DistanceMin)) {
            if (*index == 50) {
                (*index)--;
            }

            // Insérer le nœud courant dans le tableau à la bonne position
            int i = *index - 1;
            while (i >= 0 && (noeud->stats.DistanceMax - noeud->stats.DistanceMin) > (result[i].DistanceMax - result[i].DistanceMin)) {
                result[i + 1] = result[i];
                i--;
            }

            result[i + 1] = noeud->stats;
            (*index)++;
        }

        Top50compteurs(noeud->droite, result, index);
    }
}


void afficherTopsDistances(StatsVille topVilles[], int compteur) {
    printf("Top 50 des routes avec le plus grand écart entre la valeur max et la valeur min :\n");
    for (int i = 0; i < compteur; i++) {
        printf("route n°%d : %g distance max, %g distance min, %g distance moyenne, %g écart maxmin\n",
        topVilles[i].routeID, topVilles[i].DistanceMax, topVilles[i].DistanceMin, topVilles[i].DistanceMoy, (topVilles[i].DistanceMax - topVilles[i].DistanceMin));


    }
    printf("\n");
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
    free(noeud->stats.Distances);
    free(noeud);
}

// Fonction pour libérer toute la structure AVL, y compris la racine
void libererAVLArbre(NoeudAVL **noeud) {
    if (*noeud != NULL) {
        libererAVL(*noeud);  // Libérer la mémoire des sous-arbres gauche et droit
        *noeud = NULL;    // Définir la racine sur NULL après la libération de la sous-structure AVL
    }
}

void SauvegarderResultats(const char *fichier, const StatsVille *topEcartsDistances, int topEcartsDistancescompteur) {
    FILE *f = fopen(fichier, "wb");
    if (f == NULL) {
        fprintf( "Impossible d'ouvrir le fichier de sauvegarde pour l'écriture.\n");
        perror("Error");
        exit(EXIT_FAILURE);
    }


    for (int i = 0; i < topEcartsDistancescompteur; i++) {
        fprintf(f, "%d;%d;%g;%g;%g;%g\n", (i+1), topEcartsDistances[i].routeID, topEcartsDistances[i].DistanceMin, topEcartsDistances[i].DistanceMoy, topEcartsDistances[i].DistanceMax, (topEcartsDistances[i].DistanceMax-topEcartsDistances[i].DistanceMin));
    }

    fclose(f);
}

int main() {
    // Revenir en arrière d'un dossier (aller dans le dossier "projet")
    int err = chdir("..");
    if (err)
        errx(1, "main: chdir error");

    // Ouvrir le fichier CSV en mode lecture
    const char *fichier = "data/data.csv";
    FILE *fichier = fopen(fichier, "r");

    if (fichier == NULL) {
        fprintf( "Impossible d'ouvrir le fichier CSV.\n");
        return EXIT_FAILURE;
    }
    
    NoeudAVL *avl = NULL;
    
    procedure(fichier, &avl);
    

    
    StatsVille topEcartsDistances[50];
    int topEcartsDistancescompteur = 0;

    Top50compteurs(avl, topEcartsDistances, &topEcartsDistancescompteur);

    // Affichage du résultat
    afficherTopsDistances(topEcartsDistances,topEcartsDistancescompteur);

     // Fermer le fichier
    fclose(fichier);
    SauvegarderResultats("temp/data_s.dat", topEcartsDistances, topEcartsDistancescompteur);
    
    
    // Libérer la mémoire de l'arbre AVL des départs
    libererAVLArbre(&avl);
    
    return 0; // Terminer le programme avec succès
}
