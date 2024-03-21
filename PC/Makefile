blackjack: main.o joueur.o deck.o carte.o dealer.o
	g++ -o blackjack main.o joueur.o deck.o carte.o dealer.o

main.o: main.cpp deck.h joueur.h carte.h dealer.h
	g++ -c -g main.cpp

deck.o: deck.cpp deck.h carte.h
	g++ -c -g deck.cpp

joueur.o: joueur.cpp joueur.h deck.h carte.h
	g++ -c -g joueur.cpp

carte.o: carte.cpp carte.h
	g++ -c -g carte.cpp
dealer.o:dealer.cpp dealer.h joueur.h
	g++ -c -g dealer.cpp
clean:
	rm  -f *.o blackjack
compile_with_SERIAL_COM_ON_WINDOWS: g++ .\main.cpp .\include\serial\SerialPort.cpp .\carte.cpp .\dealer.cpp .\deck.cpp .\joueur.cpp
EXECUTE_ON_WINDOWS:.\a.exe