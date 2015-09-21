#pragma once


struct Player 
{
	int lives;
	float x, y, speed, width, height;
	bool isPlayer;
	int aiTime,acc;
};

struct Ball
{
	float x, y, xVel, yVel, spin, angle, speed;
	int radius;
	const float defX, defY;
};

struct PowerUp
{
	float x, y,size, duration, effectDur;
	char effect;
	bool onScreen, inEffect;
	int affecting;
};