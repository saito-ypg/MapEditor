#include "Model.h"
#include"global.h"
namespace Model{
	
	std::vector<ModelData*> modelList_;//モデルのポインタを入れるベクタ
}
int Model::Load(std::string fileName)
{
	//ファイル名のモデルを読み込み、同じモデル名があればそのモデル番号を返す
	//同じ名前のモデルがなければ、読み込んでベクタに追加
	/*for (int i = 0; i < modelList_.size(); i++)
	{
		if (modelList_.at(i)->fileName_ == fileName)
			return i;
	}*/

	ModelData* pData=new ModelData;
	pData->fileName_ = fileName;
	pData->pFbx_ = nullptr;

	for (auto &e : modelList_)
	{
		if (e->fileName_ == fileName)
		{
			pData->pFbx_ = e->pFbx_;
			break; 
		}
	}
	if (pData->pFbx_ == nullptr)
	{
		pData->pFbx_ = new Fbx;
		pData->pFbx_->Load(fileName);
	}
	
	modelList_.push_back(pData);
	return modelList_.size() - 1;

}

void Model::SetTransform(int hModel, Transform transform)
{
	modelList_.at(hModel)->transform_ = transform;
}

void Model::Draw(int hModel)
{
	modelList_.at(hModel)->pFbx_->Draw(modelList_.at(hModel)->transform_);
}

void Model::Release(int hModel)
{
	
}

void Model::Release()
{
	bool isReffered = false;
	for (int i = 0; i < modelList_.size(); i++)
	{
		for (int j = i + 1; j < modelList_.size(); j++)
		{
			if (modelList_.at(i)->pFbx_ == modelList_.at(j)->pFbx_)
			{
				isReffered = true;
				break;
			}
		}
		if (!isReffered)
		{
			SAFE_DELETE(modelList_.at(i)->pFbx_);
		}
		SAFE_DELETE(modelList_.at(i));
	}

	modelList_.clear();
}

void Model::RayCast(int hModel, RayCastData& raydata)
{

	//RayCast前にraycastDataの各情報をワールド空間からモデル空間に変換する
	XMMATRIX invWorld=XMMatrixInverse(nullptr,modelList_.at(hModel)->transform_.GetWorldMatrix());
	/*XMVECTOR vPass{ raydata.start.x + raydata.dir.x, raydata.start.y + raydata.dir.y, raydata.start.z + raydata.dir.z , raydata.start.w + raydata.dir.w };
	XMVECTOR vStart = XMVector3TransformCoord(XMLoadFloat4(&raydata.start), invWorld);
	vPass = XMVector3TransformCoord(vPass, invWorld);
	vPass = vPass - vStart;

	XMStoreFloat4(&raydata.dir, vPass);*/
	XMVECTOR vStart = XMVector3TransformCoord(XMLoadFloat4(&raydata.start), -invWorld);
	XMStoreFloat4(&raydata.start, vStart);
//	XMStoreFloat4(&raydata.dir, XMVector4Normalize(XMLoadFloat4(&raydata.dir)));//単位ベクトルにしないとエラーが出る?
	modelList_.at(hModel)->pFbx_->RayCast(raydata);
}
