#include <iostream>

#include "range.h"

using namespace std;

namespace range {

	//------------------------------
	// 偶数判定を取り出す
	/// INTERNAL ONLY
	/// 
	// {
		struct _FilteredEvenHolder {};
		
		template <typename T>
		struct _is_even {
			bool operator()(const T& t) const {
				return t % 2 == 0;
			}
		};
	// }

	_FilteredEvenHolder filtered_even() { return _FilteredEvenHolder(); }

	template <typename Range>
	auto operator| (Range& r, const _FilteredEvenHolder&)
	{
		return r | boost::adaptors::filtered(_is_even<rangedecltype(r)>());
	}

	template <typename Range>
	auto operator| (const Range& r, const _FilteredEvenHolder&)
	{
		return r | boost::adaptors::filtered(_is_even<rangedecltype(r)>());
	}
}

#include "range_myslice.h"

void f1() {
	auto v = {1, 2, 3, 4, 5, 6, 7, 8, 9};

	cout << "f1 even" << endl;
	{
		auto q = v
			| range::filtered([](const auto& x) {
				return x % 2 == 0;
			});
		
		for (const auto&x : q) cout << x << " ";
	}
	cout << endl
		<< "f1 even for custom filter" << endl;
	{
		auto q = v | range::filtered_even();
		
		for (const auto&x : q) cout << x << " ";
	}
	cout << endl
		<< "f1 even, three element, to vector" << endl;
	{
		auto _v = v
			| range::filtered_even()
			| range::mysliced(0, 3)
			| range::to_vector();
		
		for (const auto& x : _v) cout << x << " ";
		cout << endl;
	}
}



struct Student {
	int age;
	string name;
	enum class Sex {
		Male = 0,
		Female,
	};
	Sex sex;
	
	static bool isMale(const Student& s) { return s.sex == Sex::Male; }
	static bool isAdult(const Student& s) { return s.age >= 18; }
	
	static void disp(const Student& s) {
		cout << "age: " << s.age << " name:" << s.name << " sex:" << static_cast<int>(s.sex) << endl;
	}
};

namespace range {
	namespace student {
		//------------------------------
		// 男性を取り出す
		/// INTERNAL ONLY
		/// 
		// {
			struct _FilteredMaleHolder {};
			
			template <typename T>
			struct _is_male {
				bool operator()(const Student &s) const {
					return Student::isMale(s);
				}
			};
		// }

		_FilteredMaleHolder filtered_male() { return _FilteredMaleHolder(); }

		template <typename Range>
		auto operator| (Range &r, const _FilteredMaleHolder &)
		{
			return r | boost::adaptors::filtered(_is_male<rangedecltype(r)>());
		}

		template <typename Range>
		auto operator| (const Range &r, const _FilteredMaleHolder &)
		{
			return r | boost::adaptors::filtered(_is_male<rangedecltype(r)>());
		}
		
		//------------------------------
		// 成人を取り出す
		/// INTERNAL ONLY
		/// 
		// {
			struct _FilteredAdultHolder {};
			
			template <typename T>
			struct _is_adult {
				bool operator()(const Student &s) const {
					return Student::isAdult(s);
				}
			};
		// }

		_FilteredAdultHolder filtered_adult() { return _FilteredAdultHolder(); }

		template <typename Range>
		auto operator| (Range &r, const _FilteredAdultHolder &)
		{
			return r | boost::adaptors::filtered(_is_adult<rangedecltype(r)>());
		}

		template <typename Range>
		auto operator| (const Range &r, const _FilteredAdultHolder &)
		{
			return r | boost::adaptors::filtered(_is_adult<rangedecltype(r)>());
		}
	}
}


#include "range_map.h"                        // range::to_multimap
#include <boost/range/algorithm/for_each.hpp> // boost::for_each

void f2() {
	auto v = {
		Student {age:16, name: "bob",                  sex: Student::Sex::Male},
		Student {age:10, name: "tom",                  sex: Student::Sex::Male},
		Student {age:17, name: "yukarisan",            sex: Student::Sex::Female},
		Student {age:17, name: "noto",                 sex: Student::Sex::Female},
		Student {age:23, name: "hiroshi",              sex: Student::Sex::Male},
		Student {age:23, name: "taro",                 sex: Student::Sex::Male},
		Student {age:30, name: "hijirisawashonosuke",  sex: Student::Sex::Male},
	};
	
	cout << "male & adult" << endl;
	{
		auto q = v
			| range::student::filtered_male()
			| range::student::filtered_adult();
		
		for (const auto& x : q) Student::disp(x);
	}

	cout << endl
		<< "male & adult, to age key multimap" << endl;
	{
		auto mm = v
			| range::student::filtered_male()
			| range::student::filtered_adult()
			| range::to_multimap([](const auto &x) { return x.age; });
		
		for (auto it = begin(mm), _end = end(mm); it != _end; it = mm.upper_bound(it->first)) {
			boost::for_each(mm.equal_range(it->first), [](const auto& entry) {
				Student::disp(entry.second);
			});
			cout << "- - -" << endl;
		}
	}
}

int main() {
	f1();
	f2();
	return 0;
}

