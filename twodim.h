#ifndef TWODIM_H
#define TWODIM_H

#include <cstdio>
#include <vector>
#include <queue>
#include <iostream>
#include <cassert>
#include <functional>

#include "multi.h"

namespace pattern {
	template < typename T, unsigned int NODE, unsigned int ALPHA >
	class baker_bird {
	private:
		std::vector < std::vector < T > > pat, text;
		std::function < int(T) > ordering;
	public:
		baker_bird(
			const std::vector < std::vector < T > > & _pat,
			const std::vector < std::vector < T > > & _text,
			const std::function < int(T) > & _ordering
		):
			pat(_pat), text(_text), ordering(_ordering)
		{
		
		}
		std::vector < std::pair < int, int > > find() {
			using ii = std::pair < int, int >;
			
			static_aho_corasick < char, NODE, ALPHA > aho;
			aho.make(pat,ordering);
			std::vector < int > labels = aho.get_labels();
			
			const int pat_row = pat.size();
			const int pat_col = pat[0].size();
			const int text_row = text.size();
			const int text_col = text[0].size();
		
			std::vector < int > fail;
/*
			size_t j = 0;
			for (size_t i = 1; i < pat.size(); i++) {
				while (j > 0 && pat[i] != pat[j]) {
					j = fail[j - 1];
				}
				if (pat[i] == pat[j]) {
					fail[i] = ++j;
				}
			}
*/
			fail.resize(labels.size());
			size_t j = 0;
			for (size_t i = 1; i < labels.size(); i++) {
				while (j > 0 && labels[i] != labels[j]) {
					j = fail[j - 1];
				}
				if (labels[i] == labels[j]) {
					fail[i] = ++j;
				}
			}
			std::vector < ii > re;
			std::vector < int > match_states(text_col);
			for(size_t i = 0; i < text_row; i++){
				std::vector < ii > found_patterns = aho.find(text[i]);
				std::vector < int > found_labels(text_col, -1);
				for(auto p:found_patterns){
					const int idx = p.first;
					const int typ = p.second;
					found_labels[idx] = typ;
				}
				for(size_t j = 0; j < text_col; j++){
					while(match_states[j] > 0 && (found_labels[j] != labels[match_states[j]])){
						match_states[j] = fail[match_states[j] - 1];
					}
					if(found_labels[j] == labels[match_states[j]]){
						if(match_states[j] == pat_row - 1){
							re.push_back(ii(i,j));
							match_states[j] = fail[match_states[j]];
						}
						else{
							match_states[j]++;
						}	
					}
				}
				/*
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
				*/
			}
			return re;
		}
	};
}

#endif