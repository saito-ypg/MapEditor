#include "Controller.h"
#include"Engine/Camera.h"
#include"Engine/Input.h"
namespace { const int ROTATESPD = 2.5f; }
//コンストラクタ
Controller::Controller(GameObject* parent)
    :GameObject(parent, "Controller")
{
}

//デストラクタ
Controller::~Controller()
{   
}

//初期化
void Controller::Initialize()
{
 

}

//更新
void Controller::Update()
{
   //ダメだったもの
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
   //左回転
    if (Input::IsKey(DIK_LEFT))
    {
        transform_.rotate_.y -=rotSpd;
    }

    //右回転
    if (Input::IsKey(DIK_RIGHT))
    {
        transform_.rotate_.y += rotSpd;
    }
    //上回転
    if (Input::IsKey(DIK_UP))
    {
        transform_.rotate_.x += rotSpd;
        if (transform_.rotate_.x > 45)
            transform_.rotate_.x = 44.9999;
    }

    //した回転
    if (Input::IsKey(DIK_DOWN))
    {
        transform_.rotate_.x -= rotSpd;
        if (transform_.rotate_.x <-45)
            transform_.rotate_.x = -45;
    }
    //transform_.rotate_.yの値に合わせてＹ軸回転させる行列
    XMMATRIX mRotateY = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));
    //transform_.rotate_.xの値に合わせてX軸回転させる行列
    XMMATRIX mRotateX = XMMatrixRotationX(XMConvertToRadians(transform_.rotate_.x));
    XMMATRIX mRotate = mRotateX * mRotateY;
    //現在位置をベクトルにしておく
    XMVECTOR vPos = XMLoadFloat3(&transform_.position_);

    //前後移動ベクトル
    XMVECTOR frontMove = XMVectorSet(0, 0, mvSpd ,0);            //奥向きのXMVECTOR構造体を用意し
    frontMove = XMVector3TransformCoord(frontMove, mRotateY);    //移動ベクトル


    if (Input::IsKey(DIK_W))
    {
        vPos += frontMove;
    }
    if (Input::IsKey(DIK_S))
    {
        vPos -= frontMove;   
    }
    //左右移動ベクトル
    XMVECTOR rightMove =XMVectorSet( mvSpd, 0, 0 ,0);                //右向きのXMFLOAT3構造体を用意し
    rightMove = XMVector3TransformCoord(rightMove, mRotateY);    //ベクトル回転
    if (Input::IsKey(DIK_D))
    {
        vPos += rightMove;
    }
    if (Input::IsKey(DIK_A))
    {
        vPos -= rightMove;
    }
    XMStoreFloat3(&transform_.position_, vPos);//vPosの結果をtransformに戻して反映させる
    //カメラ
    XMVECTOR vCam = XMVectorSet(0, 10, -10, 0);
    vCam = XMVector3TransformCoord(vCam, mRotate);
    Camera::SetPosition(vPos + vCam);
    Camera::SetTarget(transform_.position_);        //カメラの焦点はプレイヤーの位置
}

//描画
void Controller::Draw()
{
}

//開放
void Controller::Release()
{
}

