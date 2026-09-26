#pragma once

#include "../../s9_cflow/cflow.cpp"
#include "tb.cpp"
#include "acd.cpp"
#include <iostream>
#include <charconv>
#include <fstream>
#include <filesystem>
#include <sstream>
#include <variant>
#include <vector>
#include <optional>

using namespace std;
namespace fs = std::filesystem;

namespace s9_adad::core{


class PGnDK{
	public:
		string vn;
		string k;
};

class PGnU : public s9_cflow::CFU{
	public:
		static string es(const string& tx){
			vector<size_t> pv;
			for(size_t i = 0;;){
				i = tx.find_first_of("'\"\\", i);
				if (i == string::npos) break;
				pv.push_back(i);
				++i;
			}
			string ns;
			ns.reserve(tx.size() + pv.size());
			int pp = 0;
			for(int i = 0; i < pv.size()+1; i++){
				int pn;
				if (i < pv.size()) pn = pv[i];
				else pn = tx.size();
				ns += tx.substr(pp, pn-pp);
				if (pn < tx.size()){
					ns += "\\";
					ns += tx[pn];
					pp = pv[i]+1;
				}
			}
			return ns;
		}
		static string t_es(){
			string r1 = es("test");
			string r2 = es("'te\\st'");
			string r3 = es("\"test\"=>'T\\E\\S\\T'");
			return r1+","+r2+","+r3;
		}
		map<string,any> act() override{
			static vector<PGnDK> dtks = {
				PGnDK("target","tg"),
				PGnDK("bg","bg"),
				PGnDK("titleimg","img"),
				PGnDK("comment","cm"),
				PGnDK("attention","att"),
				PGnDK("link","ln"),
			};
			vector<TDtI>* tdt = nullptr;
 			if (obs->data.contains("tdt")){
				tdt = any_cast<vector<TDtI>*>(obs->data["tdt"]);
			}
			vector<TgI>* ttg = nullptr;
			if (obs->data.contains("ttg")){
				ttg = any_cast<vector<TgI>*>(obs->data["ttg"]);
			}
			stringstream oss;
			oss << "<?php\nConfig::set(\n\t'ad_info', array(\n\t\t'ad_target'=>array(\n";
			if (ttg){
				for(TgI& tg : *ttg){
					oss << "\t\t\t'" << es(tg.wk) << "'=>array(";
					bool nf = false;
					for(string& cd : tg.cds){
						if (!nf){
							nf = true;
						}
						else{
							oss << ", ";
						}
						oss << "'" << es(cd) << "'";
					}
					oss << "),\n";
				}
			}
			oss << "\t\t),\n\t\t'ad_data'=>array(\n";
			if (tdt){
				for(TDtI& dt : *tdt){
					oss << "\t\t\t'" << es(dt.g("cd")) << "'=>array(\n";
					for(PGnDK& dk:dtks){
						oss << "\t\t\t\t'" << dk.vn << "'=>'" << es(dt.g(dk.k)) << "',\n";
					}
					oss << "\t\t\t),\n";
				}
			}
			oss << "\t\t)\n\t)\n);\n";
			obs->data["out"] = oss.str();
			return {{"ctrl","sv"s}};
		}
		static string t_act(){
			PGnU u;
			vector<TDtI> tdt;
			for(int i = 0; i < 5; i++){
				TDtI dt;
				dt.s("cd", "code"+to_string(i));
				dt.s("tg", "tg"+to_string(i));
				dt.s("bg", "bg"+to_string(i));
				dt.s("img", "img"+to_string(i));
				dt.s("cm", "cm\\"+to_string(i));
				dt.s("att", "ATT"+to_string(i));
				dt.s("ln", "https://lnk.com/"+to_string(i));
				tdt.push_back(dt);
			}
			vector<TgI> ttg;
			for(int i = 0; i < 5; i++){
				TgI tg;
				tg.wk = "waku"+to_string(i);
				tg.cds.push_back("code1"),
				tg.cds.push_back("code2"),
				tg.cds.push_back("code3"),
				ttg.push_back(tg);
			}
			s9_cflow::CFObs obs;
			obs.data["tdt"] = &tdt;
			obs.data["ttg"] = &ttg;
			u.obs = &obs;
			map<string,any> rv = u.act();
			string& r = any_cast<string&>(obs.data["out"]);
			string c = "";
			if (rv.contains("ctrl")) c = any_cast<string>(rv["ctrl"]);
			return "[rv]="s+c+"\n"+r;
		}
};


class PSvU : public s9_cflow::CFU{
	public:
		map<string,any> act() override{
			if (!obs->data.contains("pth")){
				obs->data["err"] = "path not set"s;
				return {};
			}
			if (!obs->data.contains("out")){
				obs->data["err"] = "out not set"s;
				return {};
			}
			string& src = any_cast<string&>(obs->data["out"]);
			fs::path pth = any_cast<fs::path>(obs->data["pth"]);
			fs::path tpth = pth;
			tpth += ".temporary";
			ofstream ofs(tpth);
			if (!ofs){
				obs->data["err"] = "file not open"s;
				return {};
			}
			ofs << src;
			ofs.close();
			fs::rename(tpth, pth);
			return {};
		}
		
		static string t_act(){
			PSvU u;
			s9_cflow::CFObs obs;
			obs.data["out"] = "test"s;
			obs.data["pth"] = fs::path("../test/psvu/act.txt");
			u.obs = &obs;
			map<string,any> rv = u.act();
			string err;
			if (obs.data.contains("err")){
				err = any_cast<string>(obs.data["err"]);
			}
			string c;
			if (rv.contains("ctrl")) c = any_cast<string>(rv["ctrl"]);
			return "[rv]="s+c+"\n"+err;
		}
};

class PBld : public s9_cflow::CFBld{
	public:
		unique_ptr<s9_cflow::CFU> bld(s9_cflow::CFObs* obs, map<string,any> tp) override{
			unique_ptr<s9_cflow::CFU> u = nullptr;
			if (tp.empty()){
				return u;
			}
			if (tp.contains("ctrl")){
				string& k = any_cast<string&>(tp["ctrl"]);
				if (k == "sv"){
					u = make_unique<PSvU>();
				}
				if (u != nullptr){
					u->obs = obs;
				}
			}
			return u;
		}
		unique_ptr<s9_cflow::CFU> fs(s9_cflow::CFObs* obs) override{
			unique_ptr<s9_cflow::CFU> u = make_unique<PGnU>();
			u->obs = obs;
			return u;
		}
};

}