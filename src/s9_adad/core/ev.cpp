#pragma once

#include "../../s9_cflow/cflow.cpp"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <sstream>
#include <variant>

using namespace std;
namespace fs = std::filesystem;

namespace s9_adad::core{

struct RstEr{
	int cd;
	string er;
};

template <typename T>
class Rst{
	public:
		variant<T,RstEr> rv;
		Rst(T v){
			rv = std::move(v);
		}
		Rst(RstEr& er){
			rv = std::move(er);
		}
		bool iss(){
			return holds_alternative<T>(rv);
		}
		static string t_iss(){
			Rst r = Rst<string>("Test");
			return to_string(r.iss());
		}
		static string t_iss_n(){
			RstEr er(1, "error");
			Rst r = Rst<string>(er);
			return to_string(r.iss());
		}
		T v(){
			return std::get<T>(rv);
		}
		static string t_v(){
			Rst r = Rst<string>("Test");
			return r.v();
		}
		RstEr& er(){
			return std::get<RstEr>(rv);
		}
		static string t_er(){
			RstEr er(1, "error");
			Rst r = Rst<string>(er);
			RstEr& rer = r.er();
			return to_string(rer.cd)+"/"+rer.er;
		}
};

class Ev{
	public:
		fs::path d;
		map<string,string> prop;
		string g(string k){
			if (!prop.contains(k)) return "";
			return prop[k];
		}
		static string t_g(){
			Ev ev;
			ev.prop = {{"test", "テスト-1"}};
			return ev.g("test");
		}
		fs::path pth(string k, bool fr=false){
			string v = g(k);
			if (v.empty()) return "";
			if (v[0] == '/') return v;
			fs::path p = d / v;
			if (fr){
				if (!fs::exists(p)) return "";
			}
			return p;
		}
		static string t_pth(){
			Ev ev;
			ev.prop = {{"path", "ev/pth_nofile.txt"}};
			ev.d = fs::path("../test");
			return ev.pth("path");
		}
		static string t_pth_frn(){
			Ev ev;
			ev.prop = {{"path", "ev/pth_nofile.txt"}};
			ev.d = fs::path("../test");
			return ev.pth("path", true);
		}
		static string t_pth_fr(){
			Ev ev;
			ev.prop = {{"path", "ev/pth_fr.txt"}};
			ev.d = fs::path("../test");
			return ev.pth("path", true);
		}
		friend std::ostream& operator<<(std::ostream& os, const Ev& ev) {
			string r = "";
			for(const auto& pair : ev.prop){
				r += pair.first+"="+pair.second+"\n";
			}
			return os << "d=" << ev.d << ";prop=" << r;
		}
};

class EvBU : public s9_cflow::CFU{
	public:
		static map<string,string> lprop(fs::path pth){
			ifstream ifs(pth);
			map<string,string> m;
			if (ifs.is_open()){
				string ln;
				for(;getline(ifs, ln);){
					size_t f = ln.find('=');
					if (f != string::npos){
						string k = ln.substr(0, f);
						string v = ln.substr(f+1);
						m[k] = v;
					}
				}
			}
			return m;
		}
		static string t_lprop(){
			map<string,string> p = lprop("../test/evbu/lprop.txt");
			string r = "";
			for(const auto& pair : p){
				r += pair.first+":"+pair.second+"\n";
			}
			return r;
		}
		
		map<string,any> act() override{
			if (!obs->data.contains("in")){
				obs->data["err"] = "in file error"s;
				return {};
			}
			string inv = any_cast<string&>(obs->data["in"]);
			auto ev = make_shared<Ev>();
			ev->prop = lprop(inv);
			ev->d = fs::path(inv).parent_path();
			obs->data["out"] = ev;
			return {};
		}
		static string t_act(){
			s9_cflow::CFObs obs;
			obs.data["in"] = "../test/evbu/lprop.txt"s;
			EvBU u;
			u.obs = &obs;
			u.act();
			auto ev = any_cast<shared_ptr<Ev>>(obs.data["out"]);
			stringstream ss;
			ss << *ev;
			return ss.str();
		}
		
};

class EvBld : public s9_cflow::CFBld{
	public:
		unique_ptr<s9_cflow::CFU> bld(s9_cflow::CFObs* obs, map<string,any> tp) override{
			return nullptr;
		}
		unique_ptr<s9_cflow::CFU> fs(s9_cflow::CFObs* obs) override{
			auto u = make_unique<EvBU>();
			u->obs = obs;
			return u;
		}
		
};

class EvLdr{
	public:
		Rst<shared_ptr<Ev>> ld(string pth){
			s9_cflow::CFObs obs;
			obs.data["in"] = pth;
			EvBld bld;
			s9_cflow::CFlow f;
			f.f(&obs, &bld);
			if (!obs.data.contains("out")){
				string er = "";
				if (obs.data.contains("err")){
					er = any_cast<string&>(obs.data["err"]);
				}
				RstEr ero(1, er);
				return ero;
			}
			auto ev = any_cast<shared_ptr<Ev>>(obs.data["out"]);
			return ev;
		}
		static string t_ld(){
			EvLdr o = EvLdr();
			Rst<shared_ptr<Ev>> r = o.ld("../test/evldr/ld.txt");
			shared_ptr<Ev> ev = r.v();
			stringstream rs;
			rs << *ev;
			return rs.str();
		}
		static string t_ld_e(){
			EvLdr o = EvLdr();
			Rst<shared_ptr<Ev>> r = o.ld("../test/evldr/ld-nofile.txt");
			RstEr& er = r.er();
			return er.er + "/"+ to_string(er.cd);
		}
};


}