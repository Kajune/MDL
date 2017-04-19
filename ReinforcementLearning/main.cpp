#include <iostream>
#include <array>
#include <random>

static constexpr int N = 30;
static constexpr int kMaxWeight = N * 100 / 2;
//荷物の価値と重さ
static constexpr std::array<std::pair<int, int>, N> baggage = {
	std::make_pair(65, 27),
	std::make_pair(39, 82),
	std::make_pair(9, 85),
	std::make_pair(72, 71),
	std::make_pair(87, 91),
	std::make_pair(91, 28),
	std::make_pair(34, 92),
	std::make_pair(2, 4),
	std::make_pair(21, 43),
	std::make_pair(26, 12),
	std::make_pair(73, 12),
	std::make_pair(84, 34),
	std::make_pair(42, 75),
	std::make_pair(12, 45),
	std::make_pair(2, 1),
	std::make_pair(7, 23),
	std::make_pair(65, 23),
	std::make_pair(52, 53),
	std::make_pair(12, 54),
	std::make_pair(64, 45),
	std::make_pair(94, 95),
	std::make_pair(12, 62),
	std::make_pair(24, 74),
	std::make_pair(34, 67),
	std::make_pair(13, 76),
	std::make_pair(64, 23),
	std::make_pair(74, 45),
	std::make_pair(16, 78),
	std::make_pair(94, 23),
	std::make_pair(37, 56),
};

std::random_device rd;
std::mt19937 mt(rd());
std::uniform_int_distribution<> dist_bool(0, 1);
std::uniform_int_distribution<> dist_int(0, N - 1);
std::uniform_int_distribution<> dist_mut(0, 100);
std::uniform_real_distribution<float> dist_real(0.0f, 1.0f);

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

	void print() const {
		for (const auto& it : m_gene) {
			std::cout << it;
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
	return ret;
}

template<int N>
int SelectRoulette(float ball, const std::array<float, N>& roulette) {
	for (int i = 0; i < N; i++) {
		if (ball < roulette.at(i)) {
			return i;
		}
		ball -= roulette.at(i);
	}
	return N - 1;
}

int main() {
	std::array<gene, 30> geneList;
	constexpr int kMaxGeneration = 1000;
	for (int i = 0; i < kMaxGeneration; i++) {
		//交叉
		auto roulette = MakeRoulette(geneList);
		std::array<gene, 30> newgeneList;
		for (int j = 0; j < N; j++) {
			int lhs = 0, rhs = 0;
			do {
				lhs = SelectRoulette(dist_real(mt), roulette);
				rhs = SelectRoulette(dist_real(mt), roulette);
			} while (lhs == rhs);
			newgeneList.at(j) = gene::crossing(geneList.at(lhs), geneList.at(rhs));
		}

		//突然変異
		for (auto& it : newgeneList) {
			it.mutation();
		}

		//選択
		roulette = MakeRoulette(newgeneList);
		for (int j = 0; j < N; j++) {
			geneList.at(j) = geneList.at(SelectRoulette(dist_real(mt), roulette));
		}

		//結果表示
		auto best = geneList.begin();
		for (auto it = geneList.begin(); it != geneList.end(); it++) {
			if (it->value() > best->value()) {
				best = it;
			}
		}

		std::cout << "第" << i + 1 << "世代" << std::endl;
		std::cout << "遺伝子: [";
		best->print();
		std::cout << "]" << std::endl;
		std::cout << "最適解: " << best->value() << std::endl << std::endl;
	}
}
