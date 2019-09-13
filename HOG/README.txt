*******************************************************************************
** Hans Julien, Perraud Frédéric                       3A SLE ENSIMAG 2015-2016
**
**cpp
*******************************************************************************

**Résumé :

	Fichiers C++ permettant de traiter et visualiser le traitement d'image 
pgm contenus dans le répertoire ../img avec l'algorithme Histogram Of Gradient. 
L'execution du programme place les résultats dans le répertoire ../res : la 
première image est l'image originale, la deuxième est celle obtenue avec 
l'algorithme avec approximation linéaire sur des entiers et la dernière celle 
obtenue avec des calculs en double précision. Par ailleurs un certain nombre
d'autres fonctions sont également implantées en début du programme main()
(voir plus bas) pour donner le degré de précision des approximations linéaires.

**Utilisation :
	 
	Pour utiliser la fonction, utiliser la commande make et exécuter le
binaire ./exec_cpp. Les fichiers spécifiés ligne 146 (et présents dans le
répertoire ../img) du main.cpp seront traités et le résultat sauvegardé dans le
répertoire ../res.
	Pour visualiser (dans la console) le résultat de la comparaison entre le
calcul flottant double précision et l'approximation linéaire décommenter les
lignes à partir de la ligne 105 de main.cpp puis recompiler.

       Pour effacer les fichiers objets et l'exécutable, éxecuter make clean. Si
vous souhaiter effacer en plus les fichiers *.*~ et les fichiers résultats 
(../res/*_histo.pgm) : make clean-all.

**Contenu détaillé :

	-affine/ : contient des fichiers txt dans lesquels sont stockées les
valeurs utilisées pour les approximations linéaires
	-bin/ : contient l'exécutable
	-histo/ : contient les 16 images utilisées pour représenter les 16
orientations de gradients possibles
	-obj/ : contient les fichiers objet compilés
	-Affine.cpp et .h : classe regroupant les fonctions permettant d'écrire
les fichiers txt dans affine/ et de les lire, et l'implémentation des fonctions
d'approximation
	-Histo.cpp et .h : classe permettant de réaliser les calculs nécessaires
à l'algorithme HoG, notamment les calculs de gradient, de norme et d'argument,
et permet d'enregistrer le résultat sous forme d'une image composée des motifs
contenus dans histo/
	-HOG_Parameters.h : définit des paramètres utilisés par l'algorithme HoG
	-Image.cpp et .h : permet de stocker et gérer un tableau de valeurs
correspondant à une image
	-main.cpp : contient le programme principal qui permet d'appeler la
fonction de calcul d'histogramme sur les fichiers voulus, et contient aussi
plusieurs fonctions permettant de calculer la précision des fonctions
d'approximation
