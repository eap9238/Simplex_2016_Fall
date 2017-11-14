#include "MyEntityManager.h"
using namespace Simplex;
//  MyEntityManager
MyEntityManager* MyEntityManager::m_pInstance = nullptr;
void MyEntityManager::Init(void)
{
	m_uEntityCount = 0;
	m_entityList.clear();
}
void MyEntityManager::Release(void)
{
	for (uint myEntity = 0; myEntity < m_uEntityCount; myEntity++)
	{
		MyEntity* DelEntity = m_entityList[myEntity];
		SafeDelete(DelEntity);
	}

	m_uEntityCount = 0;
	m_entityList.clear();
}
MyEntityManager* MyEntityManager::GetInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new MyEntityManager();
	}
	return m_pInstance;
}
void MyEntityManager::ReleaseInstance()
{
	if (m_pInstance != nullptr)
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}
int Simplex::MyEntityManager::GetEntityIndex(String a_sUniqueID)
{
	for (uint myEntity = 0; myEntity < m_uEntityCount; myEntity++)
	{
		if (m_entityList[myEntity]->GetUniqueID() == a_sUniqueID)
		{
			return myEntity;
		}
	}

	//if not found
	return -1;
}
//Accessors
Model* Simplex::MyEntityManager::GetModel(uint a_uIndex)
{
	if (m_entityList.size() != 0)
	{
		if (a_uIndex >= m_uEntityCount)
		{
			a_uIndex = m_uEntityCount - 1;
		}

		return m_entityList[a_uIndex]->GetModel();
	}
	else
	{
		return nullptr;
	}
}
Model* Simplex::MyEntityManager::GetModel(String a_sUniqueID)
{
	MyEntity* myTemp = MyEntity::GetEntity(a_sUniqueID);

	if (myTemp)
	{
		return myTemp->GetModel();
	}
	else
	{
		return nullptr;
	}
}
RigidBody* Simplex::MyEntityManager::GetRigidBody(uint a_uIndex)
{
	if (m_entityList.size() != 0)
	{
		if (a_uIndex >= m_uEntityCount)
		{
			a_uIndex = m_uEntityCount - 1;
		}

		return m_entityList[a_uIndex]->GetRigidBody();
	}
	else
	{
		return nullptr;
	}
}
RigidBody* Simplex::MyEntityManager::GetRigidBody(String a_sUniqueID)
{
	MyEntity* myTemp = MyEntity::GetEntity(a_sUniqueID);

	if (myTemp)
	{
		return myTemp->GetRigidBody();
	}
	else
	{
		return nullptr;
	}
}
matrix4 Simplex::MyEntityManager::GetModelMatrix(uint a_uIndex)
{
	if (m_entityList.size() != 0)
	{
		if (a_uIndex >= m_uEntityCount)
		{
			a_uIndex = m_uEntityCount - 1;
		}

		return m_entityList[a_uIndex]->GetModelMatrix();
	}
	else
	{
		return IDENTITY_M4;
	}
}
matrix4 Simplex::MyEntityManager::GetModelMatrix(String a_sUniqueID)
{
	MyEntity* myTemp = MyEntity::GetEntity(a_sUniqueID);

	if (myTemp)
	{
		return myTemp->GetModelMatrix();
	}
	else
	{
		return IDENTITY_M4;
	}
}
void Simplex::MyEntityManager::SetModelMatrix(matrix4 a_m4ToWorld, String a_sUniqueID)
{
	MyEntity* myTemp = MyEntity::GetEntity(a_sUniqueID);

	if (myTemp)
	{
		myTemp->SetModelMatrix(a_m4ToWorld);
	}
	else
	{
		return;
	}
}
void Simplex::MyEntityManager::SetModelMatrix(matrix4 a_m4ToWorld, uint a_uIndex)
{
	if (m_entityList.size() != 0)
	{
		if (a_uIndex >= m_uEntityCount)
		{
			a_uIndex = m_uEntityCount - 1;
		}

		m_entityList[a_uIndex]->SetModelMatrix(a_m4ToWorld);
	}
	else
	{
		return;
	}
}
//The big 3
MyEntityManager::MyEntityManager(){Init();}
MyEntityManager::MyEntityManager(MyEntityManager const& other){ }
MyEntityManager& MyEntityManager::operator=(MyEntityManager const& other) { return *this; }
MyEntityManager::~MyEntityManager(){Release();};
// other methods
void Simplex::MyEntityManager::Update(void)
{
	for (uint i = 0; i < m_uEntityCount - 1; i++)
	{
		for (uint j = i + 1; j < m_uEntityCount; j++)
		{
			m_entityList[i]->IsColliding(m_entityList[j]);
		}
	}
}
void Simplex::MyEntityManager::AddEntity(String a_sFileName, String a_sUniqueID)
{
	MyEntity* myTemp = new MyEntity(a_sFileName, a_sUniqueID);

	if (myTemp->IsInitialized())
	{
		m_entityList.push_back(myTemp);
		m_uEntityCount = m_entityList.size();
	}
}
void Simplex::MyEntityManager::RemoveEntity(uint a_uIndex)
{
	if (m_entityList.size() != 0)
	{
		if (a_uIndex >= m_uEntityCount)
		{
			a_uIndex = m_uEntityCount - 1;
		}

		if (a_uIndex != m_uEntityCount - 1)
		{
			std::swap(m_entityList[a_uIndex], m_entityList[m_uEntityCount - 1]);
		}

		MyEntity* myTemp = m_entityList[m_uEntityCount - 1];

		SafeDelete(myTemp);
		m_entityList.pop_back();
		m_uEntityCount--;
		return;
	}
	else
	{
		return;
	}
}
void Simplex::MyEntityManager::RemoveEntity(String a_sUniqueID)
{
	int myIndex = GetEntityIndex(a_sUniqueID);
	RemoveEntity((uint)myIndex);
}
String Simplex::MyEntityManager::GetUniqueID(uint a_uIndex)
{
	if (m_entityList.size() != 0)
	{
		if (a_uIndex >= m_entityList.size())
		{
			a_uIndex = m_entityList.size() - 1;
		}

		return m_entityList[a_uIndex]->GetUniqueID();
	}
	else
	{
		return "";
	}
}
MyEntity* Simplex::MyEntityManager::GetEntity(uint a_uIndex)
{
	if (m_entityList.size() != 0)
	{
		if (a_uIndex >= m_entityList.size())
		{
			a_uIndex = m_entityList.size() - 1;
		}

		return m_entityList[a_uIndex];
	}
	else
	{
		return nullptr;
	}
}
void Simplex::MyEntityManager::AddEntityToRenderList(uint a_uIndex, bool a_bRigidBody)
{
	if (a_uIndex >= m_uEntityCount)
	{
		for (a_uIndex = 0; a_uIndex < m_uEntityCount; a_uIndex++)
		{
			m_entityList[a_uIndex]->AddToRenderList(a_bRigidBody);
		}
	}
	else
	{
		m_entityList[a_uIndex]->AddToRenderList(a_bRigidBody);
	}
}
void Simplex::MyEntityManager::AddEntityToRenderList(String a_sUniqueID, bool a_bRigidBody)
{
	MyEntity* myTemp = MyEntity::GetEntity(a_sUniqueID);

	if (myTemp)
	{
		myTemp->AddToRenderList(a_bRigidBody);
	}
}