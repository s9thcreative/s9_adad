#include "vtg.hpp"
#include <QApplication>
#include <QMainWindow>
#include <QSize>
#include <QFont>
#include <QObject>
#include <Qt>
#include <QMessageBox>
#include "vlib.hpp"
#include "vc.hpp"

using namespace s9_adad::core;

namespace s9_adad::view{

VTgDt::VTgDt(QWidget* p) : QWidget(p){
	vcd = new QLabel(this);
	QFont f = vcd->font();
	f.setPointSize(20);
	f.setBold(true);
	vcd->setFont(f);
	vbn = new QLabel(this);
	vbn->setScaledContents(true);
}
void VTgDt::i_dt(TDtI* dt_){
	dt = dt_;
}
QSize VTgDt::ly(QSize sz){
	int y = 0;
	int w = sz.width();
	vcd->setGeometry(0, y, w, 20);
	y += 20;
	int imh = w*100/640;
	vbn->setGeometry(0, y, w, imh);
	y += imh;
	return QSize(w, y);
}
void VTgDt::u(){
	vcd->setText(QString::fromStdString(dt->g("cd")));
	string imu = dt->g("img");
	if (cu != imu){
		vbn->clear();
		delete(ci);
		cu = imu;
		VImLd::cur()->ld(imu, [this](QPixmap* r){
			if (r != nullptr){
				this->vbn->setPixmap(*r);
				this->ci = r;
			}
		});
	}
}
string VTgDt::t_v(){
	QWidget* p = t__V::vtest_p("VTgDt");
	VTgDt v(p);
	TDtI dt;
	dt.s("cd", "code1");
	dt.s("img", "https://yupj.jp/game/s9ad/1.0/img/hanabira/title.png");
	v.i_dt(&dt);
	v.u();
	QSize sz = v.ly(QSize(600, 0xffff));
	v.setGeometry(0, 0, sz.width(), sz.height());
	t__V::vtest();
	return "view";
}

class DtItfDmy : public DtItf{
	public:
		vector<TDtI> dmyl;
		TDtI* dt_cd(string cd) override{
			for(TDtI& dt : dmyl){
				if (dt.g("cd") == cd) return &dt;
			}
			return nullptr;
		}
};

VTgLI::VTgLI(QWidget* p) : QWidget(p){
	vwk = new QLabel(this);
	QFont f = vwk->font();
	f.setPointSize(24);
	f.setBold(true);
	vwk->setFont(f);
	vad = new QPushButton("＋", this);
	QObject::connect(vad, &QPushButton::clicked, [this](){
		this->oa();
	});
	vde = new QPushButton("削除", this);
	QObject::connect(vde, &QPushButton::clicked, [this](){
		this->od();
	});
}
void VTgLI::i_tg(TgI* tg_){
	tg = tg_;
}
void VTgLI::oa(){
	VEvO ev(EvTp::TgToEdit, tg);
	VG::inst()->ev->rqEv(&ev);
}
void VTgLI::od(){
	QMessageBox::StandardButton r;
	r = QMessageBox::question(this, "確認", QString("%1 を削除します。").arg(tg->wk.c_str()), QMessageBox::Ok | QMessageBox::Cancel);
	if (r == QMessageBox::Ok){
		VEvO ev(EvTp::TgDel, &tg->wk);
		VG::inst()->ev->rqEv(&ev);
	}
}
QSize VTgLI::ly(QSize sz){
	int y = 0;
	int w = sz.width();
	vwk->setGeometry(0, y, w-50, 35);
	vde->setGeometry(w-50, y, 50, 35);
	y += 35;
	int x = 0;
	int uw = 200;
	int mh = 0;
	if (w < uw) w = uw;
	for(int i = 0; i < vbnl.size(); ++i){
		QSize isz = vbnl[i]->ly(QSize(uw, 0xffff));
		if (x + isz.width() > w){
			y += mh;
			x = 0;
			mh = 0;
		}
		vbnl[i]->setGeometry(x, y, isz.width(), isz.height());
		x += isz.width();
		if (mh < isz.height()) mh = isz.height();
	}
	if (x + uw > w){
		y += mh;
		x = 0;
		mh = 0;
	}
	vad->setGeometry(x, y+10, uw, 30);
	if (mh == 0){
		y += 50;
	}
	else{
		y += mh;
	}
	return QSize(w, y);
}
void VTgLI::u(){
	vwk->setText(QString::fromStdString(tg->wk));
	for(VTgDt* vbn:vbnl){
		delete vbn;
	}
	vbnl.clear();
	for(string& cd : tg->cds){
		TDtI* dt = dtitf->dt_cd(cd);
		VTgDt* dti = new VTgDt(this);
		dti->i_dt(dt);
		dti->u();
		vbnl.push_back(dti);
	}
}
string VTgLI::t_v(){
	QWidget* p = t__V::vtest_p("VTgLI");
	DtItfDmy dmy;
	TgI tg;
	tg.wk = "waku1";
	for(int i = 0; i < 5; ++i){
		TDtI dt;
		dt.s("cd", "code"+to_string(i));
		dt.s("tg", "tg"+to_string(i));
		dt.s("bg", "/bg/"+to_string(i));
		dt.s("img", "/img/"+to_string(i));
		dt.s("cm", "comment"+to_string(i));
		dt.s("att", "att"+to_string(i));
		dt.s("ln", "https://ggmoyou.com/"+to_string(i));
		dmy.dmyl.push_back(dt);
		tg.cds.push_back(dt.g("cd"));
	}
	VTgLI v(p);
	v.dtitf = &dmy;
	v.i_tg(&tg);
	v.u();
	QSize sz = v.ly(QSize(600, 0xffff));
	v.setGeometry(0, 0, sz.width(), sz.height());
	t__V::vtest();
	return "view";
}
VTg::VTg(QWidget* p) : QScrollArea(p){
	vin = new QWidget();
	this->setWidget(vin);
	this->setWidgetResizable(true);
}
void VTg::i_tgl(vector<TgI>* tgl_){
	tgl = tgl_;
}
QSize VTg::ly(QSize sz){
	int y = 0;
	int w = sz.width();
	int ww = w - 20;
	for(VTgLI* v:vl){
		QSize isz = v->ly(QSize(ww, 0xffff));
		v->setGeometry(0, y, isz.width(), isz.height());
		y += isz.height()+50;
	}
	vin->setMinimumSize(QSize(ww, y));
	vin->updateGeometry();
	return sz;
}
void VTg::u(){
	for(VTgLI* v:vl){
		delete v;
	}
	vl.clear();
	for(TgI& tgi:*tgl){
		VTgLI* vtg = new VTgLI(vin);
		vtg->dtitf = dtitf;
		vtg->i_tg(&tgi);
		vtg->u();
		vtg->show();
		vl.push_back(vtg);
	}
}
string VTg::t_v(){
	QWidget* p = t__V::vtest_p("VTg");
	DtItfDmy dmy;
	vector<TgI> tgl;
	for(int j = 0; j < 4; ++j){
		TgI tg;
		tg.wk = "waku"+to_string(j);
		for(int i = 0; i < 5; ++i){
			if (i % 2 == j % 2) continue;
			tg.cds.push_back("code"+to_string(i));
		}
		tgl.push_back(tg);
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
		dmy.dmyl.push_back(dt);
	}
	VTg v(p);
	v.dtitf = &dmy;
	v.i_tgl(&tgl);
	v.u();
	QSize sz = v.ly(QSize(650, 600));
	v.setGeometry(0, 0, sz.width(), sz.height());
	t__V::vtest();
	return "view";
}
VTgSel::VTgSel(QWidget* p) : QDialog(p){
	vl = new QListWidget(this);
	QFont f = vl->font();
	f.setPointSize(20);
	vl->setFont(f);
	vok = new QPushButton("反映", this);
	QObject::connect(vok, &QPushButton::clicked, this, &QDialog::accept);
	vok->setAutoDefault(false);
	vbk = new QPushButton("戻る", this);
	QObject::connect(vbk, &QPushButton::clicked, this, &QDialog::reject);
	vbk->setAutoDefault(false);
}
void VTgSel::i_dtl(vector<TDtI>* dtl_){
	dtl = dtl_;
}
void VTgSel::i_tg(TgI* tg_){
	tg = tg_;
}
vector<string> VTgSel::chk(){
	vector<string> r;
	for(int i = 0; i < dtl->size(); ++i){
		QListWidgetItem* itm = vl->item(i);
		if (itm->checkState() == Qt::Checked){
			r.push_back((*dtl)[i].g("cd"));
		}
	}
	return r;
}
QSize VTgSel::ly(QSize sz){
	int y = 0;
	int w = sz.width();
	vl->setGeometry(0, y, w, sz.height()-40);
	y = sz.height()-40;
	int x = 0;
	vok->setGeometry(x, y, w/2, 40);
	x += w/2;
	vbk->setGeometry(x, y, w/2, 40);
	x += w/2;
	return sz;
}
void VTgSel::u(){
	setWindowTitle(QString::fromStdString(tg->wk));
	vl->clear();
	for(TDtI& dt : *dtl){
		QListWidgetItem* li = new QListWidgetItem(QString::fromStdString(dt.g("cd")));
		li->setFlags((li->flags() & ~Qt::ItemIsSelectable) | Qt::ItemIsUserCheckable);
		auto fi = std::find(tg->cds.begin(), tg->cds.end(), dt.g("cd"));
		if (fi == tg->cds.end()){
			li->setCheckState(Qt::Unchecked);
		}
		else{
			li->setCheckState(Qt::Checked);
		}
		vl->addItem(li);
	}
}
string VTgSel::t_v(){
	t__V::vtest_a();
	vector<TDtI> dtl;
	TgI tg;
	tg.wk = "waku1";
	for(int i = 0; i < 5; ++i){
		TDtI dt;
		dt.s("cd", "code"+to_string(i));
		dt.s("tg", "tg"+to_string(i));
		dt.s("bg", "/bg/"+to_string(i));
		dt.s("img", "/img/"+to_string(i));
		dt.s("cm", "comment"+to_string(i));
		dt.s("att", "att"+to_string(i));
		dt.s("ln", "https://ggmoyou.com/"+to_string(i));
		dtl.push_back(dt);
		if (i % 2 == 0){
			tg.cds.push_back(dt.g("cd"));
		}
	}
	VTgSel v(nullptr);
	v.i_dtl(&dtl);
	v.i_tg(&tg);
	v.u();
	QSize sz = v.ly(QSize(650, 400));
	v.resize(sz);
	int r = v.exec();
	stringstream ss;
	vector<string> rs = v.chk();
	for(string& s : rs){
		ss << "[" << s << "]";
	}
	return "ans="+to_string(r)+ss.str();
}
}