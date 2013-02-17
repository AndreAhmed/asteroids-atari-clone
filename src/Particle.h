
#include <vector>

#include "cinder/Vector.h" 
#include "cinder/Rand.h"
#include "cinder/gl/gl.h"
#include "cinder/app/AppBasic.h"

#define RAND_RANGE(x,y) ( (x) + (rand()%((y)-(x)+1)))

enum Particle_State {Particle_Dead, Particle_Alive};

class ParticleLine
{
public:
	ParticleLine() { }
public:
	ci::Vec2f mPosition;
	ci::Vec2f mVelocity;
	float mAge;
	int   state;

};
class ParticleLineManager
{
public:
	ParticleLineManager(){}
	void Init(ci::Vec2f Pos)
	{
		double dir = 0;
		for (int i=0; i<5; i++)
		{
			ParticleLine particle;

			dir +=90;
			double xv = 0.4*cos(dir);
			double yv = 0.4*sin(dir);
			particle.mVelocity.x = xv;
			particle.mVelocity.y = yv;
			particle.mPosition   = Pos;
			particle.state       =  Particle_Alive;
			particle.mAge = RAND_RANGE(75,190);
			mParticles.push_back(particle);
		}

	}
	void Draw()
	{
		if(mParticles.size()>0)
		{
			if(mParticles[0].state == Particle_Alive)
			{
				ci::gl::drawLine(ci::Vec2f(mParticles[0].mPosition), ci::Vec2f(mParticles[0].mPosition.x - 10, mParticles[0].mPosition.y +10) );
			}
			if(mParticles.size()>1)
			{
				if(mParticles[1].state == Particle_Alive)
				{
					ci::gl::drawLine(ci::Vec2f(mParticles[1].mPosition), ci::Vec2f(mParticles[1].mPosition.x + 10, mParticles[1].mPosition.y -10) );
				}
			}
			if(mParticles.size()>2)
			{
				if(mParticles[2].state == Particle_Alive)
				{
					ci::gl::drawLine(ci::Vec2f(mParticles[2].mPosition), ci::Vec2f(mParticles[2].mPosition.x + 10, mParticles[2].mPosition.y -10) );
				}
			}
			if(mParticles.size()>3)
			{
				if(mParticles[3].state == Particle_Alive)
				{
					ci::gl::drawLine(ci::Vec2f(mParticles[3].mPosition), ci::Vec2f(mParticles[3].mPosition.x + 10, mParticles[3].mPosition.y -10) );
				}
			}
			if(mParticles.size()>4)
			{
				if(mParticles[4].state == Particle_Alive)
				{
					ci::gl::drawLine(ci::Vec2f(mParticles[4].mPosition), ci::Vec2f(mParticles[4].mPosition.x + 10, mParticles[4].mPosition.y -10) );
				}
			}
			if(mParticles.size()>5)
			{
				if(mParticles[5].state == Particle_Alive)
				{
					ci::gl::drawLine(ci::Vec2f(mParticles[5].mPosition), ci::Vec2f(mParticles[5].mPosition.x + 10, mParticles[5].mPosition.y -10) );
				}
			}
		}

	}
	void Update()
	{
		if(mParticles.size()>0)
		{

			for(std::vector<ParticleLine>::iterator iterParticle=mParticles.begin(); iterParticle!=mParticles.end();iterParticle++)
			{
				if(--iterParticle->mAge <0)
				{
					iterParticle->state = Particle_Dead;
				} 
				if(iterParticle->state == Particle_Alive)
				{

					iterParticle->mPosition+=iterParticle->mVelocity;
				}
			}
		}

		int c=0;
		for (int i=0; i<mParticles.size();i++)
		{
			if(mParticles[i].state == Particle_Dead)
			{
				c++;
			}

		}

		if(c==mParticles.size())
		{

			mParticles.clear();
		}
	}



public:

	std::vector<ParticleLine> mParticles;

};
class Particle {

public:
	Particle() { } 
	Particle( ci::Vec2f aPosition )
		: mPosition( aPosition ), mVelocity( ci::Vec2f::zero() )
	{}
public:
	ci::Vec2f mPosition, mVelocity;
	float mAge;
	int state;

};

class ParticleManager
{

public:
	ParticleManager() { }
	void Init_Particles();
	void Draw_Particles();
	int  Start_Particles(ci::Vec2f pos, ci::Vec2f vel, int age);
	void Start_Explosion(ci::Vec2f pos, int num_particles, float speed, float scale);
	void Start_ShipExplosion(ci::Vec2f pos, float speed);
	void Draw();
	void Move();
public:

	std::vector<Particle> mParticles;

};
