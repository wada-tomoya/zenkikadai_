#pragma once
#include <stack>
#include <functional>

namespace tnl {

/**************************************************************************************************************
* 
*  �g�p�@�T���v��
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
		// �R���X�g���N�^
		// arg1... ��܃N���X�� this �|�C���^���w��
		// arg2... �R�[���o�b�N�ŌĂяo��������܃N���X�̃����o���\�b�h�܂��̓����_���w��
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
		// �V�[�P���X�̃A�b�v�f�[�g ( ���t���[���Ăяo����OK )
		// arg1... �t���[���Ԃ̌o�ߎ���( �b�̃f���^�^�C�� )
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
		// ��r
		//===================================================================================
		inline bool isComparable(bool (T::*func)(const float)) const { return p_now_ == func; }

		//===================================================================================
		// �������p�@�V�[�P���X�̍ŏ��̂P�t���[������ true ���A��
		//===================================================================================
		inline bool isStart() const { return is_start_; }

		//===================================================================================
		// �V�[�P���X�̌o�ߎ��Ԃ��擾 ( �b )
		//===================================================================================
		inline float getProgressTime() const { return sum_time_; }

		//===================================================================================
		// �V�[�P���X�̌o�ߎ��Ԃ��擾 ( �b )
		//===================================================================================
		inline uint32_t getProgressFrame() const { return sum_frame_; }

		//===================================================================================
		// �V�[�P���X�̕ύX
		// arg1... ���̃t���[��������s�������܃N���X�̃��\�b�h���w��
		//===================================================================================
		inline void change(bool (T::*func)(const float)) {
			prevs_.push(now_);
			p_prevs_.push(p_now_);
			next_ = func;
			p_next_ = func;
			is_change_ = true;
		}

		//===================================================================================
		// 1�O�̃V�[�P���X�ɖ߂�
		// tips... �O�̃V�[�P���X�����݂��Ȃ���Ή������Ȃ�
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
		// �V�[�P���X�𑦍��ɕύX
		// arg1... ���s�������܃N���X�̃��\�b�h���w��
		// tisp... ���t���[����҂��������ɃV�[�P���X��ύX����
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
		// �^���R���[�`��
		// 
		//

		// �R���[�`���@�\�t���V�[�P���X�錾�}�N��
		// arg1.. ��܃N���X��
		// arg2.. �����V�[�P���X
		#define TNL_CO_SEQUENCE( class_name, start_func )	tnl::Sequence<class_name> tnl_sequence_ = tnl::Sequence<class_name>(this, start_func);

		// �R���[�`���V�[�P���X�I������
		// tips.. �R���[�`���g�p���\�b�h�̍Ō�ɕK���R�[�����邱��
		#define TNL_SEQ_CO_END								tnl_sequence_._co_reset_call_count_(); return true ;

		// �R���[�`�����s�t���[�����̎擾
		#define TNL_SEQ_CO_PROG_FRAME						tnl_sequence_._co_get_prog_frame_()

		// �R���[�`�����s���Ԃ̎擾
		#define TNL_SEQ_CO_PROG_TIME						tnl_sequence_._co_get_prog_time_()

		// ���Y�R���[�`���̔j��
		#define TNL_SEQ_CO_BREAK							{ tnl_sequence_._co_break_() ; return ; }


		// �R���[�`���������������}�N��( �t���[�����w�� )
		// arg1... ���s�t���[���� (�}�C�i�X�̒l�Ŗ������[�v)
		// arg2... �f���^�^�C��
		// arg3... �R���[�`���Ŏ��s���郆�[�U��`����( void() �����_�� )
		// tips... �������[�v���w�肵���ꍇ�ł������Ōo�߃t���[�������J�E���g���Ă��܂�
		// ....... �t���[�����J�E���g�� int32_t �𒴂���� 0 �Ƀ��Z�b�g����邱�Ƃɒ���
		#define TNL_SEQ_CO_FRM_YIELD_RETURN( lim_frame, delta_time, logic )		if (tnl_sequence_._co_yield_by_frame_( lim_frame, delta_time, logic ) ) return true ;

		// �R���[�`���������������}�N��( ���Ԏw�� )
		// arg1... ���s����
		// arg2... �f���^�^�C��
		// arg3... �R���[�`���Ŏ��s���郆�[�U��`����( void() �����_�� )
		// tips... ���Ԏw�肵���ꍇ�ł������Ōo�߃t���[�������J�E���g���Ă��܂�
		// ....... �t���[�����J�E���g�� int32_t �𒴂���� 0 �Ƀ��Z�b�g����邱�Ƃɒ���
		#define TNL_SEQ_CO_TIM_YIELD_RETURN( lim_time, delta_time, logic )		if (tnl_sequence_._co_yield_by_time_( lim_time, delta_time, logic ) ) return true ;



		//------------------------------------------------------------------------------------------------------------------------
		// 
		// ���[�U�ɂ�钼�ڃR�[���͋֎~
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

