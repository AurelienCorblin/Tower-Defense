CC = gcc
CFLAGS = -Wall -pedantic
LDLIBS = -lMLV
OBJ=$(BIN)Main.o $(BIN)Path.o $(BIN)Display.o $(BIN)Monster.o $(BIN)Tower.o $(BIN)Damage.o $(BIN)Deplacement.o $(BIN)Gem.o $(BIN)Mana.o 
SRC=src/
INC=include/
BIN=bin/


projet : $(OBJ)
	$(CC) -o projet $(OBJ) $(CFLAGS) $(LDLIBS) -lm

$(BIN)Deplacement.o: $(SRC)Deplacement.c $(INC)Path.h $(INC)Deplacement.h
	$(CC) $(CFLAGS) -c $(SRC)Deplacement.c -o $(BIN)Deplacement.o -lm

$(BIN)Gem.o: $(SRC)Gem.c $(INC)Gem.h $(INC)Path.h
	$(CC) $(CFLAGS) -c $(SRC)Gem.c -o $(BIN)Gem.o -lm

$(BIN)Mana.o: $(SRC)Mana.c $(INC)Mana.h
	$(CC) $(CFLAGS) -c $(SRC)Mana.c -o $(BIN)Mana.o -lm

$(BIN)Damage.o: $(SRC)Damage.c $(INC)Damage.h $(INC)Mana.h $(INC)Monster.h
	$(CC) $(CFLAGS) -c $(SRC)Damage.c -o $(BIN)Damage.o -lm

$(BIN)Tower.o: $(SRC)Tower.c $(INC)Tower.h $(INC)Path.h $(INC)Gem.h $(INC)Monster.h $(INC)Damage.h
	$(CC) $(CFLAGS) -c $(SRC)Tower.c -o $(BIN)Tower.o -lm

$(BIN)Path.o: $(SRC)Path.c $(INC)Path.h
	$(CC) $(CFLAGS) -c $(SRC)Path.c -o $(BIN)Path.o -lm

$(BIN)Display.o: $(SRC)Display.c $(INC)Display.h $(INC)Tower.h $(INC)Path.h $(INC)Gem.h $(INC)Monster.h $(INC)Mana.h
	$(CC) $(CFLAGS) -c $(SRC)Display.c -o $(BIN)Display.o $(LDLIBS)

$(BIN)Monster.o: $(SRC)Monster.c $(INC)Monster.h $(INC)Deplacement.h $(INC)Path.h $(INC)Mana.h
	$(CC) $(CFLAGS) -c $(SRC)Monster.c -o $(BIN)Monster.o -lm

$(BIN)Main.o : $(SRC)Main.c $(INC)Display.h $(INC)Tower.h $(INC)Path.h $(INC)Damage.h $(INC)Monster.h $(INC)Mana.h $(INC)Deplacement.h
	$(CC) $(CFLAGS) -c $(SRC)Main.c -o $(BIN)Main.o $(LDLIBS)
	
clean :
	rm bin/*.o projet
