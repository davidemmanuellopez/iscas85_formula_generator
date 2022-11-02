./spaces ./iscas85/$1.bench ./iscas85/$1.bench.clean
mkdir ./tests/$1
./generator ./iscas85/$1.bench.clean ./tests/$1/$1
