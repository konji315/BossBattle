#include "ParticleEffects.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

void ParticleEffects::RegistParticles()
{
	ParticleEffectManager& pem = singleton<ParticleEffectManager>::GetInstance();

	//�a��Hit
	{
		ParticleEmitter* halo;
		halo = new ParticleEmitter();
		halo->Initialize(
			2.0f									//��������
			, Vector3(0, 0, 0)						//���W
			, Vector2(0, 1)							//�p�x
			, L"Resources/Textures/Halo.png"		//�e�N�X�`��
			, 0.0f									//�ŏ��̊p�x
			, 0.0f									//�Ō�̊p�x
			, 0.0f									//�����傫��
			, 5.0f									//�ŏI�傫��
			, Vector4(1, 1, 0.6f, 1)					//�ŏ��̐F
			, Vector4(1, 1, 1, 0.0f)					//�Ō�̐F
			, 0.2f									//����
			, 0.0f									//�����x
			, 0.0f									//�ŏI���x
			, 0.00f									//�d��
			, 90.0f									//���ˊp�x
			, 1										//���[�g
			, true									//�o�[�X�g
			, false									//���[�v
			, ParticleEmitter::CONE);				//���˕��@

		pem.RegisterParticle("halo", halo);
	}
	//�a��
	{
		ParticleEmitter* slash;
		slash = new ParticleEmitter();
		slash->Initialize(
			2.0f									//��������
			, Vector3(0, 0, 0)						//���W
			, Vector2(0, 1)							//�p�x
			, L"Resources/Textures/slash.png"		//�e�N�X�`��
			, 0.0f									//�ŏ��̊p�x
			, 0.0f									//�Ō�̊p�x
			, 0.0f									//�����傫��
			, 10.0f									//�ŏI�傫��
			, Vector4(1, 1, 0.6f, 1)					//�ŏ��̐F
			, Vector4(1, 1, 1, 0.0f)					//�Ō�̐F
			, 0.2f									//����
			, 0.0f									//�����x
			, 0.0f									//�ŏI���x
			, 0.00f									//�d��
			, 90.0f									//���ˊp�x
			, 1										//���[�g
			, true									//�o�[�X�g
			, false									//���[�v
			, ParticleEmitter::CONE);				//���˕��@

		pem.RegisterParticle("slash", slash);
	}

	//�Ռ��g
	{
		ParticleEmitter* shockwave;
		shockwave = new ParticleEmitter();
		shockwave->Initialize(
			2.0f									//��������
			, Vector3(0, 10, 0)						//���W
			, Vector2(0, 1)							//�p�x
			, L"Resources/Textures/Line_2.png"		//�e�N�X�`��
			, 0.0f									//�ŏ��̊p�x
			, 0.0f									//�Ō�̊p�x
			, 0.0f									//�����傫��
			, 10.0f									//�ŏI�傫��
			, Vector4(1, 1, 1, 1)					//�ŏ��̐F
			, Vector4(0, 0, 0, 0)					//�Ō�̐F
			, 2.0f									//����
			, 40.0f									//�����x
			, 50.0f									//�ŏI���x
			, 0.00f									//�d��
			, 90.0f									//���ˊp�x
			, 100									//���[�g
			, true									//�o�[�X�g
			, false									//���[�v
			, ParticleEmitter::CONE);				//���˕��@

		pem.RegisterParticle("shockwave", shockwave);
	}

	//���ՃG�t�F�N�g
	{
		ParticleEmitter* footsteps;
		footsteps = new ParticleEmitter();
		footsteps->Initialize(
			1.0f									//��������
			, Vector3(0, 0, 0)						//���W
			, Vector2(0, 1)							//�p�x
			, L"Resources/Textures/Burst_2.png"		//�e�N�X�`��
			, 0.0f									//�ŏ��̊p�x
			, 0.0f									//�Ō�̊p�x
			, 0.0f									//�����傫��
			, 5.0f									//�ŏI�傫��
			, Vector4(1, 1, 1, 1)					//�ŏ��̐F
			, Vector4(1, 1, 1, 0.0f)					//�Ō�̐F
			, 0.2f									//����
			, 0.0f									//�����x
			, 0.0f									//�ŏI���x
			, 0.00f									//�d��
			, 90.0f									//���ˊp�x
			, 1										//���[�g
			, true									//�o�[�X�g
			, false									//���[�v
			, ParticleEmitter::CONE);				//���˕��@

		pem.RegisterParticle("footsteps", footsteps);
	}

	//�{�X���j�G�t�F�N�g
	{
		ParticleEmitter* explosion1;
		explosion1 = new ParticleEmitter();
		explosion1->Initialize(
			2.0f									//��������
			, Vector3(0, 0, 0)						//���W
			, Vector2(0, 1)							//�p�x
			, L"Resources/Textures/Particle_Soft.png"		//�e�N�X�`��
			, 0.0f									//�ŏ��̊p�x
			, 0.0f									//�Ō�̊p�x
			, 5.0f									//�����傫��
			, 1.0f									//�ŏI�傫��
			, Vector4(1, 1, 0.6f, 1)					//�ŏ��̐F
			, Vector4(1, 1, 1, 0.0f)					//�Ō�̐F
			, 1.0f									//����
			, 30.0f									//�����x
			, 0.0f									//�ŏI���x
			, 0.1f									//�d��
			, 180.0f									//���ˊp�x
			, 50										//���[�g
			, true									//�o�[�X�g
			, false									//���[�v
			, ParticleEmitter::SPHERE);				//���˕��@

		pem.RegisterParticle("explosion1", explosion1);
	}
}