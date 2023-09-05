#pragma once
#include<DirectXMath.h>
#include <dInput.h>
using namespace DirectX;
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dInput8.lib")

#define SAFE_RELEASE(p) if(p != nullptr){ p->Release(); p = nullptr;}

namespace Input
{
	HRESULT Initialize(HWND hWnd);
	HRESULT Update();
	void Release();
	//キーボード
	bool IsKey(int keyCode);//キーを押している間true
	bool IsKeyDown(int keyCode);//キーを押した瞬間のみtrue
	bool IsKeyUp(int keyCode);//キーを離した瞬間のみtrue
	//マウス
	bool IsMouseButton(int buttonCode);//マウスのボタンを押している間true
	bool IsMouseButtonDown(int buttonCode);//ボタンを押した瞬間のみtrue
	bool IsMouseButtonUp(int buttonCode);//ボタンを離した瞬間のみtrue
	XMFLOAT3 GetMousePosition();//マウスカーソルの位置を取得
	XMFLOAT3 GetMouseMove();//現フレームのマウスの移動量を取得
	void SetMousePosition(int x, int y);
	
};
