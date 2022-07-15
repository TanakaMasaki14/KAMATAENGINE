#pragma once
#include "assert.h"
#include "Model.h"
#include "WorldTransform.h"

///<summary>
///自キャラの弾
///</summary>
class PlayerBullet
{

public:

	/// <summary>
	/// コンストクラタ
	/// </summary>
	PlayerBullet();

	///<summary>
	///初期化
	///</summary>
	///<paramname="model">モデル</param>
	///<paramname="position">初期座標</param>
	///<paramname="Velocity">初期座標</param>
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);

	///<summary>
	///更新
	///</summary>
	void Update();

	///<summary>
	///描画
	///</summary>
	///<paramname="viewProjection">ビュープロジェクション</param>
	void Draw(const ViewProjection& viewProjection);
	void TransferMatrix();
	bool IsDead() const { return isDead_; }
private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	//PlayerBullet* bullet_ = nullptr;
	//速度
	Vector3 velocity_;
	//寿命<frm>
	static const int32_t kLifeTime = 60 * 5;
	//デスタイマー
	int32_t deathTimer_ = kLifeTime;
	//デスフラグ
	bool isDead_ = false;
};