#include "Stage.h"
#include"Engine/Model.h"
#include"resource.h"
//コンストラクタ
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

//デストラクタ
Stage::~Stage()
{
}

//初期化
void Stage::Initialize()
{
    string modelname[NUM] = { "Default","Brick","Grass","Sand","Water" };
    //モデルデータのロード
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

//開放
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
        //ダイアログができたタイミング
    case WM_INITDIALOG:
        //ラジオボタン初期値
        SendMessage(GetDlgItem(hDlg, IDC_RADIO_UP), BM_SETCHECK, BST_CHECKED, 0);
        //コンボボックス初期値
        SendMessage(GetDlgItem(hDlg, IDC_COMBO_TYPE), CB_ADDSTRING, 0,(LPARAM) "デフォルト");
        SendMessage(GetDlgItem(hDlg, IDC_COMBO_TYPE), CB_ADDSTRING, 0, (LPARAM)"レンガ");
        SendMessage(GetDlgItem(hDlg, IDC_COMBO_TYPE), CB_ADDSTRING, 0, (LPARAM)"草");
        SendMessage(GetDlgItem(hDlg, IDC_COMBO_TYPE), CB_ADDSTRING, 0, (LPARAM)"砂");
        SendMessage(GetDlgItem(hDlg, IDC_COMBO_TYPE), CB_ADDSTRING, 0, (LPARAM)"水");
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

