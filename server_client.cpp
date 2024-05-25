#include <iostream>
#include <unistd.h>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string>


int main(){

  // server address 
  struct sockaddr_in server_address;

  // create a socket
  int client_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (client_socket == -1) {
        perror("socket creation error");
        exit(errno);
  }

  // fill the server ip and port address 
  server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(9888);

  // connect
  int connected = connect(client_socket, (struct sockaddr*) &server_address, sizeof(server_address));

  if(connected == -1){
    perror("connection failed");
    close(client_socket);
    exit(errno);
  }

  const int buffsize = 4096;

  bool isConnected = true;

  while(isConnected)
  {
    // send message to server
    std::string mess;
    std::string buffer;
    buffer.resize(buffsize);
    std::getline(std::cin, mess);
    std::cout << "the message is : " << mess << "\n";
    std::cout<<"sending...\n";
    int sent = send(client_socket, mess.c_str(), mess.size(), 0);
    if(sent == -1){
      std::cout << "couldn't send\n";
      perror("sent:");
      close(client_socket);
      exit(errno);
    }
    std::cout<<"send successfully!\n";
    if(mess == "disconnect")
    {
      isConnected = false;
    }
    int reciv = recv(client_socket, buffer.data(), buffsize, 0);
    if(reciv == -1)
    {
        perror("message recive error: ");
        close(client_socket);
        exit(errno);
    }
    std::cout << buffer << "\n";
  }
  std::cout<<"disconnected successfully\n";
  close(client_socket);
  return 0;
}
