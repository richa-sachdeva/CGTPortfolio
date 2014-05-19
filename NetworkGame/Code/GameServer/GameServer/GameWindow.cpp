// For core module AG1107A - Network Game Development, Networked Game, University of Abertay, Dundee.
// By Richa Sachdeva, 1304869, M.Sc. Computer Games Technology
#include "stdafx.h"
#include "GameWindow.h"

GameWindow::GameWindow(){
}

void GameWindow::ClearWindow(){
	_mainWindow.clear();
}

void GameWindow::ShowWindow(){
	_mainWindow.display();
}

void GameWindow::DrawWindow(){
	// add the players and draw....pretty much like repaint function!!!
//	_mainWindow.draw(m_Client->GetClientPlayer());
}

void GameWindow::UpdateGameWindow(){
	this->ClearWindow();
	this->ShowWindow();
	this->DrawWindow();
}

GameWindow::~GameWindow()
{
}

sf::RenderWindow& GameWindow::GetWindow(){
	return _mainWindow;
}

int GameWindow::GetScreenWidth(){
	return SCREEN_WIDTH;
}

int GameWindow::GetScreenHeight(){
	return SCREEN_HEIGHT;
}

void GameWindow::StartNetworking(){

	while(!_networkGame.Start()){
	 }

	while(!_networkGame.IsGameStarted()){
		_networkGame.SendFirstInput(GetServerSpeed());
	}

	sf::sleep(sf::milliseconds(40));
	
	Start();

}

void GameWindow::Start(){
	
	// Initialise all variables
	
	_mainWindow.create(sf::VideoMode(SCREEN_WIDTH,SCREEN_HEIGHT,32)," Networked Game");
	_mainWindow.setPosition(sf::Vector2i(5, 100));
	this->ClearWindow();
	this->ShowWindow();

	// Initialise rest of the variables, for ease, can call different functions
	 _serverPlayer = ServerPlayer();
	 _clientPlayer = ClientPlayer();

	// Add the Coins/Bricks
	float x = 10, y = SCREEN_HEIGHT/2, offset = 35;
	for (int i = 0; i < COINS; i++){
		// sf::Color c = Random color
		coinList.push_back(Coin(x, y, 0.0, 0.0, COINRAD, sf::Color::Yellow));
		x += offset;
	}

	x = 10, y = 30, offset = 20;
	for (int i = 0; i < NUM_BULLETS; i++){
		bulletList.push_back(Bullet(x, y)); // push_front(Bullet());
		bulletList.at(i).updateColor(sf::Color::White);
		bulletList.at(i).setSpeed(0, 0);
		bulletList.at(i).SetPlayServer(true);
		bulletList.at(i).SetShown(false);
		y += offset;
		showBullet.push_back(false);
	}

	for(int i = PLAYER_LIMIT; i< NUM_BULLETS; i++){
		// This is player
		bulletList.at(i).SetPlayServer(false);
	}
	
	GameLoop();
}

void GameWindow::GameLoop(){

	// timer will update the server, based on client's value, every 100 milliseconds
	sf::Time elapsed, timer, gameTime;
	sf::Clock clock, clock1, gameClock;
	float counter = 0.85f;
	sf::Vector2f pos;
	Bullet b;
	bool bulletFired = false;
	int countS = 0;
	int countP = 0;
	bool fired = false;
	bool once = false;

	while (_mainWindow.isOpen())
    {
        sf::Event event;
		serverBullet = 0;
		clientBullet = 0;
		bulletFired = false;
		
        while (_mainWindow.pollEvent(event))
        {
			switch (event.type)
			{
				case sf::Event::Closed:
					_mainWindow.close();
					break;
				case sf::Event::KeyPressed:
					break;
			}
        }
        
		if(_serverPlayer.IsServerAlive() && _clientPlayer.IsPlayerAlive())
		{
			// Update all the variables --------------------------------
			// Update the bulletList
			if(!bulletList.empty()){
			
				countS = 0;
				countP = 0;
				// Server side bullet
				for(int i = 0; i< NUM_BULLETS/2; i++){
					b = bulletList.at(i);
					pos = b.currentPosition();	
				
					if(!bulletList.at(i).IsShown() && bulletList.at(i).IsServer()){
						countS++;
						showBullet.at(i) = false;
					}

					if((pos.y > (SCREEN_HEIGHT - 4*SERVER_RADIUS)) && (b.getColor() != sf::Color::Black)){
						if(playerTest(_clientPlayer, b)){
							_clientPlayer.PlayerDies();
							std::cout<<"player dies\n";
						}
					}

					bulletList.at(i).update(_mainWindow);
				
				}
				serverBullet = SERVER_LIMIT - countS;
				//std::cout<<" countS: "<<serverBullet<<"\n";
			
				// for player's bullet movements
				for(int i = PLAYER_LIMIT; i< NUM_BULLETS; i++){

					if(!bulletList.at(i).IsShown() && !bulletList.at(i).IsServer()){
						countP++;
						showBullet.at(i) = false;
					}

					b = bulletList.at(i);
					pos = b.currentPosition();	

					if(pos.y < 15 && b.getColor() != sf::Color::Black){
						if(serverTest(_serverPlayer, b)){
							_serverPlayer.ServerDies();
							std::cout<<"server dies\n";
						}
					}
					bulletList.at(i).update(_mainWindow);
				}
				// countP has number which are not shown
				clientBullet = PLAYER_LIMIT - countP;
			}

			for(int i = 0; i<COINS; i++){
			
				Coin c = coinList.at(i);
				int val = _networkGame.getCoinToDelete();
				if( val == i){
					c.NoCoin();
				}
	
				if(c.getCoinColor() != sf::Color::Black){
					TestForBullets(c);
					//if(c.getCoinColor() == sf::Color::Black){
					//	// Color of the coin is black which means player count should be increased
					//	_clientPlayer.increaseCoinCount(1);
					//}
					//std::cout<<"hit\n";
				}
				coinList.at(i) = c;
			}

			elapsed = clock.getElapsedTime();
			if(elapsed.asSeconds() > counter){
				// Time to fire a bullet
				sf::Vector2f pos = _serverPlayer.currentPosition();
				if(serverBullet < SERVER_LIMIT){
					fire(pos.x, pos.y, _mainWindow, false);
					serverBullet++;
					bulletFired = true;
				}
				counter += 0.85;
			}

			timer = clock1.getElapsedTime();
			if(timer.asSeconds() > 0.10){
			//	std::cout<<"100 ms done\n";
			
				// server position and all
				if(_networkGame.IsConnected()){
					_networkGame.SendClientInput(GetServerSpeed(), serverBullet, GetServerPosition(), GetClientSpeed(), GetClientPosition());
				}
				// update 
				clock1.restart();
			//	ServerSpeed(_networkGame.ServerSpeed());
				SetClientSpeed(_networkGame.ClientSpeed());
				//SetClientPosition(_networkGame.ClientPos());
			}
			else{
				// Get data from network
				_networkGame.GetInput(GetServerSpeed(), serverBullet, GetClientSpeed(), bulletFired, _serverPlayer.IsServerAlive(), _clientPlayer.IsPlayerAlive(), _serverPlayer.currentPosition().x,  gameClock.getElapsedTime().asSeconds());
				_networkGame.DoStuff();
			}
	
			// check if client is still there or not
			if(!_networkGame.clientAlive()){
				_clientPlayer.PlayerDies();
			}

			if(!_networkGame.serverAlive() || _networkGame.getClientScore() == COINS){
				_serverPlayer.ServerDies();
			}
		
			// update server after getting input from network
			//SetServerSpeed(_networkGame.ServerSpeed());
			_serverPlayer.update(_mainWindow);
		

			// Update Client from data from Network
			//SetClientSpeed(_networkGame.ClientSpeed());
			SetClientPosition(_networkGame.ClientPos());
			_clientPlayer.update(_mainWindow);

			// Clear the main window, to re-render ---------------------------------
			_mainWindow.clear();

			if(_networkGame.ClientFire())
			{
				// if true
				float currentPos = _networkGame.ClientPos(); 
				pos = _clientPlayer.currentPosition();
				if(currentPos != pos.x)
					_clientPlayer.setPosition(currentPos);

				fire(currentPos, pos.y, _mainWindow, true);
			}
		
			// Draw all the variables -----------------------------------------------
			_serverPlayer.draw(_mainWindow);
			_clientPlayer.draw(_mainWindow);

			// Draw the Coins/Bricks
			for each(Coin c in coinList){
				if(c.IsCoinShown())
					c.draw(_mainWindow);
			}

			// Draw the Bullets
			if(!bulletList.empty())
			{
				for each(Bullet b in bulletList){
					b.draw(_mainWindow);
				}
			}
		}
		else{
			GameOver();
		}
		// Display the main window -----------------------------------------------
		//gameClock.restart();
        _mainWindow.display();		
		
		if(!once){
				sf::sleep(sf::seconds(1));
				_serverPlayer.setSpeed(0.25, 0);
				once = true;
			}
    }

}

bool GameWindow::IsExiting()
{
   if(_gameState == Exiting) 
     return true;
   else 
     return false;
}

void GameWindow::ShowStartScreen(){

	sf::sleep(sf::seconds(5));

}

void GameWindow::GameOver(){
	// here show Game Over Image	
	// Here 1st check who wins and loses and likewise load the image
	// This is server so Server -> Wins/Loses
	sf::Texture image;
	if(!_serverPlayer.IsServerAlive()){
		if(image.loadFromFile("images/Game-Lose.png") != true)
		{
			return;
		}
	}
	else{
		if(image.loadFromFile("images/Game-Won.png") != true)
		{
			return;
		}
	}
	sf::Sprite sprite(image);
	
	_mainWindow.draw(sprite);
}


float GameWindow::GetClientSpeed(){
	return _clientPlayer.getDX();
}

void GameWindow::SetClientSpeed(float val){
	_clientPlayer.setSpeed(val, 0);
}

float GameWindow::GetClientPosition(){
	sf::Vector2f pos = _clientPlayer.currentPosition();
	return pos.x;
}

void GameWindow::SetClientPosition(float val){
	_clientPlayer.setPosition(val);
}

float GameWindow::GetServerSpeed(){
	return _serverPlayer.getDX();
}

float GameWindow::GetServerPosition(){
	return _serverPlayer.currentPosition().x;
}

void GameWindow::SetServerSpeed(float val){
	_serverPlayer.setSpeed(val, 0);
}

void GameWindow::SetServerPosition(float val){
	_serverPlayer.setCurrentPosition(val);
}


void GameWindow::fire(float posX, float posY, sf::RenderWindow& _mainWindow, bool player){

	if(player){
		for(int i = PLAYER_LIMIT; i<NUM_BULLETS; i++){
			if(showBullet.at(i) == false){
				//std::cout<<"inside player firing fn\n";
				bulletList.at(i).setPosition(posX, posY);
				bulletList.at(i).updateColor(sf::Color::Magenta);	
				bulletList.at(i).setSpeed(0, -BULLET_SPEED);
				bulletList.at(i).SetPlayServer(false);
				bulletList.at(i).SetShown(true);
				showBullet.at(i) = true;

				bulletList.at(i).update(_mainWindow);

				break;
			}
		}
	}
	else{
		// This is for server-> so check from 0 till 4, which one is false, and add there
		for(int i = 0; i<NUM_BULLETS/2; i++){
			if(showBullet.at(i) == false){
				bulletList.at(i).setPosition(posX, posY);
				bulletList.at(i).updateColor(sf::Color::Magenta);	
				bulletList.at(i).setSpeed(0, BULLET_SPEED);
				bulletList.at(i).SetPlayServer(true);
				bulletList.at(i).SetShown(true);
				showBullet.at(i) = true;

				bulletList.at(i).update(_mainWindow);
				break;
			}
		}
	}
}

bool GameWindow::TestForBullets(Coin& coin){

	// Coin is rectangle of size -> 20 * 30
	Bullet b;
	sf::Vector2f pos;
	for(int i = 0; i< NUM_BULLETS; i++){

		b = bulletList.at(i);
		pos = b.currentPosition();
		if(b.getColor() == sf::Color::Black || b.IsShown() == false || b.getColor() == sf::Color::White)
			continue;
		if( (pos.y > 280 && pos.y < 340) || (pos.y < 330 && pos.y > 280) ){
			if(circleTest(coin, b)){
				b.setSpeed(0, 0);
				b.updateColor(sf::Color::Black);

				if(i < NUM_BULLETS/2)
					b.SetPlayServer(true);
				else{
					b.SetPlayServer(false);
					coin.NoCoin();
				}

				b.SetShown(false);
				showBullet.at(i) = false;

				bulletList.at(i) = b; 
				return true;
			}
			
		}
	}
	return false;
}

bool GameWindow::circleTest(Coin& coin, Bullet& bullet){
	bool collide = false;

	sf::Vector2f coinPos = coin.currentPosition();
	sf::Vector2f bulletPos = bullet.currentPosition();
	sf::Vector2f distance = coinPos - bulletPos;

	sf::Vector2f coinRad = coin.getRad();
	sf::Vector2f bullRad = bullet.getRad();
	
	sf::RectangleShape coin1 = sf::RectangleShape(coinRad);
	sf::RectangleShape bullet1 = sf::RectangleShape(bullRad);

	coin1.setPosition(coinPos);
	bullet1.setPosition(bulletPos);
	//std::cout<<"outside  coin1: "<< coinPos.x << " y: "<< coinPos.y<<" bullet: "<<bulletPos.x<< " y: "<<bulletPos.y<<"\n";

	//if(distance.x * distance.x + distance.y * distance.y <= (Radius1 + Radius2) * (Radius1 + Radius2))
	if(bullet1.getGlobalBounds().intersects(coin1.getGlobalBounds())){
		collide = true;
		//std::cout<<"collide \n";
	}
	return collide;
}

bool GameWindow::playerTest(ClientPlayer& player, Bullet& obj){
	bool collide = false;

	sf::Vector2f playerPos = player.currentPosition();
	sf::Vector2f bulletPos = obj.currentPosition();

	float coinRad = player.getRad();
	sf::Vector2f dimn = sf::Vector2f(coinRad*2.0, coinRad*2.0);
	sf::Vector2f bullRad = obj.getRad();
	
	sf::RectangleShape player1 = sf::RectangleShape(sf::Vector2f(dimn));
	sf::RectangleShape bullet1 = sf::RectangleShape(bullRad);

	player1.setPosition(playerPos);
	bullet1.setPosition(bulletPos);

	if(bullet1.getGlobalBounds().intersects(player1.getGlobalBounds())){
		collide = true;
	}

	return collide;
}

bool GameWindow::serverTest(ServerPlayer& server, Bullet& obj){

	bool collide = false;

	sf::Vector2f serverPos = server.currentPosition();
	sf::Vector2f bulletPos = obj.currentPosition();

	float coinRad = server.getRad();
	sf::Vector2f dimn = sf::Vector2f(coinRad*2.0, coinRad*2.0);
	sf::Vector2f bullRad = obj.getRad();
	
	sf::RectangleShape server1 = sf::RectangleShape(sf::Vector2f(dimn));
	sf::RectangleShape bullet1 = sf::RectangleShape(bullRad);

	server1.setPosition(serverPos);
	bullet1.setPosition(bulletPos);

	if(bullet1.getGlobalBounds().intersects(server1.getGlobalBounds())){
		collide = true;
	}
	return collide;
}

GameWindow::GameState GameWindow::_gameState = Uninitialized;
sf::RenderWindow GameWindow::_mainWindow;
