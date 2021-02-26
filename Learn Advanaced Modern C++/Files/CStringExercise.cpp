#include <cstring>
#include <string.h>
#include <iostream>

void cStringExercise()
{
	std::cout << "C String Exercise"<<std::endl;
	const char hello[] = "Hello C-String!";
	const size_t helloLength = sizeof(hello) / sizeof(hello[0]);	//this is compile time, including null terminator
	std::cout<<hello<<" length : "<<std::strlen(hello)<<std::endl;
	const char userName[] = "To The User: ";
	const size_t userNameLength = sizeof(userName) / sizeof(userName[0]);	//this is compile-time, strlen is run-time
	char combined[userNameLength + helloLength + 1] = "\0";
	strcat_s(combined, userNameLength, userName);	//dest size > strlen(dest)+strlen(source). 
	strcat_s(combined, sizeof(combined)/sizeof(combined[0]), hello);
	std::cout<<"Combined string "<< combined <<std::endl;
}