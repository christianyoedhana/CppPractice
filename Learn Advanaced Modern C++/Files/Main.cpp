#include "Files.h"
#include "StreamFunctions.h"
#include "CStringExercise.h"
#include "STLStringExercise.h"
#include "StreamOverloadExercise.h"
#include "IteratorExercise.h"
#include "AlgorithmExercise.h"
#include "NumericExercise.h"

#include <array>
#include <iostream>
#include <fstream>

using std::array;
using std::cout;
using std::endl;
using std::fstream;

int main(int argc, const char* args[])
{
	if (!fileIOExercise(args[1])) return -1;
	streamFunctionsExercise();
	stringStreamExercise();
	udemySStreamExercise();
	udemySStreamExerciseAnswers();
	cStringExercise();
	STLStringExercise();
	cout << "Ostream overload exercise" << endl;
	Car redHondaAccord{ "Honda", "Accord", "Red" };
	cout << redHondaAccord << endl;
	fstream fileToAppend(args[1], std::ios_base::app);
	fileToAppend << redHondaAccord << endl;
	
	iteratorExercise();
	vectorReserveVsResize();
	inlineFriendDemo();
	
	sortingExercise();
	readAlgorithmExercise();

	ostreamIteratorExercise();
	istreamIteratorExercise();

	lambdaExercise();
	minMaxExercise();

	compileTimeFibonacci();
	calculateLogTableUsing_iota();
	innerProductExercise();
	writeOnlyExercise();
	replaceExercise();
	removeExercise();
	copyExercise();
	return 0;
}