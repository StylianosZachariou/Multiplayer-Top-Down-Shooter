 #include <iostream>
#include "Level.h"
#include "SFML\Network.hpp"
#include <string>
#include "MatchMakingLevel.h"
#include <chrono>
#include <time.h>
#include "Player.h"


// The TCP port number of the match making server
#define MATCHSERVERPORT 4444

void die(const char* message)
{
	fprintf(stderr, "Error: %s\n", message);
#ifdef _DEBUG
	abort();
#else
	exit(1);
#endif
}

//Processing The Window
void windowProcess(sf::RenderWindow* window, Input* in)
{
	// Handle window events.
	sf::Event event;
	while (window->pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed:
			window->close();
			break;
		case sf::Event::Resized:
			window->setView(sf::View(sf::FloatRect(0.f, 0.f, (float)event.size.width, (float)event.size.height)));
			break;
		case sf::Event::KeyPressed:
			// update input class
			in->setKeyDown(event.key.code);
			break;
		case sf::Event::KeyReleased:
			//update input class
			in->setKeyUp(event.key.code);
			break;
		case sf::Event::MouseMoved:
			//update input class
			in->setMousePosition(event.mouseMove.x, event.mouseMove.y);
			break;
		case sf::Event::MouseButtonPressed:
			if (event.mouseButton.button == sf::Mouse::Left)
			{
				//update input class
				in->setMouseLDown(true);
			}
			else
			{
				if (event.mouseButton.button == sf::Mouse::Right)
				{
					//update input class
					in->setMouseRDown(true);
				}
			}
			break;
		case sf::Event::MouseButtonReleased:
			if (event.mouseButton.button == sf::Mouse::Left)
			{
				//update input class
				in->setMouseLDown(false);
			}
			else
			{
				if (event.mouseButton.button == sf::Mouse::Right)
				{
					//update input class
					in->setMouseRDown(false);
				}
			}
			break;
		default:
			// don't handle other events
			break;
		}
	}
}

int main()
{
	sf::IpAddress mmServerIp;
	std::string mmserver;
	std::cout << "Enter Match Making server address:\n";
	std::cin >> mmserver;
	mmServerIp = sf::IpAddress::IpAddress(mmserver);

	//Game Server IP address
	sf::IpAddress server;

	//Game Server TCP port
	unsigned short port;

	//Game Server UDP port
    unsigned short udpPort;

	//Num of Players
	int numOfPlayers = 0;

	//This player's id
	int playerId = 0;

	//Match making tcp socket
	sf::TcpSocket matchMakingServer;

	//Game server tcp socket
	sf::TcpSocket gameServer;

	//Game server udp socket
	sf::UdpSocket gameServerUDP;
	gameServerUDP.setBlocking(false);

	//Estimate Latency
	float latency= 0;

	//GameTime
	float gameTime=-1;

	//UDP send interval
	float sendDelay = 0;

	sf::SocketSelector receiveSelector;

	//Connect to the match making server
	if (matchMakingServer.connect(mmServerIp, MATCHSERVERPORT)!= sf::Socket::Done)
	{
		die("Can't find the match making server");
	}

	//Recieve the id of the player from MM server
	sf::Packet idPacket;
	if (matchMakingServer.receive(idPacket) != sf::Socket::Done)
	{
		die("Didn't receive ID from match making server");
	}
	idPacket >> playerId;


	//Create the window
	sf::RenderWindow window(sf::VideoMode(1440, 800), "Blob Blast");


	// Initialise input, level and matchmakinglevel
	Input input;
	MatchMakingLevel mmlevel(&window, &input,playerId);
	Level level(&window, &input,playerId);

	sf::Clock clock;
	float deltaTime;

	//"Game" Loop
	while (window.isOpen())
	{
	    //Match Making
		//While the client is still in the Match making server
		while (server == sf::IpAddress::None)
		{
			//Process window and input events
			windowProcess(&window, &input);

			//If changes in ready
			if (mmlevel.handleInput())
			{
				//Send changes to match making server
				sf::Packet readyPacket;
				readyPacket << mmlevel.getReady();
				if (matchMakingServer.send(readyPacket) != sf::Socket::Done)
				{
					die("Couldn't reach match making server");
				}
			}

			//Update and Render
			mmlevel.update();
			mmlevel.render();
			
			//If the client is ready
			if (mmlevel.getReady())
			{
				//wait to receive info for game server
				sf::Packet info;
				if (matchMakingServer.receive(info) != sf::Socket::Done)
				{
					die("Couldn't reach match making server");
				}
				else
				{
					//Get Server Info
					std::string serverip;
					info >> numOfPlayers >> serverip >> port;
					if (serverip != "")
					{
						server = sf::IpAddress::IpAddress(serverip);
					}
				}
			}
		}
		

		//If the client recieved details for the game server
		//While the socket is not connected between the client and game server
		if (gameServer.connect(server, port) == sf::Socket::Error)
		{
			die("Couldn't connect to the server");
		}
	
		//Set up
		//Before the game set up
		if (gameTime < 0)
		{
			//If connected send this players id to the server
			sf::Packet idPacket;
			idPacket << playerId;
			auto start = std::chrono::steady_clock::now();
			if (gameServer.send(idPacket) != sf::Socket::Done)
			{
				die("Could't reach game server");
			}

			//Receive latency info
			if (gameServer.receive(idPacket) != sf::Socket::Done)
			{
				die("Could't receive latency information from the game server");
			}
			else
			{
				//Calculate latency
				auto end = std::chrono::steady_clock::now();
				std::chrono::duration<float> elapsedTime = end - start;
				latency = elapsedTime.count();
			}

			//Receive info for the players
			sf::Packet playersInfoPacket;
			if (gameServer.receive(playersInfoPacket) == sf::Socket::Done)
			{
				playersInfoPacket >> udpPort;
				gameServerUDP.bind(udpPort);
				for (int i = 0; i < numOfPlayers; i++)
				{
					int id;
					sf::Vector2f spawnPosition;
					playersInfoPacket >> id >> spawnPosition.x >> spawnPosition.y;
					level.spawnPlayer(id, spawnPosition);
				}

			}

			//Set Up GUI
			level.setUpGui();

			//Receive start timer
			sf::Packet startPacket;
			if (gameServer.receive(startPacket) != sf::Socket::Done)
			{
				die("Didn't receive start signal from server");
			}
			else
			{
				//Start game time
				clock.restart().asSeconds();
				gameServer.setBlocking(false);
				gameTime = latency;

			}

			// GAME LOOP///////////////////////////////////////////////////////////
			while (true)
			{
				//Calculate dt
				deltaTime = clock.restart().asSeconds();

				//Calculate current gameTime
				gameTime += deltaTime;

				//Calculate send Delay counter
				sendDelay += deltaTime;

				//Process window events and input
				windowProcess(&window, &input);

				
				
						//RECEIVE UDP Packets from server
						sf::Packet udpPacket;
						sf::IpAddress rserver;
						unsigned short rport;

						if (gameServerUDP.receive(udpPacket, rserver, rport) == sf::Socket::Done)
						{
							//If received updates for players
							int numOfUpdates;
							udpPacket >> numOfUpdates;
							for (int i = 0; i < numOfUpdates; i++)
							{
								UDPUpdates updates;
								int id = -1;
								udpPacket >> id >> updates.ppUpdate.playerPosition.x >> updates.ppUpdate.playerPosition.y >> updates.hpUpdate.bulletPosition.x >> updates.hpUpdate.bulletPosition.y >> updates.gameTime;
								if (id != playerId)
								{
									level.applyUdpUpdates(id, updates);
								}
							}
						}
					

					
						//RECEIVE TCP
						sf::Packet tcpPacket;
						if (gameServer.receive(tcpPacket) == sf::Socket::Done)
						{
							//If received TCP update
							TCPUpdates tcpUpdate;
							int hUpdateNum = 0, bUpdateNum = 0, oUpdateNum = 0;
							//Get NUmber of Updates
							tcpPacket >> hUpdateNum >> bUpdateNum >> oUpdateNum;
							//Hit Updates
							for (int j = 0; j < hUpdateNum; j++)
							{
								HitUpdate newHit;
								tcpPacket >> newHit.id >> newHit.damage;
								tcpUpdate.hitUpdate.push_back(newHit);
							}

							//Bullet Updates
							for (int j = 0; j < bUpdateNum; j++)
							{
								ProjectilesSpawnUpdate newBullet;
								int type;
								tcpPacket >> newBullet.id >> type >> newBullet.spawnPosition.x >> newBullet.spawnPosition.y >> newBullet.velocity.x >> newBullet.velocity.y >> newBullet.gameTime;
								newBullet.type = BulletType(type);
								tcpUpdate.psUpdates.push_back(newBullet);
							}

							//Object Updates
							for (int j = 0; j < oUpdateNum; j++)
							{
								ObjectUpdate object;
								tcpPacket >> object.tileMapObjectPosition >> object.objectStatus;
								tcpUpdate.oUpdate.push_back(object);
							}

							//Apply the updates
							level.applyTcpUpdates(tcpUpdate);
						}
					
				//For this player
				level.handleInput(deltaTime, gameTime);
				level.update(deltaTime, gameTime);
				level.render();

				//SEND UDP Pcket for updated player  every 0.05s
				if (sendDelay >= 0.05)
				{
					sf::Packet udpPacket;
					udpPacket = level.packageUdp(gameTime);

					//Send to the server
					if (gameServerUDP.send(udpPacket, server, udpPort) != sf::Socket::Done)
					{
						die("Could't reach game server");
					}

					//Reset the send counter
					sendDelay = 0;
				}

				//TCP UPDATES
				if (level.newBullets.size() > 0)
				{
					sf::Packet tcpPacket;
					tcpPacket = level.packageTcp();

					if (gameServer.send(tcpPacket) != sf::Socket::Done)
					{
						die("Couldn't reach game server");
					}
				}
			}
		}

	}
}