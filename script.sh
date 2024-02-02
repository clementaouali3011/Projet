#!/bin/bash

#Compile les programmes C puis revenir dans le dossier du projet
cd progc/
make
cd ../

#Déplacement des images précédentes dans le dossier demo
mv images/*.png demo/

# Vérifie et créer le dossier "temp" s'il n'existe pas
if [ ! -d "temp" ]; then
    mkdir temp
else
    # Vide le dossier "temp" s'il existe
    rm -rf temp/*
fi


# Vérifie et créer le dossier "images" s'il n'existe pas
if [ ! -d "images" ]; then
    mkdir images
else
    # Vide le dossier "images" s'il existe
    rm -rf images/*
fi

# Vérifie le nombre d'arguments
if [ "$#" -lt 2 ]; then
    echo "Erreur: Il manque des arguments à la commande. >:("
    echo "Tuto: $0 <ton fichier contenant tes données> <l'argument>"
    exit 1
fi

# Assigne les arguments à des variables
fichier="$1"
option="$2"

# Exécute une commande selon l'argument entré
case "$option" in
    "-d1")
        echo "Option -d1 selected for file $fichier"
        time LC_ALL=C sort -k6 -t";" -d data/data.csv | LC_ALL=C cut --fields=1,6 -d ';' | LC_ALL=C mawk -F"," '!_[$1]++' | tr ';' ' ' | LC_ALL=C cut --fields=2,3 -d ' ' | uniq -c | LC_ALL=C mawk '{print $2 " " $3 ";" $1}' | LC_ALL=C sort -k2 -t";" -nr | head -10 > temp/res_d1.dat
        gnuplot gnplt/test_d1.plt
        echo "Vous pouvez retrouver le graphique dans le dossier images !"
        ;;
    "-d2")
        echo "Option -d2 selected for file $fichier"
        time LC_ALL=C sort -k6 -t";" -d data/data.csv | LC_ALL=C cut --fields=5,6 -d ';' | tr ';' ',' | tr ' ' '_' | LC_ALL=C mawk -F"," '{sum[$2]+=$1} END {for (name in sum) printf "%s;%.6f\n", name, sum[name]}' | LC_ALL=C sort -nr -t';' -k2 | head -10 | tr '_' ' ' > temp/res_d2.dat
        gnuplot gnplt/test_d2.plt
        echo "Vous pouvez retrouver le graphique dans le dossier images !"
        ;;
    "-l")
        echo "Option -l selected for file $fichier"
        time LC_ALL=C sort -k1 -t";" -d data/data.csv | cut --fields=1,5 -d ';' | tr ';' ',' | tr ' ' '_' | LC_ALL=C mawk -F"," '{sum[$1]+=$2} END {for (name in sum) printf "%s;%.6f\n", name, sum[name]}' | sort -nr -t';' -k2 | head -10 | sort -nr -t';' -k1 | tr '_' ' ' | tr ',' ' ' > temp/res_l.dat
        gnuplot gnplt/test_l.plt
        echo "Vous pouvez retrouver le graphique dans le dossier images !"
        ;;
    "-s")
        echo "Option -s selected for file $fichier"
        ./progc/traitement_s
        gnuplot gnplt/test_s.plt
        echo "Vous pouvez retrouver le graphique dans le dossier images !"
        ;;
    "-t")
        echo "Option -t selected for file $fichier"
        ./progc/traitement_t
        gnuplot gnplt/test_t.plt
        echo "Vous pouvez retrouver le graphique dans le dossier images !"
        ;;
    "-h")
        echo "Voici les options et leurs utilitées :
        -d1 : afficher les conducteurs avec le plus de trajets.
        -d2 : afficher les conducteurs et la plus grande distance.
        -l : afficher les 10 trajets les plus longs."
        ;;
    *)
        echo "Erreur: Option $option inconnue."
        echo "Tuto: $0 <file> <option>"
        exit 1
        ;;
esac
