#pragma once

#include <iostream>
#include <sstream>

using namespace std;
namespace fs = std::filesystem;

namespace s9_adad::core{

class TgI{
	public:
		string wk;
		vector<string> cds;
		friend std::ostream& operator<<(std::ostream& os, const TgI& o) {
			os << o.wk;
			for(const string& s : o.cds){
				os << "["<<s<<"]";
			}
			os << endl;
			return os;
		}
};

}