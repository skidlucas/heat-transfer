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
echo -e "\nExécution du programme avec les options par defaut: "
echo " - pour tous les scenarios implémentes (-e 0..5)"
echo " - avec une plaque de taille 16*16, puis 128*128 et enfin 256*256 (-s 0, -s 2, -s 4)"
echo " - avec 4 threads puis avec 64 threads (-t 1, -t 3)"
echo " - 10000 iterations (-i 10000)"
echo " - en mesurant le temps CPU consomme (-m)"
echo -e "\nPour cette version, nous executerons le programme iteratif et le programme implementant les barrieres Posix:\n"

echo "-------------------------------------------------------"
echo -e "\t\tEXECUTION DU SCENARIO"
echo "-------------------------------------------------------"
	./bin/heatTransfer  -i 10000 -e 01 -s 024 -t 13 -m
#for j in 0 2 4
#do
#	let "s = $j + 4"
#	for ((x = 1, size = 2; x < s; x++)); do ((size *= 2)); done
#	echo -e "\t*** Plaque de $size*$size, avec 1 thread ***" 
#	./bin/heatTransfer  -i 10000 -e 0 -s $j -t 0 -m
#done

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
