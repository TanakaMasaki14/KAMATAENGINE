#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "DebugText.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "DebugCamera.h"
#include "PlayerBullet.h"
#include <memory>
#include<list>

//<summary>
//���L����
//</summary>
class Player
{
public:
	/// <summary>
	/// �R���X�g�N���^
	/// </summary>

	///<summary>///������///</summary>///<paramname="model">���f��</param>///<paramname="textureHandle">�e�N�X�`���n���h��</param>
	void Initialize(Model* model, uint32_t textureHandle);
	///<summary>
	///�X�V
	///</summary>
	void Update();
	///<summary>
	///�`��
	///</summary>
	//void Draw(ViewProjection viewProjection_);
	void Draw(ViewProjection& viewProjection);

	void Move();

	void TransferMatrix();

	void Attack();

	void OnCollision();
	//�e���X�g���擾
	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets() { return bullets_; }

	//���[���h���W���擾
	Vector3 GetWorldPosition();
private:

	WorldTransform worldTransform_;
	// ���f��
	Model* model_ = nullptr;
	// �e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;
	// ���͏���
	Input* input_ = nullptr;
	// �f�o�b�N�e�L�X�g
	DebugText* debugText_ = nullptr;

	//�e
	std::list<std::unique_ptr<PlayerBullet>>bullets_;
};