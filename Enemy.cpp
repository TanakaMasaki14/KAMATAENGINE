#include "Enemy.h"
#include "Player.h"

Vector3 vecM(Vector3& vec, Matrix4& mat) {

	Vector3 vecM = {};

	vec.x = vec.x * mat.m[0][0] + vec.y * mat.m[1][0] + vec.z * mat.m[2][0];

	vec.y = vec.x * mat.m[0][1] + vec.y * mat.m[1][1] + vec.z * mat.m[2][1];

	vec.z = vec.x * mat.m[0][2] + vec.y * mat.m[1][2] + vec.z * mat.m[2][2];

	return vec;
}


Enemy::Enemy() {

}

void Enemy::Initialize(Model* model, const Vector3& position, uint32_t textureHandle)
{
	//NULLポインタチェック
	assert(model);

	//引数として受け取ったデータをメンバ変数に記録する
	model_ = model;
	textureHandle_ = TextureManager::Load("enemy.png");
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//ワールド変換の初期化
	worldTransform_.Initialize();
	//引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;
	worldTransform_.translation_ = { 5.0f,5.0f,10.0f };

	ApproachInit();
}

void Enemy::TransferMatrix() {
	Matrix4 matIdentity;
	matIdentity.m[0][0] = 1.0f;
	matIdentity.m[1][1] = 1.0f;
	matIdentity.m[2][2] = 1.0f;
	matIdentity.m[3][3] = 1.0f;


	Matrix4 matScale;
	matScale.m[0][0] = worldTransform_.scale_.x;
	matScale.m[1][1] = worldTransform_.scale_.y;
	matScale.m[2][2] = worldTransform_.scale_.z;
	matScale.m[3][3] = 1.0f;

	Matrix4 matRotZ;
	matRotZ.m[0][0] = cos(worldTransform_.rotation_.z);
	matRotZ.m[0][1] = sin(worldTransform_.rotation_.z);
	matRotZ.m[1][0] = -sin(worldTransform_.rotation_.z);
	matRotZ.m[1][1] = cos(worldTransform_.rotation_.z);
	matRotZ.m[2][2] = 1.0f;
	matRotZ.m[3][3] = 1.0f;

	Matrix4 matRotY;

	//Y軸回転行列の各要素を設定する
	matRotY.m[0][0] = cos(worldTransform_.rotation_.y);
	matRotY.m[0][2] = -sin(worldTransform_.rotation_.y);
	matRotY.m[2][0] = sin(worldTransform_.rotation_.y);
	matRotY.m[2][2] = cos(worldTransform_.rotation_.y);
	matRotY.m[1][1] = 1.0f;
	matRotY.m[3][3] = 1.0f;

	Matrix4 matRotX;

	//X軸回転行列の各要素を設定する
	matRotX.m[1][1] = cos(worldTransform_.rotation_.x);
	matRotX.m[1][2] = sin(worldTransform_.rotation_.x);
	matRotX.m[2][1] = -sin(worldTransform_.rotation_.x);
	matRotX.m[2][2] = cos(worldTransform_.rotation_.x);
	matRotX.m[0][0] = 1.0f;
	matRotX.m[3][3] = 1.0f;

	Matrix4 matRot = matIdentity;
	//各軸の回転行列を合成
	matRot *= matRotZ;
	matRot *= matRotX;
	matRot *= matRotY;

	Matrix4 matTrans = MathUtility::Matrix4Identity();
	matTrans.m[3][0] = worldTransform_.translation_.x;
	matTrans.m[3][1] = worldTransform_.translation_.y;
	matTrans.m[3][2] = worldTransform_.translation_.z;
	matTrans.m[3][3] = 1;

	//のスケーリング・回転・平行移動を合成した行列を計算する
	worldTransform_.matWorld_ = matIdentity;
	worldTransform_.matWorld_ *= matScale;
	worldTransform_.matWorld_ *= matRot;
	worldTransform_.matWorld_ *= matTrans;

}
Vector3 Enemy::GetWorldPosition()
{
	TransferMatrix();
	//ワールド座標を入れる変数
	Vector3 worldPos;//ワールド行列の平行移動成分を取得（ワールド座標）
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}
void Enemy::Fire()
{

	TransferMatrix();

	assert(player_);
	//弾の速度
	const float kBulletSpeed = 1.0f;
	// 自キャラのワールド座標を取得する
	Vector3 playerPos = player_->GetWorldPosition();
	// 敵キャラのワールド座標を取得する
	Vector3 enemyPos = GetWorldPosition();
	// 差分ベクトル
	Vector3 velocity = playerPos - enemyPos;
	// 正規化
	velocity = MathUtility::Vector3Normalize(velocity);
	velocity = vecM(velocity, worldTransform_.matWorld_);
	//弾を生成し、初期化
	std::unique_ptr<EnemyBullet>newBullet = std::make_unique<EnemyBullet>();
	newBullet->Initialize(model_, worldTransform_.translation_, velocity);
	//弾を登録する
	bullets_.push_back(std::move(newBullet));

}

void Enemy::ApproachInit()
{


	// 発射タイマーカウントダウン
	fireTimer--;
	// 指定時間に達した
	if (fireTimer < 0) {
		// 弾を発射
		Fire();
		// 発射タイマーを初期化
		fireTimer = kFireInterval;
	}

	debugText_->SetPos(50, 100);
	debugText_->Printf(
		"fireTimer:(%f,%f)", fireTimer, kFireInterval);
}



void Enemy::Move()
{
	//キャラクターの移動ベクトル
	Vector3 move = { 0,0,0 };

	float moveSpeed = 0.2f;
	move = { 0.0f,0.01f,-moveSpeed };

	//座標移動

	worldTransform_.translation_ += move;
	TransferMatrix();
	worldTransform_.TransferMatrix();
}

void Enemy::Update() {
	//デスフラグの立った弾を削除
	bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet) {
		return bullet->IsDead();
		});

	//キャラクターの移動ベクトル
	Vector3 approachMove = { 0,0,0 };
	Vector3 leaveMove = { 0,0,0 };
	float moveSpeed = 0.1f;
	approachMove = { 0.0f,0.0f,-moveSpeed };
	leaveMove = { -0.1,0.1,-moveSpeed };

	//Fire();
	ApproachInit();
	//弾更新
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_) { bullet->Update(); }
	switch (phase_) {
	case Phase::Approach:
	default://移動（ベクトルを加算）
		worldTransform_.translation_ += approachMove;
		//規定の位置に到達したら離脱
		if (worldTransform_.translation_.z < -10.0f) {
			phase_ = Phase::Leave;
		}
		break;
	case Phase::Leave://移動（ベクトルを加算）
		worldTransform_.translation_ += leaveMove;
		break;
	}

	/*worldTransform_.rotation_.y -= 0.02f;
	worldTransform_.rotation_.x -= 0.02f;*/
	TransferMatrix();
	worldTransform_.TransferMatrix();
	debugText_->SetPos(50, 80);
	debugText_->Printf(
		"Enemy move:(%f,%f,%f)",
		worldTransform_.translation_.x,
		worldTransform_.translation_.y,
		worldTransform_.translation_.z);

}

void Enemy::Draw(ViewProjection& viewProjection)
{
#pragma region 背景スプライト描画

#pragma endregion

#pragma region 3Dオブジェクト描画
	//モデルの描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	//弾描画
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_) { bullet->Draw(viewProjection); }
#pragma endregion

#pragma region 前景スプライト描画

#pragma endregion
}