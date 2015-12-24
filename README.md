# Boost.RangeのOven拡張を自作してみる

この記事は [C++ Advent Calendar](http://www.adventar.org/calendars/926) 24日目の記事です。

最近の仕事ではもっぱらPHPばかり書いてますが、C++も触れていたいなーという事で参加しました。

# 前書き

既存のライブラリとして[PStade.Oven](http://p-stade.sourceforge.net/oven/doc/html/index.html)や高橋晶さんの[Boost.Range拡張ライブラリ](https://github.com/faithandbrave/OvenToBoost)がありますが、もっとシンプルで、アプリ毎の専用関数として使いたい、という理由と自分の勉強がてらに作成しています。

# 使い方

```cpp
#include "range.h"

auto v = {1, 2, 3, 4, 5};
// {2, 4}
auto q = v | range::filtered_even();
```

BoostのRange Adaptorsと同じくパイプラインによって繋げます。

(面倒な為、既存のboost::adaptorsライブラリをrange::で使えるようにnamespace内で
```
using namespace boost::adaptors;
```
しています)

# パイプライン記法で書けるように実装するには

パイプライン記法を使えるようにするには、operator|を糊付けとして、Range値(配列に限らない、begin()とend()を実装した範囲を表す値)と、Range値から新たなRange値を返す実装をする必要があります。

```cpp
// Rangeから要素の型を得る
template <class Range>
auto _rangetype(Range&& r) ->
	typename std::decay<
		decltype(*std::begin(r))
	>::type;
// 配列要素の型取得
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

Boost.Range.Adaptorsのfilteredを特殊化している為filteredを利用していますが、boost::iterator_rangeを返すようにfilterdに当たる部分を自作も出来ます。

この形を基本形として、Range操作関係を全部range::hogeで済ませられるようにアプリライブラリを用意しておけば、実装時も読む時もシンプルに出来るんじゃないかな、という紹介でした。

# range.hに始まる各ライブラリについて

作成の際にBoostのRangeの実装、PStade.Ovenの実装を多分に参考にしています。
2年程前に書いたC++11 & boost 1.49環境の当時のコードのままとなっていますが、今回のC++14 & boost 1.60の環境でも動作しました。

注意して作成してはいますが、何か問題がありましたらご報告をお願いします。

## range::sorted・range::stable_sortedについて

これはかなり実験的な実装となっており、実装方法はPStade.Ovenのoven::sortedと同じなのですが、boost::iterator_range生成時にもととなるRangeをshared_ptrで保持する為、高コストな処理になっています、面倒ではありますが、一端vector化し、std::sortをした方が高速です。

