target=co-mine
obj= co-mine.o  myutils.o MineGame.o  Display.o Communicate.o SocketManager.o UserStat.o

$(target):  $(obj)
		  $(CXX) -o $@ $^ -lpthread
myutils.o:myutils.h myutils.cpp
		 $(CXX)    -c myutils.cpp
co-mine.o:co-mine.cpp
		 $(CXX)  -c co-mine.cpp
MineGame.o :MineGame.cpp
		 $(CXX)  -c MineGame.cpp
Display.o :Display.cpp
		 $(CXX)  -c Display.cpp
Communicate.o :Communicate.cpp
		 $(CXX)  -c Communicate.cpp
SocketManager.o :SocketManager.cpp
		 $(CXX)  -c SocketManager.cpp
UserStat.o : UserStat.cpp
		 $(CXX)  -c UserStat.cpp
clean:
		rm $(target)|rm *.o 
