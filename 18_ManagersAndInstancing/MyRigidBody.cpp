#include "MyRigidBody.h"

Simplex::MyRigidBody::MyRigidBody(void)
{
	m_BS = new MyMesh;
	m_BS->GenerateSphere(1.0f, 5.0f, C_BLUE);
}

inline void Simplex::MyRigidBody::Render(MyCamera* a_pCamera)
{
	float* matrix = new float[16];
	memcpy(matrix, glm::value_ptr(IDENTITY_M4), 16 * sizeof(float));
	m_BS->Render(a_pCamera, IDENTITY_M4);
	SafeDelete(matrix);
}
