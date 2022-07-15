//#include <math.h>
//#include "WorldTransform.h"
//#include "TransferMatrix.h"
//
//Matrix4 Identity() {
//	// 単位行列を設定
//	Matrix4 matIdentity;
//	matIdentity.m[0][0] = 1.0f;
//	matIdentity.m[1][1] = 1.0f;
//	matIdentity.m[2][2] = 1.0f;
//	matIdentity.m[3][3] = 1.0f;
//	return matIdentity;
//}
//
//Matrix4 Scale(Vector3 scale) {
//	Matrix4 matScale;
//	matScale.m[0][0] = scale.x;
//	matScale.m[1][1] = scale.y;
//	matScale.m[2][2] = scale.z;
//	matScale.m[3][3] = 1.0f;
//
//	return matScale;
//}
//
//Matrix4 RotZ(float angle) {
//	Matrix4 matRotZ;
//	matRotZ.m[0][0] = cos(angle);
//	matRotZ.m[0][1] = sin(angle);
//
//	matRotZ.m[1][1] = cos(angle);
//	matRotZ.m[1][0] = -sin(angle);
//
//	matRotZ.m[2][2] = 1.0f;
//	matRotZ.m[3][3] = 1.0f;
//
//	return matRotZ;
//}
//
//Matrix4 RotX(float angle) {
//	Matrix4 matRotX;
//	matRotX.m[1][1] = cos(angle);
//	matRotX.m[1][2] = sin(angle);
//
//	matRotX.m[2][1] = -sin(angle);
//	matRotX.m[2][2] = cos(angle);
//
//	matRotX.m[0][0] = 1.0f;
//
//	matRotX.m[3][3] = 1.0f;
//
//	return matRotX;
//}
//
//Matrix4 RotY(float angle) {
//	Matrix4 matRotY;
//	matRotY.m[0][0] = cos(angle);
//	matRotY.m[0][2] = -sin(angle);
//
//	matRotY.m[2][0] = sin(angle);
//	matRotY.m[2][2] = cos(angle);
//
//	matRotY.m[1][1] = 1.0f;
//	matRotY.m[3][3] = 1.0f;
//
//	return matRotY;
//}
//
//
//Matrix4 Translation(Vector3 translation) {
//	Matrix4 matTrans;
//	//単位行列を代入
//	matTrans = Identity();
//
//	matTrans.m[3][0] = translation.x;
//	matTrans.m[3][1] = translation.y;
//	matTrans.m[3][2] = translation.z;
//
//	return matTrans;
//}
//Matrix4 GetMatrix(const WorldTransform& worldTransform)
//{
//	Matrix4 matWorld;
//
//	matWorld = Identity();
//	matWorld *= Scale(worldTransform.scale_);
//	matWorld *= RotZ(worldTransform.rotation_.z);
//	matWorld *= RotX(worldTransform.rotation_.x);
//	matWorld *= RotY(worldTransform.rotation_.y);
//	matWorld *= Translation(worldTransform.translation_);
//
//	return matWorld;
//}