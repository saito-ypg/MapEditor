#pragma once
#include "Engine/GameObject.h"
#include<Windows.h>
namespace INFO{

    const int XSIZE{ 5 };
    const int ZSIZE{ 15 };
    const int YLIMIT{ 50 };
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
using namespace INFO;
class Stage : public GameObject
{
private:
    int hModel_[NUM];    //モデル番号 
    BLOCKINFO table_[ZSIZE][XSIZE];
    
    enum MODE{UP,DOWN,CHANGE,SET,ALL} mode_;//ラジオボタンで選択しているモードに応じて変更
    int select_;//コンボボックスで選択されている種類
    int editHeight_;//高さ指定に使う
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

    bool isActive;//メインのウィンドウがアクティブか
    BLOCKTYPE GetBlockType(int _x, int _y);
    int GetBlockHeight(int _x, int _y);
    void SetBlockType(int _x, int _y, BLOCKTYPE _type);
    void SetBlockHeight(int _x, int _y, int _height);
    void SaveStage();//ダイアログで呼ばれる
    void LoadStage();//ダイアログで呼ばれる
    void InitStage();//ダイアログで呼ばれる。更地にするだけ
    BOOL DialogProc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp);
};