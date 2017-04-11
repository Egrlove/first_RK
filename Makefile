EXEC=xgui
LIBS= -lX11
CC=cc
OBJECTS=main.o source.o

all: $(OBJECTS) $(EXEC)

$(EXEC): $(OBJECTS)
	$(CC) -o $(EXEC) $(OBJECTS) $(LIBS)


clean :
	rm *.o
