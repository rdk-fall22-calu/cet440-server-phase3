build: git-pull
	gcc -std=c99 src/*.h src/*.c -pthread -o build/server

git-pull:
	git pull