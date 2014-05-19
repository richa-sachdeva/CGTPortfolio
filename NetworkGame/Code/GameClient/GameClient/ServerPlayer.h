// For core module AG1107A - Network Game Development, Networked Game, University of Abertay, Dundee.
// By Richa Sachdeva, 1304869, M.Sc. Computer Games Technology

#include "stdafx.h"
#include "SFML/Graphics.hpp"
#include "list"
#ifndef SERVERPLAYER_H
#define SERVERPLAYER_H

const float SERVER_RADIUS = 10.0f;
const int SERVER_Y = 10;

class ServerPlayer {

public:
	ServerPlayer();
	//ServerPlayer(float x, float y, float speedx, float speedy, float rad, sf::Color ballColor);

	~ServerPlayer();
   
    void update(sf::RenderWindow& win);
    void draw(sf::RenderWindow& win);

	
	sf::Vector2f currentPosition();
	void setPosition(float x);

	float getRad();
	float getDX(){ return dx;}
	float getDY(){ return dy;}
	void setSpeed(float x, float y);
	
	bool IsServerAlive(){return alive;}
	void ServerDies();

private:
	void constrain(sf::RenderWindow& win);

    sf::CircleShape circ;
    float radius, dx, dy;

	bool alive;
};

#endif