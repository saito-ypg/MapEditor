#include "Stage.h"
#include"Engine/Model.h"
//�R���X�g���N�^
Stage::Stage(GameObject* parent)	 :GameObject(parent, "Stage"), hModel_(-1)
{
}

//�f�X�g���N�^
Stage::~Stage()
{
}

//������
void Stage::Initialize()
{
    //���f���f�[�^�̃��[�h
    hModel_ = Model::Load("Assets/BoxDefault.fbx");
    assert(hModel_ >= 0);
}

//�X�V
void Stage::Update()
{
}

//�`��
void Stage::Draw()
{
    const int stageSize = 15;
    for (int x = 0; x < stageSize; x++)
    {
        for (int y = 0; y < stageSize; y++)
        {
            Transform boxTrans = transform_;
            boxTrans.position_.x = (float)stageSize / 2 - x;
            boxTrans.position_.z = (float)stageSize / 2 - y;
            Model::SetTransform(hModel_, boxTrans);
            Model::Draw(hModel_);
        }
    }
   

}

//�J��
void Stage::Release()
{
}