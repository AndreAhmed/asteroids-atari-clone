#include "cinder/app/AppBasic.h"
#include "Ship.h"
#include "Asteroid.h"
#include "Particle.h"
#include "Enemy.h"
#include "cinder/Text.h"
#include "cinder/gl/Texture.h"
#include <string>
#include "cinder/params/Params.h"
#include "cinder/audio/Output.h"
#include "cinder/audio/Io.h"
#include <irrKlang.h>

using namespace irrklang;

using namespace ci;
using namespace ci::app;
using namespace std;

#define Game_Demo 0 
#define GAME_Play 1
#define GAME_End  2

extern bool toggleCircles;
bool shipCollision = false;
void staticMouseDownHandler( MouseEvent event );
void staticMouseUpHandler( MouseEvent event );

// We'll create a new Cinder Application by deriving from the BasicApp class
class BasicApp : public AppBasic {
public:

	// Cinder will always call this function whenever the user drags the mouse
	void keyDown( KeyEvent event );
	void keyUp( KeyEvent event );
	void setup();
	void resize( ResizeEvent event );
	void shutdown();
	void prepareSettings( Settings *settings );
	void update();
	// Cinder calls this function 30 times per second by default
	void draw();

	bool checkCollision(Vec2f center1, Vec2f center2, float radius1, float radius2);

	Enemy					enemy;
	std::vector<Ship>		ships;
	Ship					currentShip;
	std::list<Asteroid>		asteroids;
	audio::SourceRef		mAudioShoot;
	audio::SourceRef		mAudioThrust;
	audio::SourceRef		mAudioExplosion;

	gl::Texture				mTextTexture;
	Vec2f					mSize;
	Font					mFont;
	int						game_state;
	int						score;
	int						firingTime; 
	int						numberAsteriods;
	params::InterfaceGl		mParams;
	ParticleManager		    particleManager;
	ParticleLineManager	    particleLineManager;
	// start the sound engine with default parameters
	ISoundEngine*			engine;
	int						delay;
 
};



void BasicApp::setup()
{
	this->setFullScreen(false);


	// start the sound engine with default parameters
	engine = createIrrKlangDevice();

	if (!engine)
	{
	 	return; 
	}

	mFont		    = Font( "Times New Roman", 32 );
	mParams		    = params::InterfaceGl( "App parameters", Vec2i( 200, 400 ) );
	game_state		= Game_Demo;
	firingTime		= 0;
	score			= 0;
	numberAsteriods = 0;
	delay			= 20;
	currentShip.Init(Vec2f((float)getWindowWidth()/2,(float)getWindowHeight()/2)); 
	particleManager.Init_Particles();
	enemy.Init();

	for (int i=0; i<5;i++)
	{
		Ship ship;
		ship.Init(Vec2f(10+20*i,70));
		ships.push_back(ship);
	}
	for (int i=0; i<8; i++)
	{
		Asteroid asteroid;
		int shape = rand()%2+1;
		asteroid.Init(1, 0.5);
		asteroids.push_back(asteroid);

	}
}

void BasicApp::keyDown( KeyEvent event )
{


	if(  event.getCode() == KeyEvent::KEY_RIGHT)
	{
		if(game_state == GAME_Play)
		{
		 currentShip.right = true;
		}
	}
	if( event.getCode() == KeyEvent::KEY_LEFT)
	{
		if(game_state == GAME_Play)
		{
		 currentShip.left = true;
		}
	}
	if( event.getCode() == KeyEvent::KEY_UP)
	{
		if(game_state == GAME_Play)
		{
			if(currentShip.m_state == Ship::Ship_Alive)
			{
				engine->play2D("thrust.wav");
				currentShip.m_Thrust = true;
				currentShip.up = true;
			}
		}
	}

	if(event.getCode() ==KeyEvent::KEY_SPACE)
	{

		if(game_state == GAME_Play)
		{
			if(currentShip.m_state == Ship::Ship_Alive)
			{
				engine->play2D("shot.wav");

				if(currentShip.up)
				{
					currentShip.m_Thrust = true;
				}
				currentShip.fire = true;
			}
		}
	}
	if(event.getChar() == KeyEvent::KEY_1)
	{
		delay-=10;
		if(delay<0)
			delay=1;

	}

	if(event.getChar() == KeyEvent::KEY_2)
	{
		delay+=10;

	}

	if(event.getChar()== KeyEvent::KEY_c)
	{
		toggleCircles = !toggleCircles;
	}
	 if(event.getChar()== KeyEvent::KEY_s)
	 {
		 shipCollision =!shipCollision;
	 }
}

void BasicApp::keyUp( KeyEvent event )
{

	if(  event.getCode() == KeyEvent::KEY_RIGHT)
	{
		if(game_state == GAME_Play)
		{
		currentShip.right = false;
		}
	}
	if( event.getCode() == KeyEvent::KEY_LEFT)
	{
		if(game_state == GAME_Play)
		{
		currentShip.left = false;	
		}
	}

	if( event.getCode() == KeyEvent::KEY_UP)
	{
		if(game_state == GAME_Play)
		{
		currentShip.m_Thrust = false;
		currentShip.up = false; 
		}
	}

	if(event.getCode() ==KeyEvent::KEY_SPACE)
	{
		if(game_state == GAME_Play)
		{
		if(!currentShip.up)
		{
			currentShip.m_Thrust = false;;
		}
		currentShip.fire = false;
		currentShip.m_fire = 0;
		}
		else if ( game_state == Game_Demo)
		{
			game_state = GAME_Play;
		}
	}
}


void BasicApp::update()
{

	if (game_state == Game_Demo)
	{

		for (list<Asteroid>::iterator itr = asteroids.begin(); itr!=asteroids.end(); itr++)
		{
			itr->Update();

		}

	}
	else if(game_state == GAME_Play)
	{
		for (list<Asteroid>::iterator itr = asteroids.begin(); itr!=asteroids.end(); itr++)
		{
			itr->Update();

		}

		for (list<Bullet>::iterator itr_bullet = currentShip.m_Bullets.begin(); itr_bullet!=currentShip.m_Bullets.end();)
		{

			for (list<Asteroid>::iterator itr_astroid = asteroids.begin(); itr_astroid!=asteroids.end();)
			{
				if(checkCollision(itr_bullet->getCenter(),itr_astroid->getCenter(), itr_bullet->getRadius(), itr_astroid->getRadius()))
				{

					particleManager.Start_Explosion(itr_astroid->getCenter(),10,1.0f,1.0f);
				
					engine->play2D("explode_high.wav");
				

					if(itr_astroid->m_size >= 0.5)
					{
						Asteroid asteroid_1, asteroid_2;

						asteroid_1.m_state = Asteroid_Medium;
						asteroid_1.Init(rand()%2+1, 0.3);
						asteroid_1.m_Vel.y = -itr_astroid->m_Vel.y*1.5;
						asteroid_1.m_Vel.x = -itr_astroid->m_Vel.x*1.5;
						asteroid_1.m_Pos = itr_astroid->m_Pos;

						asteroid_2.m_state = Asteroid_Medium;
						asteroid_2.Init(rand()%2+1, 0.3);
						asteroid_2.m_Vel.y = itr_astroid->m_Vel.y*1.5;
						asteroid_2.m_Vel.x = itr_astroid->m_Vel.x*1.5;
						asteroid_2.m_Pos = itr_astroid->m_Pos;


						asteroids.push_back(asteroid_1);
						asteroids.push_back(asteroid_2);


						itr_astroid = asteroids.erase(itr_astroid);
						itr_astroid->m_bHit = true;

						score +=100;

					}
					else if(itr_astroid->m_state == Asteroid_Medium)
					{
						Asteroid asteroid_1, asteroid_2;
						asteroid_1.m_state = Asteroid_Small;
						asteroid_1.Init(rand()%2+1, 0.1);
						asteroid_1.m_Vel.y = -itr_astroid->m_Vel.y*2;
						asteroid_1.m_Vel.x = -itr_astroid->m_Vel.x*2;
						asteroid_1.m_Pos = itr_astroid->m_Pos;
						asteroid_2.m_state = Asteroid_Small;
						asteroid_2.Init(rand()%2+1, 0.1);
						asteroid_2.m_Vel.y = itr_astroid->m_Vel.y*2;
						asteroid_2.m_Vel.x = itr_astroid->m_Vel.x*2;
						asteroid_2.m_Pos = itr_astroid->m_Pos;
						asteroids.push_back(asteroid_1);
						asteroids.push_back(asteroid_2);
						itr_astroid = asteroids.erase(itr_astroid);
						itr_astroid->m_bHit = true;

						score +=100;

					}
					else
					{
						score +=50;
						itr_astroid->m_bHit = true; 
						itr_astroid = asteroids.erase(itr_astroid);
					}

					itr_bullet->m_Hit = true;

					break;
				}
				else
				{
					itr_astroid++;

				}
			}
			if(itr_bullet->m_Hit)
			{
				itr_bullet = currentShip.m_Bullets.erase(itr_bullet);
			}
			else
			{
				itr_bullet++;

			}

		}

		if(shipCollision)
		{
			for (list<Asteroid>::iterator itr_astroid = asteroids.begin(); itr_astroid!=asteroids.end(); )
			{
				if(currentShip.m_state == Ship::Ship_Alive)
				{

					if(checkCollision(itr_astroid->getCenter(), currentShip.getCenter(), itr_astroid->getRadius(), currentShip.getRadius() ))
					{

						particleManager.Start_Explosion(currentShip.getCenter(),10,0.3f,1.0f);
						particleLineManager.Init(currentShip.getCenter());

						engine->play2D("explode_high.wav");

						currentShip.m_state = Ship::Ship_Dead;
						currentShip.m_Pos.x = -getWindowWidth()/2;
						currentShip.m_Pos.y = -getWindowHeight()/2;

						if(ships.size()>0)
						{
							ships.pop_back();
						}

						if(itr_astroid->m_size >= 0.5)
						{
							Asteroid asteroid_1, asteroid_2;

							asteroid_1.m_state = Asteroid_Medium;
							asteroid_1.Init(rand()%2+1, 0.3);
							asteroid_1.m_Vel.y = -itr_astroid->m_Vel.y*1.5;
							asteroid_1.m_Vel.x = -itr_astroid->m_Vel.x*1.5;
							asteroid_1.m_Pos = itr_astroid->m_Pos;

							asteroid_2.m_state = Asteroid_Medium;
							asteroid_2.Init(rand()%2+1, 0.3);
							asteroid_2.m_Vel.y = itr_astroid->m_Vel.y*1.5;
							asteroid_2.m_Vel.x = itr_astroid->m_Vel.x*1.5;
							asteroid_2.m_Pos = itr_astroid->m_Pos;


							asteroids.push_back(asteroid_1);
							asteroids.push_back(asteroid_2);


							itr_astroid = asteroids.erase(itr_astroid);
							itr_astroid->m_bHit = true;

							score +=100;

						}
						else if(itr_astroid->m_state == Asteroid_Medium)
						{
							Asteroid asteroid_1, asteroid_2;

							asteroid_1.m_state = Asteroid_Small;
							asteroid_1.Init(rand()%2+1, 0.1);
							asteroid_1.m_Vel.y = -itr_astroid->m_Vel.y*2;
							asteroid_1.m_Vel.x = -itr_astroid->m_Vel.x*2;
							asteroid_1.m_Pos = itr_astroid->m_Pos;

							asteroid_2.m_state = Asteroid_Small;
							asteroid_2.Init(rand()%2+1, 0.1);
							asteroid_2.m_Vel.y = itr_astroid->m_Vel.y*2;
							asteroid_2.m_Vel.x = itr_astroid->m_Vel.x*2;
							asteroid_2.m_Pos = itr_astroid->m_Pos;


							asteroids.push_back(asteroid_1);
							asteroids.push_back(asteroid_2);


							itr_astroid = asteroids.erase(itr_astroid);
							itr_astroid->m_bHit = true;

							score +=100;
						}
						else
						{
							score +=50;
							itr_astroid->m_bHit = true; 
							itr_astroid = asteroids.erase(itr_astroid);
						}
						break;
					}
					else
					{
						itr_astroid++;

					} 
				}
				else
				{
					itr_astroid++;
				}
			}	
		}
	 
		currentShip.m_saveField = currentShip.m_state == Ship::Ship_Dead;

		if (currentShip.m_saveField)
		{
			for (list<Asteroid>::iterator itr_astroid = asteroids.begin(); itr_astroid!=asteroids.end(); itr_astroid++) 
			{
				float distance = itr_astroid->getCenter().distance(Vec2f(getWindowWidth() / 2, getWindowHeight() / 2));
				if (distance < 100) 
				{
					currentShip.m_saveField = false;
					break;
				}
			}        
		}

		if(ships.size()<1)
		{
			game_state = GAME_End;
		}
		if(asteroids.size()<1)
		{
			game_state = GAME_End;
		}

		currentShip.Update();
		enemy.Update();
	}


	else if ( game_state == GAME_End)
	{



	}

	Sleep(delay);

}


void BasicApp::draw()
{

	// this pair of lines is the standard way to clear the screen in OpenGL
	gl::clear( Color( 0.0f, 0.0f, 0.0f ) );
	gl::setMatricesWindow( getWindowSize() );

	if(game_state == Game_Demo)
	{
		gl::enableAlphaBlending( true );
		for (list<Asteroid>::iterator itr = asteroids.begin(); itr!=asteroids.end(); itr++)
		{
			if(itr->m_bHit)
			{

			}
			itr->Draw();
		}

		Font font  = Font( "Times New Roman", 50 );
		gl::drawString("Atari Asteroids Clone" , Vec2f(app::getWindowWidth()/4,app::getWindowHeight()/4),cinder::ColorA(1,1,1,1), font);
		gl::drawString("Presse SpaceBar to Play" , Vec2f(app::getWindowWidth()/4,app::getWindowHeight()/2 + 100),cinder::ColorA(1,1,1,1), font);

	}
	else if(game_state == GAME_Play)
	{

		for (list<Asteroid>::iterator itr = asteroids.begin(); itr!=asteroids.end(); itr++)
		{
			if(itr->m_bHit)
			{

			}
			itr->Draw();

		}
		enemy.Draw();
		particleManager.Move();
		particleManager.Draw();
		particleLineManager.Update();
		particleLineManager.Draw();
		currentShip.Draw();

		for (std::vector<Ship>::iterator itrShip = ships.begin(); itrShip!=ships.end();itrShip++)
		{
			if(itrShip->m_state== Ship::Ship_Alive)
			{
				itrShip->Draw();
			}
		}
		stringstream scoreString;
		scoreString<< score;
		gl::drawString("   " + scoreString.str(), Vec2f(10,10),cinder::ColorA(1,1,1,1), mFont);
	}

	else if (game_state == GAME_End)
	{

		gl::drawString("Game Over", Vec2f(getWindowWidth()/2,getWindowHeight()/2),cinder::ColorA(1,1,1,1), mFont);
	}
}

bool BasicApp::checkCollision( Vec2f center1, Vec2f center2, float radius1, float radius2 )
{
	float radius = radius1 + radius2;
	float dx = center1.x - center2.x;
	float dy = center1.y - center2.y;

	if(( dx * dx )  + ( dy * dy )  < radius*radius)
		return true;
	else
	{
		return false;
	}
}

void BasicApp::resize( ResizeEvent event )
{
	gl::setMatricesWindow(getWindowSize());
}

void BasicApp::shutdown()
{
	engine->drop();

}

void BasicApp::prepareSettings( Settings *settings )
{
	 
		settings->setWindowSize( 800, 800 );
		settings->setFrameRate( 60.0f );
		settings->setFullScreen( false );
	 
}

// This line tells Flint to actually create the application
CINDER_APP_BASIC( BasicApp, RendererGl )