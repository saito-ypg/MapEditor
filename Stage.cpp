#include "Stage.h"
#include"Engine/Model.h"
#include"resource.h"
#include"Engine/Direct3d.h"
#include"Engine/Camera.h"
#include"Engine/Input.h"
//�R���X�g���N�^
Stage::Stage(GameObject* parent):GameObject(parent, "Stage"),hModel_{-1,-1,-1,-1,-1},select_(0)
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
      /*      SetBlockHeight(x, z, rand() % YLIMIT);
            SetBlockType(x, z, (BLOCKTYPE)(rand() %NUM));*/
        }
    }
   
    
    SetBlockType(1, 6, BRICK);
}

//�X�V
void Stage::Update()
{
    using namespace Direct3D;
    float w = (float)scrWidth_ / 2.0f;
    float h= (float)scrHeight_ / 2.0f;
    float offX = 0, offY=0;//������ς���Ƃ��̂��ߎc��
    float maxZ = 1, minZ = 0;//         �V
    XMMATRIX vp =
    {
       w,0,0,0,
       0,-h,0,0,
       0,0,maxZ-minZ,0,
       offX+w,offY+h,minZ,1
    };
    //�e�ϊ��s��̋t�s������
    XMMATRIX vpInv = XMMatrixInverse(nullptr, vp);
    XMMATRIX ProjInv= XMMatrixInverse(nullptr, Camera::GetProjectionMatrix());
    XMMATRIX viewInv = XMMatrixInverse(nullptr, Camera::GetViewMatrix());
    XMMATRIX InvMatrix = vpInv * ProjInv * viewInv;
    
    XMFLOAT3 mousePosFront = Input::GetMousePosition();
    mousePosFront.z = 0.0f;
    XMVECTOR vMousePosFront = XMVector3Transform(XMLoadFloat3(&mousePosFront), InvMatrix);
    XMFLOAT3 mousePosBack = ;
    mousePosBack.z = 1.0f;
    XMVECTOR vMousePosBack = XMVector3Transform(XMLoadFloat3(&mousePosBack), InvMatrix);

    //mouseposFront���x�N�g���ɕϊ��A�s���������
    //mousePosBack���x�N�g���ɕϊ��A�s���������
    //front����back�Ƀ��C��ł�(model�ԍ���hmodel[0]�j�A���C������������u���[�N�|�C���g
}

//�`��
void Stage::Draw()
{
    const int stageSize = 15;
    for (int x = 0; x < stageSize; x++)
    {
        for (int z = 0; z < stageSize; z++)
        {
            for (int y = 0; y <= table_[x][z].height_ ; y++)
            {                
                int type = table_[x][z].type_;
                Transform boxTrans = transform_;
                boxTrans.position_.x = x - 7;
                boxTrans.position_.z = z - 7;
                boxTrans.position_.y = y;
                Model::SetTransform(hModel_[type], boxTrans);
                Model::Draw(hModel_[type]);
                
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
    if(_x<XSIZE&&_y<ZSIZE&&_type<BLOCKTYPE::NUM && _type >= BLOCKTYPE::DEFAULT)
    table_[_x][_y].type_ = _type;
}
void Stage::SetBlockHeight(int _x, int _y, int _height)
{
    if (_x < XSIZE && _y < ZSIZE && _height <YLIMIT&&_height>=0)
    table_[_x][_y].height_ = _height;
}

BOOL Stage::DialogProc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp)
{
    switch (msg)
    {
        //�_�C�A���O���ł����^�C�~���O
    case WM_INITDIALOG:
        //���W�I�{�^�������l
        SendMessage(GetDlgItem(hDlg, IDC_RADIO_UP), BM_SETCHECK, BST_CHECKED, 0);
        //�R���{�{�b�N�X�����l
        SendMessage(GetDlgItem(hDlg, IDC_COMBO_TYPE), CB_ADDSTRING, 0,(LPARAM) "�f�t�H���g");
        SendMessage(GetDlgItem(hDlg, IDC_COMBO_TYPE), CB_ADDSTRING, 0, (LPARAM)"�����K");
        SendMessage(GetDlgItem(hDlg, IDC_COMBO_TYPE), CB_ADDSTRING, 0, (LPARAM)"��");
        SendMessage(GetDlgItem(hDlg, IDC_COMBO_TYPE), CB_ADDSTRING, 0, (LPARAM)"��");
        SendMessage(GetDlgItem(hDlg, IDC_COMBO_TYPE), CB_ADDSTRING, 0, (LPARAM)"��");
        SendMessage(GetDlgItem(hDlg, IDC_COMBO_TYPE), CB_SETCURSEL, 0, 0);
        return TRUE;

    case WM_COMMAND:
        switch (wp)
        {
        case IDC_COMBO_TYPE:
            select_ = SendMessage(GetDlgItem(hDlg, IDC_COMBO_TYPE), CB_GETCURSEL, 0, 0);
        }
        return TRUE;
    }
    return FALSE;
}

