#ifndef SINGLE_H
#define SINGLE_H
#include <string>
#include <vector>
namespace pattern {
	template < typename T >
	class kmp {
	private:
		std::vector < T > pat;
		std::vector < T > text;
		std::vector < int > fail;
	public:
		kmp(const std::vector < T >& _pat, const std::vector < T >& _text) :
			pat(_pat), text(_text) {}
		void make() {
			fail.resize(pat.size());
			size_t j = 0;
			for (size_t i = 1; i < pat.size(); i++) {
				while (j > 0 && pat[i] != pat[j]) {
					j = fail[j - 1];
				}
				if (pat[i] == pat[j]) {
					fail[i] = ++j;
				}
			}
		}
		std::vector < int > find() {
			std::vector < int > re;
			size_t j = 0;
			for (size_t i = 0; i < text.size(); i++) {
				while (j > 0 && text[i] != pat[j]) {
					j = fail[j - 1];
				}
				if (text[i] == pat[j]) {
					if (j == pat.size() - 1) {
						re.push_back(i - pat.size() + 1);
						j = fail[j];
					}
					else {
						++j;
					}
				}
			}
			return re;
		}
	};
}
#endif