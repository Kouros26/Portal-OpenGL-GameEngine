#pragma once

#include "btBulletCollisionCommon.h"
#include "Mat4/Mat4.h"
#include <vector>
#include "../../../libGL/include/Ressource/AssimpShader.h"
#include <glad/glad.h>

using namespace lm;

namespace Bullet
{
	class DebugDrawer : public btIDebugDraw
	{
		struct LineVertex
		{
			LineVertex() = default;

			LineVertex(btVector3 const& pos, btVector3 const& color) :
				Position{ (float)pos.x(), (float)pos.y(), (float)pos.z()},
				Color{ (float)color.x(), (float)color.y(), (float)color.z()}
			{}


			vec3 Position;
			vec3 Color;
		};

		struct Buffer
		{
			GLuint BufferID = 0;
			size_t Size = 0;
			size_t Capacity = 0;
		};

	public:
		DebugDrawer(int debugDrawModes, Shader* shader);
		~DebugDrawer();

		void drawLine(const btVector3& from, const btVector3& to, const btVector3& color) override;
		void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color) override;
		void reportErrorWarning(const char* warningString) override;
		void draw3dText(const btVector3& location, const char* textString) override;
		void setDebugMode(int debugMode) override;
		int getDebugMode() const override;
		void clearLines() override;
		void flushLines() override;

		void setViewProjection(const mat4& mat) {}


	private:
		int DebugFlags{ 0 };
		Buffer LinesBuffer;
		GLuint LinePointVAO = 0;
		std::vector<LineVertex> LineVertices;
		Shader* DebugDrawShader;
		mat4 ViewProjectionMatrix;
	};
}

