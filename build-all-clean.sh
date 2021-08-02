

BENCHMARK_DIRS="bubble-sort edit-distance eulers-number-approx fft-int kepler-calc mersenne nonlinear-nn nr-solver parrando rad-to-degree mnist-cnn"
# BENCHMARK_DIRS="boyer-moore-search mnist-cnn scalar-gwas-chi2 vector-gwas-chi2"

curDIR=$PWD

for dir in $BENCHMARK_DIRS; do
    cd $dir
    rm -r bin
    mkdir bin

    make clean
    make 'MODE=na'
    mv $dir.na bin/
    
    make clean
    make 'MODE=do'
    mv $dir.do bin/
    
    make clean
    make 'MODE=enc'
    mv $dir.enc bin/
    
    make clean
    cd $curDIR
done


