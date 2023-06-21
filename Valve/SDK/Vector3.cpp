#include "Vector3.h"

#include <cmath>
#include <limits>
#include "Vector2.h"
#include <corecrt_math_defines.h>




namespace SDK
{
	float x;
	float y;
	float z;

	void VectorCopy(const Vector3& src, Vector3& dst) {
		dst.x = src.x;
		dst.y = src.y;
		dst.z = src.z;
	}
	void VectorLerp(const Vector3& src1, const Vector3& src2, vec_t t, Vector3& dest) {
		dest.x = src1.x + (src2.x - src1.x) * t;
		dest.y = src1.y + (src2.y - src1.y) * t;
		dest.z = src1.z + (src2.z - src1.z) * t;
	}
	float VectorLength(const Vector3& v) {
		return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
	}
	void VectorCrossProduct(const Vector3& a, const Vector3& b, Vector3& result) {
		result.x = a.y * b.z - a.z * b.y;
		result.y = a.z * b.x - a.x * b.z;
		result.z = a.x * b.y - a.y * b.x;
	}
	vec_t NormalizeVector(Vector3& v) {
		vec_t l = v.Length();
		if (l != 0.0f) {
			v /= l;
		}
		else {
			v.x = v.y = v.z = 0.0f;
		}
		return l;
	}
	void Normalize(VVector3& v) {
		while (v.y > 180) {
			v.y -= 360;
		}
		while (v.y < -180) {
			v.y += 360;
		}
		if (v.x > 89) {
			v.x = 89;
		}
		if (v.x < -89) {
			v.x = -89;
		}
	}
	Vector3::Vector3(void) {
		Invalidate();
	}
	Vector3::Vector3(vec_t X, vec_t Y, vec_t Z) {
		x = X;
		y = Y;
		z = Z;
	}
	Vector3::Vector3(vec_t* clr) {
		x = clr[0];
		y = clr[1];
		z = clr[2];
	}

	//-----------------------------------------------------------------------------
	// initialization
	//-----------------------------------------------------------------------------

	void Vector3::Init(vec_t ix, vec_t iy, vec_t iz) {
		x = ix; y = iy; z = iz;
	}

	Vector3 Vector3::Normalized() const {
		Vector3 res = *this;
		vec_t l = res.Length();
		if (l != 0.0f) {
			res /= l;
		}
		else {
			res.x = res.y = res.z = 0.0f;
		}
		return res;
	}
	void Vector3::Normalize()
	{
		float length = sqrt(x * x + y * y + z * z);
		x /= length;
		y /= length;
		z /= length;
	}
	vec_t Vector3::NormalizeInPlace() {
		return NormalizeVector(*this);
	}

	void Vector3::Random(vec_t minVal, vec_t maxVal) {
		x = minVal + ((float)rand() / RAND_MAX) * (maxVal - minVal);
		y = minVal + ((float)rand() / RAND_MAX) * (maxVal - minVal);
		z = minVal + ((float)rand() / RAND_MAX) * (maxVal - minVal);
	}

	// This should really be a single opcode on the PowerPC (move r0 onto the vec reg)
	void Vector3::Zero() {
		x = y = z = 0.0f;
	}

	//-----------------------------------------------------------------------------
	// assignment
	//-----------------------------------------------------------------------------

	Vector3& Vector3::operator=(const Vector3& vOther) {
		x = vOther.x; y = vOther.y; z = vOther.z;
		return *this;
	}


	//-----------------------------------------------------------------------------
	// Array access
	//-----------------------------------------------------------------------------
	vec_t& Vector3::operator[](int i) {
		return ((vec_t*)this)[i];
	}

	vec_t Vector3::operator[](int i) const {
		return ((vec_t*)this)[i];
	}





	//-----------------------------------------------------------------------------
	// Base address...
	//-----------------------------------------------------------------------------
	vec_t* Vector3::Base() {
		return (vec_t*)this;
	}

	vec_t const* Vector3::Base() const {
		return (vec_t const*)this;
	}

	//-----------------------------------------------------------------------------
	// IsValid?
	//-----------------------------------------------------------------------------

	bool Vector3::IsValid() const {
		return std::isfinite(x) && std::isfinite(y) && std::isfinite(z);
	}

	//-----------------------------------------------------------------------------
	// Invalidate
	//-----------------------------------------------------------------------------

	void Vector3::Invalidate() {
		//#ifdef _DEBUG
		//#ifdef VECTOR_PARANOIA
		x = y = z = std::numeric_limits<float>::infinity();
		//#endif
		//#endif
	}

	//-----------------------------------------------------------------------------
	// comparison
	//-----------------------------------------------------------------------------

	bool Vector3::operator==(const Vector3& src) const {
		return (src.x == x) && (src.y == y) && (src.z == z);
	}

	bool Vector3::operator!=(const Vector3& src) const {
		return (src.x != x) || (src.y != y) || (src.z != z);
	}


	//-----------------------------------------------------------------------------
	// Copy
	//-----------------------------------------------------------------------------
	void	Vector3::CopyToArray(float* rgfl) const {
		rgfl[0] = x, rgfl[1] = y, rgfl[2] = z;
	}

	//-----------------------------------------------------------------------------
	// standard math operations
	//-----------------------------------------------------------------------------
	// #pragma message("TODO: these should be SSE")

	void Vector3::Negate() {
		x = -x; y = -y; z = -z;
	}

	// get the component of this vector parallel to some other given vector
	Vector3 Vector3::ProjectOnto(const Vector3& onto) {
		return onto * (this->Dot(onto) / (onto.LengthSqr()));
	}

	// FIXME: Remove
	// For backwards compatability
	void	Vector3::MulAdd(const Vector3& a, const Vector3& b, float scalar) {
		x = a.x + b.x * scalar;
		y = a.y + b.y * scalar;
		z = a.z + b.z * scalar;
	}

	Vector3 VectorLerp(const Vector3& src1, const Vector3& src2, vec_t t) {
		Vector3 result;
		VectorLerp(src1, src2, t, result);
		return result;
	}

	vec_t Vector3::Dot(const Vector3& b) const {
		return (x * b.x + y * b.y + z * b.z);
	}
	void VectorClear(Vector3& a) {
		a.x = a.y = a.z = 0.0f;
	}

	vec_t Vector3::Length(void) const {
		return sqrt(x * x + y * y + z * z);
	}

	// check a point against a box
	bool Vector3::WithinAABox(Vector3 const& boxmin, Vector3 const& boxmax) {
		return (
			(x >= boxmin.x) && (x <= boxmax.x) &&
			(y >= boxmin.y) && (y <= boxmax.y) &&
			(z >= boxmin.z) && (z <= boxmax.z)
			);
	}

	//-----------------------------------------------------------------------------
	// Get the distance from this vector to the other one 
	//-----------------------------------------------------------------------------
	vec_t Vector3::DistTo(const Vector3& vOther) const {
		Vector3 delta;
		delta = *this - vOther;
		return delta.Length();
	}

	//-----------------------------------------------------------------------------
	// Returns a vector with the min or max in X, Y, and Z.
	//-----------------------------------------------------------------------------
	Vector3 Vector3::Min(const Vector3& vOther) const {
		return Vector3(x < vOther.x ? x : vOther.x,
			y < vOther.y ? y : vOther.y,
			z < vOther.z ? z : vOther.z);
	}

	Vector3 Vector3::Max(const Vector3& vOther) const {
		return Vector3(x > vOther.x ? x : vOther.x,
			y > vOther.y ? y : vOther.y,
			z > vOther.z ? z : vOther.z);
	}


	//-----------------------------------------------------------------------------
	// arithmetic operations
	//-----------------------------------------------------------------------------

	Vector3 Vector3::operator-(void) const {
		return Vector3(-x, -y, -z);
	}

	Vector3 Vector3::operator+(const Vector3& v) const {
		return Vector3(x + v.x, y + v.y, z + v.z);
	}

	Vector3 Vector3::operator-(const Vector3& v) const {
		return Vector3(x - v.x, y - v.y, z - v.z);
	}

	Vector3 Vector3::operator*(float fl) const {
		return Vector3(x * fl, y * fl, z * fl);
	}

	Vector3 Vector3::operator*(const Vector3& v) const {
		return Vector3(x * v.x, y * v.y, z * v.z);
	}

	Vector3 Vector3::operator/(float fl) const {
		return Vector3(x / fl, y / fl, z / fl);
	}

	Vector3 Vector3::operator/(const Vector3& v) const {
		return Vector3(x / v.x, y / v.y, z / v.z);
	}

	Vector3 operator*(float fl, const Vector3& v) {
		return v * fl;
	}

	//-----------------------------------------------------------------------------
	// cross product
	//-----------------------------------------------------------------------------

	Vector3 Vector3::Cross(const Vector3& vOther) const {
		Vector3 res;
		VectorCrossProduct(*this, vOther, res);
		return res;
	}
}