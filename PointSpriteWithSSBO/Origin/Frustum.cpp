#include "Frustum.h"

#include <math.h>
#include "Misc.h"

Frustum::Frustum()
{
	
}

void Frustum::InitProgram()
{
	mProgram = CreateGPUProgram("../res/shader/Frustum.vs", "../res/shader/Frustum.fs");

	mPosLocation = glGetAttribLocation(mProgram, "pos");

	mMLocation = glGetUniformLocation(mProgram, "M");
	mVLocation = glGetUniformLocation(mProgram, "V");
	mPLocation = glGetUniformLocation(mProgram, "P");
}

void Frustum::InitPerspective(float fov, float aspect, float zNear, float zFar)
{
	float halfFov = fov / 2.0f;
	float radianHalfFov = halfFov * 3.14f / 180.0f;
	float tagHalfFov = sinf(halfFov) / cosf(halfFov);

	float yNear = tagHalfFov * zNear;
	float xNear = aspect * yNear;

	float yFar = tagHalfFov * zFar;
	float xFar = aspect * yFar;

	float vertexes[] = {
		-xNear, -yNear, -zNear,
		xNear, -yNear, -zNear,
		xNear, yNear, -zNear,
		-xNear, yNear, -zNear,
		-xFar, -yFar, -zFar,
		xFar, -yFar, -zFar,
		xFar, yFar, -zFar,
		-xFar, yFar, -zFar
	};

	unsigned int indexes[] = {
		0,1,1,2,2,3,3,0,
		4,5,5,6,6,7,7,4,
		0,4,3,7,2,6,1,5
	};

	mVBO = CreateGLBufferObject(GL_ARRAY_BUFFER, sizeof(float) * 3 * 8, GL_STATIC_DRAW, vertexes);
	mIBO = CreateGLBufferObject(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 3 * 8, GL_STATIC_DRAW, indexes);
}

void Frustum::InitOrtho(float left, float right, float bottom, float top, float zNear, float zFar)
{
	float vertexes[] = {
		left, bottom, -zNear,
		right, bottom, -zNear,
		right, top, -zNear,
		left, top, -zNear,
		left, bottom, -zFar,
		right, bottom, -zFar,
		right, top, -zFar,
		left, top, -zFar,
	};

	unsigned int indexes[] = {
		0,1,1,2,2,3,3,0,
		4,5,5,6,6,7,7,4,
		0,4,3,7,2,6,1,5
	};

	mVBO = CreateGLBufferObject(GL_ARRAY_BUFFER, sizeof(float) * 3 * 8, GL_STATIC_DRAW, vertexes);
	mIBO = CreateGLBufferObject(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 3 * 8, GL_STATIC_DRAW, indexes);
}

void Frustum::Draw(float* m, float* v, float* p)
{
	glUseProgram(mProgram);
	glUniformMatrix4fv(mMLocation, 1, GL_FALSE, m);
	glUniformMatrix4fv(mVLocation, 1, GL_FALSE, v);
	glUniformMatrix4fv(mPLocation, 1, GL_FALSE, p);

	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glEnableVertexAttribArray(mPosLocation);
	glVertexAttribPointer(mPosLocation, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
	glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glUseProgram(0);
}