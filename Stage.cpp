#include "Stage.h"
#include"Engine/Model.h"
//�R���X�g���N�^
Stage::Stage(GameObject* parent):GameObject(parent, "Stage"),hModel_{-1,-1,-1,-1,-1}
{
    for(int x=0;x<XSIZE;x++)
    {
        for (int z = 0; z < ZSIZE; z++)
        {
            table_[x][z] = { DEFAULT,0 };
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
    string modelname[NUM] = { "Default","Brick","Grass","Sand","Water" };
    //���f���f�[�^�̃��[�h
    for (int i = 0; i < NUM; i++)
    {
        hModel_[i] = Model::Load("Assets/Box"+modelname[i]+".fbx");
        assert(hModel_[i] >= 0);
    }
    for (int z = 0; z < XSIZE; z++)
    {
        for (int x = 0; x < ZSIZE; x++)
        {
            table_[x][z].type_=DEFAULT;
        }
    }

    SetBlockHeight(2, 2, 5);
    SetBlockType(1, 6, BRICK);
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
            for (int y = 0; y < YLIMIT; y++)
            {
                
                if (y <= table_[x][z].height_)
                {
                    int type = table_[x][z].type_;
                    Transform boxTrans = transform_;
                    boxTrans.position_.x =x-7;
                    boxTrans.position_.z =z-7;
                    boxTrans.position_.y = y;
                    Model::SetTransform(hModel_[type], boxTrans);
                    Model::Draw(hModel_[type]);
                }
            }
        }
    }
   

}

//�J��
void Stage::Release()
{
}

void Stage::SetBlockType(int _x, int _y, BLOCKTYPE _type)
{
    if(_x<XSIZE&&_y<ZSIZE&&_type<BLOCKTYPE::NUM)
    table_[_x][_y].type_ = _type;
}
void Stage::SetBlockHeight(int _x, int _y, int _height)
{
    table_[_x][_y].height_ = _height;
}
