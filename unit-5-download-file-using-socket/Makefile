.PHONY: build
build:
	gcc -o client client.c
	gcc -o server server.c
.PHONY: server
server:
	./server toB.txt
.PHONY: client
client:
	./client fromA.txt
clean:
	rm -rf client server toB.txt
