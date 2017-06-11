-----------Result of parallel------------
time ./srt 1-test.ppm >1-test.ppm.tmp
real    0m42.722s
user    0m42.721s
sys     0m0.002s
mv 1-test.ppm.tmp 1-test.ppm
time ./srt 2-test.ppm >2-test.ppm.tmp
real    0m20.823s
user    0m41.364s
sys     0m0.003s
mv 2-test.ppm.tmp 2-test.ppm
time ./srt 4-test.ppm >4-test.ppm.tmp
real    0m11.124s
user    0m43.319s
sys     0m0.006s
mv 4-test.ppm.tmp 4-test.ppm
time ./srt 8-test.ppm >8-test.ppm.tmp
real    0m5.922s
user    0m44.854s
sys     0m0.002s
mv 8-test.ppm.tmp 8-test.ppm

We can see from result that as the thread doubles, the efficiency will also double. 
 
This is the result of embarassingly parallel, one where little or no effort is needed to separate the problem into a number of parallel tasks. 

the user call and system call 's time do not change, while the real time shrinks, which means the thread will not improve the algorithm but just make the program run lower time. 

There is slightly increase in user time. Maybe because creating new threads takes more time. 

-------------problem run into -----------
because I separate all of the variables out of the main function, 
it was a chaos when a lot of variables are not defined or defined in a wrong place.

Also, I had a hard time passing my argument into threads, because threads do not know where they are. Later I created a struct to solve this problem.
