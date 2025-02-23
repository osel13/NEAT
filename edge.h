#pragma once

#include <utility>

namespace neural_network {
	class EdgeId {
		int from_id;
		int to_id;
		friend class NeatNetwork;
		friend struct EdgeHasher;
	public:
		EdgeId(int from_id, int to_id);
		bool operator<(const EdgeId& other) const;
		bool operator==(const EdgeId& other) const;
		std::size_t operator()() const;
	};

	class Edge {
		EdgeId id;
		int innovation_id;
		float weight;
		bool is_disabled;
		friend class NeatNetwork;
		friend struct EdgeHasher;
	public:
		Edge(unsigned from_id, unsigned to_id, unsigned innovation_id);
		bool operator==(const Edge& other) const;
		std::size_t operator()() const;
		void set_disabled(const bool value = true);
		float evaluate(const float input) const;
		Edge crossover(const Edge& other) const;
	};

	struct EdgeHasher {
		std::size_t operator()(const Edge& edge) const {
			return EdgeHasher{}(edge.id);
		}

		std::size_t operator()(const EdgeId& edge_id) const {
			return std::hash<int>{}(edge_id.from_id) ^ std::hash<int>{}(edge_id.to_id);
		}
	};
} // namespace neural_network