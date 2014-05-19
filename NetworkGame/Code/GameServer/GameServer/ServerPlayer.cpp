// For core module AG1107A - Network Game Development, Networked Game, University of Abertay, Dundee.
// By Richa Sachdeva, 1304869, M.Sc. Computer Games Technology
#include "stdafx.h"
#include <cmath>
#include <stdio.h>
#include "ServerPlayer.h"

ServerPlayer::ServerPlayer()
{
	circ = sf::CircleShape(SERVER_RADIUS, 4);
	circ.setFillColor(sf::Color::Red);
    circ.setPosition(SCREEN_WIDTH/2 - 10, 10);
    dx = 0.0;//25;
    dy = 0;	
	alive = true;
}

ServerPlayer::~ServerPlayer(){

}

void ServerPlayer::update(sf::RenderWindow& win)
{
	constrain(win);
    circ.move(dx, dy);
}

void ServerPlayer::draw(sf::RenderWindow& win)
{
	win.draw(circ);
}

void ServerPlayer::constrain(sf::RenderWindow& win){
	
	sf::Vector2u winSize = win.getSize();
    sf::Vector2f pos = circ.getPosition();

    if (pos.x < 0)
    {
        dx = std::abs(dx);
    }
    else if (pos.x + SERVER_RADIUS * 2 > winSize.x)
    {
        dx = -std::abs(dx);
    }
	 if (pos.y < 0)
    {
        dy = std::abs(dy);
    }
    else if (pos.y + SERVER_RADIUS * 2 > winSize.y)
    {
        dy = -std::abs(dy);
    }
}

sf::Vector2f ServerPlayer::currentPosition(){
	return circ.getPosition();
}

void ServerPlayer::setCurrentPosition(float val){
	circ.setPosition(val, 10);
}

float ServerPlayer::getRad(){
	return circ.getRadius();
}

void ServerPlayer::setSpeed(float x, float y){
	dx = x;
	dy = 0;
}

void ServerPlayer::ServerDies(){
	alive = false;
	circ.setFillColor(sf::Color::Black);
}

