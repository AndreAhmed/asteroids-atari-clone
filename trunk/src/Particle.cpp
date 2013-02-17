
#include "Particle.h"
 
using namespace ci;

void ParticleManager::Init_Particles()
{
	for (int i=0; i<10; i++)
	{
		Particle particle;
		particle.mAge = 0;
		particle.mPosition = Vec2f(0,0);
		particle.mVelocity = Vec2f(0,0);
		mParticles.push_back(particle);
	}
	for (int i = 0; i<mParticles.size(); i++)
	{
		mParticles[i].state =  Particle_Dead;
	}
}

int ParticleManager::Start_Particles( ci::Vec2f pos, ci::Vec2f vel, int age )
{
	for (int i = 0; i<mParticles.size(); i++)
	{
		if (mParticles[i].state == Particle_Dead)
		{

			mParticles[i].state = Particle_Alive;
			mParticles[i].mVelocity = vel;
			mParticles[i].mPosition = pos;
			mParticles[i].mAge = age;
			return i;
		}
	}
	return -1;
}

void ParticleManager::Start_Explosion( ci::Vec2f pos, int num_particles, float speed, float scale )
{
	 
	// iterate and start particles from the blast emitter postiion
	int index;
	for (index = 0; index < mParticles.size(); index++)
	{
		double dir = RAND_RANGE(0,359);

		float explosion_speed = (float)(4.0*(float)app::getWindowWidth()/800) * ((float)RAND_RANGE(75,125))/(float)100;
		double xv = explosion_speed*speed*cos(dir);
		double yv = explosion_speed*speed*sin(dir);

		// start the particle
		int id  = Start_Particles(pos,ci::Vec2f(xv, yv),15*scale);
	 
		// if id isn't valid, no more particles, no need to continue
		if (id==-1)
			return;
	}
}

void ParticleManager::Move()
{
	for (int i=0; i<mParticles.size(); i++)
	{
		if(mParticles[i].state == Particle_Alive)
		{
			mParticles[i].mPosition+= mParticles[i].mVelocity;

			// update life counter
			if (--mParticles[i].mAge < 0)
			{

				mParticles[i].state  = Particle_Dead;
			}
		}
	}
 
}

void ParticleManager::Draw()
{
	for (int i=0; i<mParticles.size(); i++)
	{
		if(mParticles[i].state == Particle_Alive)
		{
			gl::color(1,1,1);
			gl::drawSolidCircle(mParticles[i].mPosition, 1);
		}

		else {
			return;
		}
	}
}
