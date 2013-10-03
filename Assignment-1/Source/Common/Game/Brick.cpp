#include "Brick.h"
#include "../Constants/Constants.h"
#include "../Screen Manager/ScreenManager.h"
#include "../OpenGL/OpenGL.h"

Brick::Brick()
{

}

Brick::Brick(float positionX, float positionY)
{
	m_BrickTexture = new OpenGLTexture("brickTexture2");

	m_PositionX = (positionX * (ScreenManager::getInstance()->getScreenWidth()));
	m_PositionY = (positionY * (ScreenManager::getInstance()->getScreenHeight()));
}

Brick::~Brick()
{

}

void Brick::update(double delta)
{

}

void Brick::paint()
{
	OpenGLRenderer::getInstance()->drawTexture(m_BrickTexture, m_PositionX, m_PositionY, getWidth(), getHeight());
}

void Brick::reset()
{
	//Get the screen width and height
	float screenWidth = ScreenManager::getInstance()->getScreenWidth();
	float screenHeight = ScreenManager::getInstance()->getScreenHeight();

	setWidth(screenWidth * GAME_BRICK_WIDTH_PERCENTAGE);
	setHeight(screenHeight * GAME_BRICK_HEIGHT_PERCENTAGE);

	setActive(true);
}

const char* Brick::getType()
{
	return GAME_BRICK_TYPE;
}

void Brick::setSize(float aWidth, float aHeight)
{
	setWidth(aWidth);
	setHeight(aHeight);
}

void Brick::setWidth(float aWidth)
{
	m_Width = aWidth;
}

void Brick::setHeight(float aHeight)
{
	m_Height = aHeight;
}

void Brick::getSize(float &aWidth, float &aHeight)
{
	aWidth = getWidth();
	aHeight = getHeight();
}

float Brick::getWidth()
{
	return m_Width;
}

float Brick::getHeight()
{
	return m_Height;
}

void Brick::setActive(bool active)
{
	m_IsActive = active;
}