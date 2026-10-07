#include "vlib.hpp"
#include <QApplication>
#include <QMainWindow>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QSize>
#include <QFont>
#include <QObject>
#include <QTimer>
#include <iostream>

namespace s9_adad::view{
VEvO::VEvO(int ev_, void* o_){
	ev = ev_;
	o = o_;
}
void VEv::rqEv(VEvO* ev){
	doEv(ev);
}
void VEvDmy::rqEv(VEvO* ev){
	std::cout << "type=" << ev->ev << ";opt=" << (ev->o != nullptr) << std::endl;
}
void VEvDmy::doEv(VEvO* ev){
}
VImLd* VImLd::cur(){
	if (_cur == nullptr){
		_cur = new VImLd();
	}
	return _cur;
}
void VImLd::ld(string url, std::function<void(QPixmap*)> cb){
//	std::cout << url << " call" << endl;
	QUrl u(QString::fromStdString(url));
	QNetworkRequest req(u);
	QNetworkReply* r = VG::inst()->net->get(req);
	QObject::connect(r, &QNetworkReply::finished, [r, cb](){
		QPixmap* p = nullptr;
		if (r->error() == QNetworkReply::NoError){
			QByteArray d = r->readAll();
			p = new QPixmap();
			if (!p->loadFromData(d)){
				delete p;
				p = nullptr;
			}
		}
		cb(p);
		r->deleteLater();
	});

}
string VImLd::t_ld(){
	t__V::vtest_a();
	VG::inst()->net = new QNetworkAccessManager(t__V::vtest_app);
	VImLd ild;
	ild.ld("https://yupj.jp/game/s9ad/1.0/img/hanabira/title.png", [](QPixmap* p){
		if (p != nullptr){
			p->save("../test/vimld/ld.png", "png");
		}
		else{
			std::cout << "image load error" << std::endl;
		}
	});
	t__V::vtest_app->exec();
	return "test";
}
string VImLd::t_ld_x(){
	t__V::vtest_a();
	VG::inst()->net = new QNetworkAccessManager(t__V::vtest_app);
	VImLd ild;
	ild.ld("https://yupj.jp/game/s9ad/1.0/img/hanabira/titlex.png", [](QPixmap* p){
		if (p != nullptr){
			p->save("../test/vimld/ld_x.png", "png");
		}
		else{
			std::cout << "image load error" << std::endl;
		}
	});
	t__V::vtest_app->exec();
	return "test";
}

void VImLdDmy::ld(string url, std::function<void(QPixmap*)> cb){
	QPixmap* im = new QPixmap("../dummy/vdt/title.png");
	cb(im);
}
VG* VG::inst(){
	static VG* vg = new VG();
	return vg;
}

QWidget* t__V::vtest_p(QString ttl){
	int argc = 0;
	char* argv = nullptr;
	VImLd::_cur = new VImLdDmy();
	vtest_app = new QApplication(argc, &argv);
	vtest_mw = new QMainWindow();
	vtest_mw->setWindowTitle("test " + ttl);
	vtest_mw->resize(900, 800);
	QWidget* wg = new QWidget();
	vtest_mw->setCentralWidget(wg);
	VG::inst()->ev = new VEvDmy();
	return wg;
}
void t__V::vtest(){
	vtest_mw->show();
	vtest_app->exec();
}
void t__V::vtest_a(){
	int argc = 0;
	char* argv = nullptr;
	vtest_app = new QApplication(argc, &argv);
}

void t__V::vtest_wt(int ms, std::function<void()> f){
	QTimer::singleShot(ms, f);
}
}