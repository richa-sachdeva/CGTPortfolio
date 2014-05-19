// For core module AG1107A - Network Game Development, Networked Game, University of Abertay, Dundee.
// By Richa Sachdeva, 1304869, M.Sc. Computer Games Technology
#include "stdafx.h"
#include "GameNetwork.h"

GameNetwork::GameNetwork(){

	quit = false;
	game = false;
	connected = false;
}

GameNetwork::~GameNetwork(){

}

bool GameNetwork::Start(){
	
	socket.setBlocking(false);
	std::cout<<"waiting for connection \n";
    sf::TcpListener listener;
    listener.listen(PORT);
  
	// at this point client is connected
	if(listener.accept(socket) != sf::Socket::Done)
		return false;
	connected = true;
	return true;
}

bool GameNetwork::Stop(){
	return false;
}

void GameNetwork::SendClientInput(float dx, int bullets, float pos, float clientdx, float clientPos){
	// Function to Sync the Player with the Server, every 100 or so milliseconds
	sf::Packet packetSend;
	globalMutex.lock(); 
	sendBob.startGame = true;
	sendBob.serverX = pos;
	sendBob.serverDx = dx;
	sendBob.serverBullets = bullets;
	sendBob.serverFire = true;
	sendBob.clientX = clientPos;
	sendBob.clientDx = clientdx;
	sendBob.clientBullets = rcvBob.clientBullets;
	sendBob.clientFire = rcvBob.clientFire;

	sendBob.clientScore = rcvBob.clientScore;
	sendBob.coinToDelete = rcvBob.coinToDelete;
    globalMutex.unlock();

	globalMutex.lock();
	packetSend << sendBob.startGame << sendBob.serverX << sendBob.serverDx << sendBob.serverBullets << sendBob.serverFire << sendBob.clientX << sendBob.clientDx << sendBob.clientBullets << sendBob.clientFire << sendBob.serverAlive << sendBob.clientAlive; 
	globalMutex.unlock();

}

void GameNetwork::DoStuff(void){
	
		sf::Packet packetSend, packetReceive;
     
		globalMutex.lock();
		packetSend << sendBob.startGame << sendBob.serverDx << sendBob.serverBullets << sendBob.serverFire << sendBob.clientDx << sendBob.serverAlive << sendBob.clientAlive << sendBob.serverX << sendBob.serverTime; 
        globalMutex.unlock();

        socket.send(packetSend);

        socket.receive(packetReceive);      
		if(packetReceive >> rcvBob.startGame >> rcvBob.clientDx >> rcvBob.clientBullets >> rcvBob.clientFire >> rcvBob.serverAlive >> rcvBob.clientAlive >> rcvBob.clientX >> rcvBob.coinToDelete >> rcvBob.clientScore)
        {	
			if(oldBob.clientDx != rcvBob.clientDx || oldBob.clientBullets != rcvBob.clientBullets || oldBob.clientScore != rcvBob.clientScore || oldBob.coinToDelete != rcvBob.coinToDelete){
			
			std::cout << "msg recvd- clientDx: " << rcvBob.clientDx << "clientX: " << rcvBob.clientX << " bullets " << rcvBob.clientBullets << " coin: " << rcvBob.coinToDelete << " score: " << rcvBob.clientScore << "\n";
				
				oldBob.clientX = rcvBob.clientX;
				oldBob.startGame = rcvBob.startGame;
				oldBob.clientBullets = rcvBob.clientBullets;
				oldBob.clientFire = rcvBob.clientFire;

				oldBob.clientDx = rcvBob.clientDx;

				oldBob.serverAlive = rcvBob.serverAlive;
				oldBob.clientAlive = rcvBob.clientAlive;

				oldBob.clientX = rcvBob.clientX;

				oldBob.coinToDelete = rcvBob.coinToDelete;

				oldBob.clientScore = rcvBob.clientScore;
			}
		}
}


void GameNetwork::GetInput(float speed, int serverBullets, float clientSpeed, bool fired, bool sDead, bool pDead, float serverPos,  float seconds){

	static float oldServerDX, oldServerX;
	static int oldBullets = 0;

	if( oldServerX != serverPos)
		oldServerX = serverPos;

	if( oldServerDX != speed)
		oldServerDX = speed;
	
	if(serverBullets < 0)
		serverBullets = 0;
	if( oldBullets != serverBullets){
		if(oldBullets < serverBullets)
			oldBullets = serverBullets - oldBullets;
		else
			oldBullets = serverBullets;
	}

	globalMutex.lock(); 
	sendBob.serverDx = oldServerDX;
	sendBob.serverX = serverPos;
	sendBob.serverBullets = oldBullets;
	sendBob.serverFire = fired;
	sendBob.clientDx = clientSpeed;
	sendBob.serverAlive = sDead;
	sendBob.clientAlive = pDead;
	sendBob.serverTime =  seconds;
    globalMutex.unlock();
}


bool GameNetwork::SendFirstInput(float sdx){

	globalMutex.lock(); 
	sendBob.serverDx = sdx;
	sendBob.serverFire = false;
	sendBob.startGame = true;
    globalMutex.unlock();

	quit = false;
	DoStuff();
	return true;

}


Character GameNetwork::oldBob;