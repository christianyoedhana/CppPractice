#pragma once
#include "CRTPBase.h"
#include "CRTPSub1.h"

//You think this won't happen to you. Wait until you try a 3-way automerge and the merger picks the wrong line to merge
//class Malformed : public CRTPBase<CRTPSub1>	//This will generate an error similar to : Error	C2280	'Malformed::Malformed(void)': attempting to reference a deleted function

class Malformed : public CRTPBase<Malformed>
{
public:
	void act1();
	void act2() const;
private:
	CRTPSub1 m_sub1;	//This class uses CRTPSub1
};