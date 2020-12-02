make 

for file in `ls build`
do
    echo "*********************"
    echo "Lab02 " ${file} " program :"
    time mpiexec -n 9 ./build/${file}
done
