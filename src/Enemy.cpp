#include "Enemy.h"
#include "cinder/app/App.h"
#include "cinder/gl/gl.h"
#include "cinder/Rand.h"

using namespace ci;
#define RAND_RANGE(x,y) ( (x) + (rand()%((y)-(x)+1)))

void Enemy::Draw()
{
	if(m_State == Enemy_Alive)
	{
		float ratio = (float)app::getWindowWidth()/app::getWindowHeight();
	
		gl::pushMatrices();

		gl::translate(m_Pos*ratio);
		gl::scale(0.5f,0.5f);
		gl::drawLine(Vec2f(40, 0), Vec2f(60, 0));
		gl::drawLine(Vec2f(60, 0), Vec2f(70, 20));
		gl::drawLine(Vec2f(70, 20),Vec2f(100, 40));
		gl::drawLine(Vec2f(100, 40),Vec2f(70, 60));
		gl::drawLine(Vec2f(70, 60),Vec2f(30, 60));
		gl::drawLine(Vec2f(30, 60),Vec2f(0, 40));
		gl::drawLine(Vec2f(0, 40),Vec2f(30, 20));
		gl::drawLine(Vec2f(30, 20),Vec2f(40, 0));
		gl::drawLine(Vec2f(70,20), Vec2f(30,20));
		gl::drawLine(Vec2f(100,40), Vec2f(0,40));

		gl::popMatrices();
	}
}

Enemy::~Enemy()
{

}

Enemy::Enemy()
{

}

void Enemy::Update()
{

	/*if (m_State==Enemy_Dead)
		return;*/
 
	// translate Enemy
 
	m_Pos+=m_Vel;
	m_Counter++;
	--m_LifeSpan;
	// Move the ship up, down, or keep it level every x% of the screen.
	if (m_Counter > 250 )
	{
	
		switch((int) Rand::randInt(0, 3)) {
		case 0:m_Vel.y = 0.5; break;
		case 1:m_Vel.y = 0; break;
		case 2:m_Vel.y = -0.5; break;
		}

		m_Counter = 0;
	}

	if (m_Pos.x > app::getWindowWidth()+50)
	{
		m_Pos.x = 0;
		if(m_LifeSpan<0)
		{
			 
			m_Counter=0;
		 
			m_State = Enemy_Dead;
		}
		
	}

	else if (m_Pos.x < -50)
	{
		if(m_LifeSpan<0)
		{
		 
			m_Counter=0;
			
			m_State = Enemy_Dead;
		}
		m_Pos.x = app::getWindowWidth()+m_Pos.x;

	}
	if (m_Pos.y < -50)
	{
	if(m_LifeSpan<0)
	{
		 
		m_Counter=0;
		 
	}

		m_Pos.y = app::getWindowHeight()+m_Pos.y;
	}
	else if (m_Pos.y > app::getWindowHeight()+50)
	{
		if(m_LifeSpan<0)
		{
			 
			m_Counter=0;
			 
			m_State = Enemy_Dead;
		}
		m_Pos.y =  0;
		 
	}

	if(m_State == Enemy_Dead)
	{
		m_LifeSpan = 200;
		m_Counter  = 0;
		m_State = Enemy_Alive;
	}

	////// test for off screen, wrap around if needed...
	//if (m_Pos.x > app::getWindowWidth())
	//{
	//	// reset position
	//	m_Pos.x  = 0;

	//	// check lifespan
	//	if (m_LifeSpan < 0)
	//		m_State = Enemy_Dead;

	//} // end if
	//else
	//	if (m_Pos.x < 0)
	//	{
	//		// reset position
	//		 m_Pos.x = app::getWindowWidth();

	//		if (m_LifeSpan < 0)
	//			m_State = Enemy_Dead;

	//	} // end if

	//	if (m_Pos.y > app::getWindowHeight())
	//	{
	//		// reset position

	//		if (m_LifeSpan < 0)
	//			m_State = Enemy_Dead;

	//		m_Pos.y = 0;
	//	} // end if
	//	else
	//		if (m_Pos.y < 0)
	//		{
	//			// reset position
	//			m_Pos.y = app::getWindowHeight();

	//			if (m_LifeSpan < 0)
	//				m_State = Enemy_Dead;
	//		} // end if
	//		 
	//		if (m_State == Enemy_Dead)
	//		{
	//			 

	//		} // end if
}

void Enemy::Init()
{
	 
	int range = Rand::randInt(1,5);
	m_State = Enemy_Alive;

	int left = 0;
	int right = app::getWindowWidth() - left;
	int top = app::getWindowHeight()/2;
	int bottom =app::getWindowHeight() - top;
	
	switch( range )
	{
	case 1:
		m_Pos.x =  left;
		m_Pos.y =  app::getWindowHeight()/9;
		m_Vel.x = 1;
		break;
	case 2:
		m_Pos.x    =  left;
		m_Pos.y    =  bottom;
		m_Vel.x    = 1;
		break;

	case 3:
		m_Pos.x     =  right;
		m_Pos.y     =  app::getWindowHeight()/9;
		m_Vel.x     = -1;
		break;

	case 4:
		m_Pos.x     =  right;
		m_Pos.y     =  bottom;
		m_Vel.x		= -1;
		break;
	default:
		break;
	}
 	
	// compute velocity based on direction and level
	m_Vel.x *=2;
	m_Vel.y = 0;
	m_Counter = 200;
	m_Dir = m_Counter;
	m_SavedDy = 2;
	m_lastDirChange = m_Counter;
	m_LifeSpan = 300;
}

