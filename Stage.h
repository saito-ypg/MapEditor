#pragma once
#include "Engine/GameObject.h"
#include<Windows.h>
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
    int hModel_[NUM];    //���f���ԍ� 
    BLOCKINFO table_[XSIZE][ZSIZE];
    
    enum MODE{UP,DOWN,CHANGE} mode_;//���W�I�{�^���őI�����Ă��郂�[�h�ɉ����ĕύX
    int select_;//�R���{�{�b�N�X�őI������Ă�����

public:
    //�R���X�g���N�^
    Stage(GameObject* parent);

    //�f�X�g���N�^
    ~Stage();

    //������
    void Initialize() override;

    //�X�V
    void Update() override;

    //�`��
    void Draw() override;

    //�J��
    void Release() override;

    void SetBlockType(int _x, int _y, BLOCKTYPE _type);
    void SetBlockHeight(int _x, int _y, int _height);

    BOOL DialogProc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp);
};