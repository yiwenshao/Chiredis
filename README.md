# Chiredis
Chiredis is a redis interface written in c. It is built upon hiredis to add supports for access to redis cluster. for more information, please read the wiki.

2016-8-28
use gcc -o p test.c -lhiredis -lchiredis -I/usr/local/include/chiredis -lpthread to compile.
use make install to install dynamic lib
