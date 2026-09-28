#include "s9_adad/core/ev.cpp"
#include "s9_adad/core/tb.cpp"
#include "s9_adad/core/ac.cpp"
#include "s9_adad/core/pg.cpp"
#include "s9_adad/view/vdt.hpp"
#include "s9_adad/view/vtg.hpp"
#include "s9_adad/view/vlib.hpp"
#include "s9_adad/view/vc.hpp"
#include "s9_adad/a.cpp"
#include "iostream"

string calltest(string cls, string mth);

int main(int ct, char** ag){
	if (ct <= 2) return 1;
	string cls = ag[1];
	string mth = ag[2];
	string r = calltest(cls, mth);
	cout << cls << ".t_" << mth << "=" << r << endl;
	return 0;
}

string calltest(string cls, string mth){
	if (cls == "Ev"){
		if (mth == "g") return s9_adad::core::Ev::t_g();
		if (mth == "pth") return s9_adad::core::Ev::t_pth();
		if (mth == "pth_fr_n") return s9_adad::core::Ev::t_pth_frn();
		if (mth == "pth_fr") return s9_adad::core::Ev::t_pth_fr();
	}
	if (cls == "EvBU"){
		if (mth == "lprop") return s9_adad::core::EvBU::t_lprop();
		if (mth == "act") return s9_adad::core::EvBU::t_act();
	}
	if (cls == "EvLdr"){
		if (mth == "ld") return s9_adad::core::EvLdr::t_ld();
		if (mth == "ld_e") return s9_adad::core::EvLdr::t_ld_e();
	}
	if (cls == "Rst"){
		if (mth == "iss") return s9_adad::core::Rst<string>::t_iss();
		if (mth == "iss_n") return s9_adad::core::Rst<string>::t_iss_n();
		if (mth == "v") return s9_adad::core::Rst<string>::t_v();
		if (mth == "er") return s9_adad::core::Rst<string>::t_er();
	}
	if (cls == "TDx"){
		if (mth == "ii") return s9_adad::core::TDx::t_ii();
		if (mth == "ii_n") return s9_adad::core::TDx::t_ii_n();
		if (mth == "si") return s9_adad::core::TDx::t_si();
		if (mth == "si_n") return s9_adad::core::TDx::t_si_n();
	}
	if (cls == "TDtI"){
		if (mth == "g") return s9_adad::core::TDtI::t_g();
		if (mth == "g_n") return s9_adad::core::TDtI::t_g_n();
		if (mth == "s") return s9_adad::core::TDtI::t_s();
	}
	if (cls == "TBLdU"){
		if (mth == "act") return s9_adad::core::TBLdU::t_act();
	}
	if (cls == "TBSvU"){
		if (mth == "act") return s9_adad::core::TBSvU::t_act();
	}
	if (cls == "TBIdxU"){
		if (mth == "act") return s9_adad::core::TBIdxU::t_act();
	}
	if (cls == "TBAc"){
		if (mth == "ld") return s9_adad::core::TBAc::t_ld();
		if (mth == "sv") return s9_adad::core::TBAc::t_sv();
		if (mth == "sv_e") return s9_adad::core::TBAc::t_sv_e();
		if (mth == "bi") return s9_adad::core::TBAc::t_bi();
		if (mth == "gi") return s9_adad::core::TBAc::t_gi();
		if (mth == "gi_n") return s9_adad::core::TBAc::t_gi_n();
		if (mth == "g") return s9_adad::core::TBAc::t_g();
		if (mth == "g_n") return s9_adad::core::TBAc::t_g_n();
		if (mth == "ad") return s9_adad::core::TBAc::t_ad();
		if (mth == "ad_n") return s9_adad::core::TBAc::t_ad_n();
		if (mth == "dei") return s9_adad::core::TBAc::t_dei();
		if (mth == "de") return s9_adad::core::TBAc::t_de();
	}
	if (cls == "PGnU"){
		if (mth == "es") return s9_adad::core::PGnU::t_es();
		if (mth == "act") return s9_adad::core::PGnU::t_act();
	}
	if (cls == "PSvU"){
		if (mth == "act") return s9_adad::core::PSvU::t_act();
	}
	if (cls == "AC"){
		if (mth == "ldev") return s9_adad::core::AC::t_ldev();
		if (mth == "btg") return s9_adad::core::AC::t_btg();
		if (mth == "ldd") return s9_adad::core::AC::t_ldd();
		if (mth == "dtL") return s9_adad::core::AC::t_dtL();
		if (mth == "dt") return s9_adad::core::AC::t_dt();
		if (mth == "dtIi") return s9_adad::core::AC::t_dtIi();
		if (mth == "dtAd") return s9_adad::core::AC::t_dtAd();
		if (mth == "dtAd_n") return s9_adad::core::AC::t_dtAd_n();
		if (mth == "dtEd") return s9_adad::core::AC::t_dtEd();
		if (mth == "dtEd_n") return s9_adad::core::AC::t_dtEd_n();
		if (mth == "dtDe") return s9_adad::core::AC::t_dtDe();
		if (mth == "tgL") return s9_adad::core::AC::t_tgL();
		if (mth == "tg") return s9_adad::core::AC::t_tg();
		if (mth == "tgIi") return s9_adad::core::AC::t_tgIi();
		if (mth == "tgIi_x") return s9_adad::core::AC::t_tgIi_x();
		if (mth == "tgAd") return s9_adad::core::AC::t_tgAd();
		if (mth == "tgAd_x") return s9_adad::core::AC::t_tgAd_x();
		if (mth == "tgAss") return s9_adad::core::AC::t_tgAss();
		if (mth == "tgAss_x") return s9_adad::core::AC::t_tgAss_x();
		if (mth == "tgDe") return s9_adad::core::AC::t_tgDe();
		if (mth == "pOu") return s9_adad::core::AC::t_pOu();
	}
	if (cls == "VDtLI"){
		if (mth == "v") return s9_adad::view::VDtLI::t_v();
	}
	if (cls == "VDtL"){
		if (mth == "v") return s9_adad::view::VDtL::t_v();
	}
	if (cls == "VDtEd"){
		if (mth == "v") return s9_adad::view::VDtEd::t_v();
		if (mth == "v_n") return s9_adad::view::VDtEd::t_v_n();
	}
	if (cls == "VDt"){
		if (mth == "v") return s9_adad::view::VDt::t_v();
	}
	if (cls == "VTgDt"){
		if (mth == "v") return s9_adad::view::VTgDt::t_v();
	}
	if (cls == "VTgLI"){
		if (mth == "v") return s9_adad::view::VTgLI::t_v();
	}
	if (cls == "VTg"){
		if (mth == "v") return s9_adad::view::VTg::t_v();
	}
	if (cls == "VTgSel"){
		if (mth == "v") return s9_adad::view::VTgSel::t_v();
	}
	if (cls == "VImLd"){
		if (mth == "ld") return s9_adad::view::VImLd::t_ld();
		if (mth == "ld_x") return s9_adad::view::VImLd::t_ld_x();
	}
	if (cls == "VM"){
		if (mth == "m") return s9_adad::view::VM::t_m();
		if (mth == "att") return s9_adad::view::VM::t_att();
		if (mth == "att_tg") return s9_adad::view::VM::t_att_tg();
	}
	if (cls == "VC"){
		if (mth == "v") return s9_adad::view::VC::t_v();
		if (mth == "v_tg") return s9_adad::view::VC::t_v_tg();
		if (mth == "v_tgs") return s9_adad::view::VC::t_v_tgs();
	}
	if (cls == "EvM"){
		if (mth == "doEv_dta") return s9_adad::EvM::t_doEv_dta();
	}
	return "\033[31m[ERROR] not call\033[0m";
}
