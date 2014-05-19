// For core module AG1107A - Network Game Development, Networked Game, University of Abertay, Dundee.
// By Richa Sachdeva, 1304869, M.Sc. Computer Games Technology
// code taken from http://www.edparrish.net/cs11/13f/lesson12.php#12.2 and modified to suit the current application 
#include <cmath>
#include "coin.h"
#include "stdafx.h"
#include <stdio.h>

Coin::Coin()
{
	//circ = EllipseShape(sf::Vector2f(10, 30));
	circ = sf::RectangleShape(sf::Vector2f(10, 20));
    circ.setFillColor(sf::Color::Green);
    dx = COIN_SPEED;
    dy = COIN_SPEED;
	coinShown = true;
}

Coin::Coin(float x, float y, float speedx, float speedy, float rad, sf::Color ballColor)
{
   // circ = sf::CircleShape(rad);
	//circ = EllipseShape(sf::Vector2f(10, 20));
	circ = sf::RectangleShape(sf::Vector2f(20, 30));
    circ.setFillColor(ballColor);
    circ.setPosition(x, y);
    dx = speedx;
    dy = speedy;
	radius = rad;
	coinShown = true;
}

Coin::~Coin(){

}

void Coin::update(sf::RenderWindow& win)
{
//	constrain(win);
    circ.move(dx, dy);
}

void Coin::draw(sf::RenderWindow& win)
{
	win.draw(circ);
}

sf::Vector2f Coin::currentPosition(){
	return circ.getPosition();
}

sf::Vector2f Coin::getRad(){
	return circ.getSize();//    circ.getRadius();
}

void Coin::updateColor(sf::Color c){
	circ.setFillColor(c);
}

sf::Color Coin::getCoinColor(){
	return circ.getFillColor();
}

void Coin::NoCoin(){
	circ.setFillColor(sf::Color::Black);
	coinShown = false;
}