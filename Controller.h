#pragma once
#include "Engine/GameObject.h"

class Controller : public GameObject
{
    XMVECTOR camPos;
    XMVECTOR camTarget;
    XMVECTOR vCam;
public:
    //�R���X�g���N�^
    Controller(GameObject* parent);

    //�f�X�g���N�^
    ~Controller();

    //������
    void Initialize() override;

    //�X�V
    void Update() override;

    //�`��
    void Draw() override;

    //�J��
    void Release() override;
};