#pragma once

#include "../../s9_cflow/cflow.cpp"
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


class TDx{
	public:
		static constexpr uint nf = -1;
		map<string,uint> idx;
		bool ii(const string& k){
			return idx.contains(k);
		}
		static string t_ii(){
			TDx o;
			o.idx["test"] = 1;
			return to_string(o.ii("test"));
		}
		static string t_ii_n(){
			TDx o;
			return to_string(o.ii("test"));
		}
		
		uint si(const string& k){
			if (!ii(k)){
				return nf;
			}
			return idx[k];
		}
		static string t_si(){
			TDx o;
			o.idx["test"] = 1;
			return to_string(o.si("test"));
		}
		static string t_si_n(){
			TDx o;
			return to_string(o.si("test"));
		}

};

class TDtI{
	public:
		map<string,string> dt;
		string g(const string& k){
			if (dt.contains(k)){
				return dt[k];
			}
			return "";
		}
		static string t_g(){
			TDtI o;
			o.dt["test"] = "TEST";
			return o.g("test");
		}
		static string t_g_n(){
			TDtI o;
			return o.g("test");
		}

		void s(const string& k, string v){
			dt[k] = v;
		}
		
		static string t_s(){
			TDtI o;
			o.s("test", "TEST");
			return o.dt["test"];
		}

		friend std::ostream& operator<<(std::ostream& os, const TDtI& dto) {
			string r = "";
			for(const auto& pair : dto.dt){
				r += pair.first+"="+pair.second+"\n";
			}
			return os << r;
		}

};

template<typename T>
class U1Bld : public s9_cflow::CFBld{
	public:
		unique_ptr<s9_cflow::CFU> bld(s9_cflow::CFObs* obs, map<string,any> tp) override{
			return nullptr;
		}
		unique_ptr<s9_cflow::CFU> fs(s9_cflow::CFObs* obs) override{
			unique_ptr<T> u = make_unique<T>();
			u->obs = obs;
			return u;
		}
};

class TBLdU : public s9_cflow::CFU{
	public:
		map<string,any> act() override{
			if (!obs->data.contains("pth")){
				obs->data["err"] = "path not set"s;
				return {};
			}
			vector<string> hdr;
			if (obs->data.contains("hdr")){
				hdr = any_cast<vector<string>>(obs->data["hdr"]);
			}
			int hsz = 0;
			if (obs->data.contains("hdrsz")){
				hsz = any_cast<int>(obs->data["hdrsz"]);
			}
			fs::path pth = any_cast<fs::path>(obs->data["pth"]);
			ifstream ifs(pth);
			vector<TDtI> ol;
			if (ifs.is_open()){
				string ln;
				for(;getline(ifs, ln);){
					if (hsz > 0){
						hsz--;
						continue;
					}
					TDtI dt;
					size_t pt = 0;
					int hidx = 0;
					for(;;){
						size_t npt = ln.find('\t', pt);
						if (npt == string::npos){
							npt = ln.size();
						}
						string k;
						if (hidx < hdr.size()){
							k = hdr[hidx];
						}
						else{
							k = "col_"s + std::to_string(hidx);
						}
						hidx++;
						string v = ln.substr(pt, npt-pt);
						dt.s(k, v);
						if (npt == ln.size()) break;
						pt = npt+1;
					}
					ol.push_back(dt);
				}
			}
			obs->data["lst"] = ol;
			return {};
		}
		
		static string t_act(){
			TBLdU u;
			s9_cflow::CFObs obs;
			obs.data["pth"] = fs::path("../test/tbldu/act.txt");
			vector<string> hdr = {"code", "name", "path", "explanation"};
			obs.data["hdr"] = hdr;
			obs.data["hdrsz"] = 1;
			u.obs = &obs;
			map<string,any> r = u.act();
			stringstream ss;
			vector<TDtI> rl = any_cast<vector<TDtI>>(obs.data["lst"]);
			for(TDtI dt : rl){
				ss << dt << endl;
			}
			return ss.str();
		}

};

class TBSvU : public s9_cflow::CFU{
	public:
		map<string,any> act() override{
			if (!obs->data.contains("pth")){
				obs->data["err"] = "path not set"s;
				return {};
			}
			if (!obs->data.contains("lst")){
				obs->data["err"] = "list not set"s;
				return {};
			}
			vector<TDtI> ol = any_cast<vector<TDtI>>(obs->data["lst"]);
			if (!obs->data.contains("hdr")){
				obs->data["err"] = "header not set"s;
			}
			vector<string> hdr = any_cast<vector<string>>(obs->data["hdr"]);
			int hsz = 0;
			if (obs->data.contains("hdrsz")){
				hsz = any_cast<int>(obs->data["hdrsz"]);
			}
			fs::path pth = any_cast<fs::path>(obs->data["pth"]);
			fs::path tpth = pth;
			tpth += ".temporary";
			ofstream ofs(tpth);
			if (!ofs){
				obs->data["err"] = "file not open"s;
				return {};
			}
			for (;hsz != 0; hsz--){
				int fst = 1;
				for(const string& htx : hdr){
					if (fst){
						fst = 0;
					}
					else{
						ofs << '\t';
					}
					ofs << htx;
				}
				ofs << '\n';
			}
			for(TDtI dt : ol){
				int fst = 1;
				for(const string& htx : hdr){
					if (fst){
						fst = 0;
					}
					else{
						ofs << '\t';
					}
					string otx = dt.g(htx);
					ofs << otx;
				}
				ofs << '\n';
			}
			ofs.close();
			fs::rename(tpth, pth);
			return {};
		}
		
		static string t_act(){
			TBSvU u;
			vector<TDtI> ol;
			for(int i = 0; i < 4; i++){
				TDtI dt;
				dt.s("code", "code-"+to_string(i));
				dt.s("name", "名前"+to_string(i));
				dt.s("path", "/path/path2/path-"+to_string(i));
				dt.s("explanation", "説明文その"+to_string(i));
				ol.push_back(dt);
			}
			s9_cflow::CFObs obs;
			obs.data["pth"] = fs::path("../test/tbsvu/act.txt");
			vector<string> hdr = {"code", "name", "path", "explanation"};
			obs.data["hdr"] = hdr;
			obs.data["hdrsz"] = 1;
			obs.data["lst"] = ol;
			u.obs = &obs;
			map<string,any> r = u.act();
			if (obs.data.contains("err")){
				return any_cast<string>(obs.data["err"]);
			}
			return "success";
		}

};

class TBIdxU : public s9_cflow::CFU{
	public:
		map<string,any> act() override{
			TDx tdx;
			if (obs->data.contains("lst") && obs->data.contains("ik")){
				vector<TDtI>& ol = any_cast<vector<TDtI>&>(obs->data["lst"]);
				string& ik = any_cast<string&>(obs->data["ik"]);
				for(int i = 0; i < ol.size(); i++){
					TDtI& dt = ol[i];
					if (dt.dt.contains(ik) && !tdx.idx.contains(dt.dt[ik])){
						tdx.idx[dt.dt[ik]] = i;
					}
				}
			}
			obs->data["idx"] = std::move(tdx);
			return {};
		}
		
		static string t_act(){
			TBIdxU u;
			vector<TDtI> ol;
			for(int i = 0; i < 4; i++){
				TDtI dt;
				dt.s("code", "code-"+to_string(i));
				dt.s("name", "名前"+to_string(i));
				dt.s("path", "/path/path2/path-"+to_string(i));
				dt.s("explanation", "説明文その"+to_string(i));
				ol.push_back(dt);
			}
			s9_cflow::CFObs obs;
			obs.data["lst"] = ol;
			obs.data["ik"] = "code"s;
			u.obs = &obs;
			map<string,any> r = u.act();
			TDx rx = any_cast<TDx>(obs.data["idx"]);
			stringstream ss;
			for(auto p : rx.idx){
				ss << p.first << ":" << p.second << endl;
			}
			return ss.str();
		}

};

class TBAc{
	public:
		enum class St{
			Ok,
			Err
		};
		fs::path pth;
		vector<string> hdr;
		int hdrsz;
		vector<TDtI> dt;
		string ik = "";
		optional<TDx> idx = nullopt;
		St ld(){
			U1Bld<TBLdU> bld;
			s9_cflow::CFObs obs;
			obs.data["pth"] = pth;
			obs.data["hdr"] = hdr;
			obs.data["hdrsz"] = hdrsz;
			s9_cflow::CFlow f;
			f.f(&obs, &bld);
			if (obs.data.contains("err") || !obs.data.contains("lst")){
				return St::Err;
			}
			dt = any_cast<vector<TDtI>>(obs.data["lst"]);
			bi();
			return St::Ok;
		}
		static string t_ld(){
			TBAc o;
			o.pth = "../test/tbac/ld.txt";
			o.hdr = {"code","name","path","explanation"};
			o.hdrsz = 1;
			St r = o.ld();
			stringstream ss;
			ss << static_cast<int>(r) << endl;
			for(TDtI d : o.dt){
				ss << d << endl;
			}
			return ss.str();
		}
		
		St sv(){
			U1Bld<TBSvU> bld;
			s9_cflow::CFObs obs;
			obs.data["pth"] = pth;
			obs.data["hdr"] = hdr;
			obs.data["hdrsz"] = hdrsz;
			obs.data["lst"] = dt;
			s9_cflow::CFlow f;
			f.f(&obs, &bld);
			if (obs.data.contains("err")){
				return St::Err;
			}
			return St::Ok;
		}
		static string t_sv(){
			TBAc o;
			o.pth = "../test/tbac/sv.txt";
			o.hdr = {"code","name","path","explanation"};
			o.hdrsz = 1;
			vector<TDtI> ol;
			for(int i = 0; i < 4; i++){
				TDtI dt;
				dt.s("code", "code-"+to_string(i));
				dt.s("name", "名前"+to_string(i));
				dt.s("path", "/path/path2/path-"+to_string(i));
				dt.s("explanation", "説明文その"+to_string(i));
				ol.push_back(dt);
			}
			o.dt = ol;
			St r = o.sv();
			return to_string(static_cast<int>(r));
		}
		static string t_sv_e(){
			TBAc o;
			o.pth = "../test/tbac-nodir/sv.txt";
			o.hdr = {"code","name","path","explanation"};
			o.hdrsz = 1;
			vector<TDtI> ol;
			for(int i = 0; i < 4; i++){
				TDtI dt;
				dt.s("code", "code-"+to_string(i));
				dt.s("name", "名前"+to_string(i));
				dt.s("path", "/path/path2/path-"+to_string(i));
				dt.s("explanation", "説明文その"+to_string(i));
				ol.push_back(dt);
			}
			o.dt = ol;
			St r = o.sv();
			return to_string(static_cast<int>(r));
		}

		void bi(){
			if (ik == "") return;
			U1Bld<TBIdxU> bld;
			s9_cflow::CFObs obs;
			obs.data["lst"] = dt;
			obs.data["ik"] = ik;
			s9_cflow::CFlow f;
			f.f(&obs, &bld);
			idx = any_cast<TDx>(obs.data["idx"]);
		}
		static string t_bi(){
			TBAc o;
			vector<TDtI> ol;
			for(int i = 0; i < 4; i++){
				TDtI dt;
				dt.s("code", "code-"+to_string(i));
				dt.s("name", "名前"+to_string(i));
				dt.s("path", "/path/path2/path-"+to_string(i));
				dt.s("explanation", "説明文その"+to_string(i));
				ol.push_back(dt);
			}
			o.dt = ol;
			o.ik = "code";
			o.bi();
			stringstream ss;
			TDx tdx = o.idx.value();
			for(auto p : tdx.idx){
				ss << p.first << ":" << p.second << endl;
			}
			return ss.str();
		}
		
		TDtI* gi(uint idx){
			if (idx >= dt.size()) return nullptr;
			return &dt[idx];
		}
		static string t_gi(){
			TBAc o;
			vector<TDtI> ol;
			for(int i = 0; i < 4; i++){
				TDtI dt;
				dt.s("code", "code-"+to_string(i));
				dt.s("name", "名前"+to_string(i));
				dt.s("path", "/path/path2/path-"+to_string(i));
				dt.s("explanation", "説明文その"+to_string(i));
				ol.push_back(dt);
			}
			o.dt = ol;
			TDtI* r = o.gi(2);
			stringstream ss;
			if (r == nullptr){
				ss << "null" << endl;
			}
			else{
				ss << *r << endl;
			}
			return ss.str();
		}
		static string t_gi_n(){
			TBAc o;
			vector<TDtI> ol;
			for(int i = 0; i < 4; i++){
				TDtI dt;
				dt.s("code", "code-"+to_string(i));
				dt.s("name", "名前"+to_string(i));
				dt.s("path", "/path/path2/path-"+to_string(i));
				dt.s("explanation", "説明文その"+to_string(i));
				ol.push_back(dt);
			}
			o.dt = ol;
			TDtI* r = o.gi(8);
			stringstream ss;
			if (r == nullptr){
				ss << "null" << endl;
			}
			else{
				ss << *r << endl;
			}
			return ss.str();
		}

		TDtI* g(string k){
			if (idx == nullopt){
				return nullptr;
			}
			uint i = idx.value().si(k);
			if (i == -1) return nullptr;
			return gi(i);
		}
		static string t_g(){
			TBAc o;
			vector<TDtI> ol;
			TDx idx;
			for(int i = 0; i < 4; i++){
				TDtI dt;
				dt.s("code", "code-"+to_string(i));
				dt.s("name", "名前"+to_string(i));
				dt.s("path", "/path/path2/path-"+to_string(i));
				dt.s("explanation", "説明文その"+to_string(i));
				ol.push_back(dt);
				idx.idx["code-"+to_string(i)] = i;
			}
			o.dt = ol;
			o.idx = idx;
			TDtI* r = o.g("code-2");
			stringstream ss;
			if (r == nullptr){
				ss << "null" << endl;
			}
			else{
				ss << *r << endl;
			}
			return ss.str();
		}

		static string t_g_n(){
			TBAc o;
			vector<TDtI> ol;
			TDx idx;
			for(int i = 0; i < 4; i++){
				TDtI dt;
				dt.s("code", "code-"+to_string(i));
				dt.s("name", "名前"+to_string(i));
				dt.s("path", "/path/path2/path-"+to_string(i));
				dt.s("explanation", "説明文その"+to_string(i));
				ol.push_back(dt);
				idx.idx["code-"+to_string(i)] = i;
			}
			o.dt = ol;
			o.idx = idx;
			TDtI* r = o.g("code-6");
			stringstream ss;
			if (r == nullptr){
				ss << "null" << endl;
			}
			else{
				ss << *r << endl;
			}
			return ss.str();
		}

		bool ad(TDtI& ndt){
			if (idx != nullopt){
				if (idx->ii(ndt.g(ik))){
					return false;
				}
			}
			dt.push_back(ndt);
			bi();
			return true;
		}

		static string t_ad(){
			TBAc o;
			vector<TDtI> ol;
			TDx tdx;
			o.idx = tdx;
			for(int i = 0; i < 2; i++){
				TDtI dt;
				dt.s("code", "code-"+to_string(i));
				dt.s("name", "名前"+to_string(i));
				dt.s("path", "/path/path2/path-"+to_string(i));
				dt.s("explanation", "説明文その"+to_string(i));
				ol.push_back(dt);
				o.idx->idx["code-"+to_string(i)];
			}
			o.dt = ol;
			o.ik = "code"s;
			TDtI ndt;
			ndt.s("code", "code-999");
			ndt.s("name", "名前新しい");
			ndt.s("path", "/path/path2/path-new");
			ndt.s("explanation", "新説明文");

			bool r = o.ad(ndt);
			stringstream ss;
			ss << r << endl;
			for(TDtI d : o.dt){
				ss << d << endl;
			}
			for(auto p : o.idx.value().idx){
				ss << p.first << ":" << p.second << endl;
			}
			return ss.str();
		}

		static string t_ad_n(){
			TBAc o;
			vector<TDtI> ol;
			TDx tdx;
			o.idx = tdx;
			for(int i = 0; i < 2; i++){
				TDtI dt;
				dt.s("code", "code-"+to_string(i));
				dt.s("name", "名前"+to_string(i));
				dt.s("path", "/path/path2/path-"+to_string(i));
				dt.s("explanation", "説明文その"+to_string(i));
				ol.push_back(dt);
				o.idx->idx["code-"+to_string(i)];
			}
			o.dt = ol;
			o.ik = "code"s;
			TDtI ndt;
			ndt.s("code", "code-1");
			ndt.s("name", "名前新しい");
			ndt.s("path", "/path/path2/path-new");
			ndt.s("explanation", "新説明文");

			bool r = o.ad(ndt);
			stringstream ss;
			ss << r << endl;
			for(TDtI d : o.dt){
				ss << d << endl;
			}
			for(auto p : o.idx.value().idx){
				ss << p.first << ":" << p.second << endl;
			}
			return ss.str();
		}

		void dei(uint i){
			dt.erase(dt.begin() + i);
			bi();
		}

		static string t_dei(){
			TBAc o;
			vector<TDtI> ol;
			for(int i = 0; i < 4; i++){
				TDtI dt;
				dt.s("code", "code-"+to_string(i));
				dt.s("name", "名前"+to_string(i));
				dt.s("path", "/path/path2/path-"+to_string(i));
				dt.s("explanation", "説明文その"+to_string(i));
				ol.push_back(dt);
			}
			o.dt = ol;

			o.ik = "code"s;
			o.dei(2);
			stringstream ss;
			for(TDtI d : o.dt){
				ss << d << endl;
			}
			for(auto p : o.idx.value().idx){
				ss << p.first << ":" << p.second << endl;
			}
			return ss.str();
		}


		bool de(string k){
			if (idx == nullopt) return false;
			uint i = idx.value().si(k);
			if (i == -1) return false;
			dei(i);
			return true;
		}

		static string t_de(){
			TBAc o;
			vector<TDtI> ol;
			TDx idx;
			for(int i = 0; i < 4; i++){
				TDtI dt;
				dt.s("code", "code-"+to_string(i));
				dt.s("name", "名前"+to_string(i));
				dt.s("path", "/path/path2/path-"+to_string(i));
				dt.s("explanation", "説明文その"+to_string(i));
				ol.push_back(dt);
				idx.idx["code-"+to_string(i)] = i;
			}
			o.dt = ol;
			o.idx = idx;
			o.ik = "code"s;

			bool r = o.de("code-1");
			stringstream ss;
			ss << r << endl;
			for(TDtI d : o.dt){
				ss << d << endl;
			}
			for(auto p : o.idx.value().idx){
				ss << p.first << ":" << p.second << endl;
			}
			return ss.str();
		}


};


}