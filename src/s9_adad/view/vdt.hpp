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
#include <functional>
#include "../core/tb.cpp"

using namespace std;

namespace s9_adad::view{
class VDtLI : public QWidget{
	Q_OBJECT
	public:
		QLabel* vcd = nullptr;
		QLabel* vbn = nullptr;
		s9_adad::core::TDtI* dt = nullptr;
		string cu;
		QPixmap* ci = nullptr;
		explicit VDtLI(QWidget* p=nullptr);
		~VDtLI() = default;
		void i_dt(s9_adad::core::TDtI* dt);
		QSize ly(QSize sz);
		void u();
		static string t_v();
};
class VDtL : public QWidget{
	Q_OBJECT
	public:
		QPushButton* vad = nullptr;
		QListWidget* vl = nullptr;
		vector<s9_adad::core::TDtI>* dtl = nullptr;
		vector<VDtLI*> li;
		std::function<void()> cba = nullptr;
		std::function<void(s9_adad::core::TDtI*)> cbi = nullptr;
		explicit VDtL(QWidget* p=nullptr);
		~VDtL() = default;
		void i_dtl(vector<s9_adad::core::TDtI>* dtl);
		void oa();
		void oi(s9_adad::core::TDtI* itm);
		QSize ly(QSize sz);
		void u();
		static string t_v();
};

class VDtEd : public QWidget{
	Q_OBJECT
	public:
		QLabel* vflbs[7] = {nullptr};
		QLineEdit* vflds[7] = {nullptr};
		QPushButton* ved = nullptr;
		QPushButton* vde = nullptr;
		QListWidget* vl = nullptr;
		s9_adad::core::TDtI* dt = nullptr;
		explicit VDtEd(QWidget* p=nullptr);
		~VDtEd() = default;
		void i_dt(s9_adad::core::TDtI* dt);
		void oe();
		void od();
		QSize ly(QSize sz);
		void u();
		static string t_v();
		static string t_v_n();
};

class VDt : public QWidget{
	Q_OBJECT
	public:
		VDtL* vl = nullptr;
		VDtEd* ved = nullptr;
		vector<s9_adad::core::TDtI>* dtl = nullptr;
		explicit VDt(QWidget* p=nullptr);
		~VDt() = default;
		void i_dtl(vector<s9_adad::core::TDtI>* dtl);
		void oa();
		void oi(s9_adad::core::TDtI* dt);
		QSize ly(QSize sz);
		void u();
		static string t_v();
};
}