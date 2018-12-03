#pragma once

#include <memory>
#include <vector>
#include <cmath>
#include <random>
#include <assert.h>

template <typename TGenerator, typename TState, typename TSimulationPolicy>
class MCSearchTree {
public:
    typedef TGenerator Generator;

    struct MCSTNode {
    public:
        MCSTNode(TGenerator& generator, std::unique_ptr<TState>&& _state, MCSTNode* _parent)
            : state(std::move(_state)), parent(_parent),
              random(generator), childStates(state->Expand()) {
            expandedNodes.resize(childStates.size(), false);
            childNodes.resize(childStates.size());
        }

        void MakeRoot() {
            parent = nullptr;
        }
        void Expand();
        void SimulateAndPropagate();
        bool FullyExpanded() const {
            return childStates.size() == expanded;
        }
        std::unique_ptr<MCSTNode> GetChild(const TState& chosen_state) {
            assert(FullyExpanded());
            for (auto& child : childNodes) {
                if(*child->state == chosen_state) {
                    return std::move(child);
                }
            }
            assert(false);
        }
        MCSTNode* Select(bool recurse = true) {
            assert(FullyExpanded());
            assert(!childStates.empty());
            double best_payoff = -9000.0;
            MCSTNode* best = nullptr;
            for (auto& child : childNodes) {
                double score = static_cast<double>(child->payoff) / child->total +
                        std::sqrt(2.0 * std::log(total)/child->total);
                if (score > best_payoff) {
                    best_payoff = score;
                    best = child.get();
                }
            }
            assert(best != nullptr);
            if (best->FullyExpanded() && recurse && !best->childStates.empty()) {
                return best->Select();
            }
            return best;
        }

        const TState* State() const {
            return state.get();
        }

    private:
        std::unique_ptr<TState> state;
        int total = 0;
        int payoff = 0;
        MCSTNode* parent = nullptr;
        TGenerator& random;
        std::vector<std::unique_ptr<TState>> childStates;
        std::vector<std::unique_ptr<MCSTNode>> childNodes;
        std::vector<bool> expandedNodes;
        int expanded = 0;
    };

    explicit MCSearchTree(TGenerator& _generator, std::unique_ptr<TState>&& _state, int _max_iterations = 100)
                            : root(std::make_unique<MCSTNode>(_generator, std::move(_state), nullptr)),
                              generator(_generator),
                              max_iterations(_max_iterations) {
    }

    const TState* GetNextMove() {
        for(int i = 0; i < max_iterations || !root->FullyExpanded(); ++i) {
            Select()->Expand();
        }
        const TState* state = root->Select(false)->State();
        root = std::move(root->GetChild(*state));
        root->MakeRoot();
        return root->State();
    }
    void ApplyMove(const TState& state) {
        while(!root->FullyExpanded()) {
            Select()->Expand();
        }
        root = std::move(root->GetChild(state));
        root->MakeRoot();
    }

private:
    std::unique_ptr<MCSTNode> root;
    TGenerator& generator;
    int max_iterations = 0;

    MCSTNode* Select() {
        return root->FullyExpanded() ? root->Select() : root.get();
    }
};

template<typename TGenerator, typename TState, typename TSimulationPolicy>
void MCSearchTree<TGenerator, TState, TSimulationPolicy>::MCSTNode::Expand()
{
    if (childStates.empty()) {
        SimulateAndPropagate();
        return;
    }
    assert(!FullyExpanded());
    int index = std::uniform_int_distribution<int>{0, childStates.size() - expanded - 1}(random);
    for (int i = 0; i < expandedNodes.size(); ++i) {
        if (expandedNodes[i]) {
            continue;
        }
        if (index == 0) {
            childNodes[i] = std::make_unique<MCSTNode>(random, std::move(childStates[i]), this);
            expandedNodes[i] = true;
            childNodes[i]->SimulateAndPropagate();
            ++expanded;
            return;
        }
        --index;
    }
    assert(false);
}

template<typename TGenerator, typename TState, typename TSimulationPolicy>
void MCSearchTree<TGenerator, TState, TSimulationPolicy>::MCSTNode::SimulateAndPropagate()
{
    payoff = TSimulationPolicy{random}.Simulate(state.get());
    total = 1;
    for(MCSTNode* node = parent; node != nullptr; node = node->parent) {
        node->payoff += payoff;
        node->total += 1;
    }
}
