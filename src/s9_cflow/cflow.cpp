#pragma once

#include <map>
#include <string>
#include <any>
#include <memory>
#include <iostream>

namespace s9_cflow{

using namespace std;

class CFObs{
	public:
		map<string,any> data;
};

class CFU{
	public:
		CFObs* obs;
		virtual ~CFU() = default;
		virtual map<string,any> act() = 0;
};

class CFBld{
	public:
		virtual ~CFBld() = default;
		virtual unique_ptr<CFU> bld(CFObs* obs, map<string,any> r) = 0;
		virtual unique_ptr<CFU> fs(CFObs* obs) = 0;
};

class CFlow{
	public:
		void f(CFObs* obs, CFBld* bld){
			unique_ptr<CFU> u = bld->fs(obs);
			for(;;){
				map<string,any> r = u->act();
				u = bld->bld(obs, r);
				if (u == nullptr){
					break;
				}
			}
		}
};

};