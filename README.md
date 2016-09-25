## Chiredis

Chiredis is a redis client library written in c. It is built upon hiredis to add supports for access to redis cluster. 
To use chiredis, you need to follow the following steps.

## install hiredis

hiredis is the official redis client library which can be downloaded from the official website. To test whether you have installed hiredis successfully, go to the ./tests directory and use the following command 
to compile the file test_1.c:

**gcc -o test_1 test_1.c -l hiredis**

If it compiles, then go to the next step.

## install Chiredis

use the command **make install** to install Chiredis. To verify that you have installed in successfully, go to the ./Demo directory, and use the following command to compile:
**make**

If the demos compile, then you have finished installing Chiredis.

## use Chiredis

In the directory ./Demos, I provide you with some examples to show how to use Chiredis.

