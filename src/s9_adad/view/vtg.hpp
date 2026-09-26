#pragma once

#include <QWidget>
#include <QPixmap>
#include <QLabel>
#include <QSize>
#include <string>
#include <vector>
#include <QPushButton>
#include <QListWidget>
#include <QLineEdit>
#include <QScrollArea>
#include <QDialog>
#include <functional>
#include "../core/tb.cpp"
#include "../core/acd.cpp"

using namespace std;

namespace s9_adad::view{
class VTgDt : public QWidget{
	Q_OBJECT
	public:
		QLabel* vcd = nullptr;
		QLabel* vbn = nullptr;
		s9_adad::core::TDtI* dt = nullptr;
		string cu;
		QPixmap* ci = nullptr;
		explicit VTgDt(QWidget* p=nullptr);
		~VTgDt() = default;
		void i_dt(s9_adad::core::TDtI* dt);
		QSize ly(QSize sz);
		void u();
		static string t_v();
};
class DtItf{
	public:
		virtual ~DtItf() = default;
		virtual s9_adad::core::TDtI* dt_cd(string cd) = 0;
};
class VTgLI : public QWidget{
	Q_OBJECT
	public:
		QLabel* vwk = nullptr;
		vector<VTgDt*> vbnl;
		QPushButton* vde = nullptr;
		QPushButton* vad = nullptr;
		DtItf* dtitf = nullptr;
		s9_adad::core::TgI* tg = nullptr;
		explicit VTgLI(QWidget* p=nullptr);
		~VTgLI() = default;
		void i_tg(s9_adad::core::TgI* tg);
		void oa();
		void od();
		QSize ly(QSize sz);
		void u();
		static string t_v();
};
class VTg : public QScrollArea{
	Q_OBJECT
	public:
		QWidget* vin= nullptr;
		vector<VTgLI*> vl;
		DtItf* dtitf = nullptr;
		vector<s9_adad::core::TgI>* tgl = nullptr;
		explicit VTg(QWidget* p=nullptr);
		~VTg() = default;
		void i_tgl(vector<s9_adad::core::TgI>* tgl);
		QSize ly(QSize sz);
		void u();
		static string t_v();
};
class VTgSel : public QDialog{
	Q_OBJECT
	public:
		QListWidget* vl = nullptr;
		QPushButton* vok = nullptr;
		QPushButton* vbk = nullptr;
		vector<s9_adad::core::TDtI>* dtl = nullptr;
		s9_adad::core::TgI* tg = nullptr;
		explicit VTgSel(QWidget* p=nullptr);
		~VTgSel() = default;
		void i_dtl(vector<s9_adad::core::TDtI>* dtl);
		void i_tg(s9_adad::core::TgI* tg);
		vector<string> chk();
		QSize ly(QSize sz);
		void u();
		static string t_v();
};
}