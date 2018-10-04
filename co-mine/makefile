target=co-mine

$(target): myutils.o co-mine.o
		  $(CXX) -o $@ $^
myutils.o:myutils.h myutils.cpp
		 $(CXX)    -c myutils.cpp
co-mine.o:co-mine.cpp
		 $(CXX)  -c co-mine.cpp

clean:
		rm $(target)|rm *.o 
