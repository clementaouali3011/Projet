Pour utiliser le script shell entrez ceci dans le terminal: ./script.sh data.csv -(l'argument que vous voulez par exemple h pour voir les options et leurs utilités)
PS : Le fichier data.csv étant trop lourd je n'ai pas pu le push


Les programmes c et leurs executables sont dans le dossier progc. Le fichier traitement_t.c fait le traitement T et le fichier traitemebt_s.c fait le traitement S. Il faut bien vérifier que le fichier data.csv se situe dans le dossier data, sinon ça ne marchera pas.
Si tout se passe bien, en sortie des traitements t et s il y a 2 fichier.dat qui sont de la même forme que les fichiers .dat fournis par le prof. C'est-à-dire : 

Pour le traitement t : VILLE;NbTrajets;NbDeparts

Pour le traitement s : classementécartminmax;routeID;minDistances;moyDistances;maxDistances;max-min
