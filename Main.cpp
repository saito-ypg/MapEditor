#include <Windows.h>
#pragma comment(lib, "winmm.lib")
#include<cstdlib>
#include<DirectXCollision.h>
#include"Engine/Direct3D.h"
#include"Engine/Camera.h"
#include"Engine/Input.h"
#include"Engine/Model.h"
#include"Engine/RootJob.h"
#include"resource.h"
const char* WIN_CLASS_NAME = "SampleGame";
const char* GAME_TITLE = "サンプルゲーム";
const int WINDOW_WIDTH = 800;  //ウィンドウの幅
const int WINDOW_HEIGHT = 600; //ウィンドウの高さ
RootJob *pRootJob = nullptr;

//プロトタイプ宣言
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DialogProc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp);

//エントリーポイント
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow)
{
	/*XMVECTOR beginP = XMVectorSet(3,5,1,0);
	XMVECTOR dirVec = XMVectorSet(0, -1, 0, 0);
	XMVECTOR p1 = XMVectorSet(0, 0, 0, 0), p2 = XMVectorSet(0, 0, 3, 0), p3 = XMVectorSet(3, 0, 0, 0);
	float dist;

	bool result = TriangleTests::Intersects(beginP, dirVec, p1, p2, p3, dist);*/
	//ウィンドウクラス（設計図）を作成
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);             //この構造体のサイズ
	wc.hInstance = hInstance;		               //インスタンスハンドル
	wc.lpszClassName = WIN_CLASS_NAME;            //ウィンドウクラス名
	wc.lpfnWndProc = WndProc;                   //ウィンドウプロシージャ
	wc.style = CS_VREDRAW | CS_HREDRAW;         //スタイル（デフォルト）
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION); //アイコン
	wc.hIconSm = LoadIcon(NULL, IDI_WINLOGO);   //小さいアイコン
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);   //マウスカーソル
	wc.lpszMenuName = NULL;                     //メニュー（なし）
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); //背景（白）
	
	
	RegisterClassEx(&wc);  //クラスを登録

	//ウィンドウサイズの計算
	RECT winRect = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
	AdjustWindowRect(&winRect, WS_OVERLAPPEDWINDOW, FALSE);
	int winW = winRect.right - winRect.left;     //ウィンドウ幅
	int winH = winRect.bottom - winRect.top;     //ウィンドウ高さ


   //ウィンドウを作成

	HWND hWnd = CreateWindow(
		WIN_CLASS_NAME,         //ウィンドウクラス名
		GAME_TITLE,     //タイトルバーに表示する内容
		WS_OVERLAPPEDWINDOW, //スタイル（普通のウィンドウ）
		CW_USEDEFAULT,       //表示位置左（おまかせ）
		CW_USEDEFAULT,       //表示位置上（おまかせ）
		winW,                 //ウィンドウ幅
		winH,                 //ウィンドウ高さ
		NULL,                //親ウインドウ（なし）
		NULL,                //メニュー（なし）
		hInstance,           //インスタンス
		NULL                 //パラメータ（なし）
	);
	

   //ウィンドウを表示
	ShowWindow(hWnd, nCmdShow);

	HRESULT hr;
	hr=Direct3D::Initialize(WINDOW_WIDTH, WINDOW_HEIGHT, hWnd);
	if (FAILED(hr)) 
	{ 
		PostQuitMessage(0); 
		
	}//プログラム終了
	

	Fbx* pFbx = new Fbx;
	pFbx->Load("Assets/BoxDefault.fbx");
	RayCastData dat;
	dat.dir = { 0,-1,0,0 };
	dat.start = { 0,5,0,0 };
	pFbx->RayCast(dat);

	hr = Input::Initialize(hWnd);
	if (FAILED(hr))
	{
		MessageBox(nullptr, "入力の初期化に失敗しました", "エラー", MB_OK);
		PostQuitMessage(0);	
	}
	
	

	Camera::Initialize();

	pRootJob = new RootJob(nullptr);//GameObject木構造の最上位のため、親無し
	pRootJob->Initialize();

	//ダイアログ表示
	HWND hDlg = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, (DLGPROC)DialogProc);

   //メッセージループ（何か起きるのを待つ）

	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	while (msg.message != WM_QUIT)
	{
		//メッセージあり
		if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		//メッセージなし
		else
		{
			timeBeginPeriod(1);
			static DWORD startTime = timeGetTime();
			DWORD nowTime = timeGetTime();
			static DWORD countFps = 0;
			static DWORD lastUpdateTime = nowTime;
			if (nowTime - startTime >= 1000)
			{
				char str[16];
				wsprintf(str, "%u", countFps);

				SetWindowText(hWnd, str);

				countFps = 0;
				startTime = nowTime;
			}
			if ((nowTime - lastUpdateTime)*60 <= 1000.0f )
			{
				continue;
			}
			lastUpdateTime = nowTime;
			countFps++;
			
			timeEndPeriod(1);
	
			//ゲームの処理
			//カメラ更新
			Camera::Update();
			//入力の処理
			Input::Update();
			
			pRootJob->UpdateSub();
			//描画
			Direct3D::BeginDraw();
			
			//ルートジョブからすべての子オブジェクトのdrawを呼ぶ
			pRootJob->DrawSub();

			Direct3D::EndDraw();
		}
	}
	Model::Release();
	pRootJob->ReleaseSub();
	SAFE_DELETE(pRootJob);

	Input::Release();
	Direct3D::Release();
	

	return 0;
}



//ウィンドウプロシージャ（何かあった時によばれる関数）
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);  //プログラム終了
		return 0;
	case WM_MOUSEMOVE:
		Input::SetMousePosition(LOWORD(lParam), HIWORD(lParam));
		return 0;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);//どのケースにも当てはまらない場合、デフォルトの動きをする
	
}
//ダイアログプロシージャ（ダイアログ専用の関数）
BOOL CALLBACK DialogProc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case WM_INITDIALOG:

		return 0;
	case WM_COMMAND:
		switch (LOWORD(wp))
		{
		
		}

	}
	return FALSE;
}