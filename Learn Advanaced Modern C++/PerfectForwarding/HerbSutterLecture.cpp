//Back to Basic lecture CPPcon 2014 by Herb Sutter
//2 problems with universal reference use:
//1. The function must reside in a header. OK I have been using ugly #include "templateFile.cpp" following the advice from Fluentcpp.com, then relying on the template
//user to create a specialization on the type. That way I can keep my header clear of implementation. I just don't know how portable this is.
//2. The function cannot be virtual. Can it call virtual functions?

#include <string>
#include <iostream>

using namespace std;

class Base {
private:
	virtual void grabString(const string& toGrab) = 0;
	virtual void grabString(string&& toGram) noexcept = 0;
	//OK so far intellisense is not complaining.
public:
	template <typename String> void copyString(String&& toGrab) {
		//so hold on wait a minute:
		//1 Compiler does not know what String is. It can be std::string, someone::UCS2String, others::UTF8String, etc. you can't create a template virtual function.
		//So how does this call an impl?
	}
};