#pragma once

#include <MCST.h>
#include <GameState.h>

template <Team team, typename TGenerator>
class SimpleCheckerPolicy {
public:
    explicit SimpleCheckerPolicy(TGenerator& generator) : random(generator) {
    }
    static_assert (team != Team::None, "team parameter could not be none");

    int Simulate(const GameState* state) {
        assert(state != nullptr);
        auto children = state->Expand();
        if (state->IsTerminal() || children.empty()) {
            return state->CurrentTeam != team ? 1 : 0;
        }
        auto distr = std::uniform_int_distribution<std::size_t>{0, children.size() -1};
        auto child = std::move(children[distr(random)]);
        while (!state->IsTerminal()) {
            children = child->Expand();
            if (children.empty()) {
                return child->CurrentTeam != team ? 1 : 0;
            }
            auto distr = std::uniform_int_distribution<std::size_t>{0, children.size() -1};
            child = std::move(children[distr(random)]);
        }
        return child->CurrentTeam != team ? 1 : 0;
    }

private:
    TGenerator& random;
};

template <Team team, typename TGenerator>
class WeightedCheckerPolicy {
public:
    explicit WeightedCheckerPolicy(TGenerator& generator) : random(generator) {
    }
    static_assert (team != Team::None, "team parameter could not be none");

    int Simulate(const GameState* state) {
        assert(state != nullptr);
        auto children = state->Expand();
        if (state->IsTerminal() || children.empty()) {
            return Score(state);
        }
        auto distr = std::uniform_int_distribution<std::size_t>{0, children.size() -1};
        auto child = std::move(children[distr(random)]);
        while (!state->IsTerminal()) {
            children = child->Expand();
            if (children.empty()) {
                return Score(child.get());
            }
            auto distr = std::uniform_int_distribution<std::size_t>{0, children.size() -1};
            child = std::move(children[distr(random)]);
        }
        return Score(child.get());
    }

private:
    TGenerator& random;
    int Score(const GameState* state) {
        if (state->CurrentTeam == team) {
            return 0;
        }
        int score = 0;
        for (auto cell : state->State) {
            if (cell == team) {
                score += 10;
                if (IsQueen(cell)) {
                    score += 100;
                }
            }
        }
        return score;
    }
};

template <Team team, typename TGenerator>
using SimpleCheckerMCST = MCSearchTree<TGenerator, GameState, SimpleCheckerPolicy<team, TGenerator>>;
template <Team team, typename TGenerator>
using WeightedCheckerMCST = MCSearchTree<TGenerator, GameState, WeightedCheckerPolicy<team, TGenerator>>;

class IPlayer {
public:
    virtual std::unique_ptr<GameState> GetMove(const GameState* current) = 0;
    virtual void OpponentMove(const GameState* current) = 0;
};

template <typename TGenerator>
class RandomPlayer : public IPlayer {
public:
    explicit RandomPlayer(TGenerator& generator) : random(generator) {
    }

    // IPlayer interface
    virtual std::unique_ptr<GameState> GetMove(const GameState* current) override {
        auto children = current->Expand();
        auto distr = std::uniform_int_distribution<std::size_t>{0, children.size() -1};
        return std::move(children[distr(random)]);
    }
    virtual void OpponentMove(const GameState*) override {
    }

private:
    TGenerator& random;
};

template <typename MCST>
class MCSTPlayer : public IPlayer {
public:
    MCSTPlayer(typename MCST::Generator& generator, std::unique_ptr<GameState>&& _state, int _max_iterations = 100)
        : tree(generator, std::move(_state), _max_iterations) {
    }

    // IPlayer interface
    virtual std::unique_ptr<GameState> GetMove(const GameState*) override {
        return std::make_unique<GameState>(*tree.GetNextMove());
    }
    virtual void OpponentMove(const GameState* opponent) override {
        tree.ApplyMove(*opponent);
    }

private:
    MCST tree;
};
