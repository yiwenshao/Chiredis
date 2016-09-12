a=`ls`
for item in ${a[@]};do
    if [ $item = "process.py" ] || [ $item = "use_process.sh" ];then
        echo " "
    else
        echo $item
        python process.py $item
    fi
done
