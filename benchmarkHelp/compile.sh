if [ $1 = "-c" ];then
gcc -o benchmarkHelp benchmarkHelp.c
fi

if [ $1 = "-t" ];then
gcc -o test test.c benchmarkHelp.c 
fi
