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
    camPos = Camera::GetPosition();
    
}

//更新
void Controller::Update()
{
}

//描画
void Controller::Draw()
{
}

//開放
void Controller::Release()
{
}

