#include "Controller.h"
#include"Engine/Camera.h"
#include"Engine/Input.h"
namespace { const int ROTATESPD = 2.5f; }
//コンストラクタ
Controller::Controller(GameObject* parent)
    :GameObject(parent, "Controller"),camPos(XMVectorZero()), camTarget(XMVectorZero())
{
}

//デストラクタ
Controller::~Controller()
{   
}

//初期化
void Controller::Initialize()
{
    camPos= Camera::GetPosition();
    camTarget=Camera::GetTarget();

}

//更新
void Controller::Update()
{
    float  const move = 0.1;
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
   /* if (Input::IsKey(DIK_UP)) 
    {
    }
    if (Input::IsKey(DIK_DOWN)) 
    {
    }*/
    Camera::SetTarget(camTarget);
    XMVECTOR vCam = camTarget;
    vCam = XMVector3TransformCoord(vCam, mRotY);
    XMFLOAT3 camPosY;
    XMStoreFloat3(&camPosY, camPos + vCam);

    Camera::SetPosition(camPosY);
   
}

//描画
void Controller::Draw()
{
}

//開放
void Controller::Release()
{
}

