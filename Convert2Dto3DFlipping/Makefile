OBJ = Convert2Dto3DFlipping

%: %.cpp
	g++ $(shell pkg-config --cflags --libs opencv) -o $@ $<

all: $(OBJ)
	cp -f $(OBJ) ~/bin/

clean:
	rm -f $(OBJ)
	rm -f ~/bin/$(OBJ)
