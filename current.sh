pre=""
for((i=0;i<200;i++))
do
    pre=${pre}a
done

for((i=0;i<500;i++))
do
   echo key$i value${i}${pre} >> single
done
