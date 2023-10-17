#include<sstream>
#include "Stage.h"
#include"Engine/Model.h"
#include"resource.h"
#include"Engine/Direct3d.h"
#include"Engine/Camera.h"
#include"Engine/Input.h"
#include<windowsx.h>
//コンストラクタ
Stage::Stage(GameObject* parent):GameObject(parent, "Stage"),hModel_{-1,-1,-1,-1,-1},select_(0),isActive(false)
{
    for(int x=0;x<XSIZE;x++)
    {
        for (int z = 0; z < ZSIZE; z++)
        {
            table_[x][z] = { DEFAULT,0 };
        }
    }
    //table_[7][7].height_ = 4;
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
        }
    }
   
    
 
}

//更新
void Stage::Update()
{

    if (!Input::IsMouseButtonDown(0)||!isActive)
    {
        return;
    }
    using namespace Direct3D;
    float w = (float)(scrWidth_ / 2.0f);
    float h= (float)(scrHeight_ / 2.0f);
    float offX = 0.0f, offY=0.0f;//いずれ変えるときのため残す
    float maxZ = 1.0f, minZ = 0.0f;//         〃
    XMMATRIX vp =
    {
       w,0,0,0,
       0,-h,0,0,
       0,0,maxZ-minZ,0,
       offX+w,offY+h,minZ,1
    };
    //各変換行列の逆行列を作る
    XMMATRIX vpInv = XMMatrixInverse(nullptr, vp);
    XMMATRIX ProjInv= XMMatrixInverse(nullptr, Camera::GetProjectionMatrix());
    XMMATRIX viewInv = XMMatrixInverse(nullptr, Camera::GetViewMatrix());

    XMMATRIX InvMatrix = vpInv * ProjInv * viewInv;
    
    XMFLOAT3 mousePosFront = Input::GetMousePosition();
    mousePosFront.z = 0.0f;
    XMVECTOR vMousePosFront = XMVector3TransformCoord(XMLoadFloat3(&mousePosFront), InvMatrix);
    XMFLOAT3 mousePosBack = Input::GetMousePosition();
    mousePosBack.z = 1.0f;
    XMVECTOR vMousePosBack = XMVector3TransformCoord(XMLoadFloat3(&mousePosBack), InvMatrix);
    int hitX = -1, hitZ = -1;
    float hitDist =9999999;
    for(int x=0;x<XSIZE;x++)
    {
        for (int z = 0; z < ZSIZE; z++)
        {
            for (int y = 0; y <= table_[x][z].height_; y++)
            {
                RayCastData dat;
                XMStoreFloat4(&dat.start, vMousePosFront);
                XMStoreFloat4(&dat.dir, vMousePosBack - vMousePosFront);
                dat.hit = false;
                Transform t;
                t.position_ ={ (float)x-7 , (float)y, (float)z-7 };
                Model::SetTransform(hModel_[0], t);
                Model::RayCast(hModel_[0], dat);
                if (dat.hit) {
                    /*if (hitDist == -1.0f)
                    {
                        hitDist = dat.dist;
                        hitX = x, hitZ = z;
                    }
                    else */if (/*hitX > 0 && */dat.dist<hitDist)
                    {
                        hitDist = dat.dist;
                        hitX = x, hitZ = z;
                    }
                    
                    break;

                }
          
                
            }
        }
    }
    BLOCKINFO& hitBox = table_[hitX][hitZ];
    switch (mode_)
    {
    case MODE::UP:
        
        if(hitBox.height_<YLIMIT)
        hitBox.height_++;
        break;
    case MODE::DOWN: 
        if (hitBox.height_>0)
            hitBox.height_--;
        break;
    case MODE::CHANGE:
        SetBlockType(hitX, hitZ, (BLOCKTYPE)select_);
        break;
    case MODE::SET:
        SetBlockHeight(hitX, hitZ, editHeight_);
        break;
    case MODE::ALL: 
        SetBlockType(hitX, hitZ, (BLOCKTYPE)select_);
        SetBlockHeight(hitX, hitZ, editHeight_);
        break;
    }

    //mouseposFrontをベクトルに変換、行列をかける
    //mousePosBackもベクトルに変換、行列をかける
    //frontからbackにレイを打ち(model番号はhmodel[0]）、レイが当たったらブレークポイント
}

//描画
void Stage::Draw()
{

    for (int x = 0; x < XSIZE; x++)
    {
        for (int z = 0; z < ZSIZE; z++)
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

BLOCKTYPE Stage::GetBlockType(int _x, int _y)
{
    return table_[_x][_y].type_;
}

int Stage::GetBlockHeight(int _x, int _y)
{
    return table_[_x][_y].height_;
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
void Stage::SaveStage()
{
    char fileName[MAX_PATH] = "無題.map";  //ファイル名を入れる変数
    //「ファイルを保存」ダイアログの設定
    OPENFILENAME ofn;                         	//名前をつけて保存ダイアログの設定用構造体
    ZeroMemory(&ofn, sizeof(ofn));            	//構造体初期化
    ofn.lStructSize = sizeof(OPENFILENAME);   	//構造体のサイズ
    ofn.lpstrFilter = TEXT("マップデータ(*.map)\0*.map\0")        //─┬ファイルの種類
        TEXT("テキストファイル(*.txt)\0*.txt\0")				  //  |
        TEXT("すべてのファイル(*.*)\0*.*\0\0");					  //─┘
    ofn.lpstrFile = fileName;               	//ファイル名
    ofn.nMaxFile = MAX_PATH;               	//パスの最大文字数
    ofn.Flags = OFN_OVERWRITEPROMPT;   		//フラグ（同名ファイルが存在したら上書き確認）
    ofn.lpstrDefExt = "map";                  	//デフォルト拡張子
    //「ファイルを保存」ダイアログ
    BOOL selFile;
    selFile = GetSaveFileName(&ofn);
    //キャンセルしたら中断
    if (selFile == FALSE) return;

    //ここからファイル内容
    HANDLE hFile;        //ファイルのハンドル
    hFile = CreateFile(
        fileName,                 //ファイル名
        GENERIC_WRITE,           //アクセスモード（書き込み用）
        0,                      //共有（なし）
        NULL,                   //セキュリティ属性（継承しない）
        CREATE_ALWAYS,           //作成方法
        FILE_ATTRIBUTE_NORMAL,  //属性とフラグ（設定なし）
        NULL);                  //拡張属性（なし）
    std::string writeStr;
   
    for (int h = 0; h < ZSIZE; h++)
    {
        for (int w = 0; w < XSIZE; w++)
        {
            writeStr += std::to_string(table_[w][h].type_) + " " + std::to_string(table_[w][h].height_);
            /*if (w < XSIZE - 1)
            {
                writeStr += ",";
            }
            else
            {
                writeStr += "\n";
            }*/
            writeStr += ",";
        }
        writeStr += "\n";
    }
    DWORD dwBytes = 0;  //書き込み位置
    WriteFile(
        hFile,                   //ファイルハンドル
        writeStr.c_str(),                  //保存するデータ（文字列）
        (DWORD)strlen(writeStr.c_str()),   //書き込む文字数
        &dwBytes,                //書き込んだサイズを入れる変数
        NULL);                   //オーバーラップド構造体（今回は使わない）

    CloseHandle(hFile);
}

void Stage::LoadStage()
{
    char fileName[MAX_PATH] = "無題.map";  //ファイル名を入れる変数
    //「ファイルを保存」ダイアログの設定
    OPENFILENAME ofn;                         	//名前をつけて保存ダイアログの設定用構造体
    ZeroMemory(&ofn, sizeof(ofn));            	//構造体初期化
    ofn.lStructSize = sizeof(OPENFILENAME);   	//構造体のサイズ
    ofn.lpstrFilter = TEXT("マップデータ(*.map)\0*.map\0")        //─┬ファイルの種類
        TEXT("テキストファイル(*.txt)\0*.txt\0")				  //   |
        TEXT("すべてのファイル(*.*)\0*.*\0");					  //─┘
    ofn.lpstrFile = fileName;               	//ファイル名
    ofn.nMaxFile = MAX_PATH;               	//パスの最大文字数
    ofn.Flags = OFN_FILEMUSTEXIST;   		//フラグ（同名ファイルが存在したら上書き確認）
    ofn.lpstrDefExt = "map";                  	//デフォルト拡張子
    //「ファイルを開く/」ダイアログ
    BOOL selFile;
    selFile = GetOpenFileName(&ofn);
    //キャンセルしたら中断
    if (selFile == FALSE)
        return;
    /////////ここから読み込み///////
    HANDLE hFile;        //ファイルのハンドル
    hFile = CreateFile(
        fileName,                 //ファイル名
        GENERIC_READ,           //アクセスモード（読み込み用）
        0,                      //共有（なし）
        NULL,                   //セキュリティ属性（継承しない）
        OPEN_EXISTING,           //作成方法
        FILE_ATTRIBUTE_NORMAL,  //属性とフラグ（設定なし）
        NULL);                  //拡張属性（なし）
     //ファイルのサイズを取得
    DWORD fileSize = GetFileSize(hFile, NULL);

    //ファイルのサイズ分メモリを確保
    char* data;
    data = new char[fileSize];

    DWORD dwBytes = 0; //読み込み位置

    BOOL isread=ReadFile(
        hFile,     //ファイルハンドル
        data,      //データを入れる変数
        fileSize,  //読み込むサイズ
        &dwBytes,  //読み込んだサイズ
        NULL);     //オーバーラップド構造体（今回は使わない）
    if (isread == FALSE)
    {
        MessageBox(nullptr, "指定したファイルを読み込めませんでした", "エラー", MB_OK);
        return;
    }
    CloseHandle(hFile);
    std::istringstream iss{ data };
    std::string Line;
    /*char bufSpace;*/
    //char bufComma;
    
    int indexY = 0;
    while (std::getline(iss, Line, '\n'))
    {
        
        std::istringstream sLine{ Line };
        std::string info;
        int indexX = 0;
        while (std::getline(sLine,info, ','))
        {
            std::stringstream sInfo{ info };
            int type;
            int height;
            sInfo >>type>>height;
            SetBlockHeight(indexX, indexY, height);
            SetBlockType(indexX,indexY,(BLOCKTYPE)type);
            indexX++;
        }
        indexY++;
    }


}
void Stage::InitStage()
{
    for (int i = 0; i < ZSIZE; i++)
    {
        for (int j = 0; j < XSIZE; j++)
        {
            SetBlockHeight(j, i, 0);
            SetBlockType(j, i, DEFAULT);
        }
    }
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

        Edit_SetText(GetDlgItem(hDlg, ID_EDIT_HEIGHT), "0");
        char buf[4];
        GetWindowText(GetDlgItem(hDlg, ID_EDIT_HEIGHT), buf, 4);
        editHeight_ = std::stoi(buf);
        //EnableWindow(GetDlgItem(hDlg, IDC_COMBO_TYPE), FALSE);
        return TRUE;

    case WM_COMMAND:

        switch (LOWORD(wp))
        {

        case IDC_RADIO_UP:
            mode_ = MODE::UP; break;
        case IDC_RADIO_DOWN:
            mode_ = MODE::DOWN; break;
        case IDC_RADIO_CHANGE:
            mode_ = MODE::CHANGE; break;
        case IDC_RADIO_HEIGHT:
            mode_ = MODE::SET; break;
        case IDC_RADIO_ALL:
            mode_ = MODE::ALL; break;
        case IDC_COMBO_TYPE:
            if (HIWORD(wp) == CBN_SELCHANGE)
                select_ = (int)SendMessage(GetDlgItem(hDlg, IDC_COMBO_TYPE), CB_GETCURSEL, 0, 0);
            break;
        case ID_EDIT_HEIGHT:
            if (HIWORD(wp) == EN_CHANGE)
			{
				char buf[4];
				GetWindowText(GetDlgItem(hDlg, ID_EDIT_HEIGHT), buf, 4);
				editHeight_ = std::stoi(buf);
			}
            break;
        }
      /*  if(LOWORD(wp)==IDC_RADIO_CHANGE||LOWORD(wp)==IDC_RADIO_ALL || LOWORD(wp) == IDC_COMBO_TYPE)
        {
            EnableWindow(GetDlgItem(hDlg, IDC_COMBO_TYPE), TRUE);
        }
        else
        {
            EnableWindow(GetDlgItem(hDlg, IDC_COMBO_TYPE), FALSE);
        }
        if (LOWORD(wp) == IDC_RADIO_HEIGHT || LOWORD(wp) == IDC_RADIO_ALL||LOWORD(wp)==ID_EDIT_HEIGHT)
        {

            EnableWindow(GetDlgItem(hDlg, ID_EDIT_HEIGHT), TRUE);
        }
        else
        {
            EnableWindow(GetDlgItem(hDlg, ID_EDIT_HEIGHT), FALSE);
        }
        */

        return TRUE;
    case WM_ACTIVATE:
        if(LOWORD(wp)==WA_CLICKACTIVE)
            isDialogActive_ = true;
        else if(LOWORD(wp)==WA_INACTIVE)
            isDialogActive_ = false;
        return TRUE;
    }
    
    return FALSE;
}

