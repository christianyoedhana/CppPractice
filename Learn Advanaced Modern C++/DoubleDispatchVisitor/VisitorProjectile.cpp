#include "VisitorTank.h"
#include "VisitorProjectile.h"
#include <typeinfo>
#include <iostream>
using namespace std;

void Projectile::damage(Tank& target) {
	cout << __FUNCSIG__ << endl;
}

void Projectile::damage(Tiger& target){
	cout << __FUNCSIG__ << endl;
}

void Projectile::damage(Sherman& target) {
	cout << __FUNCSIG__ << endl;
}

void Round76MM::damage(Tank& target){
	cout << __FUNCSIG__ << endl;
};

void Round76MM::damage(Tiger& target) {
	cout << __FUNCSIG__ << endl;
}
void Round88MM::damage(Tank& target){
	cout << __FUNCSIG__  << endl;
};

void Round88MM::damage(Sherman& target) {
	cout << __FUNCSIG__ << endl;
}