#pragma once

#include "view/vc.hpp"
#include <iostream>
#include "view/vlib.hpp"
#include "core/ac.cpp"
#include <vector>
#include <string>
#include "vr.hpp"



using namespace s9_adad::core;
using namespace s9_adad::view;
using namespace std;

namespace s9_adad{

class G{
	public:
		static G* inst(){
			static G* o = new G();
			return o;
		}
		ACA* ac = nullptr;
		VCI* vc = nullptr;
};
class ACDmy : public ACA{
	public:
		vector<TDtI> dtlo;
		vector<TgI> tglo;
		string md;
		ACDmy(string md_=""){
			md = md_;
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
		string ckDt(TDtI& d, bool aa) override{
			std::cout << "ck dt " << d << aa << endl;
			if (md == "er"){
				return "param check error\n";
			}
			return "";
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
		string ckTg(const string& k) override{
			std::cout << "ck tg " << k<< endl;
			if (md == "er"){
				return "param check error\n";
			}
			return "";
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
			if (md == "er"){
				return false;
			}
			return true;
		}
};


class VSpM : public VSp,public DtItf{
	public:
		int pg = Pg_Dt;
		VSpM(){
		}
		~VSpM() = default;
		int cr() override{
			return pg;
		}
		static string t_cr(){
			VSpM sp;
			sp.pg = VSp::Pg_Tg;
			return to_string(sp.cr());
		}
		void i_cr(int pg_) override{
			pg = pg_;
		}
		static string t_i_cr(){
			VSpM sp;
			sp.i_cr(VSp::Pg_Tg);
			return to_string(sp.pg);
		}
		vector<s9_adad::core::TDtI>* dtl() override{
			return G::inst()->ac->dtL();
		}
		static string t_dtl(){
			G::inst()->ac = new ACDmy();
			VSpM sp;
			vector<TDtI>* r = sp.dtl();
			stringstream ss;
			for(TDtI& d:*r){
				ss << d << "\n";
			}
			return ss.str();
		}
		vector<s9_adad::core::TgI>* tgl() override{
			return G::inst()->ac->tgL();
		}
		static string t_tgl(){
			G::inst()->ac = new ACDmy();
			VSpM sp;
			vector<TgI>* r = sp.tgl();
			stringstream ss;
			for(TgI& d:*r){
				ss << d.wk << "\n";
				for(string v:d.cds){
					ss << "[" << v << "]";
				}
				ss << "\n";
			}
			return ss.str();
		}
		s9_adad::core::TDtI* dt_cd(string cd) override{
			return G::inst()->ac->dt(cd);
		}
		static string t_dt_cd(){
			G::inst()->ac = new ACDmy();
			VSpM sp;
			TDtI* r = sp.dt_cd("code1");
			stringstream ss;
			ss << *r << "\n";
			return ss.str();
		}
		DtItf* dtitf() override{
			return this;
		}
		static string t_dtitf(){
			VSpM sp;
			DtItf* r = sp.dtitf();
			return to_string(r == &sp);
		}
};

class VCDmy : public VCI{
	public:
		~VCDmy() = default;
		VSp* o_sp() override{
			static VSpM* sp = new VSpM();
			return sp;
		}
		void dTgSel(s9_adad::core::TgI* tg) override{
			std::cout << "call dTgSel" << std::endl;
		}
		void u() override{
			std::cout << "call u" << std::endl;
		}
		void dEr(string msg) override{
			std::cout << "call dEr:" << msg << std::endl;
		}
		string dITx(string msg) override{
			std::cout << "call dITx:" << msg << std::endl;
			return "dummy";
		}
};


class EvM : public VEv{
	public:
		void doEv(VEvO* ev){
			if (ev->ev == EvTp::DtAdd){
				TDtI* dt = (TDtI*)(ev->o);
				string m = G::inst()->ac->ckDt(*dt, true);
				if (!m.empty()){
					G::inst()->vc->dEr(m);
					return;
				}
				G::inst()->ac->dtAd(*dt);
				G::inst()->vc->u();
			}
			else if (ev->ev == EvTp::DtEdit){
				TDtI* dt = (TDtI*)(ev->o);
				string m = G::inst()->ac->ckDt(*dt, false);
				if (!m.empty()){
					G::inst()->vc->dEr(m);
					return;
				}
				G::inst()->ac->dtEd(*dt);
				G::inst()->vc->u();
			}
			else if (ev->ev == EvTp::DtDel){
				string* cd = (string*)(ev->o);
				bool ii = G::inst()->ac->dtIi(*cd);
				if (!ii){
					G::inst()->vc->dEr("データがみつかりません");
					return;
				}
				G::inst()->ac->dtDe(*cd);
				G::inst()->vc->u();
			}
			else if (ev->ev == EvTp::TgAdd){
				string* wk = (string*)(ev->o);
				string m = G::inst()->ac->ckTg(*wk);
				if (!m.empty()){
					G::inst()->vc->dEr(m);
					return;
				}
				G::inst()->ac->tgAd(*wk);
				G::inst()->vc->u();
			}
			else if (ev->ev == EvTp::TgToEdit){
				TgI* tg = (TgI*)(ev->o);
				G::inst()->vc->dTgSel(tg);
			}
			else if (ev->ev == EvTp::TgEdit){
				TgI* tg = (TgI*)(ev->o);
				bool ii = G::inst()->ac->tgIi(tg->wk);
				if (!ii){
					G::inst()->vc->dEr("データがみつかりません");
					return;
				}
				G::inst()->ac->tgAss(tg->wk, tg->cds);
				G::inst()->vc->u();
			}
			else if (ev->ev == EvTp::TgDel){
				string* wk = (string*)(ev->o);
				bool ii = G::inst()->ac->tgIi(*wk);
				if (!ii){
					G::inst()->vc->dEr("データがみつかりません");
					return;
				}
				G::inst()->ac->tgDe(*wk);
				G::inst()->vc->u();
			}
			else if (ev->ev == EvTp::MnDt){
				int pg = VSp::Pg_Dt;
				if (G::inst()->vc->o_sp()->cr() != pg){
					G::inst()->vc->o_sp()->i_cr(pg);
					G::inst()->vc->u();
				}
			}
			else if (ev->ev == EvTp::MnTg){
				int pg = VSp::Pg_Tg;
				if (G::inst()->vc->o_sp()->cr() != pg){
					G::inst()->vc->o_sp()->i_cr(pg);
					G::inst()->vc->u();
				}
			}
			else if (ev->ev == EvTp::MnTgAdd){
				string wk = G::inst()->vc->dITx("枠コード");
				if (wk.empty()) return;
				string m = G::inst()->ac->ckTg(wk);
				if (!m.empty()){
					G::inst()->vc->dEr(m);
					return;
				}
				bool r = G::inst()->ac->tgAd(wk);
				if (r){
					G::inst()->vc->u();
				}
			}
			else if (ev->ev == EvTp::MnPhp){
				bool r = G::inst()->ac->pOu();
				if (!r){
					G::inst()->vc->dEr("PHP作成時にエラーが発生しました");
				}
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
		static string t_doEv_dta_er(){
			EvM ev;
			G::inst()->ac = new ACDmy("er");
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
		static string t_doEv_dte(){
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
			VEvO evo(EvTp::DtEdit, &dti);
			ev.doEv(&evo);
			return "do";
		}
		static string t_doEv_dte_er(){
			EvM ev;
			G::inst()->ac = new ACDmy("er");
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
			VEvO evo(EvTp::DtEdit, &dti);
			ev.doEv(&evo);
			return "do";
		}
		static string t_doEv_dtd(){
			EvM ev;
			G::inst()->ac = new ACDmy();
			G::inst()->vc = new VCDmy();
			VG::inst()->ev = new VEvDmy();
			string cd = "code2";
			VEvO evo(EvTp::DtDel, &cd);
			ev.doEv(&evo);
			return "do";
		}
		static string t_doEv_dtd_er(){
			EvM ev;
			G::inst()->ac = new ACDmy();
			G::inst()->vc = new VCDmy();
			VG::inst()->ev = new VEvDmy();
			string cd = "code_notfound";
			VEvO evo(EvTp::DtDel, &cd);
			ev.doEv(&evo);
			return "do";
		}
		static string t_doEv_tga(){
			EvM ev;
			G::inst()->ac = new ACDmy();
			G::inst()->vc = new VCDmy();
			VG::inst()->ev = new VEvDmy();
			string wk = "waku_new";
			VEvO evo(EvTp::TgAdd, &wk);
			ev.doEv(&evo);
			return "do";
		}
		static string t_doEv_tga_er(){
			EvM ev;
			G::inst()->ac = new ACDmy("er");
			G::inst()->vc = new VCDmy();
			VG::inst()->ev = new VEvDmy();
			string wk = "waku_new";
			VEvO evo(EvTp::TgAdd, &wk);
			ev.doEv(&evo);
			return "do";
		}
		static string t_doEv_tgte(){
			EvM ev;
			G::inst()->ac = new ACDmy();
			G::inst()->vc = new VCDmy();
			VG::inst()->ev = new VEvDmy();
			TgI tgi;
			tgi.wk = "waku";
			tgi.cds.push_back("code1");
			VEvO evo(EvTp::TgToEdit, &tgi);
			ev.doEv(&evo);
			return "do";
		}
		static string t_doEv_tge(){
			EvM ev;
			G::inst()->ac = new ACDmy();
			G::inst()->vc = new VCDmy();
			VG::inst()->ev = new VEvDmy();
			TgI tgi;
			tgi.wk = "waku1";
			tgi.cds.push_back("code1");
			VEvO evo(EvTp::TgEdit, &tgi);
			ev.doEv(&evo);
			return "do";
		}
		static string t_doEv_tge_er(){
			EvM ev;
			G::inst()->ac = new ACDmy();
			G::inst()->vc = new VCDmy();
			VG::inst()->ev = new VEvDmy();
			TgI tgi;
			tgi.wk = "waku_no";
			tgi.cds.push_back("code1");
			VEvO evo(EvTp::TgEdit, &tgi);
			ev.doEv(&evo);
			return "do";
		}
		static string t_doEv_tgd(){
			EvM ev;
			G::inst()->ac = new ACDmy();
			G::inst()->vc = new VCDmy();
			VG::inst()->ev = new VEvDmy();
			string wk = "waku1";
			VEvO evo(EvTp::TgDel, &wk);
			ev.doEv(&evo);
			return "do";
		}
		static string t_doEv_tgd_er(){
			EvM ev;
			G::inst()->ac = new ACDmy();
			G::inst()->vc = new VCDmy();
			VG::inst()->ev = new VEvDmy();
			string wk = "waku_no";
			VEvO evo(EvTp::TgDel, &wk);
			ev.doEv(&evo);
			return "do";
		}
		static string t_doEv_mdt(){
			EvM ev;
			G::inst()->ac = new ACDmy();
			G::inst()->vc = new VCDmy();
			VG::inst()->ev = new VEvDmy();
			VEvO evo(EvTp::MnDt, nullptr);
			ev.doEv(&evo);
			return "do"+to_string(G::inst()->vc->o_sp()->cr());
		}
		static string t_doEv_mtg(){
			EvM ev;
			G::inst()->ac = new ACDmy();
			G::inst()->vc = new VCDmy();
			VG::inst()->ev = new VEvDmy();
			VEvO evo(EvTp::MnTg, nullptr);
			ev.doEv(&evo);
			return "do"+to_string(G::inst()->vc->o_sp()->cr());
		}
		static string t_doEv_mtga(){
			EvM ev;
			G::inst()->ac = new ACDmy();
			G::inst()->vc = new VCDmy();
			VG::inst()->ev = new VEvDmy();
			VEvO evo(EvTp::MnTgAdd, nullptr);
			ev.doEv(&evo);
			return "do";
		}
		static string t_doEv_mp(){
			EvM ev;
			G::inst()->ac = new ACDmy();
			G::inst()->vc = new VCDmy();
			VG::inst()->ev = new VEvDmy();
			VEvO evo(EvTp::MnPhp, nullptr);
			ev.doEv(&evo);
			return "do";
		}
		static string t_doEv_mp_x(){
			EvM ev;
			G::inst()->ac = new ACDmy("er");
			G::inst()->vc = new VCDmy();
			VG::inst()->ev = new VEvDmy();
			VEvO evo(EvTp::MnPhp, nullptr);
			ev.doEv(&evo);
			return "do";
		}
};

class A{
	public:
		void ex(){
			AC ac;
			G::inst()->ac = &ac;
			VC vc;
			G::inst()->vc = &vc;
			ac.ldd();
			vc.ini(string(Vr::AN) + " " + Vr::V);
			VG::inst()->ev = new EvM();
			vc.sp = new VSpM();
			vc.st();
		}
		static string t_ex(){
			AC::evdef = "../test/a/ex.txt";
			A a;
			a.ex();
			return "exec";
		}
};


}