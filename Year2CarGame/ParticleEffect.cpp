#include "stdafx.h"

ParticleEffect::ParticleEffect()
{

}

ParticleEffect::~ParticleEffect()
{
	if (_VAO != GL_NONE)
	{
		glDeleteVertexArrays(1, &_VAO);
	}
	if (_VBO_Position != GL_NONE)
	{
		glDeleteBuffers(1, &_VBO_Position);
	}	if (_VBO_Size != GL_NONE)
	{
		glDeleteBuffers(1, &_VBO_Position);
	}	if (_VBO_Alpha != GL_NONE)
	{
		glDeleteBuffers(1, &_VBO_Alpha);
	}

	if (_Particles.Positions != nullptr)
	{
		delete[] _Particles.Positions;
		delete[] _Particles.Velocities;
		delete[] _Particles.Alpha;
		delete[] _Particles.Ages;
		delete[] _Particles.Lifetimes;
		delete[] _Particles.Size;
	}
}

bool ParticleEffect::Init(const std::string &textureFile, unsigned int maxParticles, unsigned int rate)
{
	_Texture.LoadTexture(textureFile, false);
	_Texture.SetFiltering(TEXTURE_FILTER_MAG_NEAREST,
		TEXTURE_FILTER_MIN_NEAREST);

	_MaxParticles = maxParticles;
	_Rate = (float) rate;

	_Particles.Positions = new glm::vec3[_MaxParticles];
	_Particles.Velocities = new glm::vec3[_MaxParticles];
	_Particles.Alpha = new float[_MaxParticles];
	_Particles.Ages = new float[_MaxParticles];
	_Particles.Lifetimes = new float[_MaxParticles];
	_Particles.Size = new float[_MaxParticles];

	//Opengl Memory
	glGenVertexArrays(1, &_VAO);
	glGenBuffers(1, &_VBO_Position);
	glGenBuffers(1, &_VBO_Size);
	glGenBuffers(1, &_VBO_Alpha);

	glBindVertexArray(_VAO);

	glEnableVertexAttribArray(0);	//Vertices
	glEnableVertexAttribArray(1);	//Sizes
	glEnableVertexAttribArray(2);	//Alpha

	glBindBuffer(GL_ARRAY_BUFFER, _VBO_Position);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * maxParticles, NULL, GL_DYNAMIC_DRAW);
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glBindBuffer(GL_ARRAY_BUFFER, _VBO_Size);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * maxParticles, NULL, GL_DYNAMIC_DRAW);
	glVertexAttribPointer((GLuint)1, 1, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glBindBuffer(GL_ARRAY_BUFFER, _VBO_Alpha);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * maxParticles, NULL, GL_DYNAMIC_DRAW);
	glVertexAttribPointer((GLuint)2, 1, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
	glBindVertexArray(GL_NONE);
	return true;
}

void ParticleEffect::Update(float elapsed)
{
	int NumToSpawn = (int)_Rate;

	/// Create new particles ///
	while (
		//We have not reached the particle cap and...
		_NumCurrentParticles < _MaxParticles &&
		//We have more particles to generate this frame...
		NumToSpawn > 0)
	{
		_Particles.Alpha[_NumCurrentParticles]		= LerpAlpha.x;
		_Particles.Ages[_NumCurrentParticles]		= 0.0f;
		_Particles.Lifetimes[_NumCurrentParticles]	= RandomRangef(RangeLifetime.x, RangeLifetime.y);
		_Particles.Size[_NumCurrentParticles]		= LerpSize.x;
		_Particles.Positions[_NumCurrentParticles] = vec3(RandomRangef(RangeX.x, RangeX.y), RandomRangef(RangeY.x, RangeY.y), RandomRangef(RangeZ.x, RangeZ.y));

		//send the particle in a random direction, with a velocity between our range
		if (AbsoluteVelocity == false)
		{
			_Particles.Velocities[_NumCurrentParticles] = vec3(RandomRangef(-1.0f, 1.0f), RandomRangef(-1.0f, 1.0f), RandomRangef(-1.0f, 1.0f));
			normalize(_Particles.Velocities[_NumCurrentParticles]);
		}
		else
		{
			_Particles.Velocities[_NumCurrentParticles] = vec3(RandomRangef(0.0f, 1.0f), RandomRangef(0.0f, 1.0f), RandomRangef(0.0f, 1.0f));
		}
		_Particles.Velocities[_NumCurrentParticles].x *= RandomRangef(RangeVelocityX.x, RangeVelocityX.y);
		_Particles.Velocities[_NumCurrentParticles].y *= RandomRangef(RangeVelocityY.x, RangeVelocityY.y);
		_Particles.Velocities[_NumCurrentParticles].z *= RandomRangef(RangeVelocityZ.x, RangeVelocityZ.y);

		//counters...
		_NumCurrentParticles++;
		NumToSpawn--;
	}

	/// Update existing particles ///
	for (unsigned i = 0; i < _NumCurrentParticles; i++)
	{
		_Particles.Ages[i] += elapsed;

		if (_Particles.Ages[i] > _Particles.Lifetimes[i])
		{
			//remove the particle by replacing it with the one at the top of the stack
			_Particles.Alpha[i]		= _Particles.Alpha[_NumCurrentParticles - 1];
			_Particles.Ages[i]		= _Particles.Ages[_NumCurrentParticles - 1];
			_Particles.Lifetimes[i] = _Particles.Lifetimes[_NumCurrentParticles - 1];
			_Particles.Positions[i] = _Particles.Positions[_NumCurrentParticles - 1];
			_Particles.Size[i]		= _Particles.Size[_NumCurrentParticles - 1];
			_Particles.Velocities[i] = _Particles.Velocities[_NumCurrentParticles - 1];

			_NumCurrentParticles--;
			continue;
		}

		_Particles.Positions[i] += _Particles.Velocities[i] * elapsed;
		if (HasGravity)
		{
			_Particles.Positions[i] += glm::vec3(0.0, -0.2 * elapsed, 0.0);
		}

		float interp = _Particles.Ages[i] / _Particles.Lifetimes[i];
		
		_Particles.Alpha[i] = Lerp(LerpAlpha.x, LerpAlpha.y, interp);
		_Particles.Size[i]	= Lerp(LerpSize.x, LerpSize.y, interp);
	}

	//Update opengl on the changes
	glBindBuffer(GL_ARRAY_BUFFER, _VBO_Position);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vec3) * _NumCurrentParticles, &_Particles.Positions[0]);

	glBindBuffer(GL_ARRAY_BUFFER, _VBO_Size);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * _NumCurrentParticles, &_Particles.Size[0]);
	
	glBindBuffer(GL_ARRAY_BUFFER, _VBO_Alpha);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * _NumCurrentParticles, &_Particles.Alpha[0]);

	glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
}

void ParticleEffect::Render()
{
	if (_NumCurrentParticles == 0)
	{
		return;
	}
	_Texture.BindTexture(0);
	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_BLEND);
	glDepthMask(GL_FALSE);

	glBindVertexArray(_VAO);
	glDrawArrays(GL_POINTS, 0, _NumCurrentParticles);
	glBindVertexArray(GL_NONE);

	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
	glBindTexture(GL_TEXTURE_2D, GL_NONE);

}
