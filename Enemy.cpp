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
	//NULL�|�C���^�`�F�b�N
	assert(model);

	//�����Ƃ��Ď󂯎�����f�[�^�������o�ϐ��ɋL�^����
	model_ = model;
	textureHandle_ = TextureManager::Load("enemy.png");
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//���[���h�ϊ��̏�����
	worldTransform_.Initialize();
	//�����Ŏ󂯎�����������W���Z�b�g
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

	//Y����]�s��̊e�v�f��ݒ肷��
	matRotY.m[0][0] = cos(worldTransform_.rotation_.y);
	matRotY.m[0][2] = -sin(worldTransform_.rotation_.y);
	matRotY.m[2][0] = sin(worldTransform_.rotation_.y);
	matRotY.m[2][2] = cos(worldTransform_.rotation_.y);
	matRotY.m[1][1] = 1.0f;
	matRotY.m[3][3] = 1.0f;

	Matrix4 matRotX;

	//X����]�s��̊e�v�f��ݒ肷��
	matRotX.m[1][1] = cos(worldTransform_.rotation_.x);
	matRotX.m[1][2] = sin(worldTransform_.rotation_.x);
	matRotX.m[2][1] = -sin(worldTransform_.rotation_.x);
	matRotX.m[2][2] = cos(worldTransform_.rotation_.x);
	matRotX.m[0][0] = 1.0f;
	matRotX.m[3][3] = 1.0f;

	Matrix4 matRot = matIdentity;
	//�e���̉�]�s�������
	matRot *= matRotZ;
	matRot *= matRotX;
	matRot *= matRotY;

	Matrix4 matTrans = MathUtility::Matrix4Identity();
	matTrans.m[3][0] = worldTransform_.translation_.x;
	matTrans.m[3][1] = worldTransform_.translation_.y;
	matTrans.m[3][2] = worldTransform_.translation_.z;
	matTrans.m[3][3] = 1;

	//�̃X�P�[�����O�E��]�E���s�ړ������������s����v�Z����
	worldTransform_.matWorld_ = matIdentity;
	worldTransform_.matWorld_ *= matScale;
	worldTransform_.matWorld_ *= matRot;
	worldTransform_.matWorld_ *= matTrans;

}
Vector3 Enemy::GetWorldPosition()
{
	TransferMatrix();
	//���[���h���W������ϐ�
	Vector3 worldPos;//���[���h�s��̕��s�ړ��������擾�i���[���h���W�j
	// ���[���h�s��̕��s�ړ��������擾�i���[���h���W�j
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}
void Enemy::Fire()
{

	TransferMatrix();

	assert(player_);
	//�e�̑��x
	const float kBulletSpeed = 1.0f;
	// ���L�����̃��[���h���W���擾����
	Vector3 playerPos = player_->GetWorldPosition();
	// �G�L�����̃��[���h���W���擾����
	Vector3 enemyPos = GetWorldPosition();
	// �����x�N�g��
	Vector3 velocity = playerPos - enemyPos;
	// ���K��
	velocity = MathUtility::Vector3Normalize(velocity);
	velocity = vecM(velocity, worldTransform_.matWorld_);
	//�e�𐶐����A������
	std::unique_ptr<EnemyBullet>newBullet = std::make_unique<EnemyBullet>();
	newBullet->Initialize(model_, worldTransform_.translation_, velocity);
	//�e��o�^����
	bullets_.push_back(std::move(newBullet));

}

void Enemy::ApproachInit()
{


	// ���˃^�C�}�[�J�E���g�_�E��
	fireTimer--;
	// �w�莞�ԂɒB����
	if (fireTimer < 0) {
		// �e�𔭎�
		Fire();
		// ���˃^�C�}�[��������
		fireTimer = kFireInterval;
	}

	debugText_->SetPos(50, 100);
	debugText_->Printf(
		"fireTimer:(%f,%f)", fireTimer, kFireInterval);
}



void Enemy::Move()
{
	//�L�����N�^�[�̈ړ��x�N�g��
	Vector3 move = { 0,0,0 };

	float moveSpeed = 0.2f;
	move = { 0.0f,0.01f,-moveSpeed };

	//���W�ړ�

	worldTransform_.translation_ += move;
	TransferMatrix();
	worldTransform_.TransferMatrix();
}

void Enemy::Update() {
	//�f�X�t���O�̗������e���폜
	bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet) {
		return bullet->IsDead();
		});

	//�L�����N�^�[�̈ړ��x�N�g��
	Vector3 approachMove = { 0,0,0 };
	Vector3 leaveMove = { 0,0,0 };
	float moveSpeed = 0.1f;
	approachMove = { 0.0f,0.0f,-moveSpeed };
	leaveMove = { -0.1,0.1,-moveSpeed };

	//Fire();
	ApproachInit();
	//�e�X�V
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_) { bullet->Update(); }
	switch (phase_) {
	case Phase::Approach:
	default://�ړ��i�x�N�g�������Z�j
		worldTransform_.translation_ += approachMove;
		//�K��̈ʒu�ɓ��B�����痣�E
		if (worldTransform_.translation_.z < -10.0f) {
			phase_ = Phase::Leave;
		}
		break;
	case Phase::Leave://�ړ��i�x�N�g�������Z�j
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
#pragma region �w�i�X�v���C�g�`��

#pragma endregion

#pragma region 3D�I�u�W�F�N�g�`��
	//���f���̕`��
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	//�e�`��
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_) { bullet->Draw(viewProjection); }
#pragma endregion

#pragma region �O�i�X�v���C�g�`��

#pragma endregion
}