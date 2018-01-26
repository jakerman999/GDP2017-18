#include "cAssimpBasic.h"
#include "cAssimpBasic_Imp.h"

cAssimpBasic::cAssimpBasic()
{
	this->m_pImp = new cAssimpBasic_Imp();


	return;
}

cAssimpBasic::~cAssimpBasic()
{
	delete this->m_pImp;

	return;
}
