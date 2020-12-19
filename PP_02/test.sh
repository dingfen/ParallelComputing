for i in $(seq 2 16)
do
  time -o report2_${i} mpiexec -n ${i} build/2
  time -o report3_${i} mpiexec -n ${i} build/3
done