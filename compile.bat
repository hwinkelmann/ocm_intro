gcc -O3 -D NETYAROZE -o main.o main.c
gcc -Xlinker -Map -Xlinker mapfile -Xlinker -Ttext -Xlinker 80100000 main.o -o main
yardos auto