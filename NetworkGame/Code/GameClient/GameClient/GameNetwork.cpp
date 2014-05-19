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
	if(socket.connect(IPADDRESS, PORT) == sf::Socket::Done)
    {
        std::cout << "Client Connected\n";
		connected = true;
        return true;
    }
	return false;
}

bool GameNetwork::Stop(){
	return false;
}

void GameNetwork::ReceiveServerInput(){

	 sf::Packet packetReceive;

    socket.receive(packetReceive);      
	if(packetReceive >> rcvBob.startGame >> rcvBob.serverX >> rcvBob.serverDx >> rcvBob.serverBullets >> rcvBob.serverFire >> rcvBob.clientX >> rcvBob.clientDx >> rcvBob.clientBullets >> rcvBob.clientFire >> rcvBob.serverAlive >> rcvBob.clientAlive >> rcvBob.coinToDelete >> rcvBob.clientScore)
    { 	
			oldBob.startGame = rcvBob.startGame;

			oldBob.serverX = rcvBob.serverX;
			oldBob.serverDx = rcvBob.serverDx;
			oldBob.serverBullets = rcvBob.serverBullets;
			oldBob.serverFire = rcvBob.serverFire;
			oldBob.serverAlive = rcvBob.serverAlive;

			oldBob.clientX = rcvBob.clientX;
			oldBob.clientDx = rcvBob.clientDx;
			oldBob.clientBullets = rcvBob.clientBullets;
			oldBob.clientFire = rcvBob.clientFire;
			oldBob.clientAlive = rcvBob.clientAlive;

			oldBob.coinToDelete = rcvBob.coinToDelete;
			oldBob.clientScore = rcvBob.clientScore;
	}
}

void GameNetwork::DoStuff(void){
	
		sf::Packet packetSend, packetReceive;
     
		globalMutex.lock();
		packetSend << sendBob.startGame << sendBob.clientDx << sendBob.clientBullets << sendBob.clientFire << sendBob.serverAlive << sendBob.clientAlive << sendBob.clientX << sendBob.coinToDelete << sendBob.clientScore; 
        globalMutex.unlock();

        socket.send(packetSend);
			
        socket.receive(packetReceive);      
		if(packetReceive >> rcvBob.startGame >> rcvBob.serverDx >> rcvBob.serverBullets >> rcvBob.serverFire >> rcvBob.clientDx >> rcvBob.serverAlive >> rcvBob.clientAlive >> rcvBob.serverX >> rcvBob.serverTime)
        { 
			if(oldBob.serverDx != rcvBob.serverDx || oldBob.serverBullets != rcvBob.serverBullets){
	
				std::cout << "msg recvd- clientx: " << rcvBob.serverDx << " time " << rcvBob.serverTime << "\n";
	
				oldBob.serverDx = rcvBob.serverDx;
				oldBob.startGame = rcvBob.startGame;
				oldBob.serverBullets = rcvBob.serverBullets;
				oldBob.serverFire = rcvBob.serverFire;

				oldBob.clientDx = rcvBob.clientDx;
				oldBob.serverAlive = rcvBob.serverAlive;
				oldBob.clientAlive = rcvBob.clientAlive;

				oldBob.serverX = rcvBob.serverX;
				oldBob.serverTime = rcvBob.serverTime;
			}
		}
}

void GameNetwork::GetInput(float speed, int clientBullets, bool fired, bool sDead, bool pDead, float clientPos){

	int serverX, clientX;
	static float oldClientDX, oldClientX;
	static int oldBullets = 0;
  
	if( oldClientX != clientPos)
		oldClientX = clientPos;

	if ( oldClientDX != speed)
		oldClientDX = speed;

	if(clientBullets < 0)
		clientBullets = 0;
	if( oldBullets != clientBullets){
		if(oldBullets < clientBullets)
			oldBullets = clientBullets - oldBullets;
		else
			oldBullets = clientBullets;
	}

	globalMutex.lock(); 
	sendBob.clientDx = oldClientDX; // client's speed
	sendBob.clientX = oldClientX;
	sendBob.clientBullets = oldBullets;
	sendBob.clientFire = fired;
	sendBob.serverAlive = sDead;
	sendBob.clientAlive = pDead;
    globalMutex.unlock();
}



bool GameNetwork::SendFirstInput(void){
  
	bool s = false;
	char c;
	std::cout<<" Connected with server, press y to start the game\n";
	std::cin >> c;
	if(c == 'y')
		s = true;
	
	globalMutex.lock(); 
	sendBob.clientDx = 0;
	sendBob.clientBullets = 0;
	sendBob.clientFire = false;
	sendBob.startGame = s;
    globalMutex.unlock();

	quit = false;
	DoStuff();
	return s;
}

void GameNetwork::setCoinToDelete(int val){
	sendBob.coinToDelete = val;
}

void GameNetwork::setClientScore(int val){
	sendBob.clientScore = val;
}

Character GameNetwork::oldBob;
