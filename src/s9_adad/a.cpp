#include "view/vc.hpp"
#include <iostream>
#include "view/vlib.hpp"
#include "core/ac.cpp"
#include <vector>
#include <string>




using namespace s9_adad::core;
using namespace s9_adad::view;
using namespace std;

namespace s9_adad{

class VCDmy : public VCI{
	public:
		~VCDmy() = default;
		void dTgSel(s9_adad::core::TgI* tg) override{
			std::cout << "call dTgSel" << std::endl;
		}
		void u() override{
			std::cout << "call u" << std::endl;
		}
};

class ACDmy : public ACA{
	public:
		vector<TDtI> dtlo;
		vector<TgI> tglo;
		ACDmy(){
			for(int j = 0; j < 4; ++j){
				TgI tg;
				tg.wk = "waku"+to_string(j);
				for(int i = 0; i < 5; ++i){
					if (i % 2 == j % 2) continue;
					tg.cds.push_back("code"+to_string(i));
				}
				tglo.push_back(tg);
			}
			for(int i = 0; i < 5; ++i){
				TDtI dt;
				dt.s("cd", "code"+to_string(i));
				dt.s("tg", "tg"+to_string(i));
				dt.s("bg", "/bg/"+to_string(i));
				dt.s("img", "/img/"+to_string(i));
				dt.s("cm", "comment"+to_string(i));
				dt.s("att", "att"+to_string(i));
				dt.s("ln", "https://ggmoyou.com/"+to_string(i));
				dtlo.push_back(dt);
			}
		}
		~ACDmy() = default;
		vector<TDtI>* dtL() override{
			return &dtlo;
		}
		TDtI* dt(const string& k) override{
			for(TDtI& dt : dtlo){
				if (dt.g("cd") == k) return &dt;
			}
			return nullptr;
		}
		bool dtIi(const string& k) override{
			for(TDtI& dt : dtlo){
				if (dt.g("cd") == k) return true;
			}
			return false;
		}
		bool dtAd(TDtI& d) override{
			std::cout << "dt add do" << d<< endl;
			return true;
		}
		bool dtEd(TDtI& d) override{
			std::cout << "dt edit do" << d<< endl;
			return true;
		}
		bool dtDe(const string& k) override{
			std::cout << "dt del do" << k<< endl;
			return true;
		}
		vector<TgI>* tgL() override{
			return &tglo;
		}
		TgI* tg(const string& k) override{
			for(TgI& tg : tglo){
				if (tg.wk == k) return &tg;
			}
			return nullptr;
		}
		bool tgIi(const string& k) override{
			for(TgI& tg : tglo){
				if (tg.wk == k) return true;
			}
			return false;
		}
		bool tgAd(const string& k) override{
			std::cout << "tg add do" << k<< endl;
			return true;
		}
		bool tgAss(const string& k, const vector<string>& cds) override{
			std::cout << "tg asset do" << k<< endl;
			for(string s: cds){
				std::cout << s << endl;
			}
			return true;
		}
		bool tgDe(const string& k) override{
			std::cout << "tg del do" << k<< endl;
			return true;
		}
		bool pOu() override{
			std::cout << "php out" << endl;
			return true;
		}
};

class G{
	public:
		static G* inst(){
			static G* o = new G();
			return o;
		}
		ACA* ac = nullptr;
		VCI* vc = nullptr;
};

class EvM : public VEv{
	public:
		void doEv(VEvO* ev){
			if (ev->ev == EvTp::DtAdd){
				TDtI* dt = (TDtI*)(ev->o);
				G::inst()->ac->dtAd(*dt);
				G::inst()->vc->u();
			}
		}
		static string t_doEv_dta(){
			EvM ev;
			G::inst()->ac = new ACDmy();
			G::inst()->vc = new VCDmy();
			VG::inst()->ev = new VEvDmy();
			TDtI dti;
			dti.s("cd", "code1");
			dti.s("tg", "tg");
			dti.s("bg", "/bg/test.png");
			dti.s("img", "/img/test.png");
			dti.s("cm", "cm1");
			dti.s("att", "att1");
			dti.s("ln", "https://ggmoyou.com/ln1");
			VEvO evo(EvTp::DtAdd, &dti);
			ev.doEv(&evo);
			return "do";
		}
};

}