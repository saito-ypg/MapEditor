#pragma once
#include "Engine/GameObject.h"
namespace {

    const int XSIZE{ 15 };
    const int ZSIZE{ 15 };
    const int YLIMIT{ 10 };
    enum BLOCKTYPE
    {
        DEFAULT, BRICK, GRASS, SAND, WATER,NUM
    };
    struct BLOCKINFO
    {
        BLOCKTYPE type_;
        int height_;
    };
}
class Stage : public GameObject
{
   
    
    int hModel_[NUM];    //モデル番号 
    BLOCKINFO table_[XSIZE][ZSIZE];
public:
    //コンストラクタ
    Stage(GameObject* parent);

    //デストラクタ
    ~Stage();

    //初期化
    void Initialize() override;

    //更新
    void Update() override;

    //描画
    void Draw() override;

    //開放
    void Release() override;

    void SetBlockType(int _x, int _y, BLOCKTYPE _type);
    void SetBlockHeight(int _x, int _y, int _height);
};