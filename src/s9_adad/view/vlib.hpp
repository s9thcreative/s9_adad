#pragma once

#include <QPixmap>
#include <QApplication>
#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QString>
#include <string>
#include <functional>

using namespace std;

namespace s9_adad::view{

class VEvO{
	public:
		int ev = 0;
		void* o = nullptr;
		VEvO(int ev, void* o);
		virtual ~VEvO() = default;
};
class VEv{
	public:
		virtual void rqEv(VEvO* ev);
		virtual void doEv(VEvO* ev) = 0;
		virtual ~VEv() = default;
};
class VEvDmy : public VEv{
	public:
		void rqEv(VEvO* ev) override;
		void doEv(VEvO* ev) override;
		~VEvDmy() = default;
};
class VImLd{
	public:
		inline static VImLd* _cur = nullptr;
		static VImLd* cur();
		virtual ~VImLd() = default;
		virtual void ld(string url, std::function<void(QPixmap*)> cb);
		static string t_ld();
		static string t_ld_x();
};
class VImLdDmy : public VImLd{
	public:
		void ld(string url, std::function<void(QPixmap*)> cb) override;
};
class VG{
	public:
		static VG* inst();
		QApplication* app = nullptr;
		QNetworkAccessManager* net = nullptr;
		VEv* ev = nullptr;
};
class t__V{
	public:
		inline static QApplication* vtest_app = nullptr;
		inline static QMainWindow* vtest_mw = nullptr;
		static QWidget* vtest_p(QString ttl="");
		static void vtest();
		static void vtest_a();
};
}