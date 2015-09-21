#include "Header.h"
#include "sfwdraw.h"
#include <cmath>
#include <time.h>

void drawMenu(float screenH,float screenW, Player &selector,PowerUp pTypes[]);
void drawChar(char test, float x, float y, float size);
void drawPaddle(Player player);
void drawBall(Ball &ball);
void updatePlayer(Player &player, int playerNum, int screenH, int screenW, Ball ball);
void updateSelector(Player &selector,float loc[],int &timer);
void drawPowerup(PowerUp pUp);
void resetPowerUps(PowerUp pUp[],int size);
void resetPowerUps(PowerUp pUp);
void initPowerUpTypes(PowerUp pUp[], int size);
void updatePUp(PowerUp pUp);
void launchBall(Ball &ball, Player &player1, Player &player2, PowerUp pUp[]);
void startGame(Player p1, Player p2);


int main()
{
	int screenW = 1000, screenH = 700, lives = 3, dfRadius = 20, listSize = 0, p1Del = 0, p2Del = 0, pUpDel = 0;
	float dfWidth = 20.0f, dfHeight = 80.0f, dfPspeed = (float)screenH/120.0f, spinMulti = 150.0f,speedMin = 100.0f,speedMax = 200.0f;
	
	
	Ball ball = {(float)screenW/2.0f, (float)screenH/2.0f, 0.0f, 0.0f, 0.30f, 0.0f,0.0f, dfRadius,(float)screenW/2.0f,(float)screenH/2.0f};
	srand(time(NULL));
	Player selector{ 0,screenW / 2.0f + 4.0f*(screenH / 40.0f + 5.0f),screenH / 2.0f,0.0f,15.0f,20.0f,false,0,0 };
	
	PowerUp powerUps[5];
	initPowerUpTypes(powerUps,5);
	
	PowerUp pList[6];
	resetPowerUps(pList,6);
	

	sfw::initContext(screenW, screenH, "Wheel Pong");
	Player player1{ lives,dfWidth/1.25f,(float)screenH / 2.0f,dfPspeed,dfWidth,dfHeight,true,0,0 };
	Player player2{ lives,(float)screenW - dfWidth/1.25f,(float)screenH / 2.0f ,dfPspeed,dfWidth,dfHeight,true,0,0 };
	

	drawMenu((float)screenH,(float)screenW,selector,powerUps);

	while (sfw::stepContext())
	{
		drawPaddle(player1);
		drawPaddle(player2);
		drawBall(ball);

		for (int i = 0; i < 6; i++)
		{
			drawPowerup(pList[i]);
		}


		if (player1.lives > 0 && player2.lives > 0)
		{
			ball.y += ball.yVel * sfw::getDeltaTime();
			ball.x += ball.xVel * sfw::getDeltaTime();
			ball.speed = sqrt(pow(ball.xVel, 2) + pow(ball.yVel, 2));

			updatePlayer(player1, 1, screenH, screenW, ball);
			updatePlayer(player2, 2, screenH, screenW, ball);
			
			p1Del--;
			p2Del--;
			if (sfw::getKey('a')&&p1Del <= 0) 
			{
				player1.isPlayer = !player1.isPlayer;
				p1Del = 20;
			}
			if (sfw::getKey('l') && p2Del <= 0)
			{
				player2.isPlayer = !player2.isPlayer;
				p2Del = 20;
			}

			for (int i = 0; i < 6; i++)
			{
				drawPowerup(pList[i]);
				updatePUp(pList[i]);
			}

			if (ball.speed < speedMin)
			{
				if (ball.xVel < 0)
					ball.xVel -= 25;
				else
					ball.xVel += 25;
				if (ball.yVel < 0)
					ball.yVel -= 15;
				else
					ball.yVel += 15;
			}
			if (ball.speed > speedMax)
			{
				if (ball.xVel < 0)
					ball.xVel += 20;
				else
					ball.xVel -= 20;
				if (ball.yVel < 0)
					ball.yVel += 20;
				else
					ball.yVel -= 20;
			}

			if (ball.y > (float)screenH - ball.radius - 5.0f)
			{
				ball.y = (float)screenH - ball.radius - 10.0f;
				ball.xVel = ball.xVel + -spinMulti*ball.spin;
				ball.yVel = -ball.yVel + abs(spinMulti / 2.0f*ball.spin);
				//ball.yVel *= -1;
			}
			if (ball.y < ball.radius + 5.0f)
			{
				ball.y = ball.radius + 10.0f;
				ball.xVel = ball.xVel + spinMulti*ball.spin;
				ball.yVel = -ball.yVel - abs(spinMulti / 2.0f*ball.spin);

				//ball.yVel *= -1;

			}
			if (ball.x > (float)screenW - ball.radius)
			{
				player2.lives--;
				launchBall(ball, player1, player2, pList);

			}
			if (ball.x < ball.radius)
			{
				player1.lives--;
				launchBall(ball, player1, player2, pList);

			}
			if (ball.x <= player1.x + player1.width / 2.0f + ball.radius)
			{
				if (ball.y >= player1.y + player1.height / 4.0f && ball.y <= player1.y + player1.height / 2.0f)
				{
					if (ball.spin > 0)
						ball.spin *= -1;
				}
				if (ball.y >= player1.y - player1.height / 2.0f && ball.y <= player1.y - player1.height / 4.0f)
				{
					if (ball.spin < 0)
						ball.spin *= -1;
				}
				if (ball.y >= player1.y - player1.height / 2.0f && ball.y <= player1.y + player1.height / 2.0f)
				{
					ball.x = player1.x + player1.width / 2.0f + ball.radius;
					ball.yVel = ball.yVel + -spinMulti*ball.spin;
					ball.xVel = -ball.xVel + abs(spinMulti / 2.0f*ball.spin);
				}
				if (sqrt(pow(ball.x - (player1.x), 2) + pow(ball.y - (player1.y + player1.height / 2.0f), 2)) <= player1.width / 2.0f + ball.radius)
				{
					ball.xVel = 10;
					ball.yVel = 80;
					if (ball.spin > 0)
						ball.spin *= -1;
				}
				if (sqrt(pow(ball.x - (player1.x), 2) + pow(ball.y - (player1.y - player1.height / 2.0f), 2)) <= player1.width / 2.0f + ball.radius)
				{
					ball.xVel = 10;
					ball.yVel = -80;
					if (ball.spin < 0)
						ball.spin *= -1;
				}
			}
			if (ball.x >= player2.x - player2.width / 2.0f - ball.radius)
			{
				if (ball.y >= player2.y + player2.height / 4.0f && ball.y <= player2.y + player2.height / 2.0f)
				{
					if (ball.spin < 0)
						ball.spin *= -1;
				}
				if (ball.y >= player2.y - player2.height / 2.0f && ball.y <= player2.y - player2.height / 4.0f)
				{
					if (ball.spin > 0)
						ball.spin *= -1;
				}
				if (ball.y >= player2.y - player2.height / 2.0f && ball.y <= player2.y + player2.height / 2.0f)
				{
					ball.x = player2.x - player2.width / 2.0f - ball.radius;
					ball.yVel = ball.yVel + spinMulti*ball.spin;
					ball.xVel = -ball.xVel - abs(spinMulti / 2.0f*ball.spin);
				}
				if (sqrt(pow(ball.x - (player2.x), 2) + pow(ball.y - (player2.y + player2.height / 2.0f), 2)) <= player2.width / 2.0f + ball.radius)
				{
					ball.xVel = -10;
					ball.yVel = 80;
					if (ball.spin < 0)
						ball.spin *= -1;
				}
				if (sqrt(pow(ball.x - (player2.x), 2) + pow(ball.y - (player2.y - player2.height / 2.0f), 2)) <= player2.width / 2.0f + ball.radius)
				{
					ball.xVel = -10;
					ball.yVel = -80;
					if (ball.spin > 0)
						ball.spin *= -1;
				}
			}

		}

		if (player1.lives <= 0 || player2.lives <= 0)
		{
			if (sfw::getKey(' '))
				drawMenu(screenH, screenW, selector, powerUps);
		}
		if (selector.lives == 2)
			break;
		if (selector.acc == 1)
		{
			if (selector.lives == 0)
			{
				startGame(player1, player2);
				player2.isPlayer = false;
				launchBall(ball, player1, player2, pList[]);
				selector.acc = 0;
			}
			if (selector.lives == 1)
			{
				startGame(player1, player2);
				player2.isPlayer = true;
				launchBall(ball, player1, player2, pList[]);
				selector.acc = 0;
			}
		}

	}

	return 0;
}

void startGame(Player p1, Player p2)
{
	p1.lives = 3;
	p2.lives = 3;
	p1.isPlayer = true;
}

void launchBall(Ball &ball, Player &player1, Player &player2, PowerUp pUp[])
{
	ball.x = ball.defX;
	ball.y = ball.defY;
	ball.xVel = rand() % 140 - 70;
	ball.yVel = rand() % 140 - 70;
	if (abs(ball.xVel) < 20)
		if (ball.xVel < 0)
			ball.xVel -= 30;
		else
			ball.xVel += 30;
	player1.y = ball.defY;
	player2.y = ball.defY;
	for (int i = 0; i < 6; i++)
	{
		resetPowerUps(pUp[i]);
		updatePUp(pUp[i]);
	}

}

void updatePUp(PowerUp pUp)
{
	pUp.duration--;
	if (pUp.duration <= 0)
	{
		pUp.onScreen = false;
	}
	if (pUp.inEffect)
	{
		pUp.effectDur--;
		if (pUp.effectDur <= 0)
		{
			pUp.inEffect = false;

		}
	}
}

void initPowerUpTypes(PowerUp pUp[], int size)
{
	pUp[0].effect = 'I';
	pUp[1].effect = 'B';
	pUp[2].effect = 'L';
	pUp[3].effect = 'S';
	pUp[4].effect = 'F';
	for (int i = 0; i < size; i++)
	{
		pUp[i].duration = 1000;
		pUp[i].effectDur = 500;
		pUp[i].x = -pUp[i].size;
		pUp[i].y = -pUp[i].size;
		pUp[i].onScreen = false;
		pUp[i].inEffect = false;
		pUp[i].affecting = 0;
	}
}

void resetPowerUps(PowerUp pUp)
{
	pUp.duration = 0;
	pUp.effectDur = 0;
	pUp.onScreen = false;
}

void resetPowerUps(PowerUp pUp[],int size)
{
	for (int i = 0; i < size; i++)
	{
		pUp[i].x = 0;
		pUp[i].y = 0;
		pUp[i].size = 0;
		pUp[i].duration = 1000;
		pUp[i].effectDur = 500;
		pUp[i].onScreen = false;
		pUp[i].inEffect = false;
		pUp[i].affecting = 0;
	}


	return;
}

void drawPowerup(PowerUp pUp)
{
	if (pUp.onScreen)
	{
		sfw::drawCircle(pUp.x, pUp.y, pUp.size);
		drawChar(pUp.effect, pUp.x, pUp.y, pUp.size*3.0f / 4.0f);
	}
}

void updateSelector(Player &selector,float loc[3],int &timer)
{
	if (sfw::getKey('w') && timer <= 0)
	{
		if (selector.lives <= 0)
			selector.lives = 2;
		else
			--selector.lives;
		timer = 200;

	}
	if (sfw::getKey('s') && timer <= 0)
	{
		if (selector.lives >= 2)
			selector.lives = 0;
		else
			++selector.lives;
		timer = 200;
	}
	timer--;
	selector.y = loc[selector.lives];
	return;
}

void updatePlayer(Player &player, int playerNum, int screenH,int screenW,Ball ball)
{
	int trajectory = -1;
	if (player.isPlayer)
	{
		if (sfw::getKey('s') && playerNum == 1) player.y += player.speed;
		if (sfw::getKey('w') && playerNum == 1) player.y -= player.speed;
		if (sfw::getKey('k') && playerNum == 2) player.y += player.speed;
		if (sfw::getKey('i') && playerNum == 2) player.y -= player.speed;
	}
	else
	{
		if ((playerNum == 1 && ball.xVel < 0 || playerNum == 2 && ball.xVel > 0))
		{
			if (playerNum == 1)
			{
				trajectory = ball.y + abs(( ball.x) / ball.xVel)*ball.yVel + 2*player.height * player.acc;
				if (trajectory > screenH)
					trajectory = screenH + (abs(( ball.x) / ball.xVel) - abs((screenH - ball.y) / ball.yVel))*-ball.yVel + 2 * player.height * player.acc;
				else if (trajectory < 0)
					trajectory = (abs(( ball.x) / ball.xVel) - abs((ball.y) / ball.yVel))*-ball.yVel + 2 * player.height * player.acc;
			}
			if (playerNum == 2)
			{
				trajectory = ball.y + abs((screenW -ball.x) / ball.xVel)*ball.yVel + 2 * player.height * player.acc;
				if (trajectory > screenH)
					trajectory = screenH + (abs((screenW -ball.x) / ball.xVel) - abs((screenH - ball.y) / ball.yVel))*-ball.yVel + 2 * player.height * player.acc;
				else if (trajectory < 0)
					trajectory = (abs((screenW -ball.x) / ball.xVel) - abs((ball.y) / ball.yVel))*-ball.yVel + 2 * player.height * player.acc;
			}
		}



		player.aiTime-=3;
		if ((playerNum == 1 && ball.xVel > 0) || (playerNum == 2 && ball.xVel < 0))
		{
			trajectory = screenH / 2.0f;
			player.acc = 0;
		}
		if (trajectory > player.y && player.aiTime <= 0)
		{
			player.y += player.speed;
			player.aiTime += 5;
			if (player.aiTime == 5)
				player.acc = rand() % 3 - 1;
		}
		if (trajectory < player.y && player.aiTime <= 0)
		{
			player.y -= player.speed;
			player.aiTime += 5;
			if (player.aiTime == 5)
				player.acc = rand() % 3 - 1;
		}
		trajectory = -1;
		
	}
	if (player.y > (float)screenH - player.height / 2.0f - 15.0f)
		player.y = (float)screenH - player.height / 2.0f - 15.0f;
	if (player.y < player.height / 2.0f + 15.0f)
		player.y = player.height / 2.0f + 15.0f;


}

void drawBall(Ball &ball)
{
	sfw::drawCircle(ball.x, ball.y, ball.radius);
	sfw::drawLine(ball.x + sin(ball.angle * PI / 180) * ball.radius, ball.y + cos(ball.angle * PI / 180) * ball.radius, ball.x + sin((ball.angle + 180) * PI / 180) * ball.radius, ball.y + cos((ball.angle + 180) * PI / 180) * ball.radius);
	sfw::drawLine(ball.x + sin((ball.angle + 45) * PI / 180) * ball.radius, ball.y + cos((ball.angle + 45) * PI / 180) * ball.radius, ball.x + sin((ball.angle + 225) * PI / 180) * ball.radius, ball.y + cos((ball.angle + 225) * PI / 180) * ball.radius);
	sfw::drawLine(ball.x + sin((ball.angle + 90) * PI / 180) * ball.radius, ball.y + cos((ball.angle + 90) * PI / 180) * ball.radius, ball.x + sin((ball.angle + 270) * PI / 180) * ball.radius, ball.y + cos((ball.angle + 270) * PI / 180) * ball.radius);
	sfw::drawLine(ball.x + sin((ball.angle + 135) * PI / 180) * ball.radius, ball.y + cos((ball.angle + 135) * PI / 180) * ball.radius, ball.x + sin((ball.angle + 315) * PI / 180) * ball.radius, ball.y + cos((ball.angle + 315) * PI / 180) * ball.radius);
	ball.angle += 10*ball.spin;

	return;
}

void drawPaddle(Player player)
{
	sfw::drawLine(player.x + player.width / 2.0f, player.y + player.height / 2.0f, player.x + player.width / 2.0f, player.y - player.height / 2.0f);
	sfw::drawLine(player.x - player.width / 2.0f, player.y + player.height / 2.0f, player.x - player.width / 2.0f, player.y - player.height / 2.0f);
	for (int i = 0; i < 180; i+=10)
	{
		sfw::drawLine(player.x + cos(i*PI / 180) * (player.width / 2.0f), player.y + sin(i*PI/180)*(player.width/2.0f) + player.height/2.0f, player.x + cos((i+10)*PI/180)*(player.width / 2.0f), player.y + sin((i+10)*PI/180)*(player.width/2.0f) + player.height/2.0f);
		sfw::drawLine(player.x + cos(i*PI / 180) * (player.width / 2.0f), player.y - sin(i*PI/180)*(player.width/2.0f) - player.height/2.0f, player.x + cos((i+10)*PI/180)*(player.width / 2.0f), player.y - sin((i+10)*PI/180)*(player.width/2.0f) - player.height/2.0f);
	}
	if(player.speed >0)
		drawChar(player.lives, player.x, player.y, player.width*7.0f/8.0f);
}

void drawChar(char test, float x, float y, float size)
{
	const float toRad = PI / 180;
	switch (test)
	{
	case 'a':
		sfw::drawLine(x + size / 4.0f, y + size / 2.0f, x + size / 4.0f, y + size / 8.0f);
		sfw::drawLine(x, y + size / 4.0f, x + size / 4.0f, y + size / 4.0f);
		sfw::drawLine(x, y + size / 16.0f * 7.0f, x + size / 4.0f, y + size / 16.0f * 7.0f);
		for (int i = 0; i < 180; i += 10)
		{
			sfw::drawLine(x + cos(i* toRad)*(size / 4.0f), y + (size / 8.0f) - sin(i*toRad)*(size / 8.0f), x + cos((i + 10)*toRad)*(size / 4.0f), y + (size / 8.0f) - sin((i + 10)*toRad)*(size / 8.0f));
			sfw::drawLine(x + cos((i + 90)*toRad)*(size / 4.0f), y + (size / 32.0f*11.0f) - sin((i + 90)*toRad)*(size / 32.0f*3.0f), x + cos((i + 100)*toRad)*(size / 4.0f), y + (size / 32.0f*11.0f) - sin((i + 100)*toRad)*(size / 32.0f*3.0f));
		}
		break;
	case 'A':
		sfw::drawLine(x - size / 4.0f, y + size / 2.0f, x, y - size / 2.0f);
		sfw::drawLine(x + size / 4.0f, y + size / 2.0f, x, y - size / 2.0f);
		sfw::drawLine(x - size / 8.0f, y, x + size / 8.0f, y);
		break;
	case 'b':
		sfw::drawLine(x - size / 4.0f, y - size / 2.0f, x - size / 4.0f, y + size / 2.0f);
		sfw::drawLine(x - size / 4.0f, y, x, y);
		sfw::drawLine(x - size / 4.0f, y + size / 2.0f, x, y + size / 2.0f);
		for (int i = 0; i < 180; i += 10)
			sfw::drawLine(x + cos((i - 90)*toRad)*size / 4.0f, y + size / 4.0f + sin((i - 90)*toRad)*size / 4.0f, x + cos((i - 80)*toRad)*size / 4.0f, y + size / 4.0f + sin((i - 80)*toRad)*size / 4.0f);
		break;
	case 'B':
		sfw::drawLine(x - size / 4.0f, y - size / 2.0f, x - size / 4.0f, y + size / 2.0f);
		sfw::drawLine(x - size / 4.0f, y - size / 2.0f, x, y - size / 2.0f);
		sfw::drawLine(x - size / 4.0f, y, x, y);
		sfw::drawLine(x - size / 4.0f, y + size / 2.0f, x, y + size / 2.0f);
		for (int i = 0; i < 180; i += 10)
		{
			sfw::drawLine(x + cos((i - 90)*toRad)*size / 4.0f, y + size / 4.0f + sin((i - 90)*toRad)*size / 4.0f, x + cos((i - 80)*toRad)*size / 4.0f, y + size / 4.0f + sin((i - 80)*toRad)*size / 4.0f);
			sfw::drawLine(x + cos((i - 90)*toRad)*size / 4.0f, y - size / 4.0f + sin((i - 90)*toRad)*size / 4.0f, x + cos((i - 80)*toRad)*size / 4.0f, y - size / 4.0f + sin((i - 80)*toRad)*size / 4.0f);
		}
		break;
	case 'c':
		sfw::drawLine(x, y, x + size / 4.0f, y);
		sfw::drawLine(x, y + size / 2.0f, x + size / 4.0f, y + size / 2.0f);
		for (int i = 0; i < 180; i += 10)
			sfw::drawLine(x + cos((i + 90)*toRad)*size / 4.0f, y + size / 4.0f + sin((i + 90)*toRad)*size / 4.0f, x + cos((i + 100)*toRad)*size / 4.0f, y + size / 4.0f + sin((i + 100)*toRad)*size / 4.0f);
		break;
	case 'C':
		sfw::drawLine(x, y - size / 2.0f, x + size / 4.0f, y - size / 2.0f);
		sfw::drawLine(x, y + size / 2.0f, x + size / 4.0f, y + size / 2.0f);
		for (int i = 0; i < 180; i += 10)
			sfw::drawLine(x + cos((i + 90)*toRad)*size / 4.0f, y + sin((i + 90)*toRad)*size / 2.0f, x + cos((i + 100)*toRad)*size / 4.0f, y + sin((i + 100)*toRad)*size / 2.0f);
		break;
	case 'd':
		sfw::drawLine(x + size / 4.0f, y - size / 2.0f, x + size / 4.0f, y + size / 2.0f);
		sfw::drawLine(x, y, x + size / 4.0f, y);
		for (int i = 0; i < 270; i += 10)
			sfw::drawLine(x + cos((i + 90)*toRad)*size / 4.0f, y + size / 4.0f - sin((i + 90)*toRad)*size / 4.0f, x + cos((i + 100)*toRad)*size / 4.0f, y + size / 4.0f - sin((i + 100)*toRad)*size / 4.0f);
		break;
	case 'D':
		sfw::drawLine(x - size / 4.0f, y - size / 2.0f, x - size / 4.0f, y + size / 2.0f);
		sfw::drawLine(x - size / 4.0f, y - size / 2.0f, x, y - size / 2.0f);
		sfw::drawLine(x - size / 4.0f, y + size / 2.0f, x, y + size / 2.0f);
		for (int i = 0; i < 180; i += 10)
			sfw::drawLine(x + cos((i - 90)*toRad)*size / 4.0f, y - sin((i - 90)*toRad)*size / 2.0f, x + cos((i - 80)*toRad)*size / 4.0f, y - sin((i - 80)*toRad)*size / 2.0f);
		break;
	case 'e':
		sfw::drawLine(x - size / 4.0f, y + size / 4.0f, x + size / 4.0f, y + size / 4.0f);
		sfw::drawLine(x, y + size / 2.0f, x + size / 4.0f, y + size / 2.0f);
		for (int i = 0; i < 270; i += 10)
			sfw::drawLine(x + cos(i*toRad)*size / 4.0f, y + size / 4.0f - sin(i*toRad)*size / 4.0f, x + cos((i + 10)*toRad)*size / 4.0f, y + size / 4.0f - sin((i + 10)*toRad)*size / 4.0f);
		break;
	case 'E':
		sfw::drawLine(x - size / 4.0f, y - size / 2.0f, x - size / 4.0f, y + size / 2.0f);
		sfw::drawLine(x - size / 4.0f, y - size / 2.0f, x + size / 4.0f, y - size / 2.0f);
		sfw::drawLine(x - size / 4.0f, y, x, y);
		sfw::drawLine(x - size / 4.0f, y + size / 2.0f, x + size / 4.0f, y + size / 2.0f);
		break;
	case 'f':
		sfw::drawLine(x, y - size / 4.0f, x, y + size / 2.0f);
		sfw::drawLine(x - size / 4.0f, y, x + size / 4.0f, y);
		for (int i = 0; i < 180; i += 10)
			sfw::drawLine(x + size / 8.0f + cos(i*toRad)*size / 8.0f, y - size / 4.0f - sin(i *toRad)*size / 4.0f, x + size / 8.0f + cos((i + 10)*toRad)*size / 8.0f, y - size / 4.0f - sin((i + 10)*toRad)*size / 4.0f);
		break;
	case 'F':
		sfw::drawLine(x - size / 4.0f, y - size / 2.0f, x - size / 4.0f, y + size / 2.0f);
		sfw::drawLine(x - size / 4.0f, y - size / 2.0f, x + size / 4.0f, y - size / 2.0f);
		sfw::drawLine(x - size / 4.0f, y, x, y);
		break;
	case 'g':
		sfw::drawLine(x + size / 4.0f, y + size / 4.0f, x + size / 4.0f, y + size*3.0f / 4.0f);
		sfw::drawCircle(x, y + size / 4.0f, size / 4.0f);
		for (int i = 0; i < 180; i += 10)
			sfw::drawLine(x+cos(i*toRad)*size/4.0f,y+size*3.0f/4.0f + sin(i*toRad)*size/4.0f, x + cos((i+10)*toRad)*size / 4.0f, y + size*3.0f / 4.0f + sin((i + 10)*toRad)*size / 4.0f);
		break;
	case 'G':
		sfw::drawLine(x, y - size / 2.0f, x + size / 4.0f, y - size / 2.0f);
		sfw::drawLine(x, y, x + size / 4.0f, y);
		sfw::drawLine(x, y + size / 2.0f, x + size / 4.0f, y + size / 2.0f);
		sfw::drawLine(x + size / 4.0f, y, x + size / 4.0f, y + size / 2.0f);
		for (int i = 0; i < 180; i += 10)
			sfw::drawLine(x + cos((i + 90)*toRad)*size / 4.0f, y - sin((i + 90)*toRad)*size / 2.0f, x + cos((i + 100)*toRad)*size / 4.0f, y - sin((i + 100)*toRad)*size / 2.0f);
		break;
	case 'h':
		sfw::drawLine(x - size / 4.0f, y - size / 2.0f, x - size / 4.0f, y + size / 2.0f);
		sfw::drawLine(x + size / 4.0f, y + size / 8.0f, x + size / 4.0f, y + size / 2.0f);
		for (int i = 0; i < 180; i += 10)
			sfw::drawLine(x + cos(i* toRad)*(size / 4.0f), y + (size / 8.0f) - sin(i*toRad)*(size / 8.0f), x + cos((i + 10)*toRad)*(size / 4.0f), y + (size / 8.0f) - sin((i + 10)*toRad)*(size / 8.0f));
		break;
	case 'H':
		sfw::drawLine(x - size / 4.0f, y - size / 2.0f, x - size / 4.0f, y + size / 2.0f);
		sfw::drawLine(x - size / 4.0f, y, x + size / 4.0f, y);
		sfw::drawLine(x + size / 4.0f, y - size / 2.0f, x + size / 4.0f, y + size / 2.0f);
		break;
	case 'i':
		sfw::drawLine(x - size / 4.0f, y + size / 2.0f, x + size / 4.0f, y + size / 2.0f);
		sfw::drawLine(x, y, x, y + size / 2.0f);
		sfw::drawLine(x - size / 4.0f, y, x, y);
		sfw::drawLine(x, y - size*3.0f / 8.0f, x, y - size / 8.0f);
		break;
	case 'I':
		sfw::drawLine(x, y + size / 2.0f, x, y - size / 2.0f);
		sfw::drawLine(x + size / 4.0f, y + size / 2.0f, x - size / 4.0f, y + size / 2.0f);
		sfw::drawLine(x + size / 4.0f, y - size / 2.0f, x - size / 4.0f, y - size / 2.0f);
		break;
	case 'j':
		sfw::drawLine(x, y, x, y + size *3.0f/ 4.0f);
		sfw::drawLine(x - size / 4.0f, y, x, y);
		sfw::drawLine(x, y - size*3.0f / 8.0f, x, y - size / 8.0f);
		for (int i = 0; i < 180; i += 10)
			sfw::drawLine(x - size / 8.0f + cos((i + 180)*toRad)*size / 8.0f, y + size*3.0f / 4.0f - sin((i + 180)*toRad)*size / 8.0f, x - size / 8.0f + cos((i + 190)*toRad)*size / 8.0f, y + size*3.0f / 4.0f - sin((i + 190)*toRad)*size / 8.0f);
		break;
	case 'J':
		sfw::drawLine(x - size / 4.0f, y - size / 2.0f, x + size / 4.0f, y - size / 2.0f);
		sfw::drawLine(x + size / 4.0f, y - size / 2.0f, x + size / 4.0f, y + size / 4.0f);
		for (int i = 0; i < 180; i += 10)
			sfw::drawLine(x + cos((i + 180)*toRad)*size / 4.0f, y + size / 4.0f - sin((i + 180)*toRad)*size / 4.0f, x + cos((i + 190)*toRad)*size / 4.0f, y + size / 4.0f - sin((i + 190)*toRad)*size / 4.0f);
		break;
	case 'k':
		sfw::drawLine(x - size / 4.0f, y - size / 2.0f, x - size / 4.0f, y + size / 2.0f);
		sfw::drawLine(x - size / 4.0f, y, x + size / 4.0f, y + size / 2.0f);
		sfw::drawLine(x - size / 4.0f, y, x + size / 4.0f, y - size / 4.0f);
		break;
	case 'K':
		sfw::drawLine(x - size / 4.0f, y - size / 2.0f, x - size / 4.0f, y + size / 2.0f);
		sfw::drawLine(x - size / 4.0f, y, x + size / 4.0f, y + size / 2.0f);
		sfw::drawLine(x - size / 4.0f, y, x + size / 4.0f, y - size / 2.0f);
		break;
	case 'l':
		sfw::drawLine(x, y - size / 2.0f, x, y + size / 2.0f);
		sfw::drawLine(x - size / 4.0f, y - size / 2.0f, x, y - size / 2.0f);
		sfw::drawLine(x - size / 4.0f, y + size / 2.0f, x + size / 4.0f, y + size / 2.0f);
		break;
	case 'L':
		sfw::drawLine(x - size / 4.0f, y - size / 2.0f, x - size / 4.0f, y + size / 2.0f);
		sfw::drawLine(x - size / 4.0f, y + size / 2.0f, x + size / 4.0f, y + size / 2.0f);
		break;
	case 'm':
		sfw::drawLine(x - size / 4.0f, y + size / 8.0f, x - size / 4.0f, y + size / 2.0f);
		sfw::drawLine(x, y + size / 8.0f, x, y + size / 2.0f);
		sfw::drawLine(x + size / 4.0f, y + size / 8.0f, x + size / 4.0f, y + size / 2.0f);
		for (int i = 0; i < 180; i += 10)
		{
			sfw::drawLine(x - size / 8.0f + cos(i*toRad)*size / 8.0f, y + size / 8.0f - sin(i*toRad)*size / 8.0f, x - size / 8.0f + cos((i + 10)*toRad)*size / 8.0f, y + size / 8.0f - sin((i + 10)*toRad)*size / 8.0f);
			sfw::drawLine(x + size / 8.0f + cos(i*toRad)*size / 8.0f, y + size / 8.0f - sin(i*toRad)*size / 8.0f, x + size / 8.0f + cos((i + 10)*toRad)*size / 8.0f, y + size / 8.0f - sin((i + 10)*toRad)*size / 8.0f);
		}
		break;
	case 'M':
		sfw::drawLine(x - size / 4.0f, y - size / 2.0f, x - size / 4.0f, y + size / 2.0f);
		sfw::drawLine(x - size / 4.0f, y - size / 2.0f, x, y);
		sfw::drawLine(x + size / 4.0f, y - size / 2.0f, x, y);
		sfw::drawLine(x + size / 4.0f, y - size / 2.0f, x + size / 4.0f, y + size / 2.0f);
		break;
	case 'n':
		sfw::drawLine(x - size / 4.0f, y, x - size / 4.0f, y + size / 2.0f);
		sfw::drawLine(x + size / 4.0f, y + size / 8.0f, x + size / 4.0f, y + size / 2.0f);
		for (int i = 0; i < 180; i += 10)
			sfw::drawLine(x + cos(i* toRad)*(size / 4.0f), y + (size / 8.0f) - sin(i*toRad)*(size / 8.0f), x + cos((i + 10)*toRad)*(size / 4.0f), y + (size / 8.0f) - sin((i + 10)*toRad)*(size / 8.0f));
		break;
	case 'N':
		sfw::drawLine(x - size / 4.0f, y - size / 2.0f, x - size / 4.0f, y + size / 2.0f);
		sfw::drawLine(x - size / 4.0f, y - size / 2.0f, x + size / 4.0f, y + size / 2.0f);
		sfw::drawLine(x + size / 4.0f, y - size / 2.0f, x + size / 4.0f, y + size / 2.0f);
		break;
	case 'o':
		sfw::drawCircle(x, y + size / 4.0f, size / 4.0f);
		break;
	case 'O':
		sfw::drawLine(x - size / 16.0f, y + size / 2.0f, x + size / 16.0f, y + size / 2.0f);
		sfw::drawLine(x - size / 16.0f, y - size / 2.0f, x + size / 16.0f, y - size / 2.0f);
		for (int i = 0; i < 180; i += 10)
		{
			sfw::drawLine(x + size / 16.0f + cos((i - 90)*toRad)*size *3.0f / 16.0f, y - sin((i - 90)*toRad)*size / 2.0f, x + size / 16.0f + cos((i - 80)*toRad)*size*3.0f / 16.0f, y - sin((i - 80)*toRad)*size / 2.0f);
			sfw::drawLine(x - size / 16.0f + cos((i + 90)*toRad)*size *3.0f / 16.0f, y - sin((i + 90)*toRad)*size / 2.0f, x - size / 16.0f + cos((i + 100)*toRad)*size*3.0f / 16.0f, y - sin((i + 100)*toRad)*size / 2.0f);
		}
		break;
	case 'p':
		sfw::drawLine(x - size / 4.0f, y, x - size / 4.0f, y + size);
		sfw::drawLine(x - size / 4.0f, y, x, y);
		sfw::drawLine(x - size / 4.0f, y + size / 2.0f, x, y + size / 2.0f);
		for (int i = 0; i < 180; i += 10)
			sfw::drawLine(x + cos((i - 90)*toRad)*size / 4.0f, y + size / 4.0f - sin((i - 90)*toRad)*size / 4.0f, x + cos((i - 80)*toRad)*size / 4.0f, y + size / 4.0f - sin((i - 80)*toRad)*size / 4.0f);
		break;
	case 'P':
		sfw::drawLine(x - size / 4.0f, y - size / 2.0f, x - size / 4.0f, y + size / 2.0f);
		sfw::drawLine(x - size / 4.0f, y - size / 2.0f, x, y - size / 2.0f);
		sfw::drawLine(x - size / 4.0f, y, x, y);
		for (int i = 0; i < 180; i += 10)
			sfw::drawLine(x + cos((i + 270)*toRad)*(size / 4.0f), y - (size / 4.0f) - sin((i + 270)*toRad)*(size / 4.0f), x + cos((i + 280)*toRad)*(size / 4.0f), y - (size / 4.0f) - sin((i + 280)*toRad)*(size / 4.0f));
		break;
	case 'q':
		sfw::drawLine(x + size / 4.0f, y + size / 4.0f, x + size / 4.0f, y + size*3.0f / 4.0f);
		sfw::drawCircle(x, y + size / 4.0f, size / 4.0f);
		for (int i = 0; i < 180; i += 10)
			sfw::drawLine(x + size *3.0f / 8.0f + cos(i*toRad)*size / 8.0f, y + size*3.0f / 4.0f + sin(i*toRad)*size / 8.0f, x + size *3.0f / 8.0f + cos((i + 10)*toRad)*size / 8.0f, y + size*3.0f / 4.0f + sin((i + 10)*toRad)*size / 8.0f);
		break;
	case 'Q':
		sfw::drawLine(x - size / 16.0f, y + size / 2.0f, x + size / 16.0f, y + size / 2.0f);
		sfw::drawLine(x - size / 16.0f, y - size / 2.0f, x + size / 16.0f, y - size / 2.0f);
		sfw::drawLine(x + size / 8.0f, y + size / 4.0f, x + size / 4.0f, y + size / 2.0f);
		for (int i = 0; i < 180; i += 10)
		{
			sfw::drawLine(x + size / 16.0f + cos((i - 90)*toRad)*size *3.0f / 16.0f, y - sin((i - 90)*toRad)*size / 2.0f, x + size / 16.0f + cos((i - 80)*toRad)*size*3.0f / 16.0f, y - sin((i - 80)*toRad)*size / 2.0f);
			sfw::drawLine(x - size / 16.0f + cos((i + 90)*toRad)*size *3.0f / 16.0f, y - sin((i + 90)*toRad)*size / 2.0f, x - size / 16.0f + cos((i + 100)*toRad)*size*3.0f / 16.0f, y - sin((i + 100)*toRad)*size / 2.0f);
		}
		break;
	case 'r':
		sfw::drawLine(x - size / 4.0f, y, x - size / 4.0f, y + size / 2.0f);
		for (int i = 0; i < 180; i += 10)
			sfw::drawLine(x + cos(i* toRad)*(size / 4.0f), y + (size / 8.0f) - sin(i*toRad)*(size / 8.0f), x + cos((i + 10)*toRad)*(size / 4.0f), y + (size / 8.0f) - sin((i + 10)*toRad)*(size / 8.0f));
		break;
	case 'R':
		sfw::drawLine(x - size / 4.0f, y - size / 2.0f, x - size / 4.0f, y + size / 2.0f);
		sfw::drawLine(x - size / 4.0f, y, x + size / 4.0f, y + size / 2.0f);
		sfw::drawLine(x - size / 4.0f, y, x, y);
		sfw::drawLine(x - size / 4.0f, y - size / 2.0f, x, y - size / 2.0f);
		for (int i = 0; i < 180; i += 10)
			sfw::drawLine(x + cos((i + 270)*toRad)*(size / 4.0f), y - (size / 4.0f) - sin((i + 270)*toRad)*(size / 4.0f), x + cos((i + 280)*toRad)*(size / 4.0f), y - (size / 4.0f) - sin((i + 280)*toRad)*(size / 4.0f));
		break;
	case 's':
		sfw::drawLine(x, y, x + size / 4.0f, y);
		sfw::drawLine(x, y + size / 2.0f, x - size / 4.0f, y + size / 2.0f);
		for (int i = 0; i < 180; i += 10)
		{
			sfw::drawLine(x + cos((i + 90)*toRad)*(size / 4.0f), y + (size / 8.0f) - sin((i + 90)*toRad)*(size / 8.0f), x + cos((i + 100)*toRad)*(size / 4.0f), y + (size / 8.0f) - sin((i + 100)*toRad)*(size / 8.0f));
			sfw::drawLine(x + cos((i + 270)*toRad)*(size / 4.0f), y + (size *3.0f / 8.0f) - sin((i + 270)*toRad)*(size / 8.0f), x + cos((i + 280)*toRad)*(size / 4.0f), y + (size *3.0f / 8.0f) - sin((i + 280)*toRad)*(size / 8.0f));
		}
		break;
	case 'S':
		sfw::drawLine(x, y - size / 2.0f, x + size / 4.0f, y - size / 2.0f);
		sfw::drawLine(x, y + size / 2.0f, x - size / 4.0f, y + size / 2.0f);
		for (int i = 0; i < 180; i += 10)
		{
			sfw::drawLine(x + cos((i + 90)*toRad)*(size / 4.0f), y - (size / 4.0f) - sin((i + 90)*toRad)*(size / 4.0f), x + cos((i + 100)*toRad)*(size / 4.0f), y - (size / 4.0f) - sin((i + 100)*toRad)*(size / 4.0f));
			sfw::drawLine(x + cos((i + 270)*toRad)*(size / 4.0f), y + (size / 4.0f) - sin((i + 270)*toRad)*(size / 4.0f), x + cos((i + 280)*toRad)*(size / 4.0f), y + (size / 4.0f) - sin((i + 280)*toRad)*(size / 4.0f));
		}
		break;
	case 't':
		sfw::drawLine(x, y - size / 2.0f, x, y + size / 2.0f);
		sfw::drawLine(x - size / 4.0f, y, x + size / 4.0f, y);
		sfw::drawLine(x, y + size / 2.0f, x + size / 4.0f, y + size / 2.0f);
		for (int i = 0; i < 180; i += 10)
			sfw::drawLine(x + size / 8.0f + cos((i + 180)*toRad)*size / 8.0f, y + size*3.0f / 8.0f - sin((i + 180)*toRad)*size / 8.0f, x + size / 8.0f + cos((i + 190)*toRad)*size / 8.0f, y + size*3.0f / 8.0f - sin((i + 190)*toRad)*size / 8.0f);
		break;
	case 'T':
		sfw::drawLine(x, y - size / 2.0f, x, y + size / 2.0f);
		sfw::drawLine(x - size / 4.0f, y - size / 2.0f, x + size / 4.0f, y - size / 2.0f);
		break;
	case 'u':
		sfw::drawLine(x + size / 4.0f, y, x + size / 4.0f, y + size / 2.0f);
		sfw::drawLine(x - size / 4.0f, y, x - size / 4.0f, y + size*3.0f / 8.0f);
		for (int i = 0; i < 180; i += 10)
			sfw::drawLine(x + cos((i + 180)*toRad)*size / 4.0f, y + size*3.0f / 8.0f - sin((i + 180)*toRad)*size / 8.0f, x + cos((i + 190)*toRad)*size / 4.0f, y + size*3.0f / 8.0f - sin((i + 190)*toRad)*size / 8.0f);
		break;
	case 'U':
		sfw::drawLine(x + size / 4.0f, y - size / 2.0f, x + size / 4.0f, y + size / 2.0f);
		sfw::drawLine(x - size / 4.0f, y - size / 2.0f, x - size / 4.0f, y + size*3.0f / 8.0f);
		for (int i = 0; i < 180; i += 10)
			sfw::drawLine(x + cos((i + 180)*toRad)*size / 4.0f, y + size*3.0f / 8.0f - sin((i + 180)*toRad)*size / 8.0f, x + cos((i + 190)*toRad)*size / 4.0f, y + size*3.0f / 8.0f - sin((i + 190)*toRad)*size / 8.0f);
		break;
	case 'v':
		sfw::drawLine(x - size / 4.0f, y, x, y + size / 2.0f);
		sfw::drawLine(x + size / 4.0f, y, x, y + size / 2.0f);
		break;
	case 'V':
		sfw::drawLine(x - size / 4.0f, y - size / 2.0f, x, y + size / 2.0f);
		sfw::drawLine(x + size / 4.0f, y - size / 2.0f, x, y + size / 2.0f);
		break;
	case 'w':
		sfw::drawLine(x - size / 4.0f, y , x - size / 8.0f, y + size / 2.0f);
		sfw::drawLine(x - size / 8.0f, y + size / 2.0f, x, y);
		sfw::drawLine(x, y, x + size / 8.0f, y + size / 2.0f);
		sfw::drawLine(x + size / 4.0f, y,x + size / 8.0f, y + size / 2.0f);
		break;
	case 'W':
		sfw::drawLine(x - size / 4.0f, y - size / 2.0f, x - size / 8.0f, y + size / 2.0f);
		sfw::drawLine(x - size / 8.0f, y + size / 2.0f, x, y);
		sfw::drawLine(x, y, x + size / 8.0f, y + size / 2.0f);
		sfw::drawLine(x + size / 8.0f, y + size / 2.0f, x + size / 4.0f, y - size / 2.0f);
		break;
	case 'x':
		sfw::drawLine(x - size / 4.0f, y, x + size / 4.0f, y + size / 2.0f);
		sfw::drawLine(x - size / 4.0f, y + size / 2.0f, x + size / 4.0f, y);
		break;
	case 'X':
		sfw::drawLine(x - size / 4.0f, y - size / 2.0f, x + size / 4.0f, y + size / 2.0f);
		sfw::drawLine(x + size / 4.0f, y - size / 2.0f, x - size / 4.0f, y + size / 2.0f);
		break;
	case 'y':
		sfw::drawLine(x - size / 4.0f, y, x, y + size / 2.0f);
		sfw::drawLine(x + size / 4.0f, y, x - size / 4.0f, y + size);
		break;
	case 'Y':
		sfw::drawLine(x - size / 4.0f, y - size / 2.0f, x, y);
		sfw::drawLine(x + size / 4.0f, y - size / 2.0f, x, y);
		sfw::drawLine(x, y, x, y + size / 2.0f);
		break;
	case 'z':
		sfw::drawLine(x - size / 4.0f, y, x + size/4.0f, y);
		sfw::drawLine(x - size / 4.0f, y + size / 2.0f, x + size / 4.0f, y + size / 2.0f);
		sfw::drawLine(x + size / 4.0f, y, x - size / 4.0f, y + size / 2.0f);
		break;
	case 'Z':
		sfw::drawLine(x - size / 4.0f, y - size / 2.0f, x + size / 4.0f, y - size / 2.0f);
		sfw::drawLine(x - size / 4.0f, y + size / 2.0f, x + size / 4.0f, y + size / 2.0f);
		sfw::drawLine(x + size / 4.0f, y - size / 2.0f, x - size / 4.0f, y + size / 2.0f);
		break;
	case 0:
		for (int i = 0; i < 360; i += 10)
			sfw::drawLine(x + cos(i*toRad)*size / 4.0f, y + sin(i*toRad)*size / 2.0f, x + cos((i + 10)*toRad)*size / 4.0f, y + sin((i + 10)*toRad)*size / 2.0f);
		break;
	case 1:
		sfw::drawLine(x, y - size / 2.0f, x, y + size / 2.0f);
		sfw::drawLine(x - size / 4.0f, y + size / 2.0f, x + size / 4.0f, y + size / 2.0f);
		sfw::drawLine(x, y - size / 2.0f, x - size / 4.0f, y - size / 4.0f);
		break;
	case 2:
		sfw::drawLine(x + size / 4.0f, y - size*3.0f / 8.0f, x - size / 4.0f, y + size / 2.0f);
		sfw::drawLine(x + size / 4.0f, y + size / 2.0f, x - size / 4.0f, y + size / 2.0f);
		for (int i = 0; i < 180; i += 10)
			sfw::drawLine(x + cos(i*toRad)*size / 4.0f, y - size*3.0f / 8.0f - sin(i*toRad)*size / 8.0f, x + cos((i + 10)*toRad)*size / 4.0f, y - size*3.0f / 8.0f - sin((i + 10)*toRad)*size / 8.0f);
		break;
	case 3:
		sfw::drawLine(x - size / 4.0f, y - size / 2.0f, x, y - size / 2.0f);
		sfw::drawLine(x - size / 4.0f, y + size / 2.0f, x, y + size / 2.0f);
		for (int i = 0; i < 180; i += 10)
		{
			sfw::drawLine(x + cos((i - 90)*toRad)*size / 4.0f, y + size / 4.0f + sin((i - 90)*toRad)*size / 4.0f, x + cos((i - 80)*toRad)*size / 4.0f, y + size / 4.0f + sin((i - 80)*toRad)*size / 4.0f);
			sfw::drawLine(x + cos((i - 90)*toRad)*size / 4.0f, y - size / 4.0f + sin((i - 90)*toRad)*size / 4.0f, x + cos((i - 80)*toRad)*size / 4.0f, y - size / 4.0f + sin((i - 80)*toRad)*size / 4.0f);
		}
		break;
	case 4:
		sfw::drawLine(x, y - size / 2.0f, x, y + size / 2.0f);
		sfw::drawLine(x, y - size / 2.0f, x - size / 4.0f, y);
		sfw::drawLine(x - size / 4.0f, y, x + size / 4.0f, y);
		break;
	case 5:
		sfw::drawLine(x - size / 4.0f, y - size / 2.0f, x + size / 4.0f, y - size / 2.0f);
		sfw::drawLine(x - size / 4.0f, y - size / 2.0f, x - size / 4.0f, y);
		sfw::drawLine(x - size / 4.0f, y, x, y);
		sfw::drawLine(x - size / 4.0f, y + size / 2.0f, x, y + size / 2.0f);
		for (int i = 0; i < 180; i += 10)
			sfw::drawLine(x + cos((i - 90)*toRad)*size / 4.0f, y + size / 4.0f + sin((i - 90)*toRad)*size / 4.0f, x + cos((i - 80)*toRad)*size / 4.0f, y + size / 4.0f + sin((i - 80)*toRad)*size / 4.0f);
		break;
	case 6:
		sfw::drawCircle(x, y + size / 4.0f, size / 4.0f);
		sfw::drawLine(x - size / 4.0f, y - size / 4.0f, x - size / 4.0f, y + size / 4.0f);
		sfw::drawLine(x, y - size / 2.0f, x + size / 4.0f, y - size / 2.0f);
		for (int i = 0; i < 90; i += 10)
			sfw::drawLine(x + cos((i + 90)*toRad)*size / 4.0f, y - size / 4.0f - sin((i + 90)*toRad)*size / 4.0f, x + cos((i + 100)*toRad)*size / 4.0f, y - size / 4.0f - sin((i + 100)*toRad)*size / 4.0f);
		break;
	case 7:
		sfw::drawLine(x - size / 4.0f, y - size / 2.0f, x + size / 4.0f, y - size / 2.0f);
		sfw::drawLine(x - size / 4.0f, y + size / 2.0f, x + size / 4.0f, y - size / 2.0f);
		break;
	case 8:
		sfw::drawCircle(x, y + size / 4.0f, size / 4.0f);
		sfw::drawCircle(x, y - size / 4.0f, size / 4.0f);
		break;
	case 9:
		sfw::drawCircle(x, y - size / 4.0f, size / 4.0f);
		sfw::drawLine(x + size / 4.0f, y + size / 4.0f, x + size / 4.0f, y - size / 4.0f);
		sfw::drawLine(x - size / 4.0f, y + size / 2.0f, x, y + size / 2.0f);
		for (int i = 0; i < 90; i += 10)
			sfw::drawLine(x + cos((i - 90)*toRad)*size / 4.0f, y + size / 4.0f - sin((i - 90)*toRad)*size / 4.0f, x + cos((i - 80)*toRad)*size / 4.0f, y + size / 4.0f - sin((i - 80)*toRad)*size / 4.0f);
		break;
	}
	return;
}

void drawMenu(float screenH, float screenW,Player &selector,PowerUp pTypes[])
{
	float gap = 5.0f, size = screenH / 20.0f, loc[3] = { screenH / 2.0f,screenH*2.0f / 3.0f,screenH - (size + 5) };
	int timer = 0;
	while (sfw::stepContext())
	{
		drawPaddle(selector);
		for (int i = 0; i < 5; i++)
		{
			pTypes[i].size = screenH / 28.0f;
			pTypes[i].x = pTypes[i].size + 10.0f;
			pTypes[i].y = (i+1.0f)*screenH / 6.0f;
			pTypes[i].onScreen = true;
			pTypes[i].effectDur = -1;
			drawPowerup(pTypes[i]);
		}
		
		{
			drawChar('I', pTypes[0].x + pTypes[0].size * 1.5f, pTypes[0].y, pTypes[0].size);
			drawChar('n', pTypes[0].x + pTypes[0].size * 2.5f, pTypes[0].y, pTypes[0].size);
			drawChar('v', pTypes[0].x + pTypes[0].size * 3.5f, pTypes[0].y, pTypes[0].size);
			drawChar('e', pTypes[0].x + pTypes[0].size * 4.5f, pTypes[0].y, pTypes[0].size);
			drawChar('r', pTypes[0].x + pTypes[0].size * 5.5f, pTypes[0].y, pTypes[0].size);
			drawChar('t', pTypes[0].x + pTypes[0].size * 6.5f, pTypes[0].y, pTypes[0].size);
		}

		{
			drawChar('B', pTypes[1].x + pTypes[1].size * 1.5f, pTypes[1].y, pTypes[1].size);
			drawChar('i', pTypes[1].x + pTypes[1].size * 2.5f, pTypes[1].y, pTypes[1].size);
			drawChar('g', pTypes[1].x + pTypes[1].size * 3.5f, pTypes[1].y, pTypes[1].size);
		}

		{
			drawChar('L', pTypes[2].x + pTypes[2].size * 1.5f, pTypes[2].y, pTypes[2].size);
			drawChar('i', pTypes[2].x + pTypes[2].size * 2.5f, pTypes[2].y, pTypes[2].size);
			drawChar('t', pTypes[2].x + pTypes[2].size * 3.5f, pTypes[2].y, pTypes[2].size);
			drawChar('t', pTypes[2].x + pTypes[2].size * 4.5f, pTypes[2].y, pTypes[2].size);
			drawChar('l', pTypes[2].x + pTypes[2].size * 5.5f, pTypes[2].y, pTypes[2].size);
			drawChar('e', pTypes[2].x + pTypes[2].size * 6.5f, pTypes[2].y, pTypes[2].size);
		}

		{
			drawChar('S', pTypes[3].x + pTypes[3].size * 1.5f, pTypes[3].y, pTypes[0].size);
			drawChar('l', pTypes[3].x + pTypes[3].size * 2.5f, pTypes[3].y, pTypes[0].size);
			drawChar('o', pTypes[3].x + pTypes[3].size * 3.5f, pTypes[3].y, pTypes[0].size);
			drawChar('w', pTypes[3].x + pTypes[3].size * 4.5f, pTypes[3].y, pTypes[0].size);
		}

		{
			drawChar('F', pTypes[4].x + pTypes[4].size * 1.5f, pTypes[4].y, pTypes[4].size);
			drawChar('a', pTypes[4].x + pTypes[4].size * 2.5f, pTypes[4].y, pTypes[4].size);
			drawChar('s', pTypes[4].x + pTypes[4].size * 3.5f, pTypes[4].y, pTypes[4].size);
			drawChar('t', pTypes[4].x + pTypes[4].size * 4.5f, pTypes[4].y, pTypes[4].size);
		}

		updateSelector(selector, loc,timer);
		{
			drawChar('W', screenW / 2.0f - 4.0f * (size / 2.0f + gap) - size / 4.0f, size + 5, size);
			drawChar('h', screenW / 2.0f - 3.0f * (size / 2.0f + gap) - size / 4.0f, size + 5, size);
			drawChar('e', screenW / 2.0f - 2.0f * (size / 2.0f + gap) - size / 4.0f, size + 5, size);
			drawChar('e', screenW / 2.0f - 1.0f * (size / 2.0f + gap) - size / 4.0f, size + 5, size);
			drawChar('l', screenW / 2.0f - 0.0f * (size / 2.0f + gap) - size / 4.0f, size + 5, size);
			drawChar('g', screenW / 2.0f + 4.0f * (size / 2.0f + gap) + size / 4.0f, size + 5, size);
			drawChar('n', screenW / 2.0f + 3.0f * (size / 2.0f + gap) + size / 4.0f, size + 5, size);
			drawChar('o', screenW / 2.0f + 2.0f * (size / 2.0f + gap) + size / 4.0f, size + 5, size);
			drawChar('P', screenW / 2.0f + 1.0f * (size / 2.0f + gap) + size / 4.0f, size + 5, size);
		}

		{
			drawChar(1, screenW / 2.0f + 0.0f * (size / 2.0f + gap) + size / 2.0f, screenH / 2.0f, size);
			drawChar('P', screenW / 2.0f + 1.0f * (size / 2.0f + gap) + size / 2.0f, screenH / 2.0f, size);
		}

		{
			drawChar(2, screenW / 2.0f + 0.0f * (size / 2.0f + gap) + size / 2.0f, screenH*2.0f / 3.0f, size);
			drawChar('P', screenW / 2.0f + 1.0f * (size / 2.0f + gap) + size / 2.0f, screenH*2.0f / 3.0f, size);
		}

		{
			drawChar('E', screenW / 2.0f - 1.0f * (size / 2.0f + gap) - size / 4.0f, screenH - (size + 5), size);
			drawChar('X', screenW / 2.0f - gap / 2.0f - size / 4.0f, screenH - (size + 5), size);
			drawChar('I', screenW / 2.0f + gap / 2.0f + size / 4.0f, screenH - (size + 5), size);
			drawChar('T', screenW / 2.0f + 1.0f * (size / 2.0f + gap) + size / 4.0f, screenH - (size + 5), size);
		}

		if (sfw::getKey(' '))
		{
			if (selector.lives == 2)
			{
				sfw::termContext();
				break;
			}
			if (selector.lives == 1 || selector.lives == 0)
			{
				selector.acc = 1;
				break;
			}
		}
	}
	return;
}