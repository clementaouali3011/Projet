README

Les programmes c et leurs executables sont dans le dossier progc. Le fichier traitement_t.c fait le traitement T et le fichier traitemebt_s.c fait le traitement S. Il faut bien vérifier que le fichier data.csv se situe dans le dossier data, sinon ça ne marchera pas. Il faut aussi un dossier temp (je suis pas sûr que le programme crée automatiquement le dossier s'il existe pas, à tester). 
Si tout se passe bien, en sortie des traitements t et s il y a 2 fichier.txt appelés respectivement  data_t.txt et data_s.txt qui sont de la même forme que les fichiers .dat fournis par le prof. C'est-à-dire : 

Pour le traitement t : VILLE;NbTrajets;NbDeparts

Pour le traitement s : classementécartminmax;routeID;minDistances;moyDistances;maxDistances;max-min
