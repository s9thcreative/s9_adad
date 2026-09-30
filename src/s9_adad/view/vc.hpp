#pragma once

#include <QMenuBar>
#include <vector>
#include <QMainWindow>
#include "vlib.hpp"
#include "vtg.hpp"
#include "vdt.hpp"
#include "../core/tb.cpp"
#include "../core/acd.cpp"
#include <string>

using namespace std;

namespace s9_adad::view{
	class EvTp{
		public:
			enum{
				DtAdd,
				DtEdit,
				DtDel,
				TgAdd,
				TgToEdit,
				TgEdit,
				TgDel,
				MnDt,
				MnTg,
				MnTgAdd,
				MnPhp
			};
	};
	class VM{
		public:
			enum class Cnd{
				Dt,
				Tg
			};
			vector<QAction*> ms;
			~VM() = default;
			void m(QMenuBar* b);
			static string t_m();
			void om(int i);
			void att(Cnd cnd);
			static string t_att();
			static string t_att_tg();
	};
	class VSp{
		public:
			enum{
				Pg_Dt,
				Pg_Tg
			};
			virtual ~VSp() = default;
			virtual int cr() = 0;
			virtual vector<s9_adad::core::TDtI>* dtl() = 0;
			virtual vector<s9_adad::core::TgI>* tgl() = 0;
			virtual DtItf* dtitf() = 0;
	};
	class VCI{
		public:
			virtual ~VCI() = default;
			virtual void dTgSel(s9_adad::core::TgI* tg) = 0;
			virtual void u() = 0;
			virtual void dEr(string) = 0;
	};
	class VC : public VCI{
		public:
			VSp* sp = nullptr;
			VM* m = nullptr;
			VDt* vdt = nullptr;
			VTg* vtg = nullptr;
			QMainWindow* w;
			void ini(string ttl);
			void st();
			void dTgSel(s9_adad::core::TgI* tg) override;
			void u() override;
			void dEr(string) override;
			static string t_v();
			static string t_v_tg();
			static string t_v_tgs();
			static string t_v_er();
	};
}