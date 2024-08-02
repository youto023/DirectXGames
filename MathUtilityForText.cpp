#include"MathUtilityForText.h"
#include<cmath>
#include<numbers>
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rot, const Vector3& translate) {
    //
	Vector3 dm;
    dm = scale;
	dm = rot;

	//
	Matrix4x4 result{
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		translate.x,
		translate.y,
		translate.z,
		1.0f};
	return result;
}
// Vector3の足し算
Vector3& operator+=(Vector3& lhv, const Vector3& rhv) {
	lhv.x += rhv.x;
	lhv.y += rhv.y;
	lhv.z += rhv.z;
	return lhv;
}
float EaseInOut(float x1, float x2, float t) { 
	float easedT = -(std::cosf(std::numbers::pi_v<float> * t) - 1.0f) / 2.0f;
	return Lerp(x1, x2, easedT);
}
float Lerp(float x1, float x2, float t) { return (1.0f - t) * x1 + t * x2; }

Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t) { return Vector3(Lerp(v1.x, v2.x, t), Lerp(v1.y, v2.y, t), Lerp(v1.z, v2.z, t)); }
// 2項演算子オーバーロード
// Vector３の足し算
const Vector3 operator+(const Vector3& v1, const Vector3& v2) { 
	Vector3 temp(v1);
	return temp += v2; }
