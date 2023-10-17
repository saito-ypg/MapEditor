#pragma once
#include "Engine/GameObject.h"
#include<Windows.h>
namespace INFO{

    const int XSIZE{ 15 };
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
    int hModel_[NUM];    //���f���ԍ� 
    BLOCKINFO table_[XSIZE][ZSIZE];
    
    enum MODE{UP,DOWN,CHANGE,SET,ALL} mode_;//���W�I�{�^���őI�����Ă��郂�[�h�ɉ����ĕύX
    int select_;//�R���{�{�b�N�X�őI������Ă�����
    bool isDialogActive_;
    int editHeight_;//�����ݒ�Ɏg���B
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

    bool isActive;
    BLOCKTYPE GetBlockType(int _x, int _y);
    int GetBlockHeight(int _x, int _y);
    void SetBlockType(int _x, int _y, BLOCKTYPE _type);
    void SetBlockHeight(int _x, int _y, int _height);
    void SaveStage();//main�̃_�C�A���O�ŌĂ΂��
    void LoadStage();//
    void InitStage();//�����\��
    BOOL DialogProc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp);
};