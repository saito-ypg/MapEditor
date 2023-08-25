#pragma once
#include "Engine/GameObject.h"

class Controller : public GameObject
{
    XMVECTOR camPos;
    XMVECTOR camRot;

public:
    //コンストラクタ
    Controller(GameObject* parent);

    //デストラクタ
    ~Controller();

    //初期化
    void Initialize() override;

    //更新
    void Update() override;

    //描画
    void Draw() override;

    //開放
    void Release() override;
};