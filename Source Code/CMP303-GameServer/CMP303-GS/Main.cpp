#include "SFML\Network.hpp"
#include <iostream>
#include "Level.h"
#include "Player.h"

#define MATCHMAKINGPORT 4445

struct Client
{
	int id;
	sf::TcpSocket* tcpSocket;
	sf::UdpSocket* udpSocket;
};

std::vector<sf::Vector2f> spawnPositions = { {0,0},{4952,0},{0,4108},{4952,4108},{2476,0},{2476,4108},{4568,2054},{384,2054} };

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
	sf::IpAddress mmServerIp;
	std::string mmserver;
	std::cout << "Enter Match Making server address:\n";
	std::cin >> mmserver;
	mmServerIp = sf::IpAddress::IpAddress(mmserver);

	//DECLARATIONS
	//Selector of all clients
	sf::SocketSelector selector;
	//All players
	std::vector<Client> players;
	//TCP socket for matchmaking server
	sf::TcpSocket matchMakingServer;
	//Listener for clients
	sf::TcpListener clientListener;
	//Variables
	int numOfPlayers;
	bool ready = false;
	bool start = false;

	printf("Game Server\n");

	//Connect to match making server
	if (matchMakingServer.connect(mmServerIp, MATCHMAKINGPORT) != sf::Socket::Done)
	{
		die("Couldn't reach match making server");
	}
	else
	{
		printf("Connected");
	}

	//Dont Move on until all players are ready
	while (!ready)
	{
		sf::Packet readyPacket;
		if (matchMakingServer.receive(readyPacket) == sf::Socket::Done)
		{
			readyPacket >> numOfPlayers;
			ready = true;
		}

	}

	//Bind the listener to the port
	if (clientListener.listen(MATCHMAKINGPORT) != sf::Socket::Done)
	{
		die("Couldn't set up client listener");
	}

	//Get all clients tcp sockets
	while (players.size() != numOfPlayers && numOfPlayers > 0)
	{
		Client newClient;
		sf::TcpSocket* clientSocket;
		clientSocket = new sf::TcpSocket;
		clientListener.setBlocking(false);

		//Accept new client
		if (clientListener.accept(*clientSocket) != sf::Socket::Done)
		{
			delete clientSocket;
		}
		else
		{
			//Add new client to vector
			newClient.tcpSocket = clientSocket;
			sf::UdpSocket* udpSocket;
			udpSocket = new sf::UdpSocket;
			udpSocket->bind(sf::Socket::AnyPort);
			newClient.udpSocket = udpSocket;
			players.push_back(newClient);
			selector.add(*players.back().tcpSocket);
			selector.add(*players.back().udpSocket);


			sf::Packet idPacket;
			if (players.back().tcpSocket->receive(idPacket) != sf::Socket::Done)
			{
				die("Didn't receive ID from client");
			}

			//For calculating latency purposes
			if (players.back().tcpSocket->send(idPacket) != sf::Socket::Done)
			{
				die("Couldn't send ID to client");
			}

			if (players.back().id < 0)
			{
				idPacket >> players.back().id;
			}
		}
	}

	//Sent Spawn info to players
	for (int i = 0; i < players.size(); i++)
	{
		sf::Packet spawnPacket;
		spawnPacket << players[i].udpSocket->getLocalPort();
		for (int j = 0; j < players.size(); j++)
		{
			spawnPacket << players[j].id << spawnPositions[players[j].id].x << spawnPositions[players[j].id].y;
		}

		if (players[i].tcpSocket->send(spawnPacket) != sf::Socket::Done)
		{
			//Player Disconnected
		}
	}

	//Asynchronous IO
	for (int i = 0; i < players.size(); i++)
	{
		players[i].tcpSocket->setBlocking(false);
		players[i].udpSocket->setBlocking(false);

	}

	Level level;

	//Spawn Players
	for (int i = 0; i < players.size(); i++)
	{
		level.spawnPlayer(players[i].id, spawnPositions[players[i].id]);
	}

	//GameTime
	float gameTime = 0;

	//UDP send interval
	float sendDelay = 0.1;


	sf::Clock clock;
	float deltaTime = clock.restart().asSeconds();
	//TIME SET UP
	for (int i = 0; i < players.size(); i++)
	{
		sf::Packet startPacket;
		if (players[i].tcpSocket->send(startPacket) != sf::Socket::Done)
		{

		}
	}

	//GAME LOOP///////////////////////////////////////////////////////
	while (true)
	{

		deltaTime = clock.restart().asSeconds();

		//Calculate current gameTime
		gameTime += deltaTime;

		//Calculate send Delay counter
		sendDelay += deltaTime;

		//Calculate dt
		//RECEIVE NEW INFO
		std::vector<sf::Packet> allinfoUdp, allinfoTcp;
		sf::Packet allUpdates;
		int numOfUpdatesUdp = 0;
		int numOfUpdatesTcp = 0;

		//Check all player sockets for receive
		for (int i = 0; i < players.size(); i++)
		{

			sf::Packet tcpPacket;
			if (players[i].tcpSocket->receive(tcpPacket) == sf::Socket::Done)
			{
				numOfUpdatesTcp++;
				allinfoTcp.push_back(tcpPacket);
			}


			sf::Packet udpPacket;
			sf::IpAddress ip;
			unsigned short port;
			if (players[i].udpSocket->receive(udpPacket, ip, port) == sf::Socket::Done)
			{
				numOfUpdatesUdp++;
				allinfoUdp.push_back(udpPacket);
			}

		}

		//For all updates
		if (numOfUpdatesUdp > 0)
		{
			sf::Packet udpInfo;
			udpInfo << numOfUpdatesUdp;
			for (int i = 0; i < numOfUpdatesUdp; i++)
			{
				int id;
				UDPUpdates newUpdate;
				allinfoUdp[i] >> id >> newUpdate.ppUpdate.playerPosition.x >> newUpdate.ppUpdate.playerPosition.y >> newUpdate.hpUpdate.bulletPosition.x >> newUpdate.hpUpdate.bulletPosition.y >> newUpdate.gameTime;
				udpInfo << id << newUpdate.ppUpdate.playerPosition.x << newUpdate.ppUpdate.playerPosition.y << newUpdate.hpUpdate.bulletPosition.x << newUpdate.hpUpdate.bulletPosition.y << newUpdate.gameTime;
				
				//Apply any updates
				level.applyUdpUpdates(id, newUpdate);

			}

			//Send the packet back to other players
			for (int j = 0; j < players.size(); j++)
			{
				sf::IpAddress ip = players[j].tcpSocket->getRemoteAddress();
				unsigned short port = players[j].udpSocket->getLocalPort();
				sf::Socket::Status status = players[j].udpSocket->send(udpInfo, ip, port);
				if (status == sf::Socket::Status::Partial)
				{
					players[j].udpSocket->send(udpInfo, ip, port);
				}
			}
		}

		//If there are any TCP updates
		if (numOfUpdatesTcp > 0)
		{
			for (int i = 0; i < numOfUpdatesTcp; i++)
			{
				int id;
				int bullets = 0;
				TCPUpdates newUpdate;
				allinfoTcp[i] >> id >> bullets;

				for (int k = 0; k < bullets; k++)
				{
					int type;
					ProjectilesSpawnUpdate newProjectile;
					allinfoTcp[i] >> type >> newProjectile.spawnPosition.x >> newProjectile.spawnPosition.y >> newProjectile.velocity.x >> newProjectile.velocity.y >> newProjectile.gameTime;
					newProjectile.id = id;
					newProjectile.type = BulletType(type);
					newUpdate.psUpdates.push_back(newProjectile);
					level.newBullets.push_back(newProjectile);
				}
				//Apply Updates
				level.applyTcpUpdates(id, newUpdate, gameTime);
			}
		}

		//UPDATE GAME WORLD
		level.update(deltaTime, gameTime);


		//If there are any tcp send them
		int totalTCPUpdates = level.newBullets.size() + level.newChangedObject.size() + level.newHit.size();
		if (totalTCPUpdates > 0)
		{
			sf::Packet finalPacket;
			finalPacket = level.packageTcp();
			for (int j = 0; j < players.size(); j++)
			{
				sf::Socket::Status status = players[j].tcpSocket->send(finalPacket);
				if (status == sf::Socket::Status::Partial)
				{
					players[j].tcpSocket->send(finalPacket);
				}
			}
		}
	}

}