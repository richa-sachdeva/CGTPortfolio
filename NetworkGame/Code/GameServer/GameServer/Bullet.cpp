// For core module AG1107A - Network Game Development, Networked Game, University of Abertay, Dundee.
// By Richa Sachdeva, 1304869, M.Sc. Computer Games Technology
// code taken from http://www.edparrish.net/cs11/13f/lesson12.php#12.2 and modified to suit the current application 
#include <cmath>
#include "bullet.h"
#include "stdafx.h"
#include <stdio.h>

Bullet::Bullet()
{
//    circ = sf::CircleShape(BULLET_RADIUS);
	circ = sf::RectangleShape(sf::Vector2f(5, 15));
	circ.setFillColor(sf::Color::Yellow);
	circ.setPosition(0, 0);
    dx = 0;
    dy = BULLET_SPEED;
	shown = true;
}

Bullet::~Bullet(){

}

Bullet::Bullet(float x, float y){
	
	circ = sf::RectangleShape(sf::Vector2f(5, 15));
	circ.setFillColor(sf::Color::Cyan);
	circ.setPosition(x, y);
    dx = 0;
    dy = BULLET_SPEED;
	shown = false;
	playerType = false;
}

void Bullet::setPosition(float x, float y){
	circ.setPosition(x, y);
}

void Bullet::setSpeed(float x, float y){
	dx = x;
	dy = y;
}

void Bullet::update(sf::RenderWindow& win)
{
	constrain(win);
    circ.move(dx, dy);
}

void Bullet::draw(sf::RenderWindow& win)
{
	win.draw(circ);
}

void Bullet::destroy(){
	//this->d Bullet();
}

void Bullet::constrain(sf::RenderWindow& win){
	
	sf::Vector2u winSize = win.getSize();
    sf::Vector2f pos = circ.getPosition();

	if (pos.y < 0)
    {
        dy = std::abs(dy);
		circ.setFillColor(sf::Color::Black);
		setSpeed(0, 0);
		shown = false;
    }
    else if (pos.y + BULLET_RADIUS * 2 > winSize.y)
    {
        dy = -std::abs(dy);
		circ.setFillColor(sf::Color::Black);
		setSpeed(0, 0);
		shown = false;
    }
}

sf::Vector2f Bullet::currentPosition(){
	return circ.getPosition();
}

sf::Vector2f Bullet::getRad(){
	return circ.getSize();
}

sf::Color Bullet::getColor(){
	return circ.getFillColor();
}

void Bullet::updateColor(sf::Color c){
	shown = true;
	circ.setFillColor(c);
}