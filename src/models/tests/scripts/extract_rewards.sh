for environment in Pendulum MountainCar MountainCar3D; 
do
    for i in 1 2 3 4 5 6 7 8; 
    do
        awk -f $HOME/scripts/moving_average.awk -v K=1000 -v Column=2 ${environment}_d${i}.out >${environment}_d${i}.reward; 
    done;
done
