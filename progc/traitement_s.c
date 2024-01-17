#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>
#include <math.h>
#include <unistd.h>
#include <err.h>



// Structure pour stocker les statistiques sur une ville dans l'AVL
typedef struct {
    int routeID;
    double *Distances;
    int tailleTabDistances;
    double DistanceMin;
    double DistanceMax;
    double DistanceMoy;
} CityStats;

// Structure pour les nœuds de l'AVL
typedef struct AVLNode {
    CityStats data;
    struct AVLNode *left;
    struct AVLNode *right;
    int height;
} AVLNode;


// Fonction pour calculer la hauteur d'un nœud
int height(AVLNode *node) {
    if (node == NULL) return 0;
    return node->height;
}

// Fonction pour obtenir le maximum de deux nombres
int max(int a, int b) {
    return (a > b) ? a : b;
}

// Fonction pour effectuer une rotation à droite
AVLNode *rotateRight(AVLNode *y) {
    if (y == NULL || y->left == NULL) {
        return y;  // Rien à faire s'il n'y a pas de sous-arbre droit
    }
    AVLNode *x = y->left;
    AVLNode *T2 = x->right;

    // Effectuer la rotation
    x->right = y;
    y->left = T2;

    // Mettre à jour les hauteurs
    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    // Retourner le nouveau nœud racine
    return x;
}

// Fonction pour effectuer une rotation à gauche
AVLNode *rotateLeft(AVLNode *x) {
    if (x == NULL || x->right == NULL) {
        return x;  // Rien à faire s'il n'y a pas de sous-arbre droit
    }
    AVLNode *y = x->right;
    AVLNode *T2 = y->left;

    // Effectuer la rotation
    y->left = x;
    x->right = T2;

    // Mettre à jour les hauteurs
    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    // Retourner le nouveau nœud racine
    return y;
}



// Fonction pour obtenir le facteur d'équilibre d'un nœud
int getBalance(AVLNode *node) {
    if (node == NULL) return 0;
    return height(node->left) - height(node->right);
}


//fonction qui assigne la valeur maximum du tab Distances à la variable DistanceMax
void MajDistanceMAX (AVLNode *root){
    if (root == NULL) {
        return;  // Cas de base : nœud NULL
    }
    root->data.DistanceMax = root->data.Distances[root->data.tailleTabDistances - 1];  // Utiliser la dernière valeur

    for (int i=0;i<root->data.tailleTabDistances; i++){
        if (root->data.Distances[i]>root->data.DistanceMax){
            root->data.DistanceMax = root->data.Distances[i];
        }
    }
}    

//fonction qui assigne la valeur minimum du tab Distances à la variable DistanceMin
void MajDistanceMIN (AVLNode *root){
    if (root == NULL) {
        return;  // Cas de base : nœud NULL
    }
    root->data.DistanceMin = root->data.Distances[root->data.tailleTabDistances - 1];  // Utiliser la dernière valeur

    for (int i=0;i < root->data.tailleTabDistances; i++){
        if (root->data.Distances[i]<root->data.DistanceMin){
            root->data.DistanceMin = root->data.Distances[i];
        }
    }    
}



void MajDistanceMOY (AVLNode *root){
    if (root == NULL) {
        return;  // Cas de base : nœud NULL
    }

    if (root->data.tailleTabDistances == 0) {
        root->data.DistanceMoy = 0;  // Cas de base : tableau vide
    } else {
        double sum = 0;
        for (int i = 0; i < root->data.tailleTabDistances; i++) {
            sum += root->data.Distances[i];
        }
        root->data.DistanceMoy = sum / root->data.tailleTabDistances;
    }
}

// Fonction pour insérer un élément dans un AVL
AVLNode *insertAVL(AVLNode *root, int routeID, double distance) {
    // Si l'arbre est vide, créer un nouveau nœud
    if (root == NULL) {
        AVLNode *newNode = (AVLNode *)malloc(sizeof(AVLNode));
        if (newNode == NULL) {
            fprintf(stderr, "Erreur d'allocation de mémoire.\n");
            exit(EXIT_FAILURE);
        }
        newNode->data.routeID = routeID;
        newNode->data.Distances = malloc(sizeof(double));
        if (newNode->data.Distances == NULL) {
            fprintf(stderr, "Erreur d'allocation de mémoire pour routeIDs.\n");
            exit(EXIT_FAILURE);
        }
        newNode->data.tailleTabDistances = 1;  // Initialize tailleTabDistances to 1
        newNode->data.Distances[0] = distance;        
        newNode->data.DistanceMax = distance;
        newNode->data.DistanceMin = distance;
        newNode->data.DistanceMoy = distance;
        newNode->height = 1;
        newNode->left = newNode->right = NULL;
        return newNode;
    }

    // Sinon, comparer et insérer récursivement

    if (routeID < root->data.routeID) {
        root->left = insertAVL(root->left, routeID, distance);
    } else {
        root->right = insertAVL(root->right, routeID, distance);
    }
    // Mettre à jour la hauteur du nœud actuel
    root->height = 1 + max(height(root->left), height(root->right));

    // Équilibrer l'arbre après l'insertion
    int balance = getBalance(root);

    // Cas Gauche-Gauche
    if (balance > 1 && routeID < root->left->data.routeID) {
        return rotateRight(root);
    }

    // Cas Droit-Droit
    if (balance < -1 && routeID > root->right->data.routeID) {
        return rotateLeft(root);
    }

    // Cas Gauche-Droite
    if (balance > 1 && routeID > root->left->data.routeID) {
        root->left = rotateLeft(root->left);
        return rotateRight(root);
    }

    // Cas Droit-Gauche
    if (balance < -1 && routeID < root->right->data.routeID) {
        root->right = rotateRight(root->right);
        return rotateLeft(root);
    }

    return root;
}


// Fonction de recherche dans un AVL
AVLNode *searchAVL(AVLNode *root, int routeID) {
    // Parcours récursif de l'arbre
    if (root == NULL){
        return NULL;
    }
    
    if (routeID == root->data.routeID) {
        return root;
    }
    if (routeID < root->data.routeID){
        return searchAVL(root->left, routeID);
    } else {
        return searchAVL(root->right, routeID);
    }
}

void Distance (AVLNode **avl, const char *line){
    int routeID;
    double distance;

    // Utiliser le point comme séparateur décimal
    setlocale(LC_NUMERIC, "C");
    
    if (sscanf(line, "%d;%*d;%*255[^;];%*255[^;];%lf;%*[^;\n]", &routeID, &distance) == 2) {
        AVLNode *existingNode = searchAVL(*avl, routeID);
        if (existingNode == NULL) {
            *avl = insertAVL(*avl, routeID, distance);
        } else {
            
            existingNode->data.Distances = (double *)realloc(existingNode->data.Distances, sizeof(double) * (existingNode->data.tailleTabDistances + 1));

            if (existingNode->data.Distances == NULL) {
                fprintf(stderr, "Erreur d'allocation dynamique de mémoire pour routeIDs.\n");
                exit(EXIT_FAILURE);
            }
            existingNode->data.Distances[existingNode->data.tailleTabDistances] = distance;
            existingNode->data.tailleTabDistances++; // Increment tailleTabDistances
            MajDistanceMAX (existingNode);
            MajDistanceMIN (existingNode);
            MajDistanceMOY (existingNode);
        }
    } else {
        fprintf(stderr, "Erreur de lecture de la ligne du fichier.\n");
        exit(EXIT_FAILURE);
    }
}

void process(FILE *fichier, AVLNode **avl) {
    setlocale(LC_ALL, "");

    char buffer[256];
    if (fgets(buffer, sizeof(buffer), fichier) == NULL) {
        fprintf(stderr, "Erreur lors de la lecture de la première ligne du fichier.\n");
        exit(EXIT_FAILURE);
    }

    char line[256];
    while (fgets(line, sizeof(line), fichier) != NULL) {
        Distance (avl, line);
    }
}


void getTop50Counts(const AVLNode *root, CityStats *result, int *index) {
    if (root != NULL) {
        getTop50Counts(root->left, result, index);

        if (*index < 50 || (root->data.DistanceMax - root->data.DistanceMin) > (result[49].DistanceMax - result[49].DistanceMin)) {
            if (*index == 50) {
                (*index)--;
            }

            // Insérer le nœud courant dans le tableau à la bonne position
            int i = *index - 1;
            while (i >= 0 && (root->data.DistanceMax - root->data.DistanceMin) > (result[i].DistanceMax - result[i].DistanceMin)) {
                result[i + 1] = result[i];
                i--;
            }

            result[i + 1] = root->data;
            (*index)++;
        }

        getTop50Counts(root->right, result, index);
    }
}


void printTopsDistances(CityStats topCities[], int count) {
    printf("Top 50 des routes avec le plus grand écart entre la valeur max et la valeur min :\n");
    for (int i = 0; i < count; i++) {
        printf("route n°%d : %g distance max, %g distance min, %g distance moyenne, %g écart maxmin\n",
        topCities[i].routeID, topCities[i].DistanceMax, topCities[i].DistanceMin, topCities[i].DistanceMoy, (topCities[i].DistanceMax - topCities[i].DistanceMin));


    }
    printf("\n");
}


// Fonction pour libérer récursivement la mémoire d'un arbre AVL
void freeAVL(AVLNode *root) {
    if (root == NULL) {
        return;
    }

    // Libérer les sous-arbres gauche et droit
    freeAVL(root->left);
    freeAVL(root->right);

    // Libérer la mémoire du nœud courant, y compris routeIDs
    free(root->data.Distances);
    free(root);
}

// Fonction pour libérer toute la structure AVL, y compris la racine
void freeAVLTree(AVLNode **root) {
    if (*root != NULL) {
        freeAVL(*root);  // Libérer la mémoire des sous-arbres gauche et droit
        *root = NULL;    // Définir la racine sur NULL après la libération de la sous-structure AVL
    }
}

void saveResultsToFile(const char *filename, const CityStats *topEcartsDistances, int topEcartsDistancesCount) {
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        fprintf(stderr, "Impossible d'ouvrir le fichier de sauvegarde pour l'écriture.\n");
        perror("Error");
        exit(EXIT_FAILURE);
    }


    for (int i = 0; i < topEcartsDistancesCount; i++) {
        fprintf(file, "%d;%d;%g;%g;%g;%g\n", (i+1), topEcartsDistances[i].routeID, topEcartsDistances[i].DistanceMin, topEcartsDistances[i].DistanceMoy, topEcartsDistances[i].DistanceMax, (topEcartsDistances[i].DistanceMax-topEcartsDistances[i].DistanceMin));
    }

    fclose(file);
}

int main() {
    // Revenir en arrière d'un dossier (aller dans le dossier "projet")
    int err = chdir("..");
    if (err)
        errx(1, "main: chdir error");

    // Ouvrir le fichier CSV en mode lecture
    const char *filename = "data/data.csv";
    FILE *fichier = fopen(filename, "r");

    if (fichier == NULL) {
        fprintf(stderr, "Impossible d'ouvrir le fichier CSV.\n");
        return EXIT_FAILURE;
    }
    
    AVLNode *avl = NULL;
    
    process(fichier, &avl);
    

    
    CityStats topEcartsDistances[50];
    int topEcartsDistancesCount = 0;

    getTop50Counts(avl, topEcartsDistances, &topEcartsDistancesCount);

    // Affichage du résultat
    printTopsDistances(topEcartsDistances,topEcartsDistancesCount);

     // Fermer le fichier
    fclose(fichier);
    saveResultsToFile("temp/data_s.dat", topEcartsDistances, topEcartsDistancesCount);
    
    
    // Libérer la mémoire de l'arbre AVL des départs
    freeAVLTree(&avl);
    
    return 0; // Terminer le programme avec succès
}
