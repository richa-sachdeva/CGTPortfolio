// For core module AG1107A - Network Game Development, Networked Game, University of Abertay, Dundee.
// By Richa Sachdeva, 1304869, M.Sc. Computer Games Technology
#include "stdafx.h"
#include "GameWindow.h"

GameWindow::GameWindow(){
	/*thread = 0;
	_networkGame = GameNetwork();*/
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
		_networkGame.SendFirstInput();
	}

	sf::sleep(sf::milliseconds(10));
	
	Start();
	
}

void GameWindow::Start(){
	
	// Initialise all variables
	_mainWindow.create(sf::VideoMode(SCREEN_WIDTH,SCREEN_HEIGHT,32)," Networked Game");
	_mainWindow.setPosition(sf::Vector2i(730, 100));
	this->ClearWindow();
	this->ShowWindow();

	// Initialise rest of the variables
	_serverPlayer = ServerPlayer();
	_clientPlayer = ClientPlayer();

	// Add the Coins/Bricks
	float x = 10, y = SCREEN_HEIGHT/2, offset = 35;
	for (int i = 0; i < COINS; i++){
		// sf::Color c = Random color
		coinList.push_back(Coin(x, y, 0.0, 0.0, COINRAD, sf::Color::Yellow));
		x += offset;
	}

	for each(Coin c in coinList){
		c.setCoinShow(true);
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

	//sf::sleep(sf::milliseconds(10));
	GameLoop();
}

void GameWindow::GameLoop(){

	Bullet b;
	sf::Vector2f pos;
	sf::Time timer, elapsed;
	sf::Clock clock, clock1;
	float counter = 0.85f;
	int countS = 0;
	int countP = 0;
	bool bulletFired = false;

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
					if(event.key.code == sf::Keyboard::Left)
					{
						_clientPlayer.moveLeft(_mainWindow);
						break;
					}
					else if(event.key.code == sf::Keyboard::Right){
						_clientPlayer.moveRight(_mainWindow);
						break;
					}
					else if(event.key.code == sf::Keyboard::Down){
						_clientPlayer.moveDown(_mainWindow);
						break;
					}
					else if(event.key.code == sf::Keyboard::Space){
						sf::Vector2f b2pos = _clientPlayer.currentPosition();
						if(clientBullet < PLAYER_LIMIT){		
							fire(b2pos.x, b2pos.y, _mainWindow, true);
							clientBullet++;
							bulletFired = true;
						}
						break;
					}
				default:
					break;
			}
        }
        
		// Update all the variables --------------------------------
		if(_serverPlayer.IsServerAlive() && _clientPlayer.IsPlayerAlive())
		{
			// Update the bulletList
			if(!bulletList.empty()){
			
				countS = 0;
				countP = 0;
				// Server side bullet -> Code deleted
				// for player's bullet movements
				for(int i = 0; i< NUM_BULLETS/2; i++){
					if(!bulletList.at(i).IsShown() && bulletList.at(i).IsServer()){
						countS++;
						showBullet.at(i) = false;
					}

					b = bulletList.at(i);
					pos = b.currentPosition();	

					if((pos.y > (SCREEN_HEIGHT - 4*SERVER_RADIUS)) && (b.getColor() != sf::Color::Black)){
						if(playerTest(_clientPlayer, b)){
							_clientPlayer.PlayerDies();
							std::cout<<"player dies\n";
						}
					}

					bulletList.at(i).update(_mainWindow);
				}
				serverBullet = SERVER_LIMIT - countS;

				// Client Side
				for(int i = PLAYER_LIMIT; i< NUM_BULLETS; i++){
					// This is for player
					b = bulletList.at(i);
					pos = b.currentPosition();	

					if(!bulletList.at(i).IsShown() && !bulletList.at(i).IsServer()){
						countP++;
						showBullet.at(i) = false;
					}
					if(pos.y < 15 && b.getColor() != sf::Color::Black){
						if(serverTest(_serverPlayer, b)){
							_serverPlayer.ServerDies();
							std::cout<<"server dies\n";
						}
					}

					bulletList.at(i).update(_mainWindow);
				}
				clientBullet = PLAYER_LIMIT - countP;
			}
			// This is the number which is active
			//std::cout<<"player bullet: "<<clientBullet<<"\n";
			
			for(int i = 0; i<COINS; i++){
			
				Coin c = coinList.at(i);
				if(c.getCoinColor() != sf::Color::Black){
					TestForBullets(c);
					if(c.getCoinColor() == sf::Color::Black){
						// Color of the coin is black which means player count should be increased
						_networkGame.setCoinToDelete(i);
						_clientPlayer.increaseCoinCount(1);
						_networkGame.setClientScore(_clientPlayer.getCoins());
					}
				}
				coinList.at(i) = c;
			}

			timer = clock1.getElapsedTime();
			if(timer.asSeconds() > 0.10){
				//std::cout<<"100 ms done\n";
				// server position and all
				if(_networkGame.IsConnected()){
					_networkGame.ReceiveServerInput();
				}
				clock1.restart();
				ServerSpeed(_networkGame.ServerSpeed());
			//	SetClientPosition(_networkGame.ClientPos());
			}
			else{
				_networkGame.GetInput(GetClientSpeed(), clientBullet, bulletFired, _serverPlayer.IsServerAlive(), _clientPlayer.IsPlayerAlive(), _clientPlayer.currentPosition().x);
				_networkGame.DoStuff();
			}
		
			// check if client is still there or not
			if(!_networkGame.clientAlive()){
				_clientPlayer.PlayerDies();
			}

			if(!_networkGame.serverAlive() || _clientPlayer.getCoins() == COINS){
				_serverPlayer.ServerDies();
			}

			// update server after getting input from network
			//ServerSpeed(_networkGame.ServerSpeed());
			pos.x = _networkGame.ServerPos();
			if(pos.x > 0)
				ServerPosition(pos.x);
			else
				ServerSpeed(_networkGame.ServerSpeed());
			_serverPlayer.update(_mainWindow);

			// Update client
			//SetClientSpeed(_networkGame.ClientSpeed());
			_clientPlayer.update(_mainWindow);

			// Clear the main window, to re-render ---------------------------------
			_mainWindow.clear();

			//serverBullet = _networkGame.BulletsFiredServer();
			elapsed = clock.getElapsedTime();  // elapsed.asSeconds()
			if(_networkGame.ServerFire() && _networkGame.serverTime() > counter){
				pos.x = _networkGame.ServerPos();
				pos.y = _serverPlayer.currentPosition().y;	
				if (_serverPlayer.currentPosition().x != pos.x)
					_serverPlayer.setPosition(pos.x);

				if(pos.x > 5)
					fire(_serverPlayer.currentPosition().x, pos.y, _mainWindow, false);
				counter += 0.85;
			}

			// Draw all the variables -----------------------------------------------
			_serverPlayer.draw(_mainWindow);
			_clientPlayer.draw(_mainWindow);

			// Draw the Coins/Bricks
			for each(Coin c in coinList){
			
				if(c.IsCoinShown() && c.getCoinColor() != sf::Color::Black)
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
        _mainWindow.display();
    }

}

bool GameWindow::IsExiting()
{
   if(_gameState == Exiting) 
     return true;
   else 
     return false;
}

void GameWindow::GameOver(){
	// here show Game Over Image	
	sf::Texture image;
	if(!_clientPlayer.IsPlayerAlive()){
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

float GameWindow::GetClientPosition(){
	sf::Vector2f pos = _clientPlayer.currentPosition();
	return pos.x;
}

void GameWindow::SetClientSpeed(float val){
	_clientPlayer.setSpeed(val, 0);
}

void GameWindow::SetClientPosition(float val){
	_clientPlayer.setPosition(val);
}

void GameWindow::ServerSpeed(float val){
	_serverPlayer.setSpeed(val, 0);
}

void GameWindow::ServerPosition(float val){
	_serverPlayer.setPosition(val);
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
