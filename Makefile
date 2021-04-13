CC = g++
OUT_DIR = build
CFLAGS = -c -Wall -Werror -Wpedantic -Wunreachable-code -Wextra -g -std=c++14
TFLAGS = -D TEST
INC = -I src/includes
all : compiler

compiler : main.o lexer.o parser.o lib.o table.o dfa.o
			$(CC) main.o lexer.o parser.o table.o dfa.o lib.o -o compiler
		    @mkdir -p $(OUT_DIR)
		    @mv *.o $(OUT_DIR)
		    @mv compiler $(OUT_DIR)

test:   main_test.o lexer.o dfa.o
			$(CC) main.o lexer.o dfa.o -o compiler
		    @mkdir -p $(OUT_DIR)
		    @mv *.o $(OUT_DIR)
		    @mv compiler $(OUT_DIR)

main.o : src/main.cpp
			$(CC) $(CFLAGS) src/main.cpp $(OUTPUT) $(INC)

lexer.o :  src/lexer.cpp
			$(CC) $(CFLAGS) src/lexer.cpp $(OUTPUT) $(INC)

parser.o :  src/parser.cpp
			$(CC) $(CFLAGS) src/parser.cpp $(OUTPUT) $(INC)

dfa.o :  src/dfa.cpp
			$(CC) $(CFLAGS) src/dfa.cpp $(OUTPUT) $(INC)

main_test.o : src/main.cpp
			$(CC) $(CFLAGS) $(TFLAGS) src/main.cpp $(OUTPUT) $(INC)

lib.o : src/lib.cpp
			$(CC) $(CFLAGS) src/lib.cpp $(OUTPUT) $(INC)

table.o: src/table.cpp
			$(CC) $(CFLAGS) src/table.cpp $(OUTPUT) $(INC)

clean : 
			rm -rf $(OUT_DIR) compiler
			