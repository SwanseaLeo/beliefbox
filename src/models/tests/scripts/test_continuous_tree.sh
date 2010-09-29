#! /bin/bash

exe=./bin/continuous_factored_tree_rl

environment=Pendulum
model=BVMM
T=100000
iter=10
erand=0.01
arand=0.001
ulimit -v 2000000

for D in 1 2 3 4 5 6 7 8
do
    for environment in Pendulum MountainCar
    do
	echo $T $D $D_c
    cmdline="$exe $environment $model $iter $T $D $erand $arand"
	echo $cmdline
	nice -n 19 $cmdline | grep STATS >${environment}_d${D}.out &
    done
    wait;
done
