#pragma once

#ifndef __MYRIGIDBODY_H_
#define __MYRIGIDBODY_H_

#include "MyMesh.h"

namespace Simplex
{

	class MyRigidBody
	{
	public:
		MyRigidBody(void);
		~MyRigidBody();
		void Render(MyCamera* a_pCamera);
		Simplex::MyMesh* m_BS = nullptr;

	private:

	};

	MyRigidBody::~MyRigidBody()
	{
	}

}

#endif // __MYRIGIDBODY _H_
