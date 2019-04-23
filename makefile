all:	 TestMessage TestRedirection TestLectureEcriture terminal autorisation acquisition

message.o: message.c message.h
	gcc -Wall -c message.c

alea.o: alea.h alea.c
	gcc -Wall -c alea.c

TestMessage: message.o alea.o TestMessage.c
	gcc -Wall TestMessage.c message.o alea.o -o  TestMessage

TestRedirection: TestRedirection.c
	gcc -Wall TestRedirection.c -o  TestRedirection

lectureEcriture.o: lectureEcriture.c lectureEcriture.h
	gcc -c -Wall lectureEcriture.c

TestLectureEcriture: lectureEcriture.o TestLectureEcriture.c
	gcc lectureEcriture.o TestLectureEcriture.c -o TestLectureEcriture

terminal: terminal.c TestLectureEcriture.o message.o
	gcc lectureEcriture.o message.o terminal.c -o terminal

autorisation: autorisation.c lectureEcriture.o message.o
	gcc autorisation.c lectureEcriture.o message.o -o autorisation

acquisition: acquisition.c lectureEcriture.o
	gcc acquisition.c lectureEcriture.o -o acquisition

clean:	
	rm -f *.o *~ 

cleanall: clean
	rm TestRedirection TestMessage TestLectureEcriture terminal autorisation acquisition
