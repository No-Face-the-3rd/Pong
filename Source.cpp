#include "Header.h"
#include "sfwdraw.h"
#include <cmath>
#include <time.h>

void drawMenu(float screenH,float screenW, Player &selector);
void drawChar(char test, float x, float y, float size);
void drawPaddle(Player player);
void drawBall(Ball &ball);
void updatePlayer(Player &player, int playerNum, int screenH);
void updateSelector(Player &selector,float loc[],int &timer);
void drawPowerup(PowerUp pList[]);


int main()
{
	int screenW = 1000, screenH = 700, lives = 3, dfRadius = 20;
	float dfWidth = 20.0f, dfHeight = 80.0f, dfPspeed = 0.50f, dfBspeed = 0.40f, speedToSpin = 650.0f, spinMulti = 200.0f, velDif = 0.0f;
	Ball ball = {screenW / 2.0f, screenH/2.0f, 70.0f, 70.0f, 0.15f, 0.0f,0.0f, dfRadius};

	Player selector{ 0,screenW / 2.0f + 4.0f*(screenH / 20.0f / 2.0f + 5.0f),screenH / 2.0f,0.0f,15.0f,20.0f };

	sfw::initContext(screenW, screenH, "Wheel Pong");
	Player player1{ lives,dfWidth/1.25f,(float)screenH / 2.0f,dfPspeed,dfWidth,dfHeight };
	Player player2{ lives,(float)screenW - dfWidth/1.25f,(float)screenH / 2.0f ,dfPspeed,dfWidth,dfHeight };
	

	drawMenu((float)screenH,(float)screenW,selector);

	while (sfw::stepContext())
	{
		drawPaddle(player1);
		drawPaddle(player2);
		drawBall(ball);

		ball.y += ball.yVel* sfw::getDeltaTime();
		ball.x += ball.xVel* sfw::getDeltaTime();
		//ball.speed = sqrt(pow(ball.xVel, 2) + pow(ball.yVel, 2));


		updatePlayer(player1, 1, screenH);
		updatePlayer(player2, 2, screenH);

		drawChar(1, screenW / 2.0f, screenH / 2.0f, screenH / 20.0f);
		drawChar('g', screenW / 2.0f + screenW/30.0f, screenH / 2.0f, screenH / 20.0f);
		if (ball.y > (float)screenH - ball.radius - 5.0f) 
		{
			ball.y = (float)screenH - ball.radius - 5.0f;
			velDif = ball.yVel - sqrt(abs(pow(ball.speed, 2.0f) - pow(ball.xVel - spinMulti*ball.spin, 2.0f)));
			if (velDif < 0)
				ball.yVel = -ball.yVel - velDif;
			if (velDif > 0)
				ball.yVel = -ball.yVel + velDif;
			ball.xVel =ball.xVel - spinMulti * ball.spin;
		}
		if (ball.y < ball.radius + 5.0f)
		{
			ball.y = ball.radius + 5.0f;
			velDif = ball.yVel + sqrt(abs(pow(ball.speed, 2.0f) - pow(ball.xVel + spinMulti*ball.spin, 2.0f)));
			if (velDif < 0)
				ball.yVel = -ball.yVel + velDif;
			if (velDif > 0)
				ball.yVel = -ball.yVel - velDif;
			ball.xVel = ball.xVel + spinMulti * ball.spin;
			//ball.xVel = ball.xVel - spinMulti * ball.spin;
			//ball.yVel = -ball.yVel - 200 * ball.spin;
			//ball.xVel += 200 * ball.spin;
		}
		if (ball.x >(float)screenW - ball.radius - 5.0f)
		{
			ball.x = (float)screenW - ball.radius - 5.0f;
			ball.xVel = -ball.xVel + 200 * ball.spin;
			ball.yVel += 200 * ball.spin;
		}
		if (ball.x < ball.radius + 5.0f)
		{
			ball.x = ball.radius + 5.0f;
			ball.xVel = -ball.xVel - 200 * ball.spin;
			ball.yVel -= 200 * ball.spin;
		}


		if (player1.lives <= 0 || player2.lives <= 0)
			drawMenu(screenH,screenW,selector);
		if (selector.lives == 2)
			break;
	}

	return 0;
}

void drawPowerup(PowerUp pList[])
{

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

void updatePlayer(Player &player, int playerNum, int screenH)
{
	if (sfw::getKey('s') && playerNum == 1) player.y += player.speed;
	if (sfw::getKey('w') && playerNum == 1) player.y -= player.speed;
	if (sfw::getKey('k') && playerNum == 2) player.y += player.speed;
	if (sfw::getKey('i') && playerNum == 2) player.y -= player.speed;
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
	if (ball.spin > 0) ball.angle += ball.spin;
	else ball.angle -= ball.spin;
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
	case 'h':
		sfw::drawLine(x - size / 4.0f, y - size / 2.0f, x - size / 4.0f, y + size / 2.0f);
		sfw::drawLine(x + size / 4.0f, y + size / 8.0f, x + size / 4.0f, y + size / 2.0f);
		for (int i = 0; i < 180; i += 10)
			sfw::drawLine(x + cos(i* toRad)*(size / 4.0f), y + (size / 8.0f) - sin(i*toRad)*(size / 8.0f), x + cos((i + 10)*toRad)*(size / 4.0f), y + (size / 8.0f) - sin((i + 10)*toRad)*(size / 8.0f));
		break;
	case 'I':
		sfw::drawLine(x, y + size / 2.0f, x, y - size / 2.0f);
		sfw::drawLine(x + size / 4.0f, y + size / 2.0f, x - size / 4.0f, y + size / 2.0f);
		sfw::drawLine(x + size / 4.0f, y - size / 2.0f, x - size / 4.0f, y - size / 2.0f);
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
	case 'n':
		sfw::drawLine(x - size / 4.0f, y, x - size / 4.0f, y + size / 2.0f);
		sfw::drawLine(x + size / 4.0f, y + size / 8.0f, x + size / 4.0f, y + size / 2.0f);
		for (int i = 0; i < 180; i += 10)
			sfw::drawLine(x + cos(i* toRad)*(size / 4.0f), y + (size / 8.0f) - sin(i*toRad)*(size / 8.0f), x + cos((i + 10)*toRad)*(size / 4.0f), y + (size / 8.0f) - sin((i + 10)*toRad)*(size / 8.0f));
		break;
	case 'o':
		sfw::drawCircle(x, y + size / 4.0f, size / 4.0f);
		break;
	case 'O':
		for (int i = 0; i < 360; i += 10)
			sfw::drawLine(x + cos(i*toRad)*size / 4.0f, y + sin(i*toRad)*size / 2.0f, x + cos((i + 10)*toRad)*size / 4.0f, y + sin((i + 10)*toRad)*size / 2.0f);
		break;
	case 'P':
		sfw::drawLine(x - size / 4.0f, y - size / 2.0f, x - size / 4.0f, y + size / 2.0f);
		sfw::drawLine(x - size / 4.0f, y - size / 2.0f, x, y - size / 2.0f);
		sfw::drawLine(x - size / 4.0f, y, x, y);
		for (int i = 0; i < 180; i += 10)
			sfw::drawLine(x + cos((i + 270)*toRad)*(size / 4.0f), y - (size / 4.0f) - sin((i + 270)*toRad)*(size / 4.0f), x + cos((i + 280)*toRad)*(size / 4.0f), y - (size / 4.0f) - sin((i + 280)*toRad)*(size / 4.0f));
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
	case 'T':
		sfw::drawLine(x, y - size / 2.0f, x, y + size / 2.0f);
		sfw::drawLine(x - size / 4.0f, y - size / 2.0f, x + size / 4.0f, y - size / 2.0f);
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
	case 'X':
		sfw::drawLine(x - size / 4.0f, y - size / 2.0f, x + size / 4.0f, y + size / 2.0f);
		sfw::drawLine(x + size / 4.0f, y - size / 2.0f, x - size / 4.0f, y + size / 2.0f);
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
	}
	return;
}

void drawMenu(float screenH, float screenW,Player &selector)
{
	float gap = 5.0f, size = screenH / 20.0f, loc[3] = { screenH / 2.0f,screenH*2.0f / 3.0f,screenH - (size + 5) };
	int timer = 0;
	while (sfw::stepContext())
	{
		drawPaddle(selector);

		updateSelector(selector, loc,timer);

		drawChar('W', screenW / 2.0f - 4.0f * (size / 2.0f + gap) - size / 4.0f, size + 5, size);
		drawChar('h', screenW / 2.0f - 3.0f * (size / 2.0f + gap) - size / 4.0f, size + 5, size);
		drawChar('e', screenW / 2.0f - 2.0f * (size / 2.0f + gap) - size / 4.0f, size + 5, size);
		drawChar('e', screenW / 2.0f - 1.0f * (size / 2.0f + gap) - size / 4.0f, size + 5, size);
		drawChar('l', screenW / 2.0f - 0.0f * (size / 2.0f + gap) - size / 4.0f, size + 5, size);
		drawChar('g', screenW / 2.0f + 4.0f * (size / 2.0f + gap) + size / 4.0f, size + 5, size);
		drawChar('n', screenW / 2.0f + 3.0f * (size / 2.0f + gap) + size / 4.0f, size + 5, size);
		drawChar('o', screenW / 2.0f + 2.0f * (size / 2.0f + gap) + size / 4.0f, size + 5, size);
		drawChar('P', screenW / 2.0f + 1.0f * (size / 2.0f + gap) + size / 4.0f, size + 5, size);

		drawChar(1, screenW / 2.0f + 0.0f * (size / 2.0f + gap) + size / 2.0f,screenH / 2.0f, size);
		drawChar('P', screenW / 2.0f + 1.0f * (size / 2.0f + gap) + size / 2.0f,screenH / 2.0f, size);

		drawChar(2, screenW / 2.0f + 0.0f * (size / 2.0f + gap) + size / 2.0f, screenH*2.0f / 3.0f, size);
		drawChar('P', screenW / 2.0f + 1.0f * (size / 2.0f + gap) + size / 2.0f, screenH*2.0f / 3.0f, size);

		drawChar('E', screenW / 2.0f - 1.0f * (size / 2.0f + gap) - size / 4.0f, screenH - (size + 5), size);
		drawChar('X', screenW / 2.0f - gap / 2.0f - size / 4.0f, screenH - (size + 5), size);
		drawChar('I', screenW / 2.0f + gap / 2.0f + size / 4.0f, screenH - (size + 5), size);
		drawChar('T', screenW / 2.0f + 1.0f * (size / 2.0f + gap) + size / 4.0f, screenH - (size + 5), size);
		if (sfw::getKey(' ') && selector.lives == 2)
		{
			if (selector.lives == 2)
			{
				sfw::termContext();
				break;
			}
		}
	}
	return;
}