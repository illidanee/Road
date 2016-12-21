#ifndef __FRUSTUM_H__
#define __FRUSTUM_H__

#include "gl/glew.h"

class Frustum
{
private:
	GLuint mVBO, mIBO, mProgram;
	GLuint mPosLocation, mMLocation, mVLocation, mPLocation;

public:
	Frustum();

	void InitProgram();
	void InitPerspective(float fov, float aspect, float zNear, float zFar);
	void Draw(float* m, float* v, float* p);
};

#endif