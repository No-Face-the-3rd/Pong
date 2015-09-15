#pragma once


struct Player 
{
	int lives;
	float x, y, speed, width, height;
};

struct Ball
{
	float x, y, xVel, yVel, spin, angle, speed;
	int radius;
};

struct PowerUp
{
	float x, y,size, duration, effectDur;
	char effect;
};