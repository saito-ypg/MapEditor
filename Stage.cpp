#include "Stage.h"
#include"Engine/Model.h"
//コンストラクタ
Stage::Stage(GameObject* parent)	 :GameObject(parent, "Stage"), hModel_(-1)
{
}

//デストラクタ
Stage::~Stage()
{
}

//初期化
void Stage::Initialize()
{
    //モデルデータのロード
    hModel_ = Model::Load("Assets/BoxDefault.fbx");
    assert(hModel_ >= 0);
}

//更新
void Stage::Update()
{
}

//描画
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

//開放
void Stage::Release()
{
}