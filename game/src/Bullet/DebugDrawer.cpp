#include "..\..\include\Bullet\DebugDrawer.h"
#include "../../include/Singleton.h"


Bullet::DebugDrawer::DebugDrawer(int debugDrawModes, Shader* shader)
	: DebugFlags(debugDrawModes)
{
	//reserve size for all lines of one box
	LineVertices.reserve(6 * 4 * 2);

	static const auto EXPECTED_VERTEX_SIZE = 6 * sizeof(float);
	static_assert(EXPECTED_VERTEX_SIZE == sizeof(LineVertex));

	//create VAO for our points : 3D position & color
	glGenVertexArrays(1, &LinePointVAO);
	glGenBuffers(1, &LinesBuffer.BufferID);

	glBindVertexArray(LinePointVAO);

	glBindBuffer(GL_ARRAY_BUFFER, LinesBuffer.BufferID);

	//position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, EXPECTED_VERTEX_SIZE, (void*)0);
	glEnableVertexAttribArray(0);
	//color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, EXPECTED_VERTEX_SIZE, (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	this->DebugDrawShader = shader;
}

Bullet::DebugDrawer::~DebugDrawer()
{
	glDeleteVertexArrays(1, &LinePointVAO);
	glDeleteBuffers(1, &LinesBuffer.BufferID);
}

void Bullet::DebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
	LineVertices.emplace_back(from, color);
	LineVertices.emplace_back(to, color);
}

void Bullet::DebugDrawer::drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color)
{
}

void Bullet::DebugDrawer::reportErrorWarning(const char* warningString)
{
}

void Bullet::DebugDrawer::draw3dText(const btVector3& location, const char* textString)
{
}

void Bullet::DebugDrawer::setDebugMode(int debugMode)
{
	DebugFlags = debugMode;
}

int Bullet::DebugDrawer::getDebugMode() const
{
	return DebugFlags;
}

void Bullet::DebugDrawer::clearLines()
{
	LineVertices.resize(0);
}

void Bullet::DebugDrawer::flushLines()
{
	DebugDrawShader->use();
	DebugDrawShader->setMat4("viewProjectionMatrix", Singleton::cam->GetViewProj());

	glBindVertexArray(LinePointVAO);

	glBufferData(GL_ARRAY_BUFFER, LineVertices.size() * sizeof(LineVertex), LineVertices.data(), GL_DYNAMIC_DRAW);

	glDrawArrays(GL_LINES, 0, LineVertices.size());

	glBindVertexArray(0);
}
