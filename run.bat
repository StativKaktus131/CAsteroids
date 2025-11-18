@echo off
cls
SET cfiles=src/main.c src/player.c src/util.c src/projectile.c
gcc %cfiles% -Iinclude -Llib -lSDL3 -o bin/main.exe
cd bin
main.exe
cd ..