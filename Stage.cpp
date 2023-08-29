#include "Stage.h"
#include"Engine/Model.h"
//�R���X�g���N�^
Stage::Stage(GameObject* parent):GameObject(parent, "Stage"),hModel_{-1,-1,-1,-1,-1}
{
    for(int x=0;x<XSIZE;x++)
    {
        for (int z = 0; z < ZSIZE; z++)
        {
            table_[x][z] = -1;
        }
    }
}

//�f�X�g���N�^
Stage::~Stage()
{
}

//������
void Stage::Initialize()
{
    string modelname[modelNum] = { "Default","Brick","Grass","Sand","Water" };
    //���f���f�[�^�̃��[�h
    for (int i = 0; i < modelNum; i++)
    {
        hModel_[i] = Model::Load("Assets/Box"+modelname[i]+".fbx");
        assert(hModel_[i] >= 0);
    }
    for (int z = 0; z < XSIZE; z++)
    {
        for (int x = 0; x < ZSIZE; x++)
        {
            table_[x][z] =x % 5;
        }
    }
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
        for (int z = 0; z < stageSize; z++)
        {
            int type = table_[x][z];
            Transform boxTrans = transform_;
            boxTrans.position_.x = 7- x;
            boxTrans.position_.z =7- z;
            Model::SetTransform(hModel_[type], boxTrans);
            Model::Draw(hModel_[type]);
        }
    }
   

}

//�J��
void Stage::Release()
{
}