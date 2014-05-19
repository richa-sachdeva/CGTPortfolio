// For core module AG1107A - Network Game Development, Networked Game, University of Abertay, Dundee.
// By Richa Sachdeva, 1304869, M.Sc. Computer Games Technology
#include "stdafx.h"
#include <iostream>
#include <SFML/Network.hpp>
#ifndef GameNetwork_H
#define GameNetwork_H



const unsigned short PORT = 5555;
const std::string IPADDRESS("127.0.0.1");//change to suit your needs

struct Character
{
   bool startGame;
   
   float serverX;
   float serverDx;
   int serverBullets;
   bool serverFire;
    bool serverAlive;

   float clientX;
   float clientDx;
   int clientBullets;
   bool clientFire;
   bool clientAlive;

   int coinToDelete;
   int clientScore;
    
   float serverTime;

   Character(){
	   startGame = false;
	   serverX = 0.0;
	   serverDx = 0.0;
	   serverBullets = 0;
	   serverFire = false;
	   serverAlive = true;

	   clientX = 0.0;
	   clientDx = 0.0;
	   clientBullets = 0;
	   clientFire = false;
	   clientAlive = true;

	   coinToDelete = -1;
	   clientScore = 0;

	  serverTime = 0.0;
   }

};



class GameNetwork{

public:
	GameNetwork();
	~GameNetwork();

	bool Start();
	bool Stop();

	void DoStuff(void);
	// Should have two versions, one only for server
	void GetInput(float, int, float, bool, bool, bool, float, float);
	// Second to send after 100ms and update the client
	void SendClientInput(float dx, int bullets, float pos, float clientdx, float clientPos);

	bool DoQuit(){return quit;}
	bool IsGameStarted(){return rcvBob.startGame;}

	bool SendFirstInput(float);

	float ServerPos(){return rcvBob.serverX;}
	float ServerSpeed(){ return rcvBob.serverDx;}
	int ServerBullets(){ return rcvBob.serverBullets;}
	bool ServerFire(){ return rcvBob.serverFire;}
	bool serverAlive(){ return rcvBob.serverAlive;}

	float ClientPos(){ return rcvBob.clientX;}
	float ClientSpeed(){ return rcvBob.clientDx;}
	int ClientBullets(){ return rcvBob.clientBullets;}
	bool ClientFire(){return rcvBob.clientFire;}
	bool clientAlive(){ return rcvBob.clientAlive;}

	int getCoinToDelete(){ return rcvBob.coinToDelete;}

	bool IsConnected(){ return connected;}

	int getClientScore(){ return rcvBob.clientScore;}
	void setClientScore(int);

private:
	Character sendBob;
	Character rcvBob;

	sf::TcpSocket socket;
	sf::Mutex globalMutex;
	bool quit;
	bool game;

	static Character oldBob;
	bool connected; 
};
#endif
