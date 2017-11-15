#include "MyRigidBody.h"
using namespace Simplex;
//Allocation
void MyRigidBody::Init(void)
{
	m_pMeshMngr = MeshManager::GetInstance();
	m_bVisibleBS = false;
	m_bVisibleOBB = true;
	m_bVisibleARBB = false;

	m_fRadius = 0.0f;

	m_v3ColorColliding = C_RED;
	m_v3ColorNotColliding = C_WHITE;

	m_v3Center = ZERO_V3;
	m_v3MinL = ZERO_V3;
	m_v3MaxL = ZERO_V3;

	m_v3MinG = ZERO_V3;
	m_v3MaxG = ZERO_V3;

	m_v3HalfWidth = ZERO_V3;
	m_v3ARBBSize = ZERO_V3;

	m_m4ToWorld = IDENTITY_M4;

	/*
	a_v3Corner[0] = vector3(m_v3MinG.x, m_v3MinG.y, m_v3MinG.x);
	a_v3Corner[1] = vector3(m_v3MinG.x, m_v3MaxG.y, m_v3MinG.x);
	a_v3Corner[2] = vector3(m_v3MinG.x, m_v3MinG.y, m_v3MaxG.x);
	a_v3Corner[3] = vector3(m_v3MinG.x, m_v3MaxG.y, m_v3MaxG.x);

	a_v3Corner[4] = vector3(m_v3MaxG.x, m_v3MinG.y, m_v3MinG.x);
	a_v3Corner[5] = vector3(m_v3MaxG.x, m_v3MaxG.y, m_v3MinG.x);
	a_v3Corner[6] = vector3(m_v3MaxG.x, m_v3MinG.y, m_v3MaxG.x);
	a_v3Corner[7] = vector3(m_v3MaxG.x, m_v3MaxG.y, m_v3MaxG.x);
	*/
}
void MyRigidBody::Swap(MyRigidBody& a_pOther)
{
	std::swap(m_pMeshMngr, a_pOther.m_pMeshMngr);
	std::swap(m_bVisibleBS, a_pOther.m_bVisibleBS);
	std::swap(m_bVisibleOBB, a_pOther.m_bVisibleOBB);
	std::swap(m_bVisibleARBB, a_pOther.m_bVisibleARBB);

	std::swap(m_fRadius, a_pOther.m_fRadius);

	std::swap(m_v3ColorColliding, a_pOther.m_v3ColorColliding);
	std::swap(m_v3ColorNotColliding, a_pOther.m_v3ColorNotColliding);

	std::swap(m_v3Center, a_pOther.m_v3Center);
	std::swap(m_v3MinL, a_pOther.m_v3MinL);
	std::swap(m_v3MaxL, a_pOther.m_v3MaxL);

	std::swap(m_v3MinG, a_pOther.m_v3MinG);
	std::swap(m_v3MaxG, a_pOther.m_v3MaxG);

	std::swap(m_v3HalfWidth, a_pOther.m_v3HalfWidth);
	std::swap(m_v3ARBBSize, a_pOther.m_v3ARBBSize);

	std::swap(m_m4ToWorld, a_pOther.m_m4ToWorld);

	std::swap(m_CollidingRBSet, a_pOther.m_CollidingRBSet);
}
void MyRigidBody::Release(void)
{
	m_pMeshMngr = nullptr;
	ClearCollidingList();
}
//Accessors
bool MyRigidBody::GetVisibleBS(void) { return m_bVisibleBS; }
void MyRigidBody::SetVisibleBS(bool a_bVisible) { m_bVisibleBS = a_bVisible; }
bool MyRigidBody::GetVisibleOBB(void) { return m_bVisibleOBB; }
void MyRigidBody::SetVisibleOBB(bool a_bVisible) { m_bVisibleOBB = a_bVisible; }
bool MyRigidBody::GetVisibleARBB(void) { return m_bVisibleARBB; }
void MyRigidBody::SetVisibleARBB(bool a_bVisible) { m_bVisibleARBB = a_bVisible; }
float MyRigidBody::GetRadius(void) { return m_fRadius; }
vector3 MyRigidBody::GetColorColliding(void) { return m_v3ColorColliding; }
vector3 MyRigidBody::GetColorNotColliding(void) { return m_v3ColorNotColliding; }
void MyRigidBody::SetColorColliding(vector3 a_v3Color) { m_v3ColorColliding = a_v3Color; }
void MyRigidBody::SetColorNotColliding(vector3 a_v3Color) { m_v3ColorNotColliding = a_v3Color; }
vector3 MyRigidBody::GetCenterLocal(void) { return m_v3Center; }
vector3 MyRigidBody::GetMinLocal(void) { return m_v3MinL; }
vector3 MyRigidBody::GetMaxLocal(void) { return m_v3MaxL; }
vector3 MyRigidBody::GetCenterGlobal(void){	return vector3(m_m4ToWorld * vector4(m_v3Center, 1.0f)); }
vector3 MyRigidBody::GetMinGlobal(void) { return m_v3MinG; }
vector3 MyRigidBody::GetMaxGlobal(void) { return m_v3MaxG; }
vector3 MyRigidBody::GetHalfWidth(void) { return m_v3HalfWidth; }
matrix4 MyRigidBody::GetModelMatrix(void) { return m_m4ToWorld; }
void MyRigidBody::SetModelMatrix(matrix4 a_m4ModelMatrix)
{
	//to save some calculations if the model matrix is the same there is nothing to do here
	if (a_m4ModelMatrix == m_m4ToWorld)
		return;

	//Assign the model matrix
	m_m4ToWorld = a_m4ModelMatrix;

	//Calculate the 8 corners of the cube
	vector3 v3Corner[8];
	//Back square
	v3Corner[0] = m_v3MinL;
	v3Corner[1] = vector3(m_v3MaxL.x, m_v3MinL.y, m_v3MinL.z);
	v3Corner[2] = vector3(m_v3MinL.x, m_v3MaxL.y, m_v3MinL.z);
	v3Corner[3] = vector3(m_v3MaxL.x, m_v3MaxL.y, m_v3MinL.z);

	//Front square
	v3Corner[4] = vector3(m_v3MinL.x, m_v3MinL.y, m_v3MaxL.z);
	v3Corner[5] = vector3(m_v3MaxL.x, m_v3MinL.y, m_v3MaxL.z);
	v3Corner[6] = vector3(m_v3MinL.x, m_v3MaxL.y, m_v3MaxL.z);
	v3Corner[7] = m_v3MaxL;

	//Place them in world space
	for (uint uIndex = 0; uIndex < 8; ++uIndex)
	{
		v3Corner[uIndex] = vector3(m_m4ToWorld * vector4(v3Corner[uIndex], 1.0f));
	}

	//Identify the max and min as the first corner
	m_v3MaxG = m_v3MinG = v3Corner[0];

	//get the new max and min for the global box
	for (uint i = 1; i < 8; ++i)
	{
		if (m_v3MaxG.x < v3Corner[i].x) m_v3MaxG.x = v3Corner[i].x;
		else if (m_v3MinG.x > v3Corner[i].x) m_v3MinG.x = v3Corner[i].x;

		if (m_v3MaxG.y < v3Corner[i].y) m_v3MaxG.y = v3Corner[i].y;
		else if (m_v3MinG.y > v3Corner[i].y) m_v3MinG.y = v3Corner[i].y;

		if (m_v3MaxG.z < v3Corner[i].z) m_v3MaxG.z = v3Corner[i].z;
		else if (m_v3MinG.z > v3Corner[i].z) m_v3MinG.z = v3Corner[i].z;
	}

	//we calculate the distance between min and max vectors
	m_v3ARBBSize = m_v3MaxG - m_v3MinG;
}
//The big 3
MyRigidBody::MyRigidBody(std::vector<vector3> a_pointList)
{
	Init();
	//Count the points of the incoming list
	uint uVertexCount = a_pointList.size();

	//If there are none just return, we have no information to create the BS from
	if (uVertexCount == 0)
		return;

	//Max and min as the first vector of the list
	m_v3MaxL = m_v3MinL = a_pointList[0];

	//Get the max and min out of the list
	for (uint i = 1; i < uVertexCount; ++i)
	{
		if (m_v3MaxL.x < a_pointList[i].x) m_v3MaxL.x = a_pointList[i].x;
		else if (m_v3MinL.x > a_pointList[i].x) m_v3MinL.x = a_pointList[i].x;

		if (m_v3MaxL.y < a_pointList[i].y) m_v3MaxL.y = a_pointList[i].y;
		else if (m_v3MinL.y > a_pointList[i].y) m_v3MinL.y = a_pointList[i].y;

		if (m_v3MaxL.z < a_pointList[i].z) m_v3MaxL.z = a_pointList[i].z;
		else if (m_v3MinL.z > a_pointList[i].z) m_v3MinL.z = a_pointList[i].z;
	}

	//with model matrix being the identity, local and global are the same
	m_v3MinG = m_v3MinL;
	m_v3MaxG = m_v3MaxL;

	//with the max and the min we calculate the center
	m_v3Center = (m_v3MaxL + m_v3MinL) / 2.0f;

	//we calculate the distance between min and max vectors
	m_v3HalfWidth = (m_v3MaxL - m_v3MinL) / 2.0f;

	//Get the distance between the center and either the min or the max
	m_fRadius = glm::distance(m_v3Center, m_v3MinL);
}
MyRigidBody::MyRigidBody(MyRigidBody const& a_pOther)
{
	m_pMeshMngr = a_pOther.m_pMeshMngr;

	m_bVisibleBS = a_pOther.m_bVisibleBS;
	m_bVisibleOBB = a_pOther.m_bVisibleOBB;
	m_bVisibleARBB = a_pOther.m_bVisibleARBB;

	m_fRadius = a_pOther.m_fRadius;

	m_v3ColorColliding = a_pOther.m_v3ColorColliding;
	m_v3ColorNotColliding = a_pOther.m_v3ColorNotColliding;

	m_v3Center = a_pOther.m_v3Center;
	m_v3MinL = a_pOther.m_v3MinL;
	m_v3MaxL = a_pOther.m_v3MaxL;

	m_v3MinG = a_pOther.m_v3MinG;
	m_v3MaxG = a_pOther.m_v3MaxG;

	m_v3HalfWidth = a_pOther.m_v3HalfWidth;
	m_v3ARBBSize = a_pOther.m_v3ARBBSize;

	m_m4ToWorld = a_pOther.m_m4ToWorld;

	m_CollidingRBSet = a_pOther.m_CollidingRBSet;
}
MyRigidBody& MyRigidBody::operator=(MyRigidBody const& a_pOther)
{
	if (this != &a_pOther)
	{
		Release();
		Init();
		MyRigidBody temp(a_pOther);
		Swap(temp);
	}
	return *this;
}
MyRigidBody::~MyRigidBody() { Release(); };
//--- a_pOther Methods
void MyRigidBody::AddCollisionWith(MyRigidBody* a_pOther)
{
	/*
		check if the object is already in the colliding set, if
		the object is already there return with no changes
	*/
	auto element = m_CollidingRBSet.find(a_pOther);
	if (element != m_CollidingRBSet.end())
		return;
	// we couldn't find the object so add it
	m_CollidingRBSet.insert(a_pOther);
}
void MyRigidBody::RemoveCollisionWith(MyRigidBody* a_pOther)
{
	m_CollidingRBSet.erase(a_pOther);
}
void MyRigidBody::ClearCollidingList(void)
{
	m_CollidingRBSet.clear();
}
bool MyRigidBody::IsColliding(MyRigidBody* const a_pOther)
{
	//check if spheres are colliding as pre-test
	bool bColliding = (glm::distance(GetCenterGlobal(), a_pOther->GetCenterGlobal()) < m_fRadius + a_pOther->m_fRadius);
	
	//if they are colliding check the SAT
	if (bColliding)
	{
		if(SAT(a_pOther) != eSATResults::SAT_NONE)
			bColliding = false;// reset to false
	}

	if (bColliding) //they are colliding
	{
		this->AddCollisionWith(a_pOther);
		a_pOther->AddCollisionWith(this);
	}
	else //they are not colliding
	{
		this->RemoveCollisionWith(a_pOther);
		a_pOther->RemoveCollisionWith(this);
	}

	return bColliding;
}
void MyRigidBody::AddToRenderList(void)
{
	if (m_bVisibleBS)
	{
		if (m_CollidingRBSet.size() > 0)
			m_pMeshMngr->AddWireSphereToRenderList(glm::translate(m_m4ToWorld, m_v3Center) * glm::scale(vector3(m_fRadius)), C_BLUE_CORNFLOWER);
		else
			m_pMeshMngr->AddWireSphereToRenderList(glm::translate(m_m4ToWorld, m_v3Center) * glm::scale(vector3(m_fRadius)), C_BLUE_CORNFLOWER);
	}
	if (m_bVisibleOBB)
	{
		if (m_CollidingRBSet.size() > 0)
			m_pMeshMngr->AddWireCubeToRenderList(glm::translate(m_m4ToWorld, m_v3Center) * glm::scale(m_v3HalfWidth * 2.0f), m_v3ColorColliding);
		else
			m_pMeshMngr->AddWireCubeToRenderList(glm::translate(m_m4ToWorld, m_v3Center) * glm::scale(m_v3HalfWidth * 2.0f), m_v3ColorNotColliding);
	}
	if (m_bVisibleARBB)
	{
		if (m_CollidingRBSet.size() > 0)
			m_pMeshMngr->AddWireCubeToRenderList(glm::translate(GetCenterGlobal()) * glm::scale(m_v3ARBBSize), C_YELLOW);
		else
			m_pMeshMngr->AddWireCubeToRenderList(glm::translate(GetCenterGlobal()) * glm::scale(m_v3ARBBSize), C_YELLOW);
	}
}

uint MyRigidBody::SAT(MyRigidBody* const a_pOther)
{
	/*
	Your code goes here instead of this comment;

	For this method, if there is an axis that separates the two objects
	then the return will be different than 0; 1 for any separating axis
	is ok if you are not going for the extra credit, if you could not
	find a separating axis you need to return 0, there is an enum in
	Simplex that might help you [eSATResults] feel free to use it.
	(eSATResults::SAT_NONE has a value of 0)
	*/

	//try a third time, yaaaaaaaay

	u[0] = vector3(m_m4ToWorld * vector4(1, 0, 0, 0));
	u[1] = vector3(m_m4ToWorld * vector4(0, 1, 0, 0));
	u[2] = vector3(m_m4ToWorld * vector4(0, 0, 1, 0));

	a_pOther->u[0] = vector3(a_pOther->m_m4ToWorld * vector4(1, 0, 0, 0));
	a_pOther->u[1] = vector3(a_pOther->m_m4ToWorld * vector4(0, 1, 0, 0));
	a_pOther->u[2] = vector3(a_pOther->m_m4ToWorld * vector4(0, 0, 1, 0));

	float ra;
	float rb;
	glm::mat3x3 R;
	glm::mat3x3 AbsR;

    // Compute rotation matrix expressing b in a's coordinate frame
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			R[i][j] = glm::dot (u[i], a_pOther->u[j]);

    // Compute translation vector t
    vector3 t = a_pOther->GetCenterGlobal() - GetCenterGlobal();
    // Bring translation into a's coordinate frame
    t = vector3(glm::dot(t, u[0]), glm::dot(t, u[1]), glm::dot(t, u[2]));

    // Compute common subexpressions. Add in an epsilon term to
    // counteract arithmetic errors when two edges are parallel and
    // their cross product is (near) null (see text for details)
    for (int i = 0; i < 3; i++)
       for (int j = 0; j < 3; j++)
           AbsR[i][j] = glm::abs(R[i][j]) + FLT_EPSILON;

    // Test axes L = A0, L = A1, L = A2
    for (int i = 0; i < 3; i++) {
        ra = m_v3HalfWidth[i];
        rb = a_pOther->m_v3HalfWidth[0] * AbsR[i][0] + a_pOther->m_v3HalfWidth[1] * AbsR[i][1] + a_pOther->m_v3HalfWidth[2] * AbsR[i][2];
        if (glm::abs(t[i]) > ra + rb)
		{
			//std::cout << "Test axes L = A0, L = A1, L = A2";
			return 1;
		}
    }

    // Test axes L = B0, L = B1, L = B2
    for (int i = 0; i < 3; i++) {
        ra = m_v3HalfWidth[0] * AbsR[0][i] + m_v3HalfWidth[1] * AbsR[1][i] + m_v3HalfWidth[2] * AbsR[2][i];
        rb = a_pOther->m_v3HalfWidth[i];
        if (glm::abs(t[0] * R[0][i] + t[1] * R[1][i] + t[2] * R[2][i]) > ra + rb)
		{
			//std::cout << "Test axes L = B0, L = B1, L = B2";
			return 1;
		}
    }

    // Test axis L = A0 x B0
    ra = m_v3HalfWidth[1] * AbsR[2][0] + m_v3HalfWidth[2] * AbsR[1][0];
    rb = a_pOther->m_v3HalfWidth[1] * AbsR[0][2] + a_pOther->m_v3HalfWidth[2] * AbsR[0][1];
	if (glm::abs(t[2] * R[1][0] - t[1] * R[2][0]) > ra + rb)
	{
		//std::cout << "Test axis L = A0 x B0";
		return 1;
	}

    // Test axis L = A0 x B1
    ra = m_v3HalfWidth[1] * AbsR[2][1] + m_v3HalfWidth[2] * AbsR[1][1];
    rb = a_pOther->m_v3HalfWidth[0] * AbsR[0][2] + a_pOther->m_v3HalfWidth[2] * AbsR[0][0];
    if (glm::abs(t[2] * R[1][1] - t[1] * R[2][1]) > ra + rb)
	{
		//std::cout << "Test axis L = A0 x B1";
		return 1;
	}

    // Test axis L = A0 x B2
    ra = m_v3HalfWidth[1] * AbsR[2][2] + m_v3HalfWidth[2] * AbsR[1][2];
    rb = a_pOther->m_v3HalfWidth[0] * AbsR[0][1] + a_pOther->m_v3HalfWidth[1] * AbsR[0][0];
    if (glm::abs(t[2] * R[1][2] - t[1] * R[2][2]) > ra + rb)
	{
		//std::cout << "Test axis L = A0 x B2";
		return 1;
	}

    // Test axis L = A1 x B0
    ra = m_v3HalfWidth[0] * AbsR[2][0] + m_v3HalfWidth[2] * AbsR[0][0];
    rb = a_pOther->m_v3HalfWidth[1] * AbsR[1][2] + a_pOther->m_v3HalfWidth[2] * AbsR[1][1];
    if (glm::abs(t[0] * R[2][0] - t[2] * R[0][0]) > ra + rb)
	{
		//std::cout << "Test axis L = A1 x B0";
		return 1;
	}

    // Test axis L = A1 x B1
    ra = m_v3HalfWidth[0] * AbsR[2][1] + m_v3HalfWidth[2] * AbsR[0][1];
    rb = a_pOther->m_v3HalfWidth[0] * AbsR[1][2] + a_pOther->m_v3HalfWidth[2] * AbsR[1][0];
    if (glm::abs(t[0] * R[2][1] - t[2] * R[0][1]) > ra + rb)
	{
		//std::cout << "Test axis L = A1 x B1";
		return 1;
	}

    // Test axis L = A1 x B2
    ra = m_v3HalfWidth[0] * AbsR[2][2] + m_v3HalfWidth[2] * AbsR[0][2];
    rb = a_pOther->m_v3HalfWidth[0] * AbsR[1][1] + a_pOther->m_v3HalfWidth[1] * AbsR[1][0];
    if (glm::abs(t[0] * R[2][2] - t[2] * R[0][2]) > ra + rb)
	{
		//std::cout << "Test axis L = A1 x B2";
		return 1;
	}

    // Test axis L = A2 x B0
    ra = m_v3HalfWidth[0] * AbsR[1][0] + m_v3HalfWidth[1] * AbsR[0][0];
    rb = a_pOther->m_v3HalfWidth[1] * AbsR[2][2] + a_pOther->m_v3HalfWidth[2] * AbsR[2][1];
    if (glm::abs(t[1] * R[0][0] - t[0] * R[1][0]) > ra + rb)
	{
		//::cout << "Test axis L = A2 x B0";
		return 1;
	}

    // Test axis L = A2 x B1
    ra = m_v3HalfWidth[0] * AbsR[1][1] + m_v3HalfWidth[1] * AbsR[0][1];
    rb = a_pOther->m_v3HalfWidth[0] * AbsR[2][2] + a_pOther->m_v3HalfWidth[2] * AbsR[2][0];
    if (glm::abs(t[1] * R[0][1] - t[0] * R[1][1]) > ra + rb)
	{
		//std::cout << "Test axis L = A2 x B1";
		return 1;
	}

    // Test axis L = A2 x B2
    ra = m_v3HalfWidth[0] * AbsR[1][2] + m_v3HalfWidth[1] * AbsR[0][2];
    rb = a_pOther->m_v3HalfWidth[0] * AbsR[2][1] + a_pOther->m_v3HalfWidth[1] * AbsR[2][0];
    if (glm::abs(t[1] * R[0][2] - t[0] * R[1][2]) > ra + rb)
	{
		//std::cout << "Test axis L = A2 x B2";
		return 1;
	}

	/* try the first, just commenting it out for now so that I can pull from it later

	//scan creeper x
	//find creeper x axis
	vector3 creeperXAxis = vector3(m_v3MaxG.x, m_v3MinG.y, 0) - vector3(m_v3MinG.x, m_v3MinG.y, 0);
	
	//project creeper onto creeperx
	vector3 creeperXCreeperMax = glm::dot(m_v3MaxG, creeperXAxis) / glm::abs(creeperXAxis);
	vector3 creeperXCreeperMin = glm::dot(m_v3MinG, creeperXAxis) / glm::abs(creeperXAxis);

	//project steve onto creeperx
	vector3 creeperXSteveMax = glm::dot(a_pOther->m_v3MaxG, creeperXAxis) / glm::abs(creeperXAxis);
	vector3 creeperXSteveMin = glm::dot(a_pOther->m_v3MinG, creeperXAxis) / glm::abs(creeperXAxis);

	/*
	//vec forme
	std::cout << "creepmax " << creeperXCreeperMax.x << ', ' + creeperXCreeperMax.y << ', ' << creeperXCreeperMax.z << std::endl;
	std::cout << "creepmin " << creeperXCreeperMin.x << ', ' + creeperXCreeperMin.y << ', ' << creeperXCreeperMin.z << std::endl;
	std::cout << "stevemax " << creeperXSteveMax.x << ', ' + creeperXSteveMax.y << ', ' << creeperXSteveMax.z << std::endl;
	std::cout << "stevemin " << creeperXSteveMin.x << ', ' + creeperXSteveMin.y << ', ' << creeperXSteveMin.z << std::endl;
	std::cout << std::endl << std::endl << std::endl;
	/

	//mag form
	std::cout << "creepmax " << creeperXCreeperMax.length() << std::endl;
	std::cout << "creepmin " << creeperXCreeperMin.length() << std::endl;
	std::cout << "stevemax " << creeperXSteveMax.length() << std::endl;
	std::cout << "stevemin " << creeperXSteveMin.length() << std::endl;
	std::cout << std::endl << std::endl << std::endl;

	//check for overlaps

	//scan creeper y
	//find creeper y axis
	vector3 creeperYAxis = vector3(m_v3MinL.x, m_v3MaxL.y, 0) - vector3(m_v3MinL.x, m_v3MinL.y, 0);

	//scan steve x
	//find steve x axis
	vector3 otherXAxis = vector3(a_pOther->m_v3MaxL.x, a_pOther->m_v3MinL.y, 0) - vector3(a_pOther->m_v3MinL.x, a_pOther->m_v3MinL.y, 0);

	//scan steve y
	//find steve y axis
	vector3 otherYAxis = vector3(a_pOther->m_v3MinL.x, a_pOther->m_v3MaxL.y, 0) - vector3(a_pOther->m_v3MinL.x, a_pOther->m_v3MinL.y, 0);
	*/

	//try the second! Now with books!

	/*
	vector3 axes[6];

	axes[0] = (vector3(m_v3MaxG.x, m_v3MinG.y, m_v3MinG.z) - m_v3MinG);
	axes[1] = (vector3(m_v3MinG.x, m_v3MaxG.y, m_v3MinG.z) - m_v3MinG);
	axes[2] = (vector3(m_v3MinG.x, m_v3MinG.y, m_v3MaxG.z) - m_v3MinG);

	axes[3] = (vector3(a_pOther->m_v3MaxG.x, a_pOther->m_v3MinG.y, a_pOther->m_v3MinG.z) - a_pOther->m_v3MinG);
	axes[4] = (vector3(a_pOther->m_v3MinG.x, a_pOther->m_v3MaxG.y, a_pOther->m_v3MinG.z) - a_pOther->m_v3MinG);
	axes[5] = (vector3(a_pOther->m_v3MinG.x, a_pOther->m_v3MinG.y, a_pOther->m_v3MaxG.z) - a_pOther->m_v3MinG);

	axes[6] = 
	axes[7] = 
	axes[8] = 
	axes[9] = 
	axes[10] = 
	axes[11] = 
	axes[12] = 
	axes[13] = 
	axes[14] = 
	axes[15] = 
	*/

		/*
		//finding a seperating axis between two edges
		vector3 edge1 = m_v3MaxG - m_v3MinG;
		vector3 edge2 = a_pOther->m_v3MaxG - a_pOther->m_v3MinG;

		vector3 sepVec = glm::cross(edge1, edge2);

		if (sepVec != vector3(0, 0, 0))
		{

			//continue, is not parallel
			//std::cout << "Seperation vector: " << sepVec.x << ", " << sepVec.y << ", " << sepVec.z << " :: " << sepVec.length() << std::endl;

			//project onto axes of two characters and axis onto Steve y
			float steveProjmax = (closestPtLineSegment(m_v3MaxG, vector3(m_v3MaxG.x, m_v3MaxG.y, m_v3MaxG.z), vector3(m_v3MaxG.x, m_v3MinG.y, m_v3MaxG.z))).length();
			float steveProjmin = (closestPtLineSegment(m_v3MinG, vector3(m_v3MaxG.x, m_v3MaxG.y, m_v3MaxG.z), vector3(m_v3MaxG.x, m_v3MinG.y, m_v3MaxG.z))).length();
			float creepProjmax = (closestPtLineSegment(a_pOther->m_v3MaxG, vector3(m_v3MaxG.x, m_v3MaxG.y, m_v3MaxG.z), vector3(m_v3MaxG.x, m_v3MinG.y, m_v3MaxG.z))).length();
			float creepProjmin = (closestPtLineSegment(a_pOther->m_v3MinG, vector3(m_v3MaxG.x, m_v3MaxG.y, m_v3MaxG.z), vector3(m_v3MaxG.x, m_v3MinG.y, m_v3MaxG.z))).length();
			float sepProj = (closestPtLineSegment(sepVec, vector3(m_v3MaxG.x, m_v3MaxG.y, m_v3MaxG.z), vector3(m_v3MaxG.x, m_v3MinG.y, m_v3MaxG.z))).length();

			if ((sepProj < steveProjmax && sepProj > steveProjmin) || (sepProj < creepProjmax && sepProj > creepProjmin))
			{
				return sepProj;
				std::cout << "NOT COLLIDING" << std::endl;
			}

			for each (vector3 axis in axes)
			{
				//test the projections onto axis


			}
		}
		else
		{
			//eges are parallel and share a plane
			//try a perpendicualr line in the plane they share
			vector3 altSepVec = glm::cross(edge1, (m_v3MaxG - a_pOther->m_v3MaxG)replace with relative first point on edge 2 and first pooint on edge 1);

			sepVec = glm::cross(edge1, altSepVec);

			if (sepVec != vector3(0, 0, 0))
			{
				//continue, is not parallel NEmore
				//std::cout << "Seperation vector with parallel: " << sepVec.x << ", " << sepVec.y << ", " << sepVec.z << " :: " << sepVec.length() << std::endl;

				for each (vector3 axis in axes)
				{
					//test the projections onto axis


				}
			}

			//these are also parallel, these points are on a line. this won't seperate
		}
		*/
/*
	float fMinCreep;
	float fMinSteve;

	float fMaxCreep;
	float fMaxSteve;

	for (int i = 0; i < 6; i++)
	{
	
		findMinMax(a_v3Corner, axes[i], fMinCreep, fMaxCreep);
		findMinMax(a_pOther->a_v3Corner, axes[i], fMinSteve, fMaxSteve);

		if (fMaxCreep < fMinSteve || fMaxSteve < fMinCreep)
		{
			switch (i)
			{
			case 0:
				return 1;
			case 1:
				return 1;
			case 2:
				return 1;
			case 3:
				return 1;
			case 4:
				return 1;
			case 5:
				return 1;
			default:
				return eSATResults::SAT_NONE;
			}
		}
	}
	*/

	//there is no axis test that separates this two objects
	return eSATResults::SAT_NONE;
}

/*
void Simplex::MyRigidBody::findMinMax(vector3 * a_v3Corners, vector3 a_vAxis, float & a_fMin, float & a_fMax)
{
	a_fMin = std::numeric_limits<float>::max();
	a_fMin = -std::numeric_limits<float>::max();

	for (int i = 0; i < 8; i++)
	{
		float dot = glm::dot(a_vAxis, a_v3Corners[i]);
		a_fMin = glm::min(dot, a_fMin);
		a_fMax = glm::max(dot, a_fMax);
	}
}
*/

/*
vector3 MyRigidBody::closestPtLineSegment(vector3 point, vector3 lineMax, vector3 lineMin)
{
	float safeguard = glm::dot(point - lineMin, lineMax - lineMin) / glm::dot(lineMax - lineMin, lineMax - lineMin);

	if (safeguard < 0.0f)
	{
		safeguard = 0.0f;
	}

	if (safeguard > glm::length(lineMax - lineMin))
	{
		safeguard = glm::length(lineMax - lineMin);
	}

	return lineMin + safeguard * (lineMax - lineMin);
}
*/

