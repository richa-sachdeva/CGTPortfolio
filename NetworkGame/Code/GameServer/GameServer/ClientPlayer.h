// For core module AG1107A - Network Game Development, Networked Game, University of Abertay, Dundee.
// By Richa Sachdeva, 1304869, M.Sc. Computer Games Technology
#include "stdafx.h"
#include <SFML/Graphics.hpp>
#ifndef CLIENTPLAYER_H
#define CLIENTPLAYER_H

const float CLIENT_RADIUS = 10.f;
const float MAX_VELOCITY = 4.0f;

class ClientPlayer
{
public:
    ClientPlayer();
    ClientPlayer(float x, float y, float speedx, float speedy, float radius, sf::Color ballColor);
	~ClientPlayer();
    void update(sf::RenderWindow& win);
    void draw(sf::RenderWindow& win);


	void setSpeed(float x, float y);
	void setPosition(float x);

	sf::Vector2f currentPosition();
	float getRad();

	void updateColor(sf::Color c);

	float getDX(){ return dx;}
	
	bool IsPlayerAlive(){return alive;}
	void PlayerDies();

	int getCoins(){ return coins;}
	void increaseCoinCount(int val){ coins += val;}

private:
	void constrain(sf::RenderWindow& win);

    sf::CircleShape circ;
	sf::CircleShape bullet;
    float radius, dx, dy;
	int coins;
	bool alive;
};
#endif