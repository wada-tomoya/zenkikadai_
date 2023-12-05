#pragma once
#include <stack>
#include <functional>

namespace tnl {

/**************************************************************************************************************
* 
*  使用法サンプル
* 

class Test {
public :
	tnl::Sequence<Test> seq_ = tnl::Sequence<Test>( this, &Test::seq1 );
	bool seq1(const float delta_time);
	bool seq2(const float delta_time);
	bool seq3(const float delta_time);
};

bool Test::seq1(const float delta_time) {
	DrawStringEx(100, 100, -1, "seq1");
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_Z)) seq_.change(&Test::seq2);
	return true;
}
bool Test::seq2(const float delta_time) {
	DrawStringEx(100, 100, -1, "seq2");
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_Z)) seq_.change(&Test::seq3);
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_X)) seq_.back();
	return true;
}
bool Test::seq3(const float delta_time) {
	DrawStringEx(100, 100, -1, "seq3");
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_X)) seq_.back();
	return true;
}

Test* inst = nullptr ;

void gameMain(float delta_time) {

	if (!init) {
		inst = new Test();
		init = true;
	}

	inst->seq_.update(delta_time);

}

*****************************************************************************************************/

	template <class T>
	class Sequence final {
	private:
		T* object_;
		std::function<bool(T*, const float)> now_;
		std::function<bool(T*, const float)> next_;
		std::stack<std::function<bool(T*, const float)>> prevs_;
		bool (T::*p_now_)(const float);
		bool (T::*p_next_)(const float);
		typedef bool (T::*p_prev_)(const float) ;
		std::stack<p_prev_> p_prevs_;
		bool is_start_ = true;
		bool is_change_ = false;
		float sum_time_ = 0;
		uint32_t sum_frame_ = 0;

		uint32_t co_call_count_ = 0;
		uint32_t co_call_through_ = 0;
		int32_t co_frame_ = 0;
		float co_time_ = 0;
		bool co_is_break_ = false;

		Sequence() {}
	public:

		//===================================================================================
		// コンストラクタ
		// arg1... 包含クラスの this ポインタを指定
		// arg2... コールバックで呼び出したい包含クラスのメンバメソッドまたはラムダ式指定
		//===================================================================================
		Sequence(T* obj, bool (T::*func)(const float))
			: object_(obj)
			, next_(func)
			, now_(func)
			, p_now_(func)
			, p_next_(func)
		{}

		~Sequence() {}

		//===================================================================================
		// シーケンスのアップデート ( 毎フレーム呼び出せばOK )
		// arg1... フレーム間の経過時間( 秒のデルタタイム )
		//===================================================================================
		inline bool update(const float deltatime) {
			sum_time_ += deltatime;
			bool ret = now_(object_, deltatime);
			sum_frame_++;
			if (!is_change_) {
				is_start_ = false;
				return ret;
			}
			now_ = next_;
			p_now_ = p_next_;
			is_start_ = true;
			sum_time_ = 0;
			sum_frame_ = 0;
			is_change_ = false;
			_co_reset_();
			return ret;
		}

		//===================================================================================
		// 比較
		//===================================================================================
		inline bool isComparable(bool (T::*func)(const float)) const { return p_now_ == func; }

		//===================================================================================
		// 初期化用　シーケンスの最初の１フレームだけ true が帰る
		//===================================================================================
		inline bool isStart() const { return is_start_; }

		//===================================================================================
		// シーケンスの経過時間を取得 ( 秒 )
		//===================================================================================
		inline float getProgressTime() const { return sum_time_; }

		//===================================================================================
		// シーケンスの経過時間を取得 ( 秒 )
		//===================================================================================
		inline uint32_t getProgressFrame() const { return sum_frame_; }

		//===================================================================================
		// シーケンスの変更
		// arg1... 次のフレームから実行させる包含クラスのメソッドを指定
		//===================================================================================
		inline void change(bool (T::*func)(const float)) {
			prevs_.push(now_);
			p_prevs_.push(p_now_);
			next_ = func;
			p_next_ = func;
			is_change_ = true;
		}

		//===================================================================================
		// 1つ前のシーケンスに戻る
		// tips... 前のシーケンスが存在しなければ何もしない
		//===================================================================================
		inline void undo() {
			if (prevs_.empty()) return;			
			next_ = prevs_.top();
			p_next_ = p_prevs_.top();
			prevs_.pop();
			p_prevs_.pop();
			is_change_ = true;
		}


		//===================================================================================
		// シーケンスを即座に変更
		// arg1... 実行させる包含クラスのメソッドを指定
		// tisp... 次フレームを待たず即座にシーケンスを変更する
		//===================================================================================
		inline void immediatelyChange(bool (T::*func)(const float)) {
			prevs_.push(now_);
			p_prevs_.push(p_now_);
			now_ = func;
			p_now_ = func;
			is_start_ = true;
			sum_time_ = 0;
			_co_reset_();
			is_change_ = false;
		}





		//------------------------------------------------------------------------------------------------------------------------
		//
		//
		// 疑似コルーチン
		// 
		//

		// コルーチン機能付きシーケンス宣言マクロ
		// arg1.. 包含クラス名
		// arg2.. 初期シーケンス
		#define TNL_CO_SEQUENCE( class_name, start_func )	tnl::Sequence<class_name> tnl_sequence_ = tnl::Sequence<class_name>(this, start_func);

		// コルーチンシーケンス終了処理
		// tips.. コルーチン使用メソッドの最後に必ずコールすること
		#define TNL_SEQ_CO_END								tnl_sequence_._co_reset_call_count_(); return true ;

		// コルーチン実行フレーム数の取得
		#define TNL_SEQ_CO_PROG_FRAME						tnl_sequence_._co_get_prog_frame_()

		// コルーチン実行時間の取得
		#define TNL_SEQ_CO_PROG_TIME						tnl_sequence_._co_get_prog_time_()

		// 当該コルーチンの破棄
		#define TNL_SEQ_CO_BREAK							{ tnl_sequence_._co_break_() ; return ; }


		// コルーチン内部処理実装マクロ( フレーム数指定 )
		// arg1... 実行フレーム数 (マイナスの値で無限ループ)
		// arg2... デルタタイム
		// arg3... コルーチンで実行するユーザ定義処理( void() ラムダ式 )
		// tips... 無限ループを指定した場合でも内部で経過フレーム数をカウントしています
		// ....... フレーム数カウントは int32_t を超えると 0 にリセットされることに注意
		#define TNL_SEQ_CO_FRM_YIELD_RETURN( lim_frame, delta_time, logic )		if (tnl_sequence_._co_yield_by_frame_( lim_frame, delta_time, logic ) ) return true ;

		// コルーチン内部処理実装マクロ( 時間指定 )
		// arg1... 実行時間
		// arg2... デルタタイム
		// arg3... コルーチンで実行するユーザ定義処理( void() ラムダ式 )
		// tips... 時間指定した場合でも内部で経過フレーム数をカウントしています
		// ....... フレーム数カウントは int32_t を超えると 0 にリセットされることに注意
		#define TNL_SEQ_CO_TIM_YIELD_RETURN( lim_time, delta_time, logic )		if (tnl_sequence_._co_yield_by_time_( lim_time, delta_time, logic ) ) return true ;



		//------------------------------------------------------------------------------------------------------------------------
		// 
		// ユーザによる直接コールは禁止
		//
		inline void		_co_break_() { co_is_break_ = true; }
		inline int32_t	_co_get_prog_frame_() { return co_frame_; }
		inline float	_co_get_prog_time_() { return co_time_; }
		inline void		_co_reset_call_count_() { co_call_count_ = 0; }
		inline bool		_co_yield_by_frame_(int32_t limit_frame, float delta_time, const std::function<void()>& logic) {
			if (co_call_count_++ == co_call_through_) {
				int32_t limit = (0 > limit_frame) ? INT32_MAX : limit_frame;
				if (co_frame_ >= limit) return true;
				co_call_count_ = 0;
				co_time_ += delta_time;
				logic();
				co_frame_++ ;
				if (limit_frame < 0 && INT32_MAX == co_frame_) co_frame_ = 0;
				if (limit_frame < 0 && !co_is_break_) return true;
				if (co_frame_ < limit && !co_is_break_) return true;
				co_call_through_++;
				co_frame_ = 0;
				co_time_ = 0;
				co_is_break_ = false;
				return true;
			}
			return false;
		}
		inline bool		_co_yield_by_time_(float limit_time, float delta_time, const std::function<void()>& logic) {
			if (co_call_count_++ == co_call_through_) {
				if (co_time_ >= limit_time) return true;
				co_call_count_ = 0;
				co_time_ += delta_time;
				logic();
				co_frame_++;
				if(INT32_MAX == co_frame_) co_frame_ = 0;
				if (co_time_ < limit_time && !co_is_break_) return true;
				co_call_through_++;
				co_frame_ = 0;
				co_time_ = 0;
				co_is_break_ = false;
				return true;
			}
			return false;
		}
		inline void		_co_reset_() {
			co_call_count_ = 0;
			co_call_through_ = 0;
			co_frame_ = 0;
			co_time_ = 0;
			co_is_break_ = false;
		}


	};

}

