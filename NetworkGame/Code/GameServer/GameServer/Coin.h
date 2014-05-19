// For core module AG1107A - Network Game Development, Networked Game, University of Abertay, Dundee.
// By Richa Sachdeva, 1304869, M.Sc. Computer Games Technology
#include "stdafx.h"
#include <SFML/Graphics.hpp>

#ifndef COIN_H
#define COIN_H

const int COINS = 20;
const float COINRAD = 15.0f;
const float COIN_SPEED = .025f;

class Coin
{
public:
    Coin();
    Coin(float x, float y, float speedx, float speedy, float radius, sf::Color ballColor);
	~Coin();
    void update(sf::RenderWindow& win);
    void draw(sf::RenderWindow& win);

	sf::Vector2f currentPosition();
	sf::Vector2f getRad();

	void updateColor(sf::Color c);
	sf::Color getCoinColor();

	void setCoinShow(bool val){ coinShown = val;}
	bool IsCoinShown(){ return coinShown;}

	void NoCoin();

private:
	//void constrain(sf::RenderWindow& win);

	//EllipseShape circ;
	sf::RectangleShape circ;
    float radius, dx, dy;
	bool coinShown;
};
#endif