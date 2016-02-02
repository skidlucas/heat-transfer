#! /bin/bash
# ------------------------------------------------------------------
# [Lucas Martinez & Lucas Soumille] execute.sh
#          Script permettant d'exécuter le programme heatTransfer avec
#		   les options par défaut fournies dans le sujet du projet.
# ------------------------------------------------------------------

echo "Exécution du programme avec les options par défaut: "
echo " - pour tous les scénarios implémentés (-e 0..5)"
echo " - avec une plaque de taille 16*16, puis 128*128 et enfin 256*256 (-s 0, -s 2, -s 4)"
echo " - avec 4 threads puis avec 64 threads (-t 1, -t 3)"
echo " - 10000 itérations (-i 10000)"
echo " - en mesurant le temps CPU consommé (-m)"
echo "-----------------------------------------------"

#./heatTransfer -i 10000 -e 0 -s 0 -t 1 -m
#trouver comment exécuter une après l'autre les différentes versions