#ifndef MULTI_H
#define MULTI_H
#include <vector>
#include <string>
#include <queue>
#include <functional>
#include <algorithm>

#include <cassert>

namespace pattern {
	template < unsigned int ALPHA >
	class static_trie_node {
	private:
		int child[ALPHA];
		int fail;
		std::vector < int > output;
		int terminal;
		int now;
	public:
		static_trie_node() :terminal(-1) {
			std::fill(child, child + ALPHA, -1);
		}

		void set_child(int x, int c) {
			assert(0 <= x && x < ALPHA);
			child[x] = c;
		}
		int get_child(int x) {
			assert(0 <= x && x < ALPHA);
			return child[x];
		}

		void set_terminal(int id) {
			this->terminal = id;
		}
		int get_terminal() {
			return this->terminal;
		}

		void set_fail(int node) {
			this->fail = node;
		}
		int get_fail() {
			return this->fail;
		}

		std::vector < int > get_output() {
			return output;
		}
		void set_output(const std::vector < int > & _output) {
			this->output = _output;
		}
	};
	template < typename T, unsigned int NODE, unsigned int ALPHA >
	class static_aho_corasick {
	private:
		const int root = 0;
		int cul_cnt = 1;
		std::vector < static_trie_node < ALPHA > > trie;
		std::function <int(T)> ordering;
		std::vector < int > labels;
		bool insert_pattern(const std::vector < T >& pat, int id) {
			int curr_node = root;
			for (int i = 0; i < pat.size(); i++) {
				//printf("cur : %d ter : %d\n", curr_node, trie[curr_node].get_terminal());
				const int curr_char = ordering(pat[i]);
				const int child_x = trie[curr_node].get_child(curr_char);
				if (child_x == -1) {
					trie[curr_node].set_child(curr_char, cul_cnt++);
				}
				curr_node = trie[curr_node].get_child(curr_char);
			}
			if (trie[curr_node].get_terminal() == -1) {
				trie[curr_node].set_terminal(id);
				this->labels.push_back(id);
				return true;
			}
			else {
				this->labels.push_back(trie[curr_node].get_terminal());
				return false;
			}
			//printf("cur : %d ter : %d\n", curr_node, trie[curr_node].get_terminal());
		}
		void make_trie(
			const std::vector < std::vector < T > >& pats,
			std::function <int(T)> _ordering
		)
		{
			this->ordering = _ordering;
			int id = 0;
			for (const auto& pat : pats) {
				//printf("id : %d\n", id);
				if (insert_pattern(pat, id)) {
					++id;
				}
			}
			printf("id : %d\n", id);
		}
		void make_fail() {
			std::queue < int > qu;
			trie[root].set_fail(root);
			qu.push(root);
			while (!qu.empty()) {
				int cur_node = qu.front();
				qu.pop();
				for (int idx = 0; idx < ALPHA; idx++) {
					int child = trie[cur_node].get_child(idx);
					if (child == -1)continue;
					if (cur_node == root) {
						trie[child].set_fail(root);
					}
					else {
						int tmp = trie[cur_node].get_fail();
						while (tmp != root && trie[tmp].get_child(idx) == -1) {
							tmp = trie[tmp].get_fail();
						}
						if (trie[tmp].get_child(idx) != -1) {
							tmp = trie[tmp].get_child(idx);
						}
						trie[child].set_fail(tmp);
					}
					int tmp = trie[child].get_fail();
					std::vector < int > ou = trie[tmp].get_output();
					if (trie[child].get_terminal() != -1) {
						ou.push_back(trie[child].get_terminal());
					}
					trie[child].set_output(ou);
					qu.push(child);
					//printf("child : %d terminal : %d\n", child, trie[child].get_fail());
				}
			}
		}
	public:
		static_aho_corasick() {
			for (int i = 0; i < NODE; i++) {
				trie.push_back(static_trie_node < ALPHA >());
			}
		}
		std::vector < int > get_labels() {
			return this->labels;
		}
		void make(
			const std::vector < std::vector < T > > & pats,
			std::function <int(T)> _ordering
		) {
			make_trie(pats, _ordering);
			make_fail();
		}
		std::vector < std::pair < int, int > > find(const std::vector < T >& text) {
			using ii = std::pair < int, int >;
			std::vector < ii > re;
			int cur_node = this->root;
			for (size_t i = 0; i < text.size(); i++) {
				T alphabet = text[i];
				int chr = ordering(alphabet);
				while (cur_node != root && trie[cur_node].get_child(chr) == -1) {
					cur_node = trie[cur_node].get_fail();
				}
				if (trie[cur_node].get_child(chr) != -1) {
					cur_node = trie[cur_node].get_child(chr);
				}
				std::vector < int > ou = trie[cur_node].get_output();
				for (int pat : ou) {
					re.push_back(ii(i, pat));
				}
				//printf("state : %d\n", cur_node);
			}
			return re;
		}
	};
}
#endif