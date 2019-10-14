#ifndef TWODIM_H
#define TWODIM_H

#include <vector>
#include <functional>
#include <algorithm>
#include "multi.h"

namespace pattern {
	template < typename T >
	class baker_bird {
	private:
		std::vector < std::vector < T > > pat, text;
		std::function < int(T) > ordering;
	public:
		baker_bird(
			const std::vector < std::vector < T > > & _pat,
			const std::vector < std::vector < T > > & _text
			const std::function < int(T) > & _ordering;
		)
			pat(_pat), text(_text), ordering(_ordering)
		{
			
		}
		void find() {
			static_aho_corasick < char, 3 * pat.size(), 2 > aho;
			aho.make()
		}
	};
}
#endif