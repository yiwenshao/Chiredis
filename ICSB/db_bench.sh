ip="192.168.1.22"
port=6667


function prepare {
    mkdir data
    rm -rf ./data/*
    make
}


if [ $# < 1 ];then
    echo $#
    exit
fi

prepare

if [ $2 = "-bind" ];then
    taskset -cp 1-22 ./tinyBenchmark $ip $port -s $1
else
    ./tinyBenchmark $ip $port -s $1
fi

