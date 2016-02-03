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

echo "-------------------------------------------------------"
echo -e "\t\tEXECUTION DU SCENARIO 0"
echo "-------------------------------------------------------"
for j in 0 2 4
do
	let "s = $j + 4"
	for ((x=1, size=2; x<s; x++)); do ((size *= 2)); done
	echo -e "\t*** Plaque de $size*$size, avec 1 thread ***" 
	#echo "s = $j " #verif
	./heatTransfer  -i 10000 -e $i -s $j -t 0 -m
done

for i in {1..5}
do
	echo "-------------------------------------------------------"
	echo -e "\t\tEXECUTION DU SCENARIO $i"
	echo "-------------------------------------------------------"
    for j in 0 2 4
    do
    	let "s = $j + 4"
    	for ((x=1, size=2; x<s; x++)); do ((size *= 2)); done
    	for k in 1 3
    	do
    		for ((x=1, threads=4; x<$k; x++)); do ((threads *= 4)); done

    		echo -e "\t*** Plaque de $size*$size, avec $threads threads ***" 
    		#echo "s = $j et t = $k" #pour vérif que les args sont bons
    		./heatTransfer  -i 10000 -e $i -s $j -t $k -m
    	done
    done
done