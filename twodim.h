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
		bool match(int tx1, int ty1){
			for(size_t i=0;i<pat.size();i++){
				for(size_t j=0;j<pat[0].size();j++){
					if(text[tx1+i][ty1+j] != pat[i][j]){
						return false;
					}
				}
			}
			return true;
		}
		bool checker(const std::vector < std::pair < int, int > > &candidate){
			using ii = std::pair < int, int >;
			std::vector < std::pair < int, int > > real_answer;
			for(size_t i=pat.size()-1;i<text.size();i++){
				for(size_t j=pat[0].size()-1;j<text[i].size();j++){
					if(match(i - pat.size() + 1,j - pat[0].size() + 1)){
						real_answer.push_back(ii(i,j));
					}
				}
			}	
			if(candidate.size() == real_answer.size()){
				for(size_t i=0;i<candidate.size();i++){
					if(candidate[i] != real_answer[i]){
						return false;
					}
				}
				return true;
			}
			else{
				return false;
			}
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
			}
			return re;
		}
	};
}

#endif