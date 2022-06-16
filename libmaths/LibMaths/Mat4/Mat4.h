#pragma once

#include "../Vec4/Vec4.h"

namespace lm
{
	template <typename T> class Mat4
	{
	private:
		Vec4<T> matrix[4];

	public:
		static const Mat4<T> identity;

		Mat4()
		{
			this->matrix[0] = Vec4<T>(1, 0, 0, 0);
			this->matrix[1] = Vec4<T>(0, 1, 0, 0);
			this->matrix[2] = Vec4<T>(0, 0, 1, 0);
			this->matrix[3] = Vec4<T>(0, 0, 0, 1);
		}

		Mat4(const T init)
		{
			this->matrix[0] = Vec4<T>(init, 0, 0, 0);
			this->matrix[1] = Vec4<T>(0, init, 0, 0);
			this->matrix[2] = Vec4<T>(0, 0, init, 0);
			this->matrix[2] = Vec4<T>(0, 0, 0, 1);
		}

		Mat4(const Vec4<T>& v1, const Vec4<T>& v2, const Vec4<T>& v3, const Vec4<T>& v4)
		{
			this->matrix[0] = v1;
			this->matrix[1] = v2;
			this->matrix[2] = v3;
			this->matrix[3] = v4;
		}

		Mat4(const Mat4<T>& mat4)
		{
			for (unsigned int i = 0; i < 4; i++)
				this->matrix[i] = mat4.matrix[i];
		}

		Mat4(Mat4<T>&& mat4) noexcept
		{
			for (unsigned int i = 0; i < 4; i++)
				this->matrix[i] = std::move(mat4.matrix[i]);
		}

		Mat4<T>& operator=(const Mat4<T>& mat4)
		{
			if (this == &mat4)
				return *this;

			for (unsigned int i = 0; i < 4; i++)
				this->matrix[i] = mat4.matrix[i];

			return *this;
		}

		Mat4<T>& operator=(Mat4<T>&& mat4) noexcept
		{
			if (this == &mat4)
				return *this;

			for (unsigned int i = 0; i < 4; i++)
				this->matrix[i] = std::move(mat4.matrix[i]);

			return *this;
		}

		const Vec4<T> operator[](int idx) const
		{
			if (idx >= 0 && idx < 4)
				return this->matrix[idx];
			return this->matrix[0];
		}

		Vec4<T>& operator[](int idx)
		{
			if (idx >= 0 && idx < 4)
				return this->matrix[idx];
			return this->matrix[0];
		}

		const T operator[](const char* idx) const
		{
			unsigned int vecIdx = idx[1] - '1';
			switch (idx[0])
			{
			case 'x': return this->matrix[vecIdx].X();
			case 'y': return this->matrix[vecIdx].Y();
			case 'z': return this->matrix[vecIdx].Z();
			case 'w': return this->matrix[vecIdx].W();

			default: return this->matrix[0].X();
			}
		}

		T& operator[](const char* idx)
		{
			unsigned int vecIdx = idx[1] - '0';
			switch (idx[0])
			{
			case 'x': return this->matrix[vecIdx].X();
			case 'y': return this->matrix[vecIdx].Y();
			case 'z': return this->matrix[vecIdx].Z();
			case 'w': return this->matrix[vecIdx].W();

			default: return this->matrix[0].X();
			}
		}

		Mat4<T> operator*(const Mat4<T>& mat4) const
		{
			Mat4<T> newMat4;

			for (unsigned int i = 0; i < 4; i++)
			{
				Vec4<T> vec4;
				for (unsigned int j = 0; j < 4; j++)
				{
					vec4[j] = this->matrix[0][j] * mat4.matrix[i].X()
						+ this->matrix[1][j] * mat4.matrix[i].Y()
						+ this->matrix[2][j] * mat4.matrix[i].Z()
						+ this->matrix[3][j] * mat4.matrix[i].W();
				}
				newMat4.matrix[i] = vec4;
			}

			return newMat4;
		}

		Vec4<T> operator*(const Vec4<T>& vec4) const
		{
			Vec4<T> newVec4;
			newVec4.X() = (this->matrix[0][0] * vec4.X()) + (this->matrix[1].X() * vec4.Y()) + (this->matrix[2].X() * vec4.Z()) + (this->matrix[3].X() * vec4.W());
			newVec4.Y() = (this->matrix[0][1] * vec4.X()) + (this->matrix[1].Y() * vec4.Y()) + (this->matrix[2].Y() * vec4.Z()) + (this->matrix[3].Y() * vec4.W());
			newVec4.Z() = (this->matrix[0].Z() * vec4.X()) + (this->matrix[1].Z() * vec4.Y()) + (this->matrix[2].Z() * vec4.Z()) + (this->matrix[3].Z() * vec4.W());
			newVec4.W() = (this->matrix[0].W() * vec4.X()) + (this->matrix[1].W() * vec4.Y()) + (this->matrix[2].W() * vec4.Z()) + (this->matrix[3].W() * vec4.W());
			return newVec4;
		}

		static Mat4<T> createTransformMatrix(const Vec3<T>& position, const Vec3<T>& rotation, const Vec3<T>& scaleVec)
		{
			return translation(position) * zRotation(rotation.Z()) * xRotation(rotation.X()) * yRotation(rotation.Y()) * scale(scaleVec);
		}

		static Mat4<T> createView(const Vec3<T>& position, const Vec3<T>& rotation)
		{
			return translation(position) * xRotation(rotation.X()) * yRotation(rotation.Y());
		}

		static Mat4<T> lookAt(lm::vec3 const& eye, lm::vec3  const& center, lm::vec3  const& up)
		{
			lm::vec3  f = (center - eye).normalized();
			lm::vec3  u = up.normalized();
			lm::vec3  r = f.crossProduct(u).normalized();
			u = r.crossProduct(f);

			lm::Mat4<T> Result = Mat4::identity;
			Result[0][0] = r.X();
			Result[1][0] = r.Y();
			Result[2][0] = r.Z();
			Result[0][1] = u.X();
			Result[1][1] = u.Y();
			Result[2][1] = u.Z();
			Result[0][2] = -f.X();
			Result[1][2] = -f.Y();
			Result[2][2] = -f.Z();
			Result[3][0] = -(r.dotProduct(eye));
			Result[3][1] = -(u.dotProduct(eye));
			Result[3][2] = f.dotProduct(eye);
			return Result;
		}


		static Mat4<T> lookAtRotation(lm::vec3 const& eye, lm::vec3  const& center, lm::vec3  const& up)
		{
			lm::vec3  f = (center - eye).normalized();
			lm::vec3  u = up.normalized();
			lm::vec3  r = f.crossProduct(u).normalized();
			u = r.crossProduct(f);

			lm::Mat4<T> Result = Mat4::identity;
			Result[0][0] = r.X();
			Result[1][0] = r.Y();
			Result[2][0] = r.Z();
			Result[0][1] = u.X();
			Result[1][1] = u.Y();
			Result[2][1] = u.Z();
			Result[0][2] = -f.X();
			Result[1][2] = -f.Y();
			Result[2][2] = -f.Z();
			Result[3][0] = -eye.X();
			Result[3][1] = -eye.Y();
			Result[3][2] = -eye.Z();
			return Result;
		}

		Mat4<T> transpose()
		{
			return Mat4<T>(Vec4<T>(this->matrix[0].X(), this->matrix[1].X(), this->matrix[2].X(), this->matrix[3].X()),
				Vec4<T>(this->matrix[0].Y(), this->matrix[1].Y(), this->matrix[2].Y(), this->matrix[3].Y()),
				Vec4<T>(this->matrix[0].Z(), this->matrix[1].Z(), this->matrix[2].Z(), this->matrix[3].Z()),
				Vec4<T>(this->matrix[0].W(), this->matrix[1].W(), this->matrix[2].W(), this->matrix[3].W()));
		}

		Mat4<T> getInverse()
		{
			std::swap(this->matrix[0].Z(), this->matrix[2].X());
			std::swap(this->matrix[0].Y(), this->matrix[1].X());
			std::swap(this->matrix[1].Z(), this->matrix[2].Y());

			this->matrix[3] *= -1;
			return *this;
		}

		static Mat4<T> translation(const Vec3<T>& translation)
		{
			Mat4<T> translate;
			translate["x3"] = translation.X();
			translate["y3"] = translation.Y();
			translate["z3"] = translation.Z();
			return translate;
		}

		static Mat4<T> scale(const Vec3<T>& scale)
		{
			Mat4<T> matrixScale;
			matrixScale["x0"] = scale.X();
			matrixScale["y1"] = scale.Y();
			matrixScale["z2"] = scale.Z();
			return matrixScale;
		}

		static Mat4<T> xRotation(float angle)
		{
			float rad = float(Vec4<T>::degreesToRadians(double(angle)));

			Mat4<T> matrixScale;
			matrixScale["y1"] = std::cosf(rad);
			matrixScale["y2"] = -std::sinf(rad);

			matrixScale["z1"] = std::sinf(rad);
			matrixScale["z2"] = std::cosf(rad);

			return matrixScale;
		}

		static Mat4<T> yRotation(float angle)
		{
			float rad = float(Vec4<T>::degreesToRadians(double(angle)));

			Mat4<T> matrixRotation;
			matrixRotation["x0"] = std::cosf(rad);
			matrixRotation["x2"] = std::sinf(rad);

			matrixRotation["z0"] = -std::sinf(rad);
			matrixRotation["z2"] = std::cosf(rad);

			return matrixRotation;
		}

		static Mat4<T> zRotation(float angle)
		{
			float rad = float(Vec4<T>::degreesToRadians(double(angle)));

			Mat4<T> matrixScale;
			matrixScale["x0"] = std::cosf(rad);
			matrixScale["x1"] = -std::sinf(rad);

			matrixScale["y0"] = std::sinf(rad);
			matrixScale["y1"] = std::cosf(rad);

			return matrixScale;
		}

		static Mat4<T> perspectiveProjection(float fovy, float aspect, float near, float far)
		{
			float scale = tanf(fovy * float(M_PI / 360)) * near;
			float r = aspect * scale;
			float l = -r;
			float t = scale;
			float b = -t;

			float x1 = 2 * near / (r - l);
			float y2 = 2 * near / (t - b);
			float x3 = (r + l) / (r - l);
			float y3 = (t + b) / (t - b);
			float z3 = -(far + near) / (far - near);
			float z4 = -(2 * far * near) / (far - near);

			lm::Mat4<T> mat4;

			mat4.matrix[0] = lm::Vec4<T>(x1, 0, 0, 0);
			mat4.matrix[1] = lm::Vec4<T>(0, y2, 0, 0);
			mat4.matrix[2] = lm::Vec4<T>(x3, y3, z3, -1);
			mat4.matrix[3] = lm::Vec4<T>(0, 0, z4, 0);

			return mat4;
		}

		const bool operator==(const Mat4<T>& mat4) const
		{
			if (this == &mat4)
				return true;

			for (unsigned int i = 0; i < 4; i++)
				if (this->matrix[i] != mat4.matrix[i])
					return false;

			return true;
		}

		const bool operator!=(const Mat4<T>& mat4) const
		{
			return !(*this == mat4);
		}

		lm::vec3 extractScale()
		{
			lm::vec3 xScalar(this->matrix[0].X(), this->matrix[0].Y(), this->matrix[0].Z());
			lm::vec3 yScalar(this->matrix[1].X(), this->matrix[1].Y(), this->matrix[1].Z());
			lm::vec3 zScalar(this->matrix[2].X(), this->matrix[2].Y(), this->matrix[2].Z());
			float x = xScalar.length();
			float y = yScalar.length();
			float z = zScalar.length();
			lm::vec3 scalar(x, y, z);
			return scalar;
		}

		lm::vec3 extractRotation(lm::vec3 scale)
		{
			float theta1, theta2, gamma1, gamma2, phi1, phi2;
			Mat4<T> rotationMatrix(Vec4<T>(this->matrix[0].X() / scale.X(), this->matrix[0].Y() / scale.X(), this->matrix[0].Z() / scale.X(), 0),
				Vec4<T>(this->matrix[1].X() / scale.Y(), this->matrix[1].Y() / scale.Y(), this->matrix[1].Z() / scale.Y(), 0),
				Vec4<T>(this->matrix[2].X() / scale.Z(), this->matrix[2].Y() / scale.Z(), this->matrix[2].Z() / scale.Z(), 0),
				Vec4<T>(0, 0, 0, 1));

			if (rotationMatrix[2].X() != 1 || rotationMatrix[2].X() != -1)
			{
				theta1 = -asin(rotationMatrix[2].X());
				theta2 = M_PI - theta1;

				gamma1 = atan2(rotationMatrix[2].Y() / cos(theta1), rotationMatrix[2].Z() / cos(theta1));
				gamma2 = atan2(rotationMatrix[2].Y() / cos(theta2), rotationMatrix[2].Z() / cos(theta2));

				phi1 = atan2(rotationMatrix[1].X() / cos(theta1), rotationMatrix[0].X() / cos(theta1));
				phi2 = atan2(rotationMatrix[1].X() / cos(theta2), rotationMatrix[0].X() / cos(theta2));
			}
			else
			{
				phi1 = 0;
				if (rotationMatrix[2].X() != -1)
				{
					theta1 = M_PI / 2;
					gamma1 = atan2(rotationMatrix[0].Y(), rotationMatrix[0].Z());
				}
				else
				{
					theta1 = -M_PI / 2;
					gamma1 = atan2(-rotationMatrix[0].Y(), -rotationMatrix[0].Z());
				}
			}

			/*if (rotationMatrix[1].Z() < 1)
			{
				if (rotationMatrix[1].Z() > -1)
				{
					gamma1 = asin(rotationMatrix[1].Z());
					theta1 = atan2(-rotationMatrix[1].X(), rotationMatrix[1].Y());
					phi1 = atan2(-rotationMatrix[0].Z(), rotationMatrix[2].Z());
				}
				else
				{
					gamma1 = -M_PI / 2;
					theta1 = -atan2(rotationMatrix[2].X(), rotationMatrix[0].Y());
					phi1 = 0;
				}
			}
			else
			{
				gamma1 = M_PI / 2;
				theta1 = atan2(rotationMatrix[2].X(), rotationMatrix[0].Y());
				phi1 = 0;
			}*/

			lm::vec3 rotateVector(gamma1 * (180 / M_PI), theta1 * (180 / M_PI), phi1 * (180 / M_PI));
			return rotateVector;
		}
	};

	template<class T> const Mat4<T> Mat4<T>::identity = Mat4<T>();

	typedef Mat4<float> mat4;
}