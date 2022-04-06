#include "SFML\Network.hpp"
#include <iostream>
#include <list>


// The TCP port number for the server
#define CLIENTPORT 4444
#define SERVERPORT 4445

//Match Info
struct MatchMakingInfo
{
	int numofPlayers=0;
	sf::IpAddress serverAddr;
	unsigned short serverPort;
};

//Client Structure
struct Client
{
	int id;
	sf::TcpSocket* socket;
};

void die(const char* message)
{
	fprintf(stderr, "Error: %s\n", message);
#ifdef _DEBUG
	abort();
#else
	exit(1);
#endif
}


int main()
{
	//DECLARATION
	
	//When ready to begin game
	bool ready = false;
	
	//All players
	std::vector<Client> clients;
	
	//Available IDS for new players
	std::vector<int> availableIDs = { 7,6,5,4,3,2,1,0 };
	
	//How many players are ready
	int readyPlayers = 0;
	
	//Game Server Socket
	sf::TcpSocket serverSocket;
	serverSocket.setBlocking(false);
	
	//Listeners
	sf::TcpListener clientListener, serverListener;
	clientListener.setBlocking(false);
	serverListener.setBlocking(false);
	
	//Info
	MatchMakingInfo info;

	printf("Server\n");

	
	// bind the client listener to a port
	if (clientListener.listen(CLIENTPORT) != sf::Socket::Done)
	{
		die("Couldn't bind client listener to port.");
	}

	//bind the game server listener to a port
	if (serverListener.listen(SERVERPORT) != sf::Socket::Done)
	{
		die("Couldn't bind server listener to port.");
	}
	sf::SocketSelector selector;
	selector.add(clientListener);
	selector.add(serverListener);

	while (true)
	{
	
		//If the players are not ready
		if (!ready)
		{
			//WHILE PLAYERS ARE JOINING
			if (selector.wait(sf::seconds(0.001)))
			{
				if (selector.isReady(serverListener))
				{
					//Accept new Server Connection
					if (serverListener.accept(serverSocket) == sf::Socket::Done)
					{
						//Add the server details to the info
						info.serverAddr = serverSocket.getRemoteAddress();	
						info.serverPort = serverSocket.getLocalPort();
						printf("Server Connected");
					}
				}

				if (selector.isReady(clientListener))
				{
					// If not all 8 players are connected
					if (clients.size() < 8)
					{
						//Create a new Client
						Client newClient;
						sf::TcpSocket* clientSocket;
						clientSocket = new sf::TcpSocket();
						clientSocket->setBlocking(false);
						//Accept the new Client connection
						if (clientListener.accept(*clientSocket) != sf::Socket::Done)
						{
							//If no connection then delete the socket pointer
							delete clientSocket;
						}
						else
						{
							//If connected

							//Check if all clients are still connected
							for (int i = 0; i < clients.size(); i++)
							{
								sf::Packet packet;
								if (clients[i].socket->send(packet) == sf::Socket::Disconnected)
								{
									availableIDs.push_back(clients[i].id);
									clients[i].socket->disconnect();
									info.numofPlayers--;
								}
							}

							//The new client is pushed in the vector and given an id
							newClient.socket = clientSocket;
							newClient.id = availableIDs.back();
							clients.push_back(newClient);
							availableIDs.pop_back();
							info.numofPlayers++;

							//Send Id to player
							sf::Packet idPacket;
							idPacket << clients.back().id;
							if (clients.back().socket->send(idPacket) != sf::Socket::Status::Done)
							{
								clients.pop_back();
							}

							std::cout << "Server socket bound to address " << clientSocket->getRemoteAddress().toString() << " and port " << clientSocket->getRemotePort() << std::endl;
							printf("Connected Succesfully\n");

						}
					}
				}

				
			}

			for (int i = 0; i < clients.size(); i++)
			{
				//if clients are still connected
				if (clients[i].socket->getRemoteAddress() != sf::IpAddress::None)
				{
					//Try to recieve if they are ready
					sf::Packet readyPacket;
					if (clients[i].socket->receive(readyPacket) == sf::Socket::Done)
					{
						//Update the ready status
						bool answer = false;
						readyPacket >> answer;
						if (answer)
						{
							readyPlayers++;
						}
						else
						{
							readyPlayers--;
						}
					}
				}

				//If all players are ready
				if (readyPlayers == info.numofPlayers && readyPlayers > 0)
				{
					ready = true;
				}


			}

		}
		else //When everyone is ready
		{
			
			//If the game server is connected
			if (info.serverAddr != sf::IpAddress::None)
			{
				//Send the num of players to the server
				sf::Packet numOfPlayersPacket;
				numOfPlayersPacket << info.numofPlayers;
				if (serverSocket.send(numOfPlayersPacket) != sf::Socket::Done)
				{
					info.serverAddr = sf::IpAddress::None;
					info.serverPort = 0;
					continue;
 				}
			
				//Put all matchmaking info in a packet and send to each of the players
				for (int i = 0; i < clients.size(); i++)
				{
					sf::Packet packet;
					packet << info.numofPlayers << info.serverAddr.toString() << info.serverPort;
				
					if (clients[i].socket->getRemoteAddress() != sf::IpAddress::None)
					{
						if (clients[i].socket->send(packet) != sf::Socket::Done)
						{
							availableIDs.push_back(clients[i].id);
							clients[i].socket->disconnect();
							info.numofPlayers--;
							continue;
						}
					}
				}
				//RESET MATCHMAKING SERVER
				for (int i = (clients.size()-1); i >= 0; i--)
				{
					clients.pop_back();
				}
	
				info.numofPlayers = 0;
				info.serverAddr = sf::IpAddress::None;
				info.serverPort = 0;
				ready = false;
				readyPlayers = 0;
				serverSocket.disconnect();
				availableIDs = { 7,6,5,4,3,2,1,0};		
			}
			else
			{
				if (selector.wait())
				{
					if (selector.isReady(serverListener))
					{
						//Accept new Server Connection
						if (serverListener.accept(serverSocket) == sf::Socket::Done)
						{
							//Add the server details to the info
							info.serverAddr = serverSocket.getRemoteAddress();
							info.serverPort = serverSocket.getLocalPort();
							printf("Server Connected");
						}

					}
				}

			}
			
		}
	}
}