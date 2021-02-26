#pragma once
#include "CRTPBase.h"

class CRTPSub2 : public CRTPBase<CRTPSub2>
{
public:
	void act1();
	void act2() const;
};