CC=g++
CCFLAGS=-Wl,--no-as-needed -std=c++11
LDFLAG=-lpthread
TARGET=push

OBJS=test.o

ALL: $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(LDFLAG)

%.o : %.cpp $(wildcard %.hpp)
	$(CC) -c $< $(CCFLAGS)

clean:
	rm -f *.o core $(TARGET)

