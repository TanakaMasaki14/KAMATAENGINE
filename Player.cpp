#include "Player.h"
#include "assert.h"

Vector3 vecmat(Vector3& vec, Matrix4& mat) {

	Vector3 vecMat = {};

	vecMat.x = vec.x * mat.m[0][0] + vec.y * mat.m[1][0] + vec.z * mat.m[2][0];

	vecMat.y = vec.x * mat.m[0][1] + vec.y * mat.m[1][1] + vec.z * mat.m[2][1];

	vecMat.z = vec.x * mat.m[0][2] + vec.y * mat.m[1][2] + vec.z * mat.m[2][2];

	return vecMat;
}


void Player::Initialize(Model* model, uint32_t textureHandle)
{
	//NULL�|�C���^�`�F�b�N
	assert(model);

	//�����Ƃ��Ď󂯎�����f�[�^�������o�ϐ��ɋL�^����
	model_ = model;
	textureHandle_ = textureHandle;

	//�V���O���g���C���X�^���X���擾����
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//���[���h�ϊ��̏�����
	worldTransform_.Initialize();
}

Vector3 Player::GetWorldPosition()
{
	// ���[���h���W������ϐ�
	Vector3 worldPos;
	// ���[���h�s��̕��s�ړ��������擾�i���[���h���W�j
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void Player::Move() {
	// �L�����N�^�[�̈ړ��x�N�g��
	Vector3 move = { 0,0,0 };

	// �L�����N�^�[�̈ړ�����
	const float kCharacterSpeed = 0.2f;

	const float kMoveLimitX = 36.0f;
	const float kMoveLimitY = 20.0f;

	//�����������ňړ��x�N�g����ύX
	if (input_->PushKey(DIK_A)) {
		move = { -kCharacterSpeed,0,0 };
	}
	else if (input_->PushKey(DIK_D)) {
		move = { kCharacterSpeed,0,0 };
	}
	else if (input_->PushKey(DIK_W)) {
		move = { 0,kCharacterSpeed,0 };
	}
	else if (input_->PushKey(DIK_S)) {
		move = { 0,-kCharacterSpeed,0 };
	}
	worldTransform_.rotation_.y -= 0.02f;
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, kMoveLimitY);

	//���W�ړ�
	worldTransform_.translation_ += move;
	worldTransform_.TransferMatrix();
}

void Player::Update() {
	//�f�X�t���O�̗������e���폜
	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet) {
		return bullet->IsDead();
		});

	Move();
	Attack();

	//�e�X�V
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) { bullet->Update(); }
	TransferMatrix();
	debugText_->SetPos(50, 50);
	debugText_->Printf(
		"move:(%f,%f,%f)",
		worldTransform_.translation_.x,
		worldTransform_.translation_.y,
		worldTransform_.translation_.z);
}

void Player::TransferMatrix() {
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

void Player::Attack()
{
	if (input_->TriggerKey(DIK_SPACE))
	{
		TransferMatrix();
		//�e�̑��x
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		velocity = vecmat(velocity, worldTransform_.matWorld_);
		//�e�𐶐����A������
		std::unique_ptr<PlayerBullet>newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initialize(model_, worldTransform_.translation_, velocity);
		//�e��o�^����
		bullets_.push_back(std::move(newBullet));
	}
}


void Player::Draw(ViewProjection& viewProjection_)
{
#pragma region �w�i�X�v���C�g�`��

#pragma endregion

#pragma region 3D�I�u�W�F�N�g�`��

	model_->Draw(worldTransform_, viewProjection_, textureHandle_);
	//�e�`��
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) { bullet->Draw(viewProjection_); }
#pragma endregion

#pragma region �O�i�X�v���C�g�`��

#pragma endregion
}
