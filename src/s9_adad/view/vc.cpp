#include "vc.hpp"
#include <QMenu>
#include <QAction>
#include <iostream>
#include <QMessageBox>
#include "vlib.hpp"

using namespace s9_adad::core;

namespace s9_adad::view{

void VM::m(QMenuBar* bar){
	if (ms.size() == 0){
		QMenu* mm = bar->addMenu("操作");
		QString labs[] = {"データ画面", "枠画面", "枠追加", "PHP書き出し"};
		for(int i = 0; i < 4; ++i){
			QAction* ac = mm->addAction(labs[i]);
			QObject::connect(ac, &QAction::triggered, [this,i](){
				this->om(i);
			});
			ms.push_back(ac);
			if (i == 2){
				mm->addSeparator();
			}
		}
	}
}
string VM::t_m(){
	t__V::vtest_p("VM::t_m");
	VM v;
	v.m(t__V::vtest_mw->menuBar());
	t__V::vtest();
	return "view";
}
void VM::om(int i){
	if (i == 0){
		VEvO ev(EvTp::MnDt, nullptr);
		VG::inst()->ev->rqEv(&ev);
	}
	else if (i == 1){
		VEvO ev(EvTp::MnTg, nullptr);
		VG::inst()->ev->rqEv(&ev);
	}
	else if (i == 2){
		VEvO ev(EvTp::MnTgAdd, nullptr);
		VG::inst()->ev->rqEv(&ev);
	}
	else if (i == 3){
		VEvO ev(EvTp::MnPhp, nullptr);
		VG::inst()->ev->rqEv(&ev);
	}
}
void VM::att(Cnd cnd){
	ms[2]->setEnabled(cnd == Cnd::Tg);
}
string VM::t_att(){
	t__V::vtest_p("VM::t_m");
	VM v;
	v.m(t__V::vtest_mw->menuBar());
	v.att(Cnd::Dt);
	t__V::vtest();
	return "view";
}
string VM::t_att_tg(){
	t__V::vtest_p("VM::t_m");
	VM v;
	v.m(t__V::vtest_mw->menuBar());
	v.att(Cnd::Tg);
	t__V::vtest();
	return "view";
}
class VSpDmy:public VSp, public DtItf{
	public:
		int pg = Pg_Dt;
		vector<TDtI> dtlo;
		vector<TgI> tglo;
		VSpDmy(){
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
		int cr() override{
			return pg;
		}
		vector<TDtI>* dtl() override{
			return &dtlo;
		}
		vector<TgI>* tgl() override{
			return &tglo;
		}
		DtItf* dtitf() override{
			return this;
		}
		TDtI* dt_cd(string cd) override{
			for(TDtI& dt : dtlo){
				if (dt.g("cd") == cd) return &dt;
			}
			return nullptr;
		}
};
void VC::ini(string ttl){
	int ac = 0;
	char* av = nullptr;
	QApplication* app = new QApplication(ac, &av);
	w = new QMainWindow();
	w->setWindowTitle(QString::fromStdString(ttl));
	w->resize(900, 800);
	m = new VM();
	m->m(w->menuBar());
	QWidget* wg = new QWidget();
	w->setCentralWidget(wg);
	vdt = new VDt(wg);
	vtg = new VTg(wg);
	VG::inst()->app = app;
	VG::inst()->net = new QNetworkAccessManager(app);
	w->show();
}
void VC::st(){
	u();
	VG::inst()->app->exec();
}
void VC::dTgSel(TgI* tg){
	VTgSel v(w);
	v.i_dtl(sp->dtl());
	v.i_tg(tg);
	v.u();
	QSize sz = v.ly(QSize(650, 400));
	v.resize(sz);
	auto r = v.exec();
	if (r != QDialog::Accepted){
		return;
	}
	TgI tgi;
	tgi.wk = tg->wk;
	tgi.cds = v.chk();
	VEvO ev(EvTp::TgEdit, &tgi);
	VG::inst()->ev->rqEv(&ev);
}
void VC::u(){
	vdt->setVisible(sp->cr() == VSp::Pg_Dt);
	vtg->setVisible(sp->cr() == VSp::Pg_Tg);
	if (sp->cr() == VSp::Pg_Dt){
		QSize wsz = w->centralWidget()->size();
		vdt->i_dtl(sp->dtl());
		vdt->u();
		vdt->ly(wsz);
		vdt->setGeometry(0, 0, wsz.width(), wsz.height());
	}
	else if (sp->cr() == VSp::Pg_Tg){
		QSize wsz = w->centralWidget()->size();
		vtg->dtitf = sp->dtitf();
		vtg->i_tgl(sp->tgl());
		vtg->u();
		vtg->ly(wsz);
		vtg->setGeometry(0, 0, wsz.width(), wsz.height());
		vtg->show();
	}
}
string VC::t_v(){
	VImLd::_cur = new VImLdDmy();
	VG::inst()->ev = new VEvDmy();
	VC vc;
	vc.sp = new VSpDmy();
	vc.ini("VC test");
	vc.st();
	return "view";
}
string VC::t_v_tg(){
	VImLd::_cur = new VImLdDmy();
	VG::inst()->ev = new VEvDmy();
	VC vc;
	vc.sp = new VSpDmy();
	((VSpDmy*)vc.sp)->pg = VSp::Pg_Tg;
	vc.ini("VC test tg");
	vc.st();
	return "view";
}
string VC::t_v_tgs(){
	VImLd::_cur = new VImLdDmy();
	VG::inst()->ev = new VEvDmy();
	VC vc;
	vc.sp = new VSpDmy();
	vc.ini("VC test tgs");
	vector<TgI>* tgl = vc.sp->tgl();
	TgI* tg = &(*tgl)[1];
	VC* vcp = &vc;
	t__V::vtest_wt(2000, [vcp,tg](){
		vcp->dTgSel(tg);
	});
	vc.st();
	return "view";
}
void VC::dEr(string msg){
	QMessageBox::critical(w, "ERROR", QString::fromStdString(msg));
}
string VC::t_v_er(){
	VC vc;
	vc.ini("dEr");
	vc.dEr("エラーメッセージ\nエラーメッセージ\nエラーメッセージ\nエラーメッセージ\nエラーメッセージ\n");
	return "view";
}
}