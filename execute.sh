#! /bin/bash
# ------------------------------------------------------------------
# [Lucas Martinez & Lucas Soumille] execute.sh
#
#          Script permettant d'executer le programme heatTransfer avec
#		   les options par defaut fournies dans le sujet du projet.
#
# Attention : il n'est pas encore totalement fonctionnel car toutes
#             les versions ne sont pas encore implementees, le reste
#             sera donc commente
# ------------------------------------------------------------------


echo "Compilation du programme"
make
echo -e "\nExécution du programme avec les options par défaut: "
echo " - pour tous les scénarios implémentés (-e 0123)"
echo " - avec une plaque de taille 16*16, puis 128*128 et enfin 256*256 (-s 024)"
echo " - avec 4 threads puis avec 64 threads (-t 13)"
echo " - 10000 itérations (-i 10000)"
echo " - en mesurant le temps CPU consommé (-m)"
echo -e "\nPour cette quatrième version, nous exécuterons le programme itératif, l'étape 1, l'étape 2 et l'étape 3:\n"

echo "-------------------------------------------------------"
echo -e "\t\tEXECUTION DU PROGRAMME"
echo "-------------------------------------------------------"

./bin/heatTransfer -m

# for j in 0 2 4
# do
# 	let "s = $j + 4"
# 	for ((x = 1, size = 2; x < s; x++)); do ((size *= 2)); done
# 	echo -e "\t*** Plaque de $size*$size, avec 1 thread ***"
# 	./bin/heatTransfer  -i 10000 -e 0 -s $j -t 0 -m
# done

# POUR LES ETAPES ULTERIEURES

# for i in {1..5}
# do
# 	echo "-------------------------------------------------------"
# 	echo -e "\t\tEXECUTION DU SCENARIO $i"
# 	echo "-------------------------------------------------------"
#     for j in 0 2 4
#     do
#     	let "s = $j + 4"
#     	for ((x = 1, size = 2; x < s; x++)); do ((size *= 2)); done
#     	for k in 1 3
#     	do
#     		for ((x=1, threads=4; x<$k; x++)); do ((threads *= 4)); done
#     		echo -e "\t*** Plaque de $size*$size, avec $threads threads ***"
#     		./bin/heatTransfer  -i 10000 -e $i -s $j -t $k -m
#     	done
#     done
# done
