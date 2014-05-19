// For core module AG1107A - Network Game Development, Networked Game, University of Abertay, Dundee.
// By Richa Sachdeva, 1304869, M.Sc. Computer Games Technology
// code taken from http://www.edparrish.net/cs11/13f/lesson12.php#12.2 and modified to suit the current application 
#include <cmath>
#include "ClientPlayer.h"
#include "stdafx.h"
#include <stdio.h>

ClientPlayer::ClientPlayer()
{
    const float SPEED = 0.0f;
    circ = sf::CircleShape(CLIENT_RADIUS);
    circ.setFillColor(sf::Color::Green);
	circ.setPosition(SCREEN_WIDTH/2, SCREEN_HEIGHT - 3*CLIENT_RADIUS);
    dx = SPEED;
    dy = SPEED;
	alive = true;
	coins = 0;
}

ClientPlayer::ClientPlayer(float x, float y, float speedx, float speedy, float rad, sf::Color ballColor)
{
    circ = sf::CircleShape(rad);
    circ.setFillColor(ballColor);
    circ.setPosition(x, y);
    dx = speedx;
    dy = speedy;
	radius = rad;
	alive = true;
}

ClientPlayer::~ClientPlayer(){

}

void ClientPlayer::setSpeed(float x, float y){
	if(dx != x)
		dx = x;
	if(dy != y)
		dy = y;
}

void ClientPlayer::setPosition(float x){
	circ.setPosition(x, SCREEN_HEIGHT - 3*CLIENT_RADIUS);
}

void ClientPlayer::update(sf::RenderWindow& win)
{
	constrain(win);
    circ.move(dx, dy);
}

void ClientPlayer::draw(sf::RenderWindow& win)
{
	win.draw(circ);
}


void ClientPlayer::constrain(sf::RenderWindow& win){
	
	sf::Vector2u winSize = win.getSize();
    sf::Vector2f pos = circ.getPosition();

    if (pos.x < 0)
    {
        dx = std::abs(dx);
    }
    else if (pos.x + CLIENT_RADIUS * 2 > winSize.x)
    {
        dx = -std::abs(dx);
    }
	 if (pos.y < 0)
    {
        dy = std::abs(dy);
    }
    else if (pos.y + CLIENT_RADIUS * 2 > winSize.y)
    {
        dy = -std::abs(dy);
    }
}

sf::Vector2f ClientPlayer::currentPosition(){
	return circ.getPosition();
}

float ClientPlayer::getRad(){
	return circ.getRadius();
}

void ClientPlayer::updateColor(sf::Color c){
	circ.setFillColor(c);
}

void ClientPlayer::PlayerDies(){
	alive = false;
	circ.setFillColor(sf::Color::Black);
}