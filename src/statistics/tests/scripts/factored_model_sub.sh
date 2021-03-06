# shell for the job:
#PBS -S /bin/bash
# use one node with 2 cores:
#PBS -lnodes=1:ppn=2:cores8
# job requires at most 2 hours, 0 minutes
#     and 0 seconds wallclock time
#PBS -lwalltime=4:00:00
# cd to the directory where the program is to be called:

cd $HOME/experiments

script_dir=$HOME/projects/beliefbox/src/statistics/tests/scripts
bin_dir=$HOME/projects/beliefbox/src/statistics/tests/bin


for model in BVMM CTW # FMC BFMC BVMM CTW
do
    echo ${bin_dir}/factored_model $environment $model $iter $T $depth $p 1.0 $states  >${out_dir}/${states}s_${depth}d_${model}.params
    time ${bin_dir}/factored_model $environment $model $iter $T $depth $p 1.0 $states  >${out_dir}/${states}s_${depth}d_${model}.out&
done

wait
