SRC = pcs_app.cpp \
	pcs_window.cpp \
	gameview.cpp

OBJECTS = $(SRC: .cpp=.o)

FLAGS = -I/boot/home/config/non-packaged/include -I/boot/home/config/develop/headers -DHAIKU

LDFLAGS = -L/boot/home/config/non-packaged/lib

.SUFFIXES: .o .cpp

all: $(OBJECTS)
	$(CC) $(OBJECTS) -o PCShifter $(LDFLAGS) -lstdc++.r4 -lbe

clean:
	rm *.o
	rm DataMage
	rm -rf datamage-package
.cpp.o:
	$(CXX) $(CFLAGS) -c -o $@ $(@:.o=.cpp)
	
