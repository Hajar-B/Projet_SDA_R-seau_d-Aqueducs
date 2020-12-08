RED='\033[0;31m'
NC='\033[0m' # No Color


# Compilation et exécution en C:
#echo "${RED} Compilation du code source en C ${NC}"
#cd code/

#make

#echo "${RED} Exécution des benchmarks en C ${NC}"
#./arraylist_analysis

#echo "${RED} Nettoyage ${NC}"
#make clean

#cd ..

# Observation des résultats
### Dans le répertoire plots, lisez le contenu du fichier plot_result

cd plots
more plot_result

### puis lancez ce script avec gnuplot

gnuplot plot_result
