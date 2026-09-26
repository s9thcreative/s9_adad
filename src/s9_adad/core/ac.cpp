#pragma once

#include "../../s9_cflow/cflow.cpp"
#include "ev.cpp"
#include "tb.cpp"
#include "acd.cpp"
#include "pg.cpp"
#include <iostream>
#include <sstream>
#include <variant>

using namespace std;
namespace fs = std::filesystem;

namespace s9_adad::core{

class AC{
	public:
		inline static AC* _inst = nullptr;
		static AC* inst(){
			if (_inst == nullptr){
				_inst = new AC();
			}
			return _inst;
		}
		inline static string evdef = "setting/core.txt"s;
		static Ev ldev(){
			string pth;
			const char* env = getenv("S9_ADAD_COREFILE");
			if (env != nullptr){
				pth = env;
			}
			else{
				pth = evdef;
			}
			EvLdr ldr = EvLdr();
			Ev ro;
			Rst<shared_ptr<Ev>> r = ldr.ld(pth);
			if (r.iss()){
				ro = *(r.v().get());
			}
			return ro;
		}
		static string t_ldev(){
			evdef = "../test/ac/core.txt"s;
			Ev ev = ldev();
			stringstream ss;
			ss << ev << endl;
			return ss.str();
		}
		static void btg(const vector<TDtI>& ttg, vector<TgI>& ol){
			ol.clear();
			for(TDtI d : ttg){
				TgI tg;
				tg.wk = d.g("wk");
				string cds = d.g("cds");
				if (!cds.empty()){
					size_t pt = 0;
					for(;;){
						size_t npt = cds.find(',', pt);
						if (npt == string::npos){
							npt = cds.size();
						}
						string v = cds.substr(pt, npt-pt);
						tg.cds.push_back(v);
						if (npt == cds.size()){
							break;
						}
						pt = npt+1;
					}
				}
				ol.push_back(tg);
			}
		}
		static string t_btg(){
			vector<TDtI> l;
			TDtI d1;
			d1.s("wk", "code1"s);
			d1.s("cds", ""s);
			l.push_back(d1);
			TDtI d2;
			d2.s("wk", "code2"s);
			d2.s("cds", "code"s);
			l.push_back(d2);
			TDtI d3;
			d3.s("wk", "code3"s);
			d3.s("cds", "code,sonota,hokano,test"s);
			l.push_back(d3);
			vector<TgI> ol;
			btg(l, ol);
			stringstream ss;
			for(TgI i : ol){
				ss << i.wk;
				for(string iv : i.cds){
					ss << "[" << iv << "]";
				}
				ss << endl;
			}
			return ss.str();
		}
		TBAc tdt;
		TBAc ttg;
		vector<TgI> tgl;
		Ev ev;
		AC(){
			ev = ldev();
			tdt.pth = fs::path(ev.pth("path.data"));
			tdt.hdr = {"cd","tg","bg", "img", "cm", "att", "ln"};
			tdt.hdrsz = 1;
			tdt.ik = "cd"s;
			ttg.pth = fs::path(ev.pth("path.target"));
			ttg.hdr = {"wk","cds"};
			ttg.hdrsz = 1;
			ttg.ik = "wk"s;
		}
		void ldd(){
			tdt.ld();
			ttg.ld();
			btg(ttg.dt, tgl);
		}
		static string t_ldd(){
			AC::evdef = "../test/ac/ldd.txt"s;
			AC ac;
			ac.ldd();
			stringstream ss;
			for(TDtI i : ac.tdt.dt){
				ss << i << endl;
			}
			for(TgI i : ac.tgl){
				ss << i.wk;
				for(string iv : i.cds){
					ss << "[" << iv << "]";
				}
				ss << endl;
			}
			return ss.str();
		}
		vector<TDtI>* dtL(){
			return &tdt.dt;
		}
		static string t_dtL(){
			AC ac;
			TDtI dti;
			dti.s("cd", "0001");
			dti.s("tg", "web");
			ac.tdt.dt.push_back(dti);
			dti = TDtI();
			dti.s("cd", "0002");
			dti.s("tg", "app");
			ac.tdt.dt.push_back(dti);
			vector<TDtI>* r = ac.dtL();
			stringstream ss;
			for(TDtI d : *r){
				ss << d << endl;
			}
			return ss.str();
		}
		TDtI* dt(const string& k){
			return tdt.g(k);
		}
		static string t_dt(){
			AC ac;
			TDtI dti;
			dti.s("cd", "0001");
			dti.s("tg", "web");
			ac.tdt.dt.push_back(dti);
			dti = TDtI();
			dti.s("cd", "0002");
			dti.s("tg", "app");
			ac.tdt.dt.push_back(dti);
			ac.tdt.bi();
			TDtI* r = ac.dt("0002"s);
			stringstream ss;
			ss << (*r) << endl;
			return ss.str();
		}
		bool dtIi(const string& k){
			if (tdt.idx != nullopt){
				return tdt.idx->ii(k);
			}
			return false;
		}
		static string t_dtIi(){
			TDx tdx;
			tdx.idx["test"s] = 0;
			AC ac;
			ac.tdt.idx = tdx;
			bool r1 = ac.dtIi("test"s);
			bool r2 = ac.dtIi("aa"s);
			return to_string(r1) + "/"+ to_string(r2);
		}
		
		bool dtAd(TDtI& d){
			bool r = tdt.ad(d);
			if (!r) return false;
			tdt.sv();
			return true;
		}
		static string t_dtAd(){
			string p = "../test/ac/dtad.txt";
			string bp = "../test/ac/dtad-b.txt";
			fs::copy_file(fs::path(bp), fs::path(p), fs::copy_options::overwrite_existing);
			AC ac;
			ac.tdt.pth = p;
			ac.tdt.ld();
			ac.tdt.bi();
			TDtI dti;
			dti.s("cd", "code6");
			dti.s("tg", "app");
			dti.s("bg", "/bg/ad");
			dti.s("img", "/img/ad");
			dti.s("cm", "add comment");
			dti.s("att", "ATT");
			dti.s("ln", "https://xxxxx.xxxxx/xxxxxx/");
			bool r = ac.dtAd(dti);
			return to_string(r);
		}
		static string t_dtAd_n(){
			string p = "../test/ac/dtad_n.txt";
			string bp = "../test/ac/dtad-b.txt";
			fs::copy_file(fs::path(bp), fs::path(p), fs::copy_options::overwrite_existing);
			AC ac;
			ac.tdt.pth = p;
			ac.tdt.ld();
			ac.tdt.bi();
			TDtI dti;
			dti.s("cd", "code5");
			dti.s("tg", "app");
			dti.s("bg", "/bg/ad");
			dti.s("img", "/img/ad");
			dti.s("cm", "add comment");
			dti.s("att", "ATT");
			dti.s("ln", "https://xxxxx.xxxxx/xxxxxx/");
			bool r = ac.dtAd(dti);
			return to_string(r);
		}
		bool dtEd(TDtI& d){
			TDtI* vd = tdt.g(d.g(tdt.ik));
			if (vd == nullptr) return false;
			for(auto p : d.dt){
				vd->s(p.first, p.second);
			}
			tdt.sv();
			return true;
		}
		static string t_dtEd(){
			string p = "../test/ac/dted.txt";
			string bp = "../test/ac/dtad-b.txt";
			fs::copy_file(fs::path(bp), fs::path(p), fs::copy_options::overwrite_existing);
			AC ac;
			ac.tdt.pth = p;
			ac.tdt.ld();
			ac.tdt.bi();
			TDtI dti;
			dti.s("cd", "code2");
			dti.s("tg", "app");
			dti.s("bg", "/bg/ed");
			dti.s("img", "/img/ed");
			dti.s("cm", "edit comment");
			dti.s("att", "ATT-ed");
			dti.s("ln", "https://yyyyy.yyyyy/yyyyy/");
			bool r = ac.dtEd(dti);
			return to_string(r);
		}
		static string t_dtEd_n(){
			string p = "../test/ac/dted_n.txt";
			string bp = "../test/ac/dtad-b.txt";
			fs::copy_file(fs::path(bp), fs::path(p), fs::copy_options::overwrite_existing);
			AC ac;
			ac.tdt.pth = p;
			ac.tdt.ld();
			ac.tdt.bi();
			TDtI dti;
			dti.s("cd", "code6");
			dti.s("tg", "app");
			dti.s("bg", "/bg/ed");
			dti.s("img", "/img/ed");
			dti.s("cm", "edit comment");
			dti.s("att", "ATT-ed");
			dti.s("ln", "https://yyyyy.yyyyy/yyyyy/");
			bool r = ac.dtEd(dti);
			return to_string(r);
		}
		bool dtDe(const string& k){
			bool r = tdt.de(k);
			if (!r) return false;
			tdt.sv();
			return true;
		}
		static string t_dtDe(){
			string p = "../test/ac/dtde.txt";
			string bp = "../test/ac/dtad-b.txt";
			fs::copy_file(fs::path(bp), fs::path(p), fs::copy_options::overwrite_existing);
			AC ac;
			ac.tdt.pth = p;
			ac.tdt.ld();
			ac.tdt.bi();
			bool r = ac.dtDe("code3");
			return to_string(r);
		}
		vector<TgI>* tgL(){
			return &tgl;
		}
		static string t_tgL(){
			AC ac;
			TgI tg;
			tg.wk = "wk001";
			tg.cds.push_back("code111");
			tg.cds.push_back("code112");
			ac.tgl.push_back(tg);
			tg = TgI();
			tg.wk = "wk002";
			tg.cds.push_back("code112");
			tg.cds.push_back("code113");
			tg.cds.push_back("code114");
			ac.tgl.push_back(tg);
			vector<TgI>* r = ac.tgL();
			stringstream ss;
			for(TgI& tgi : *r){
				ss << tgi;
			}
			return ss.str();
		}
		TgI* tg(const string& k){
			uint i = ttg.idx->si(k);
			if (i == -1) return nullptr;
			return &tgl[i];
		}
		static string t_tg(){
			AC ac;
			TgI tg;
			tg.wk = "wk001";
			tg.cds.push_back("code111");
			tg.cds.push_back("code112");
			ac.tgl.push_back(tg);
			tg = TgI();
			tg.wk = "wk002";
			tg.cds.push_back("code112");
			tg.cds.push_back("code113");
			tg.cds.push_back("code114");
			ac.tgl.push_back(tg);
			TDx tdx;
			tdx.idx["wk001"] = 0;
			tdx.idx["wk002"] = 1;
			ac.ttg.idx = tdx;
			TgI* r = ac.tg("wk002");
			stringstream ss;
			ss << *r;
			return ss.str();
		}
		bool tgIi(const string& k){
			if (ttg.idx == nullopt){
				return false;
			}
			return ttg.idx->ii(k);
		}
		static string t_tgIi(){
			AC ac;
			TgI tg;
			tg.wk = "wk001";
			tg.cds.push_back("code111");
			tg.cds.push_back("code112");
			ac.tgl.push_back(tg);
			tg = TgI();
			tg.wk = "wk002";
			tg.cds.push_back("code112");
			tg.cds.push_back("code113");
			tg.cds.push_back("code114");
			ac.tgl.push_back(tg);
			TDx tdx;
			tdx.idx["wk001"] = 0;
			tdx.idx["wk002"] = 1;
			ac.ttg.idx = tdx;
			bool r = ac.tgIi("wk002");
			return to_string(r);
		}
		static string t_tgIi_x(){
			AC ac;
			TgI tg;
			tg.wk = "wk001";
			tg.cds.push_back("code111");
			tg.cds.push_back("code112");
			ac.tgl.push_back(tg);
			tg = TgI();
			tg.wk = "wk002";
			tg.cds.push_back("code112");
			tg.cds.push_back("code113");
			tg.cds.push_back("code114");
			ac.tgl.push_back(tg);
			TDx tdx;
			tdx.idx["wk001"] = 0;
			tdx.idx["wk002"] = 1;
			ac.ttg.idx = tdx;
			bool r = ac.tgIi("wk003");
			return to_string(r);
		}
		bool tgAd(const string& k){
			TDtI dti;
			dti.s("wk", k);
			dti.s("cds", "");
			bool r = ttg.ad(dti);
			if (!r) return false;
			ttg.sv();
			btg(ttg.dt, tgl);
			return true;
		}
		static string t_tgAd(){
			string p = "../test/ac/tgad.txt";
			string bp = "../test/ac/tgad-b.txt";
			fs::copy_file(fs::path(bp), fs::path(p), fs::copy_options::overwrite_existing);
			AC ac;
			ac.ttg.pth = p;
			ac.ttg.ld();
			ac.ttg.bi();
			bool r = ac.tgAd("wk003");
			return to_string(r);
		}
		static string t_tgAd_x(){
			string p = "../test/ac/tgad_x.txt";
			string bp = "../test/ac/tgad-b.txt";
			fs::copy_file(fs::path(bp), fs::path(p), fs::copy_options::overwrite_existing);
			AC ac;
			ac.ttg.pth = p;
			ac.ttg.ld();
			ac.ttg.bi();
			bool r = ac.tgAd("wk002");
			return to_string(r);
		}
		bool tgAss(const string& k, const vector<string>& cds){
			TDtI* d = ttg.g(k);
			if (d == nullptr) return false;
			uint l = 0;
			for(const string& s : cds){
				l += s.size();
			}
			l += cds.size()-1;
			string v;
			v.reserve(l);
			bool fst = true;
			for(const string& s : cds){
				if (fst){
					fst = false;
				}
				else{
					v += ",";
				}
				v += s;
			}
			d->s("cds", v);
			ttg.sv();
			btg(ttg.dt, tgl);
			return true;
		}
		static string t_tgAss(){
			string p = "../test/ac/tgass.txt";
			string bp = "../test/ac/tgad-b.txt";
			fs::copy_file(fs::path(bp), fs::path(p), fs::copy_options::overwrite_existing);
			AC ac;
			ac.ttg.pth = p;
			ac.ttg.ld();
			ac.ttg.bi();
			vector<string> v = {"newcd11", "newcd12", "newcd13"};
			bool r = ac.tgAss("wk002", v);
			return to_string(r);
		}
		static string t_tgAss_x(){
			string p = "../test/ac/tgass_x.txt";
			string bp = "../test/ac/tgad-b.txt";
			fs::copy_file(fs::path(bp), fs::path(p), fs::copy_options::overwrite_existing);
			AC ac;
			ac.ttg.pth = p;
			ac.ttg.ld();
			ac.ttg.bi();
			vector<string> v = {"newcd11", "newcd12", "newcd13"};
			bool r = ac.tgAss("wk003", v);
			return to_string(r);
		}
		bool tgDe(const string& k){
			ttg.de(k);
			ttg.sv();
			btg(ttg.dt, tgl);
			return true;
		}
		static string t_tgDe(){
			string p = "../test/ac/tgde.txt";
			string bp = "../test/ac/tgad-b.txt";
			fs::copy_file(fs::path(bp), fs::path(p), fs::copy_options::overwrite_existing);
			AC ac;
			ac.ttg.pth = p;
			ac.ttg.ld();
			ac.ttg.bi();
			bool r = ac.tgDe("wk001");
			return to_string(r);
		}
		bool pOu(){
			string p = ev.pth("path.out.ad_config");
			s9_cflow::CFObs obs;
			obs.data["pth"] = fs::path(p);
			obs.data["tdt"] = &tdt.dt;
			obs.data["ttg"] = &tgl;
			PBld bld;
			s9_cflow::CFlow f;
			f.f(&obs, &bld);
			if (obs.data.contains("err")){
				string& err = any_cast<string&>(obs.data["err"]);
				if (!err.empty()){
					return false;
				}
			}
			return true;
		}
		static string t_pOu(){
			AC ac;
			Ev ev;
			ev.d = fs::path("../test/ac/");
			ev.prop["path.out.ad_config"] = "ad_config.php";
			ac.ev = ev;
			for(int i = 0; i < 5; i++){
				TDtI dt;
				dt.s("cd", "code"+to_string(i));
				dt.s("tg", "tg"+to_string(i));
				dt.s("bg", "/bg/"+to_string(i));
				dt.s("img", "/img/"+to_string(i));
				dt.s("cm", "comment 日本語"+to_string(i));
				dt.s("att", "ATT"+to_string(i));
				dt.s("ln", "https://ggmoyou.com/"+to_string(i));
				ac.tdt.dt.push_back(dt);
			}
			for(int i = 0; i < 3; i++){
				TgI tg;
				tg.wk = "waku"+to_string(i);
				for(int j = 0; j < i; j++){
					tg.cds.push_back("code"+to_string(j));
				}
				ac.tgl.push_back(tg);
			}
			bool r = ac.pOu();
			return to_string(r);
		}
};


}