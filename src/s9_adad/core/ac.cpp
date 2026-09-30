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

class Ck{
	public:
		static int wd(const string& v){
			for(unsigned char c:v){
				if ((c >= '0')&&(c <= '9')) continue;
				if ((c >= 'a')&&(c <= 'z')) continue;
				if ((c >= 'A')&&(c <= 'Z')) continue;
				if (c == '_') continue;
				return 1;
			}
			return 0;
		}
		static string t_wd(){
			string chk[3] = {"09azAZ_", "test-abc", "abcあああああ"};
			stringstream ss;
			for(string v:chk){
				ss << wd(v);
			}
			return ss.str();
		}
		static int ina(const string& v, vector<string>& a){
			for(string& s: a){
				if (s == v) return 0;
			}
			return 1;
		}
		static string t_ina(){
			string chk[3] = {"test1", "test2", "test3"};
			vector<string> a = {"test1", "test2"};
			stringstream ss;
			for(string v:chk){
				ss << ina(v, a);
			}
			return ss.str();
		}
		static int urla(const string& v){
			size_t p = v.find(':');
			if (p == string::npos) return 1;
			string sc = v.substr(0, p);
			if (sc != "http" && sc != "https" && sc != "itms-apps") return 1;
			if (v.size() < p+2) return 1;
			if ((v[p+1] != '/') || (v[p+2] != '/')) return 1;
			return 0;
		}
		static string t_urla(){
			string chk[5] = {"https://ggmoyou.com/test", "itms-apps://itunes.apple.com/jp/app/id868770763", "http://ggmoyou.com/nosecure", "https:test", "data:test"};
			stringstream ss;
			for(string v:chk){
				ss << urla(v);
			}
			return ss.str();
		}
};

class ACA{
	public:
		virtual ~ACA() = default;
		virtual vector<TDtI>* dtL() = 0;
		virtual TDtI* dt(const string& k) = 0;
		virtual bool dtIi(const string& k) = 0;
		virtual string ckDt(TDtI& d, bool aa) = 0;
		virtual bool dtAd(TDtI& d) = 0;
		virtual bool dtEd(TDtI& d) = 0;
		virtual bool dtDe(const string& k) = 0;
		virtual vector<TgI>* tgL() = 0;
		virtual TgI* tg(const string& k) = 0;
		virtual bool tgIi(const string& k) = 0;
		virtual string ckTg(const string& k) = 0;
		virtual bool tgAd(const string& k) = 0;
		virtual bool tgAss(const string& k, const vector<string>& cds) = 0;
		virtual bool tgDe(const string& k) = 0;
		virtual bool pOu() = 0;
};

class AC : public ACA{
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
		vector<TDtI>* dtL() override{
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
		TDtI* dt(const string& k) override{
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
		bool dtIi(const string& k) override{
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
		string ckDt(TDtI& d, bool aa) override{
			string er = "";
			string v = d.g("cd");
			if (v.empty()){
				er += "cd error\n";
			}
			else if (Ck::wd(v) != 0){
				er += "cd wd error\n";
			}
			else if (v.size() >= 20){
				er += "cd len error\n";
			}
			else{
				bool ii = dtIi(v);
				if (aa){
					if (ii){
						er += "cd already exists";
					}
				}
				else{
					if (!ii){
						er += "cd not found";
					}
				}
			}
			v = d.g("tg");
			static vector<string> tga = {"web", "app"};
			if (v.empty()){
				er += "tg error\n";
			}
			else if (Ck::ina(v, tga) != 0){
				er += "tg ina error\n";
			}
			v = d.g("bg");
			if (v.empty()){
				er += "bg error\n";
			}
			else if (v.size() >= 100){
				er += "bg len error\n";
			}
			v = d.g("img");
			if (v.empty()){
				er += "img error\n";
			}
			else if (v.size() >= 100){
				er += "img len error\n";
			}
			v = d.g("cm");
			if (v.empty()){
				er += "cm error\n";
			}
			else if (v.size() >= 300){
				er += "cm len error\n";
			}
			v = d.g("att");
			if (v.empty()){
				er += "att error\n";
			}
			else if (v.size() >= 60){
				er += "att len error\n";
			}
			v = d.g("ln");
			if (v.empty()){
				er += "ln error\n";
			}
			else if (Ck::urla(v)){
				er += "ln urla error\n";
			}
			else if (v.size() >= 100){
				er += "ln len error\n";
			}
			return er;
		}
		static string t_ckDt(){
			AC ac;
			string bp = "../test/ac/dtad-b.txt";
			ac.tdt.pth = bp;
			ac.tdt.ld();
			ac.tdt.bi();
			TDtI dti;
			dti.s("cd", "code_new");
			dti.s("tg", "app");
			dti.s("bg", "/bg/ad");
			dti.s("img", "/img/ad");
			dti.s("cm", "日本語コメント");
			dti.s("att", "ATT");
			dti.s("ln", "https://ggomoyou.com/test/");
			return ac.ckDt(dti, true);
		}
		static string t_ckDt_d(){
			AC ac;
			string bp = "../test/ac/dtad-b.txt";
			ac.tdt.pth = bp;
			ac.tdt.ld();
			ac.tdt.bi();
			TDtI dti;
			dti.s("cd", "code1");
			dti.s("tg", "app");
			dti.s("bg", "/bg/ad");
			dti.s("img", "/img/ad");
			dti.s("cm", "日本語コメント");
			dti.s("att", "ATT");
			dti.s("ln", "https://ggomoyou.com/test/");
			return ac.ckDt(dti, true);
		}
		static string t_ckDt_e(){
			AC ac;
			string bp = "../test/ac/dtad-b.txt";
			ac.tdt.pth = bp;
			ac.tdt.ld();
			ac.tdt.bi();
			TDtI dti;
			dti.s("cd", "code2");
			dti.s("tg", "app");
			dti.s("bg", "/bg/ad");
			dti.s("img", "/img/ad");
			dti.s("cm", "日本語コメント");
			dti.s("att", "ATT");
			dti.s("ln", "https://ggomoyou.com/test/");
			return ac.ckDt(dti, false);
		}
		static string t_ckDt_x(){
			AC ac;
			string bp = "../test/ac/dtad-b.txt";
			ac.tdt.pth = bp;
			ac.tdt.ld();
			ac.tdt.bi();
			TDtI dti;
			dti.s("cd", "co-de");
			dti.s("tg", "app2");
			string v = "/bg/";
			for(; v.size() < 101; v+="1234567890");
			dti.s("bg", v);
			v = "/img/";
			for(; v.size() < 101; v+="1234567890");
			dti.s("img", v);
			v = "";
			for(; v.size() < 301; v+="日本語コメントあああ");
			dti.s("cm", v);
			v = "";
			for(; v.size() < 61; v+="1234567890");
			dti.s("att", v);
			dti.s("ln", "ggomoyou.com/test/");
			return ac.ckDt(dti, true);
		}

		bool dtAd(TDtI& d) override{
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
		bool dtEd(TDtI& d) override{
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
		bool dtDe(const string& k) override{
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
		vector<TgI>* tgL() override{
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
		TgI* tg(const string& k) override{
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
		bool tgIi(const string& k) override{
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
		string ckTg(const string& k) override{
			if (k.empty()){
				return "wk error";
			}
			else if (Ck::wd(k) != 0){
				return "wk wd error";
			}
			else if (tgIi(k)){
				return "wk already exists";
			}
			return "";
		}
		static string t_ckTg(){
			string bp = "../test/ac/tgad-b.txt";
			AC ac;
			ac.ttg.pth = bp;
			ac.ttg.ld();
			ac.ttg.bi();
			string v = "waku_new";
			return ac.ckTg(v);
		}
		static string t_ckTg_x(){
			string bp = "../test/ac/tgad-b.txt";
			AC ac;
			ac.ttg.pth = bp;
			ac.ttg.ld();
			ac.ttg.bi();
			string v = "waku-new";
			return ac.ckTg(v);
		}
		static string t_ckTg_d(){
			string bp = "../test/ac/tgad-b.txt";
			AC ac;
			ac.ttg.pth = bp;
			ac.ttg.ld();
			ac.ttg.bi();
			string v = "wk001";
			return ac.ckTg(v);
		}
		
		bool tgAd(const string& k) override{
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
		bool tgAss(const string& k, const vector<string>& cds) override{
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
		bool tgDe(const string& k) override{
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
		bool pOu() override{
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