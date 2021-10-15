BENCHMARKS="bubble-sort edit-distance eulers-number-approx fft-int flood-fill gradient-descent grad-x kadane kepler-calc lda mersenne mnist-cnn nonlinear-nn nr-solver parrando rad-to-degree lda shortest-path tea-cipher triangle-count"
#BENCHMARKS="bubble-sort edit-distance eulers-number-approx fft-int kepler-calc mersenne nonlinear-nn nr-solver parrando rad-to-degree lda triangle-count test-linear-reg"
MODES="na do enc"

curDIR=$PWD

for dir in $BENCHMARKS; do
  for mode in $MODES; do

    cd $dir
    echo "--------------------------------"
    echo "Running "$dir" in MODE="$mode
    echo "--------------------------------"

    make MODE=$mode clean build test

    cd ..

  done
done
exit

#    echo "MODE=na -----"
#    # echo -n $dir ", MODE=na, "
#    ./$dir.na  > na-$dir.out  #2> na-$dir.err
#    echo ""
#    mv mem.out na-mem.out
#
#    echo "MODE=do -----"
#    # echo -n $dir ", MODE=do, "
#    for i in {1..1}
#    do
#        echo "Run" $i
#        ./$dir.do  > do-$dir.out  #2> do-$dir.err
#    done
#    echo ""
#    mv mem.out do-mem.out
#
#    echo "MODE=enc -----"
#    # echo -n $dir ", MODE=enc, "
#    for i in {1..1}
#    do
#        echo "Run" $i
#        ./$dir.enc > enc-$dir.out #2> enc-$dir.err
#    done 
#    echo ""
#    mv mem.out enc-mem.out
#
#    #echo -e "--------------------------------\n"
#    cd $curDIR
#done
#
##special mnist varient
#dir="mnist-cnn"
#    cd $dir/bin
#    echo "--------------------------------"
#    echo $dir
#    echo "--------------------------------"
#    cp ../mnist-cnn.kan .
#    cp ../mnist-test-x.knd .
#    echo "MODE=na -----"
#    #echo -n $dir ", MODE=na, "
#    for i in {1..1}
#    do
#	echo "Run" $i
#        ./$dir.na -i mnist-cnn.kan mnist-test-x.knd > na-$dir.out  #2> na-$dir.err
#    done
#    echo ""
#    mv mem.out na-mem.out
#
#    echo "MODE=do -----"
#    #echo -n $dir ", MODE=do, "
#    for i in {1..1}
#    do
#        echo "Run" $i
#        ./$dir.do -i mnist-cnn.kan mnist-test-x.knd > do-$dir.out  #2> do-$dir.err
#    done
#    echo ""
#    mv mem.out do-mem.out
#
#    echo "MODE=enc -----"
#    #echo -n $dir ", MODE=enc, "
#    for i in {1..1}
#    do
#        echo "Run" $i
#        ./$dir.enc -i mnist-cnn.kan mnist-test-x.knd > enc-$dir.out #2> enc-$dir.err
#    done
#    echo ""
#    mv mem.out enc-mem.out
#    cd $curDIR
#
