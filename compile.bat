gcc -O3 -D NETYAROZE -o main.o back.c scenes/boxes.c main.c
gcc -Xlinker -Map -Xlinker mapfile -Xlinker -Ttext -Xlinker 80100000 main.o -o main
yardos auto