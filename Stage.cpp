#include<sstream>
#include "Stage.h"
#include"Engine/Model.h"
#include"resource.h"
#include"Engine/Direct3d.h"
#include"Engine/Camera.h"
#include"Engine/Input.h"
#include<windowsx.h>
#include<Commctrl.h>
//�R���X�g���N�^
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

//�f�X�g���N�^
Stage::~Stage()
{
}

//������
void Stage::Initialize()
{
    string modelname[NUM] = { "Default","Brick","Grass","Sand","Water" };
    //���f���f�[�^�̃��[�h
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

//�X�V
void Stage::Update()
{

    if (!Input::IsMouseButtonDown(0)||!isActive)
    {
        return;
    }
    using namespace Direct3D;
    float w = (float)(scrWidth_ / 2.0f);
    float h= (float)(scrHeight_ / 2.0f);
    XMMATRIX vp =
    {
       w,0,0,0,
       0,-h,0,0,
       0,0,1,0,
       w,h,0,1
    };
    //�e�ϊ��s��̋t�s������
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
    float hitDist =FLT_MAX;
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
                    if (dat.dist<hitDist)
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

    //mouseposFront���x�N�g���ɕϊ��A�s���������
    //mousePosBack���x�N�g���ɕϊ��A�s���������
    //front����back�Ƀ��C��ł�(model�ԍ���hmodel[0]�j�A���C������������u���[�N�|�C���g
}

//�`��
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

//�J��
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
    char fileName[MAX_PATH] = "����.map";  //�t�@�C����������ϐ�
    //�u�t�@�C����ۑ��v�_�C�A���O�̐ݒ�
    OPENFILENAME ofn;                         	//���O�����ĕۑ��_�C�A���O�̐ݒ�p�\����
    ZeroMemory(&ofn, sizeof(ofn));            	//�\���̏�����
    ofn.lStructSize = sizeof(OPENFILENAME);   	//�\���̂̃T�C�Y
    ofn.lpstrFilter = TEXT("�}�b�v�f�[�^(*.map)\0*.map\0")        //�����t�@�C���̎��
        TEXT("�e�L�X�g�t�@�C��(*.txt)\0*.txt\0")				  //  |
        TEXT("���ׂẴt�@�C��(*.*)\0*.*\0\0");					  //����
    ofn.lpstrFile = fileName;               	//�t�@�C����
    ofn.nMaxFile = MAX_PATH;               	//�p�X�̍ő啶����
    ofn.Flags = OFN_OVERWRITEPROMPT;   		//�t���O�i�����t�@�C�������݂�����㏑���m�F�j
    ofn.lpstrDefExt = "map";                  	//�f�t�H���g�g���q
    //�u�t�@�C����ۑ��v�_�C�A���O
    BOOL selFile;
    selFile = GetSaveFileName(&ofn);
    //�L�����Z�������璆�f
    if (selFile == FALSE) return;

    //��������t�@�C�����e
    HANDLE hFile;        //�t�@�C���̃n���h��
    hFile = CreateFile(
        fileName,                 //�t�@�C����
        GENERIC_WRITE,           //�A�N�Z�X���[�h�i�������ݗp�j
        0,                      //���L�i�Ȃ��j
        NULL,                   //�Z�L�����e�B�����i�p�����Ȃ��j
        CREATE_ALWAYS,           //�쐬���@
        FILE_ATTRIBUTE_NORMAL,  //�����ƃt���O�i�ݒ�Ȃ��j
        NULL);                  //�g�������i�Ȃ��j
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
    DWORD dwBytes = 0;  //�������݈ʒu
    WriteFile(
        hFile,                   //�t�@�C���n���h��
        writeStr.c_str(),                  //�ۑ�����f�[�^�i������j
        (DWORD)strlen(writeStr.c_str()),   //�������ޕ�����
        &dwBytes,                //�������񂾃T�C�Y������ϐ�
        NULL);                   //�I�[�o�[���b�v�h�\���́i����͎g��Ȃ��j

    CloseHandle(hFile);
}

void Stage::LoadStage()
{
    char fileName[MAX_PATH] = "����.map";  //�t�@�C����������ϐ�
    //�u�t�@�C����ۑ��v�_�C�A���O�̐ݒ�
    OPENFILENAME ofn;                         	//���O�����ĕۑ��_�C�A���O�̐ݒ�p�\����
    ZeroMemory(&ofn, sizeof(ofn));            	//�\���̏�����
    ofn.lStructSize = sizeof(OPENFILENAME);   	//�\���̂̃T�C�Y
    ofn.lpstrFilter = TEXT("�}�b�v�f�[�^(*.map)\0*.map\0")        //�����t�@�C���̎��
        TEXT("�e�L�X�g�t�@�C��(*.txt)\0*.txt\0")				  //   |
        TEXT("���ׂẴt�@�C��(*.*)\0*.*\0");					  //����
    ofn.lpstrFile = fileName;               	//�t�@�C����
    ofn.nMaxFile = MAX_PATH;               	//�p�X�̍ő啶����
    ofn.Flags = OFN_FILEMUSTEXIST;   		//�t���O�i�����t�@�C�������݂�����㏑���m�F�j
    ofn.lpstrDefExt = "map";                  	//�f�t�H���g�g���q
    //�u�t�@�C�����J��/�v�_�C�A���O
    BOOL selFile;
    selFile = GetOpenFileName(&ofn);
    //�L�����Z�������璆�f
    if (selFile == FALSE)
        return;
    /////////��������ǂݍ���///////
    HANDLE hFile;        //�t�@�C���̃n���h��
    hFile = CreateFile(
        fileName,                 //�t�@�C����
        GENERIC_READ,           //�A�N�Z�X���[�h�i�ǂݍ��ݗp�j
        0,                      //���L�i�Ȃ��j
        NULL,                   //�Z�L�����e�B�����i�p�����Ȃ��j
        OPEN_EXISTING,           //�쐬���@
        FILE_ATTRIBUTE_NORMAL,  //�����ƃt���O�i�ݒ�Ȃ��j
        NULL);                  //�g�������i�Ȃ��j
     //�t�@�C���̃T�C�Y���擾
    DWORD fileSize = GetFileSize(hFile, NULL);

    //�t�@�C���̃T�C�Y�����������m��
    char* data;
    data = new char[fileSize];

    DWORD dwBytes = 0; //�ǂݍ��݈ʒu
    
    BOOL isread=ReadFile(
        hFile,     //�t�@�C���n���h��
        data,      //�f�[�^������ϐ�
        fileSize,  //�ǂݍ��ރT�C�Y
        &dwBytes,  //�ǂݍ��񂾃T�C�Y
        NULL);     //�I�[�o�[���b�v�h�\���́i����͎g��Ȃ��j
    if (isread == FALSE)
    {
        MessageBox(nullptr, "�w�肵���t�@�C����ǂݍ��߂܂���ł���", "�G���[", MB_OK);
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
        //�_�C�A���O���ł����^�C�~���O
    case WM_INITDIALOG:
        //���W�I�{�^�������l
        SendMessage(GetDlgItem(hDlg, IDC_RADIO_UP), BM_SETCHECK, BST_CHECKED, 0);
        //�R���{�{�b�N�X�����l
        SendMessage(GetDlgItem(hDlg, IDC_COMBO_TYPE), CB_ADDSTRING, 0,(LPARAM) "�f�t�H���g");
        SendMessage(GetDlgItem(hDlg, IDC_COMBO_TYPE), CB_ADDSTRING, 0, (LPARAM)"�����K");
        SendMessage(GetDlgItem(hDlg, IDC_COMBO_TYPE), CB_ADDSTRING, 0, (LPARAM)"��");
        SendMessage(GetDlgItem(hDlg, IDC_COMBO_TYPE), CB_ADDSTRING, 0, (LPARAM)"��");
        SendMessage(GetDlgItem(hDlg, IDC_COMBO_TYPE), CB_ADDSTRING, 0, (LPARAM)"��");
        SendMessage(GetDlgItem(hDlg, IDC_COMBO_TYPE), CB_SETCURSEL, 0, 0);

        char buf[4];
        GetWindowText(GetDlgItem(hDlg, ID_EDIT_HEIGHT), buf, 4);
        editHeight_ = std::stoi(buf);
        EnableWindow(GetDlgItem(hDlg, IDC_COMBO_TYPE), FALSE);
        EnableWindow(GetDlgItem(hDlg, ID_EDIT_HEIGHT), FALSE);
        EnableWindow(GetDlgItem(hDlg, IDC_SPIN_HEIGHT), FALSE);
        SendMessage(GetDlgItem(hDlg, IDC_SPIN_HEIGHT), UDM_SETRANGE, 0, (LPARAM)MAKEWORD(49,0));
        return TRUE;

    case WM_COMMAND:
        class ACTIVE {
        public:
            bool type = false, height = false;
            void Set(bool t, bool h) { type = t, height = h; }
        }active;//combobox,edit,spin��L���ɂ��邩�Ɏg�p
        switch (LOWORD(wp))
        {

        case IDC_RADIO_UP:
            mode_ = MODE::UP;
            active.Set(false, false); break;
        case IDC_RADIO_DOWN:
            mode_ = MODE::DOWN;
            active.Set(false, false); break;
        case IDC_RADIO_CHANGE:
            mode_ = MODE::CHANGE; 
            active.Set(true, false); break;
        case IDC_RADIO_HEIGHT:
            mode_ = MODE::SET; 
            active.Set(false,true); break;
        case IDC_RADIO_ALL:
            mode_ = MODE::ALL;
            active.Set(true, true); break;
        case IDC_COMBO_TYPE:
            if (HIWORD(wp) == CBN_SELCHANGE)
                select_ = (int)SendMessage(GetDlgItem(hDlg, IDC_COMBO_TYPE), CB_GETCURSEL, 0, 0);
            active.Set(true, false);
            break;
        case IDC_SPIN_HEIGHT:
            active.Set(false, true); break;
        case ID_EDIT_HEIGHT:
            active.Set(false,true); break;
        }
        if (IsDlgButtonChecked(hDlg, IDC_RADIO_ALL) == BST_CHECKED)
            active.Set(true, true);
		EnableWindow(GetDlgItem(hDlg, IDC_COMBO_TYPE), active.type);
		EnableWindow(GetDlgItem(hDlg, ID_EDIT_HEIGHT), active.height);
		EnableWindow(GetDlgItem(hDlg, IDC_SPIN_HEIGHT), active.height);
        
        if (HIWORD(wp) == EN_KILLFOCUS)//editControl�̓��͏I�������
        {//���g�����o��
            char buf[4];
            GetWindowText(GetDlgItem(hDlg, ID_EDIT_HEIGHT), buf, 4);
            int h = std::stoi(buf);
            if (h >= YLIMIT || h < 0)//�͈͊O�Ȃ烁�b�Z�[�W�o���Ēl�߂�
            {
                string t = { "0�`" +std::to_string(YLIMIT-1) + "�̒l�ɂ��Ă�������" };
                MessageBox(nullptr,t.c_str(), "�s���Ȓl", MB_OK);
                string tmp = std::to_string(editHeight_);
                SetWindowText(GetDlgItem(hDlg, ID_EDIT_HEIGHT), tmp.c_str());
            }
            editHeight_ = h;
        }
        return TRUE;

    }
    return FALSE;
}

