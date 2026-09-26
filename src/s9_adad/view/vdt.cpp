#include "vdt.hpp"
#include <QApplication>
#include <QMainWindow>
#include <QSize>
#include <QFont>
#include <QObject>
#include <QMessageBox>
#include "vlib.hpp"
#include "vc.hpp"

using namespace s9_adad::core;

namespace s9_adad::view{

VDtLI::VDtLI(QWidget* p) : QWidget(p){
	vcd = new QLabel(this);
	QFont f = vcd->font();
	f.setPointSize(20);
	f.setBold(true);
	vcd->setFont(f);
	vbn = new QLabel(this);
	vbn->setScaledContents(true);
}
void VDtLI::i_dt(TDtI* dt_){
	dt = dt_;
}
QSize VDtLI::ly(QSize sz){
	int y = 0;
	int w = sz.width();
	vcd->setGeometry(0, y, w, 20);
	y += 20;
	int imh = w*100/640;
	vbn->setGeometry(0, y, w, imh);
	y += imh;
	return QSize(w, y);
}
void VDtLI::u(){
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
string VDtLI::t_v(){
	QWidget* p = t__V::vtest_p();
	VDtLI v(p);
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

VDtL::VDtL(QWidget* p) : QWidget(p){
	vad = new QPushButton("追加", this);
	QObject::connect(vad, &QPushButton::clicked, [this](){
		this->oa();
	});
	vl = new QListWidget(this);
	QObject::connect(vl, &QListWidget::itemClicked, [this](QListWidgetItem *c){
		VDtLI* v = (VDtLI*)this->vl->itemWidget(c);
		this->oi(v->dt);
	});
}
void VDtL::i_dtl(vector<TDtI>* dtl_){
	dtl = dtl_;
}
void VDtL::oa(){
	cout << "add" << endl;
	if (cba != nullptr){
		cba();
	}
}
void VDtL::oi(TDtI* itm){
	cout << "item " << (itm->g("cd")) << endl;
	if (cbi != nullptr){
		cbi(itm);
	}
}
QSize VDtL::ly(QSize sz){
	int y = 0;
	int w = sz.width();
	vad->setGeometry(0, y, w, 35);
	y += 35;
	vl->setGeometry(0, y, w, sz.height()-y);
	for(int i = 0; i < vl->count(); i++){
		QListWidgetItem* itm = vl->item(i);
		VDtLI* dti = (VDtLI*)vl->itemWidget(itm);
		QSize isz = dti->ly(QSize(w-35, 65535));
		itm->setSizeHint(isz);
	}
	return sz;
}
void VDtL::u(){
	vl->clear();
	for(TDtI& dt : *dtl){
		QListWidgetItem* itm = new QListWidgetItem(vl);
		VDtLI* dti = new VDtLI();
		dti->i_dt(&dt);
		dti->u();
		vl->setItemWidget(itm, dti);
	}
}
string VDtL::t_v(){
	QWidget* p = t__V::vtest_p();
	VDtL v(p);
	vector<TDtI> dtl;
	for(int i = 0; i < 5; i++){
		TDtI dt;
		dt.s("cd", "code"+to_string(i));
		dt.s("img", "https://yupj.jp/game/s9ad/1.0/img/hanabira/title.png");
		dtl.push_back(dt);
	}
	v.i_dtl(&dtl);
	v.u();
	QSize sz = v.ly(QSize(300, 600));
	v.setGeometry(0, 0, sz.width(), sz.height());
	t__V::vtest();
	return "view";
}

VDtEd::VDtEd(QWidget* p) : QWidget(p){
	ved = new QPushButton(this);
	QObject::connect(ved, &QPushButton::clicked, [this](){
		this->oe();
	});
	vde = new QPushButton("削除", this);
	QObject::connect(vde, &QPushButton::clicked, [this](){
		this->od();
	});
	static QString labels[7] = {"cd","tg","bg","img","cm","att","ln"};
	for(int i = 0; i < 7; i++){
		QLabel* lb = new QLabel(this);
		lb->setText(labels[i]);
		QLineEdit* le = new QLineEdit(this);
		vflbs[i] = lb;
		vflds[i] = le;
	}
}
void VDtEd::i_dt(TDtI* dt_){
	dt = dt_;
}
void VDtEd::oe(){
	string s[7];
	for(int i = 0; i < 7; i++){
		s[i] = vflds[i]->text().toStdString();
	}
	TDtI ndt;
	ndt.s("tg", s[1]);
	ndt.s("bg", s[2]);
	ndt.s("img", s[3]);
	ndt.s("cm", s[4]);
	ndt.s("att", s[5]);
	ndt.s("ln", s[6]);
	if (dt == nullptr){
		ndt.s("cd", s[0]);
		VEvO ev(EvTp::DtAdd, &ndt);
		VG::inst()->ev->rqEv(&ev);
	}
	else{
		ndt.s("cd", dt->g("cd"));
		VEvO ev(EvTp::DtEdit, &ndt);
		VG::inst()->ev->rqEv(&ev);
	}
}
void VDtEd::od(){
	string cd = dt->g("cd");
	QMessageBox::StandardButton r;
	r = QMessageBox::question(this, "確認", QString("%1 を削除します。").arg(cd.c_str()), QMessageBox::Ok | QMessageBox::Cancel);
	if (r == QMessageBox::Ok){
		VEvO ev(EvTp::DtDel, &cd);
		VG::inst()->ev->rqEv(&ev);
	}
}
QSize VDtEd::ly(QSize sz){
	int y = 0;
	int w = sz.width();
	int x = 0;
	ved->setGeometry(x, y, 60, 35);
	x += 70;
	vde->setGeometry(x, y, 60, 35);
	y += 40;
	for(int i = 0; i < 7; i++){
		x = 0;
		vflbs[i]->setGeometry(x, y, 60, 35);
		x += 70;
		vflds[i]->setGeometry(x, y, w-x, 35);
		y += 40;
	}
	return QSize(w, y);
}
void VDtEd::u(){
	if (dt == nullptr){
		ved->setText("追加");
		vde->setVisible(false);
		vflds[0]->setReadOnly(false);
		for(int i = 0; i < 7; i++){
			vflds[i]->setText("");
		}
	}
	else{
		ved->setText("変更");
		vde->setVisible(true);
		vflds[0]->setReadOnly(true);
		QString vs[7] = {
			QString::fromStdString(dt->g("cd")),
			QString::fromStdString(dt->g("tg")),
			QString::fromStdString(dt->g("bg")),
			QString::fromStdString(dt->g("img")),
			QString::fromStdString(dt->g("cm")),
			QString::fromStdString(dt->g("att")),
			QString::fromStdString(dt->g("ln")),
		};
		for(int i = 0; i < 7; i++){
			vflds[i]->setText(vs[i]);
		}
	}
}
string VDtEd::t_v(){
	QWidget* p = t__V::vtest_p();
	VDtEd v(p);
	TDtI dt;
	dt.s("cd", "code1");
	dt.s("tg", "app");
	dt.s("bg", "/bg/test.png");
	dt.s("img", "/img/test.png");
	dt.s("cm", "comment");
	dt.s("att", "ATT");
	dt.s("ln", "https://ggmoyou.com/");
	v.i_dt(&dt);
	v.u();
	QSize sz = v.ly(QSize(600, 0xffff));
	v.setGeometry(0, 0, sz.width(), sz.height());
	t__V::vtest();
	return "view";
}
string VDtEd::t_v_n(){
	QWidget* p = t__V::vtest_p();
	VDtEd v(p);
	v.i_dt(nullptr);
	v.u();
	QSize sz = v.ly(QSize(600, 0xffff));
	v.setGeometry(0, 0, sz.width(), sz.height());
	t__V::vtest();
	return "view";
}

VDt::VDt(QWidget* p) : QWidget(p){
	vl = new VDtL(this);
	vl->cba = [this](){
		this->oa();
	};
	vl->cbi = [this](TDtI* dt){
		this->oi(dt);
	};
	ved = new VDtEd(this);
}
void VDt::i_dtl(vector<TDtI>* dtl_){
	dtl = dtl_;
	vl->i_dtl(dtl);
}
void VDt::oa(){
	ved->i_dt(nullptr);
	ved->u();
}
void VDt::oi(TDtI* itm){
	ved->i_dt(itm);
	ved->u();
}
QSize VDt::ly(QSize sz){
	int x = 0;
	QSize lsz;
	lsz = vl->ly(QSize(300, sz.height()));
	vl->setGeometry(x, 0, lsz.width(), lsz.height());
	x += lsz.width() + 10;
	lsz = ved->ly(QSize(sz.width() - x, sz.height()));
	ved->setGeometry(x, 0, lsz.width(), lsz.height());
	return sz;
}
void VDt::u(){
	vl->u();
	ved->u();
}
string VDt::t_v(){
	QWidget* p = t__V::vtest_p();
	VDt v(p);
	vector<TDtI> dtl;
	for(int i = 0; i < 5; i++){
		TDtI dt;
		dt.s("cd", "code"+to_string(i));
		dt.s("tg", "app");
		dt.s("bg", "/bg/test.png");
		dt.s("img", "/img/test.png");
		dt.s("cm", "comment"+to_string(i));
		dt.s("att", "ATT");
		dt.s("ln", "https://ggmoyou.com/"+to_string(i));
		dtl.push_back(dt);
	}
	v.i_dtl(&dtl);
	v.u();
	QSize sz = v.ly(QSize(900, 800));
	v.setGeometry(0, 0, sz.width(), sz.height());
	t__V::vtest();
	return "view";
}
}