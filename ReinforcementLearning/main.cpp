#include <iostream>
#include <array>
#include <random>

static constexpr int N = 30;
static constexpr int kMaxWeight = 100;
//価値と重さのペア
static constexpr std::array<std::pair<int, int>, N> baggage = {
	std::make_pair(1, 1),
};

std::random_device rd;
std::mt19937 mt(rd());
std::uniform_int_distribution<> dist_bool(0, 1);
std::uniform_int_distribution<> dist_int(0, N - 1);
std::uniform_int_distribution<> dist_mut(0, 100);
std::uniform_real_distribution<> dist_real(0.0f, 1.0f);

class gene {
	std::array<int, N> m_gene;

public:
	gene() {
		for (auto& it : m_gene) {
			it = dist_bool(mt);
		}
	}

	int value() const {
		int ret = 0;
		int weight = 0;
		for (int i = 0; i < N;i++) {
			if (m_gene.at(i)) {
				ret += baggage.at(i).first;
				weight += baggage.at(i).second;
				if (weight > kMaxWeight) {
					ret = 0;
					break;
				}
			}
		}
		return ret;
	}

	static gene crossing(const gene& parent1, const gene& parent2) {
		gene newgene;
		int point = dist_int(mt);
		for (int i = 0; i < point; i++) {
			newgene.m_gene.at(i) = parent1.m_gene.at(i);
		}
		for (int i = point; i < N; i++) {
			newgene.m_gene.at(i) = parent2.m_gene.at(i);
		}
		return newgene;
	}

	void mutation() {
		for (auto& it : m_gene) {
			if (dist_mut(mt) <= 0) {
				it = ++it % 2;
			}
		}
	}
};

template <int N>
std::array<float, N> MakeRoulette(const std::array<gene, N>& geneList) {
	int sum_value = 0;
	for (const auto& it : geneList) {
		sum_value += it.value();
	}

	std::array<float, N> ret;
	for (int i = 0;i < N;i++) {
		ret.at(i) = float(geneList.at(i).value()) / sum_value;
	}
}

template<int N>
int SelectRoulette(float ball, const std::array<float, N>& roulette) {
	for (int i = 0; i < N; i++) {
		if (ball < roulette.at(i)) {

		}
	}
}

int main() {
	std::array<gene, 30> geneList;
	constexpr int kMaxGeneration = 50;
	for (int i = 0; i < kMaxGeneration; i++) {
		//交叉

		//突然変異
		for (auto& it : geneList) {
			it.mutation();
		}

		//選択

		
	}
}
