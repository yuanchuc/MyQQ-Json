INCLUDE=myproto.o MysqlConn.o jsoncpp.o ConnectionPool.o CELLTimestamp.hpp EasyTcpServer.hpp IDGenerater.hpp 


all:myproto.o MysqlConn.o jsoncpp.o ConnectionPool.o
	g++ server.cpp ${INCLUDE} -pthread -std=c++11 -I /usr/include/mysql -L /usr/lib64/mysql -lmysqlclient -o server
myproto.o: myproto.cpp
	g++ -c myproto.cpp -pthread -std=c++11
MysqlConn.o: MysqlConn.cpp
	g++ -c MysqlConn.cpp -pthread
jsoncpp.o: jsoncpp.cpp
	g++ -c jsoncpp.cpp -pthread
ConnectionPool.o: ConnectionPool.cpp
	g++ -c ConnectionPool.cpp -pthread
clean:
	rm -rf *.o