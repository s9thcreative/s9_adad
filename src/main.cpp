#include "s9_adad/core/ev.cpp"
#include "iostream"

int main(){
	s9_cflow::CFObs obs;
	obs.data["in"] = "parent/test.txt"s;
	s9_adad::core::EvBld bld;
	s9_cflow::CFlow f;
	f.f(&obs, &bld);
	if (!obs.data.contains("out")){
		string er = "";
		if (!obs.data.contains("err")){
			er = any_cast<string&>(obs.data["err"]);
		}
		cout << er << endl;
	}
	else{
		auto ev = any_cast<shared_ptr<s9_adad::core::Ev>>(obs.data["out"]);
		cout << ev->g("test") << endl;
		cout << ev->pth("path") << endl;
	}
	return 0;
}