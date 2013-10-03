#include "Game.h"
#include "GameObject.h"
#include "Paddle.h"
#include "Ball.h"
#include "Brick.h"
#include "../OpenGL/OpenGL.h"
#include "../Constants/Constants.h"
#include "..//Utils/Utils.h"
#include "../Screen Manager/ScreenManager.h"


Game::Game()
{
	
	lifeCounter = 3;
	levelCounter = -1;
	boolGameOver = false;
	boolPause = false;

	m_BallLifeTexture = new OpenGLTexture("ballTexture");
	m_GameOverTexture = new OpenGLTexture("Gameover1");
	m_BackgroundTexture = new OpenGLTexture("backgroundTextureMoss");
	m_PauseTexture = new OpenGLTexture("pauseTexture");

	//Create a new paddle and ball
	addGameObject(new Paddle());
	addGameObject(new Ball());

	for(float c = 0.05f; c< 0.95f; c += GAME_BRICK_WIDTH_PERCENTAGE)
		{
			for(float i = 0.01f; i < 0.16f; i += GAME_BRICK_HEIGHT_PERCENTAGE)
			{
				addGameObject(new Brick( c, i));
				//brickCounter++;
				i = i + 0.01;
			}
			c = c + 0.01;
		}

	//levelUp();

	reset();
}

Game::~Game()
{
	//Delete all the GameObject's in the vector
	for(int i = 0; i < m_GameObjects.size(); i++)
	{
		delete m_GameObjects.at(i);
		m_GameObjects.at(i) = NULL;
	}

	//Clear the pointers from the vector
	m_GameObjects.clear();
}

void Game::update(double aDelta)
{
	if(boolPause == true)
	{
		return;
	}

	if (boolGameOver == true)
	{
		return;
	}
	//If the GameOver Timer is greater that zero, countdown
	if(m_GameOverTimer > 0.0)
	{
		m_GameOverTimer -= aDelta;
		if(m_GameOverTimer < 0.0)
		{
			m_GameOverTimer = 0.0;
			reset();
		}
		return;
	}

	//Get the ball GameObject, we'll use this for collision detection
	Ball* ball = (Ball*)getGameObjectByType(GAME_BALL_TYPE);
	//Cycle through all the game objects update them and check their collision detection
	for(int i = 0; i < m_GameObjects.size(); i++)
	{
		//Make sure the GameObject is active
		if(m_GameObjects.at(i)->getIsActive() == true)
		{
			//Update the GameObject
			m_GameObjects.at(i)->update(aDelta);

			//Check collision detection against the ball
			if(m_GameObjects.at(i) != ball)
			{
				ball->checkCollision(m_GameObjects.at(i));
			}
		}

		//Check if ball is off screen
	}


}

void Game::paint()
{
	OpenGLRenderer::getInstance()->drawTexture(m_BackgroundTexture, 0, 0, ScreenManager::getInstance()->getScreenWidth(), ScreenManager::getInstance()->getScreenHeight());

	if(boolGameOver == true)
	{
		OpenGLRenderer::getInstance()->drawTexture(m_GameOverTexture, 0, 0, ScreenManager::getInstance()->getScreenWidth(), ScreenManager::getInstance()->getScreenHeight());
		return;
	}

	//Cycle through and draw all the game objects
	for(int i = 0; i < m_GameObjects.size(); i++)
	{
		if(m_GameObjects.at(i)->getIsActive() == true)
		{
			m_GameObjects.at(i)->paint();
		}
	}

	//Draw the outer white walls
	/*OpenGLRenderer::getInstance()->setForegroundColor(OpenGLColorWhite());
	OpenGLRenderer::getInstance()->setLineWidth(4.0f);
	OpenGLRenderer::getInstance()->drawLine(1.0f, 0.0f, 1.0f, getHeight());
	OpenGLRenderer::getInstance()->drawLine(0.0f, 1.0f, getWidth(), 1.0f);
	OpenGLRenderer::getInstance()->drawLine(getWidth() - 1, 0.0f, getWidth() - 1, getHeight());
	OpenGLRenderer::getInstance()->setLineWidth(1.0f);*/

	for(int i = 0; i < lifeCounter; i++)
	{
		OpenGLRenderer::getInstance()->drawTexture(
													m_BallLifeTexture,
													30+(50*i), ScreenManager::getInstance()->getScreenHeight()-50,
													30,
													30
													);
	}
	//OpenGLRenderer::getInstance()->drawTexture(
	if(boolPause == true)
	{
		OpenGLRenderer::getInstance()->drawTexture(m_PauseTexture, 0, 0, ScreenManager::getInstance()->getScreenWidth(), ScreenManager::getInstance()->getScreenHeight());
	}
}

void Game::reset()
{
	//Cycle through and reset all the game objects
	for(int i = 0; i < m_GameObjects.size(); i++)
	{
		m_GameObjects.at(i)->reset();
	}

	//Reset the game over timer to zero
	m_GameOverTimer = 0.0;

	if(boolGameOver == true)
	{
		lifeCounter = 3;
		levelCounter = 0;
	}

	boolGameOver = false; 
	//boolPause = false;
}

void Game::gameOver()
{
	boolGameOver = true;
}

const char* Game::getName()
{
	return GAME_SCREEN_NAME;
}

void Game::screenWillAppear()
{
	OpenGLRenderer::getInstance()->setBackgroundColor(OpenGLColorCornflowerBlue());
}

void Game::addGameObject(GameObject* aGameObject)
{
	if(aGameObject != NULL)
	{
		m_GameObjects.push_back(aGameObject);
	}
}

GameObject* Game::getGameObjectByType(const char* aType)
{
	//Cycle through a find the game object (if it exists)
	for(unsigned int i = 0; i < m_GameObjects.size(); i++)
	{
		if(strcmp(m_GameObjects.at(i)->getType(), aType) == 0)
		{
			return m_GameObjects.at(i);
		}
	}
	return NULL;
}

void Game::mouseMovementEvent(float aDeltaX, float aDeltaY, float aPositionX, float aPositionY)
{
	//Set the paddle to the x position of the mouse
	Paddle* paddle = (Paddle*)getGameObjectByType(GAME_PADDLE_TYPE);

	//Safety check, paddle could be NULL
	if(paddle != NULL)
	{
		paddle->setX(aPositionX - (paddle->getWidth() / 2.0f));
	}
}

void Game::pause()
{
	if(boolPause == false)
	{
		boolPause = true;
		return;
	}

	if(boolPause == true)
	{
		boolPause = false;
		return;
	}
}

void Game::keyUpEvent(int aKeyCode)
{
	//short rKeyState = GetAsyncKeyState(0x52);
	//if(rKeyState)
	if (aKeyCode == KEYCODE_R)
	{
		reset();
	}

	//short spaceKeyState = GetASyncKeyState()
	if( aKeyCode == KEYCODE_SPACE)
	{
		pause();
	}
}

void Game::loseLife()
{
	lifeCounter--;
	//reset();

	if(lifeCounter <= 0)
	{
		gameOver();
	}

	
}
//
//void Game::gotRidOfABrick()
//{
//	brickCounter--;
//	if(brickCounter == 0)
//	{
//		levelUp();
//		Ball ball;
//		ball.reset();
//		reset();
//	}
//}
//
//void Game::levelUp()
//{
//	levelCounter++;
//
//	switch(levelCounter)
//	{
//	case 0:
//		for(float c = 0.05f; c< 0.95f; c += GAME_BRICK_WIDTH_PERCENTAGE)
//		{
//			for(float i = 0.01f; i < 0.16f; i += GAME_BRICK_HEIGHT_PERCENTAGE)
//			{
//				addGameObject(new Brick( c, i));
//				brickCounter++;
//				i = i + 0.01;
//			}
//			c = c + 0.01;
//		}
//
//		brickCounter = 30;
//		break;
//
//	case 1:
//		for(float c = 0.05f; c< 0.95f; c += GAME_BRICK_WIDTH_PERCENTAGE)
//		{
//			for(float i = 0.01f; i < 0.20f; i += GAME_BRICK_HEIGHT_PERCENTAGE)
//			{
//				addGameObject(new Brick( c, i));
//				brickCounter++;
//				i = i + 0.01;
//			}
//			c = c + 0.01;
//		}
//
//		//brickCounter = 40;
//		break;
//
//	case 2:
//		for(float c = 0.05f; c< 0.95f; c += GAME_BRICK_WIDTH_PERCENTAGE)
//		{
//			for(float i = 0.01f; i < 0.26f; i += GAME_BRICK_HEIGHT_PERCENTAGE)
//			{
//				addGameObject(new Brick( c, i));
//				brickCounter++;
//				i = i + 0.01;
//			}
//			c = c + 0.01;
//		}
//
//		//brickCounter = 50;
//		break;
//
//	case 3:
//		for(float c = 0.05f; c< 0.95f; c += GAME_BRICK_WIDTH_PERCENTAGE)
//		{
//			for(float i = 0.01f; i < 0.30f; i += GAME_BRICK_HEIGHT_PERCENTAGE)
//			{
//				addGameObject(new Brick( c, i));
//				brickCounter++;
//				i = i + 0.01;
//			}
//			c = c + 0.01;
//		}
//
//		//brickCounter = 60;
//		break;
//
//	default:
//		break;
//	}
//}