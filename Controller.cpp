#include "Controller.h"
#include"Engine/Camera.h"
#include"Engine/Input.h"
namespace { const int ROTATESPD = 2.5f; }
//�R���X�g���N�^
Controller::Controller(GameObject* parent)
    :GameObject(parent, "Controller")
{
}

//�f�X�g���N�^
Controller::~Controller()
{   
}

//������
void Controller::Initialize()
{
 

}

//�X�V
void Controller::Update()
{
   //�_������������
    /*   float  const move = 0.1;
    float const rot =1;
    if (Input::IsKey(DIK_LEFT))
        transform_.rotate_.y -= rot;
    if (Input::IsKey(DIK_RIGHT))
        transform_.rotate_.y += rot;
    XMMATRIX mRotY = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));
    if (Input::IsKey(DIK_W))
    {
        XMVECTOR v{ 0, 0, move,0 };
        v = XMVector3TransformCoord(v, mRotY);
        camPos += v;
        camTarget += v;
    }
    if (Input::IsKey(DIK_S))
    {
        XMVECTOR v{ 0, 0, move,0 };
        v = XMVector3TransformCoord(v, mRotY);
        camPos -= v;
        camTarget -= v;
    }
    if (Input::IsKey(DIK_A))
    {
        XMVECTOR v{move,0, 0,0 };
        v = XMVector3TransformCoord(v, mRotY);
        camPos -= v;
        camTarget -= v;
    }
    if (Input::IsKey(DIK_D))
    {
        XMVECTOR v{ move,0, 0,0 };
        v = XMVector3TransformCoord(v, mRotY);
        camPos += v;
        camTarget += v;
    }
    Camera::SetPosition(camPos);
    if (Input::IsKey(DIK_UP)) 
    {
    }
    if (Input::IsKey(DIK_DOWN)) 
    {
    }
    Camera::SetTarget(camTarget);
    XMVECTOR vCam = camTarget;
    vCam = XMVector3TransformCoord(vCam, mRotY);
    XMFLOAT3 camPosY;
    XMStoreFloat3(&camPosY, camPos + vCam);
  Camera::SetPosition(camPosY);*/
    const float rotSpd = 1.0f;
    const float mvSpd = 0.1f;
   //����]
    if (Input::IsKey(DIK_LEFT))
    {
        transform_.rotate_.y -=rotSpd;
    }

    //�E��]
    if (Input::IsKey(DIK_RIGHT))
    {
        transform_.rotate_.y += rotSpd;
    }
    //���]
    if (Input::IsKey(DIK_UP))
    {
        transform_.rotate_.x += rotSpd;
        if (transform_.rotate_.x > 45)
            transform_.rotate_.x = 44.9999;
    }

    //������]
    if (Input::IsKey(DIK_DOWN))
    {
        transform_.rotate_.x -= rotSpd;
        if (transform_.rotate_.x <-45)
            transform_.rotate_.x = -45;
    }
    //transform_.rotate_.y�̒l�ɍ��킹�Ăx����]������s��
    XMMATRIX mRotateY = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));
    //transform_.rotate_.x�̒l�ɍ��킹��X����]������s��
    XMMATRIX mRotateX = XMMatrixRotationX(XMConvertToRadians(transform_.rotate_.x));
    XMMATRIX mRotate = mRotateX * mRotateY;
    //���݈ʒu���x�N�g���ɂ��Ă���
    XMVECTOR vPos = XMLoadFloat3(&transform_.position_);

    //�O��ړ��x�N�g��
    XMVECTOR frontMove = XMVectorSet(0, 0, mvSpd ,0);            //��������XMVECTOR�\���̂�p�ӂ�
    frontMove = XMVector3TransformCoord(frontMove, mRotateY);    //�ړ��x�N�g��


    if (Input::IsKey(DIK_W))
    {
        vPos += frontMove;
    }
    if (Input::IsKey(DIK_S))
    {
        vPos -= frontMove;   
    }
    //���E�ړ��x�N�g��
    XMVECTOR rightMove =XMVectorSet( mvSpd, 0, 0 ,0);                //�E������XMFLOAT3�\���̂�p�ӂ�
    rightMove = XMVector3TransformCoord(rightMove, mRotateY);    //�x�N�g����]
    if (Input::IsKey(DIK_D))
    {
        vPos += rightMove;
    }
    if (Input::IsKey(DIK_A))
    {
        vPos -= rightMove;
    }
    XMStoreFloat3(&transform_.position_, vPos);//vPos�̌��ʂ�transform�ɖ߂��Ĕ��f������
    //�J����
    XMVECTOR vCam = XMVectorSet(0, 10, -10, 0);
    vCam = XMVector3TransformCoord(vCam, mRotate);
    Camera::SetPosition(vPos + vCam);
    Camera::SetTarget(transform_.position_);        //�J�����̏œ_�̓v���C���[�̈ʒu
}

//�`��
void Controller::Draw()
{
}

//�J��
void Controller::Release()
{
}

