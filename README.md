# Boost.Range��Oven�g�������삵�Ă݂�

���̋L���� [C++ Advent Calendar](http://www.adventar.org/calendars/926) 24���ڂ̋L���ł��B

�ŋ߂̎d���ł͂����ς�PHP�΂��菑���Ă܂����AC++���G��Ă������ȁ[�Ƃ������ŎQ�����܂����B

# �O����

�����̃��C�u�����Ƃ���[PStade.Oven](http://p-stade.sourceforge.net/oven/doc/html/index.html)�⍂���������[Boost.Range�g�����C�u����](https://github.com/faithandbrave/OvenToBoost)������܂����A�����ƃV���v���ŁA�A�v�����̐�p�֐��Ƃ��Ďg�������A�Ƃ������R�Ǝ����̕׋����Ă�ɍ쐬���Ă��܂��B

# �g����

```cpp
#include "range.h"

auto v = {1, 2, 3, 4, 5};
// {2, 4}
auto q = v | range::filtered_even();
```

Boost��Range Adaptors�Ɠ������p�C�v���C���ɂ���Čq���܂��B

(�ʓ|�ȈׁA������boost::adaptors���C�u������range::�Ŏg����悤��namespace����
```
using namespace boost::adaptors;
```
���Ă��܂�)

# �p�C�v���C���L�@�ŏ�����悤�Ɏ�������ɂ�

�p�C�v���C���L�@���g����悤�ɂ���ɂ́Aoperator|���Еt���Ƃ��āARange�l(�z��Ɍ���Ȃ��Abegin()��end()�����������͈͂�\���l)�ƁARange�l����V����Range�l��Ԃ�����������K�v������܂��B

```cpp
// Range����v�f�̌^�𓾂�
template <class Range>
auto _rangetype(Range&& r) ->
	typename std::decay<
		decltype(*std::begin(r))
	>::type;
// �z��v�f�̌^�擾
#define rangedecltype(r) decltype(_rangetype(r))

namespace range {
	//------------------------------
	/// INTERNAL ONLY
	/// 
	// {
		struct _FilteredEvenHolder {};
		
		template <typename T>
		struct _is_even {
			bool operator()(const T &t) const {
				return t % 2 == 0;
			}
		};
	// }

	_FilteredEvenHolder filtered_even() { return _FilteredEvenHolder(); }

	template <typename Range>
	auto operator| (Range &r, const _FilteredEvenHolder &)
	{
		return r | boost::adaptors::filtered(_is_even<rangedecltype(r)>());
	}

	template <typename Range>
	auto operator| (const Range &r, const _FilteredEvenHolder &)
	{
		return r | boost::adaptors::filtered(_is_even<rangedecltype(r)>());
	}
}
```

Boost.Range.Adaptors��filtered����ꉻ���Ă����filtered�𗘗p���Ă��܂����Aboost::iterator_range��Ԃ��悤��filterd�ɓ����镔����������o���܂��B

���̌`����{�`�Ƃ��āARange����֌W��S��range::hoge�ōς܂�����悤�ɃA�v�����C�u������p�ӂ��Ă����΁A���������ǂގ����V���v���ɏo����񂶂�Ȃ����ȁA�Ƃ����Љ�ł����B

# range.h�Ɏn�܂�e���C�u�����ɂ���

�쐬�̍ۂ�Boost��Range�̎����APStade.Oven�̎����𑽕��ɎQ�l�ɂ��Ă��܂��B
2�N���O�ɏ�����C++11 & boost 1.49���̓����̃R�[�h�̂܂܂ƂȂ��Ă��܂����A�����C++14 & boost 1.60�̊��ł����삵�܂����B

���ӂ��č쐬���Ă͂��܂����A������肪����܂����炲�񍐂����肢���܂��B

## range::sorted�Erange::stable_sorted�ɂ���

����͂��Ȃ�����I�Ȏ����ƂȂ��Ă���A�������@��PStade.Oven��oven::sorted�Ɠ����Ȃ̂ł����Aboost::iterator_range�������ɂ��ƂƂȂ�Range��shared_ptr�ŕێ�����ׁA���R�X�g�ȏ����ɂȂ��Ă��܂��A�ʓ|�ł͂���܂����A��[vector�����Astd::sort���������������ł��B

