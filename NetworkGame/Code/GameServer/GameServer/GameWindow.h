// For core module AG1107A - Network Game Development, Networked Game, University of Abertay, Dundee.
// By Richa Sachdeva, 1304869, M.Sc. Computer Games Technology
#pragma once
#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"

// include Game objects -> serverPlayer, clientPlayer, coins, fires, etc etc

#include "ServerPlayer.h"
#include "ClientPlayer.h"
#include "GameNetwork.h"
#include "Coin.h"
#include "Bullet.h"

static const int SCREEN_WIDTH = 700;
static const int SCREEN_HEIGHT = 600;
const int NUM_BULLETS = 10;
const int SERVER_LIMIT = 5;
const int PLAYER_LIMIT = 5;	// limit on the bullet

class GameWindow 
{
public:
	GameWindow();
	~GameWindow();

	void Start();
	void StartNetworking();
	static sf::RenderWindow& GetWindow();
	const static sf::Event& GetInput();
	
	int GetScreenWidth();
	int GetScreenHeight();

	void UpdateGameWindow();

	float GetClientSpeed();
	float GetClientPosition();

	void SetClientSpeed(float val);
	void SetClientPosition(float val);
	
	float GetServerSpeed();
	float GetServerPosition();

	void SetServerSpeed(float val);
	void SetServerPosition(float val);

private:
	bool IsExiting();
	void GameLoop();

	void ShowStartScreen();

	void ShowWindow();
	void ClearWindow();
	void DrawWindow();

	void GameOver();

	// To fire
	void fire(float, float, sf::RenderWindow& win, bool);
	bool TestForBullets(Coin& object1);
	bool circleTest(Coin& object1, Bullet& object2);
	bool playerTest(ClientPlayer& player, Bullet& obj);
	bool serverTest(ServerPlayer& server, Bullet& obj);
	// Private Variables

	enum GameState { Uninitialized, ShowingStartUpScreen, Paused, 
					ShowingMenu, Playing, Exiting };

	static GameState _gameState;
	static sf::RenderWindow _mainWindow;

	ServerPlayer _serverPlayer;
	ClientPlayer _clientPlayer;

	GameNetwork _networkGame;
	sf::Thread* thread;

	std::vector<Coin> coinList; 

	// bullet for server and client
	std::vector<Bullet> bulletList;
	unsigned int serverBullet, clientBullet;
	std::vector<bool> showBullet;


};