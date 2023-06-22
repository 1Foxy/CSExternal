#pragma once

#include <sstream>
#include <mmintrin.h>
#include "Vector2.h"
#include "Vector2.h"
#include <corecrt_math_defines.h>
#include "../../../../../../Program Files (x86)/Microsoft DirectX SDK (June 2010)/Include/d3dx9.h"

class VVector3 {
public:
	float x, y, z;

	VVector3() : x(0.0f), y(0.0f), z(0.0f) {}

	VVector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}

	VVector3 operator-(const VVector3& other) const {
		return VVector3(x - other.x, y - other.y, z - other.z);
	}

	VVector3(const VVector3& vOther) : x(vOther.x), y(vOther.y), z(vOther.z) {}

	float LengthSq() const {
		return x * x + y * y + z * z;
	}

	float Length() {
		return std::sqrt(x * x + y * y + z * z);
	}

	SDK::Vector2D CalculateAngles(const VVector3& vec) {
		float x = vec.x;
		float y = vec.y;
		float z = vec.z;

		float pitch, yaw;
		float hypotenuse = sqrt(x * x + y * y + z * z);
		pitch = ((double)atan(z / hypotenuse) * 180.0) / M_PI;
		yaw = ((double)atan(y / x) * 180.0) / M_PI;
		if (x >= 0.0) {
			yaw += 180.0;
		}

		return { pitch, yaw };
	}

	// Element-wise multiplication operator for SDK::Vector3
	VVector3 operator*(const VVector3& other) const {
		return VVector3{ x * other.x, y * other.y, z * other.z };
	}

	// Element-wise multiplication operator for SDK::Vector3 and float
	VVector3 operator*(float value) const {
		return VVector3{ x * value, y * value, z * value };
	}

	VVector3 operator+(float value) const {
		return VVector3{ x + value, y + value, z + value };
	}

	VVector3 operator+(const VVector3& other) const {
		return VVector3{ x + other.x, y + other.y, z + other.z };
	}
};


namespace SDK
{
	struct matrix_t
	{
		float matrix[4][4];

		float* operator[](const int& i) { return matrix[i]; }
	};

	struct QAngle
	{
		float x, y, z;

		float pitch, yaw, roll;
	};

	struct color_t
	{
		int r, g, b, a;
	};

	class matrix3x4_t;

	typedef float vec_t;
	
	// 3D Vector
	class Vector3 {
	public:
		// Members
		vec_t x, y, z;

		// Construction/destruction:
		Vector3(void);
		Vector3(vec_t X, vec_t Y, vec_t Z);
		Vector3(vec_t* clr);

		// Initialization
		void Init(vec_t ix = 0.0f, vec_t iy = 0.0f, vec_t iz = 0.0f);
		// TODO (Ilya): Should there be an init that takes a single float for consistency?

		// Got any nasty NAN's?
		bool IsValid() const;
		void Invalidate();

		// array access...
		vec_t operator[](int i) const;
		vec_t& operator[](int i);

		// Base address...
		vec_t* Base();
		vec_t const* Base() const;

		// Initialization methods
		void Random(vec_t minVal, vec_t maxVal);
		void Zero(); ///< zero out a vector

	

		// equality
		bool operator==(const Vector3& v) const;
		bool operator!=(const Vector3& v) const;

		// arithmetic operations
		Vector3& operator+=(const Vector3& v) {
			x += v.x; y += v.y; z += v.z;
			return *this;
		}

		Vector3& operator-=(const Vector3& v) {
			x -= v.x; y -= v.y; z -= v.z;
			return *this;
		}

		Vector3& operator*=(float fl) {
			x *= fl;
			y *= fl;
			z *= fl;
			return *this;
		}

		Vector3& operator*=(const Vector3& v) {
			x *= v.x;
			y *= v.y;
			z *= v.z;
			return *this;
		}

		Vector3& operator/=(const Vector3& v) {
			x /= v.x;
			y /= v.y;
			z /= v.z;
			return *this;
		}

		// this ought to be an opcode.
		Vector3& operator+=(float fl) {
			x += fl;
			y += fl;
			z += fl;
			return *this;
		}

		// this ought to be an opcode.
		Vector3& operator/=(float fl) {
			x /= fl;
			y /= fl;
			z /= fl;
			return *this;
		}
		Vector3& operator-=(float fl) {
			x -= fl;
			y -= fl;
			z -= fl;
			return *this;
		}

		// negate the vector components
		void	Negate();
		
		// Get the vector's magnitude.
		vec_t	Length() const;

		// Get the vector's magnitude squared.
		vec_t LengthSqr(void) const {
			return (x * x + y * y + z * z);
		}

		// return true if this vector is (0,0,0) within tolerance
		bool IsZero(float tolerance = 0.01f) const {
			return (x > -tolerance && x < tolerance&&
				y > -tolerance && y < tolerance&&
				z > -tolerance && z < tolerance);
		}

		vec_t	NormalizeInPlace();
		Vector3	Normalized() const;
		void Normalize();

		bool	IsLengthGreaterThan(float val) const {
			return LengthSqr() > val * val;
		}

		bool	IsLengthLessThan(float val) const {
			return LengthSqr() < val * val;
		}

		// check if a vector is within the box defined by two other vectors
		bool WithinAABox(Vector3 const& boxmin, Vector3 const& boxmax);

		// Get the distance from this vector to the other one.
		vec_t	DistTo(const Vector3& vOther) const;

		// Get the distance from this vector to the other one squared.
		// NJS: note, VC wasn't inlining it correctly in several deeply nested inlines due to being an 'out of line' .  
		// may be able to tidy this up after switching to VC7
		vec_t DistToSqr(const Vector3& vOther) const {
			Vector3 delta;

			delta.x = x - vOther.x;
			delta.y = y - vOther.y;
			delta.z = z - vOther.z;

			return delta.LengthSqr();
		}

		// Copy
		void	CopyToArray(float* rgfl) const;

		// Multiply, add, and assign to this (ie: *this = a + b * scalar). This
		// is about 12% faster than the actual vector equation (because it's done per-component
		// rather than per-vector).
		void	MulAdd(const Vector3& a, const Vector3& b, float scalar);

		// Dot product.
		vec_t	Dot(const Vector3& vOther) const;

		// assignment
		Vector3& operator=(const Vector3& vOther);

		// 2d
		vec_t	Length2D(void) const {
			return sqrt(x * x + y * y);
		}
		vec_t	Length2DSqr(void) const {
			return (x * x + y * y);
		}

		/// get the component of this vector parallel to some other given vector
		Vector3  ProjectOnto(const Vector3& onto);

		// copy constructors
		//	Vector(const Vector &vOther);

		// arithmetic operations
		Vector3	operator-(void) const;

		Vector3	operator+(const Vector3& v) const;
		Vector3	operator-(const Vector3& v) const;
		Vector3	operator*(const Vector3& v) const;
		Vector3	operator/(const Vector3& v) const;
		Vector3	operator*(float fl) const;
		Vector3	operator/(float fl) const;

		// Cross product between two vectors.
		Vector3	Cross(const Vector3& vOther) const;

		// Returns a vector with the min or max in X, Y, and Z.
		Vector3	Min(const Vector3& vOther) const;
		Vector3	Max(const Vector3& vOther) const;
	};

	class TableVector
	{
	public:
		vec_t x, y, z;

		operator Vector3& () { return *((Vector3*)(this)); }
		operator const Vector3& () const { return *((const Vector3*)(this)); }

		// array access...
		inline vec_t& operator[](int i)
		{
			return ((vec_t*)this)[i];
		}

		inline vec_t operator[](int i) const
		{
			return ((vec_t*)this)[i];
		}
	};

	void VectorTransform(const Vector3& in1, const matrix3x4_t& in2, Vector3& out);
	void VectorCopy(const Vector3& src, Vector3& dst);
	float VectorLength(const Vector3& v);
	void VectorLerp(const Vector3& src1, const Vector3& src2, vec_t t, Vector3& dest);
	void VectorCrossProduct(const Vector3& a, const Vector3& b, Vector3& result);
	vec_t NormalizeVector(Vector3& v);

	class __declspec(align(16)) VectorAligned : public Vector3 {
	public:
		inline VectorAligned(void) {};
		inline VectorAligned(vec_t X, vec_t Y, vec_t Z) {
			Init(X, Y, Z);
		}

	public:
		explicit VectorAligned(const Vector3& vOther) {
			Init(vOther.x, vOther.y, vOther.z);
		}

		VectorAligned& operator=(const Vector3& vOther) {
			Init(vOther.x, vOther.y, vOther.z);
			return *this;
		}

		VectorAligned& operator=(const VectorAligned& vOther) {
			this->Base()[0] = vOther.Base()[0];
			this->Base()[1] = vOther.Base()[1];
			this->Base()[2] = vOther.Base()[2];
			return *this;
		}

		float w;
	};
}