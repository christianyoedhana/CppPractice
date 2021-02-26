#pragma once
#include "CRTPBase.h"

class CRTPSub1 : public CRTPBase<CRTPSub1>{
public:
	void act1() const;
	void act2() const;
};
