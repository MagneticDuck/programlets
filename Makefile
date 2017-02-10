calculator:
	gcc calculator.c -o ./result/calculator

list:
	gcc list.c -o ./result/list

words:
	gcc words.c -o ./result/words

all: words calculator list

