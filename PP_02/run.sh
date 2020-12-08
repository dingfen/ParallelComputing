make 

for file in `ls build`
do
    echo "*********************"
    echo "Lab02 " ${file} " program :"
    time mpiexec -n 8 ./build/${file}
done
