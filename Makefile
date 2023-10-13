CC=g++
TARGET:main.exe
LIBS=-lpthread -LCommandParser -lcli -lrt
OBJS=tcp_client_db_manager.o \
		tcp_client_service_manager.o \
		tcp_new_connection_acceptor.o \
		tcp_server_controller.o \
		network_utils.o \
		tcp_client.o \
		byte_circular_buffer.o \
		tcp_msg_demarcar.o \
		tcp_msg_fixed_size_demarcar.o \
		tcp_msg_variable_size_demarcar.o \

main.exe:main.o $(OBJS) CommandParser/libcli.a
	${CC} ${OBJS} main.o -o main.exe ${LIBS}

main.o:main.cpp
	${CC} -c main.cpp -o main.o

tcp_client_db_manager.o:tcp_client_db_manager.cpp
	${CC} -c tcp_client_db_manager.cpp -o tcp_client_db_manager.o

tcp_client_service_manager.o:tcp_client_service_manager.cpp 
	${CC} -c tcp_client_service_manager.cpp -o tcp_client_service_manager.o

tcp_new_connection_acceptor.o:tcp_new_connection_acceptor.cpp
	${CC} -c tcp_new_connection_acceptor.cpp -o tcp_new_connection_acceptor.o

tcp_server_controller.o:tcp_server_controller.cpp
	${CC} -c tcp_server_controller.cpp -o tcp_server_controller.o

network_utils.o:network_utils.cpp
	${CC} -c network_utils.cpp -o network_utils.o

tcp_client.o:tcp_client.cpp
	${CC} -c tcp_client.cpp -o tcp_client.o

byte_circular_buffer.o:byte_circular_buffer.cpp
	${CC} -c byte_circular_buffer.cpp -o byte_circular_buffer.o

tcp_msg_demarcar.o:tcp_msg_demarcar.cpp
	${CC} -c tcp_msg_demarcar.cpp -o tcp_msg_demarcar.o

tcp_msg_fixed_size_demarcar.o:tcp_msg_fixed_size_demarcar.cpp
	${CC} -c tcp_msg_fixed_size_demarcar.cpp -o tcp_msg_fixed_size_demarcar.o

tcp_msg_variable_size_demarcar.o:tcp_msg_variable_size_demarcar.cpp
	${CC} -c tcp_msg_variable_size_demarcar.cpp -o tcp_msg_variable_size_demarcar.o

CommandParser/libcli.a:
	(cd CommandParser; make)

clean:
	rm -f *.o
	rm -f *exe
	(cd CommandParser; make clean)





