#pragma once

#include <vector>
#include <memory>
#include <iostream>
#include <algorithm>
#include <assert.h>

static const int BoardSize = 10;
static_assert (BoardSize % 2 == 0, "BoardSize should be even");
enum class CellStatus {
    None, Black, White, BlackQueen, WhiteQueen, Forbidden
};

enum class Team {
    None, Black, White
};

static Team TeamOfCell(CellStatus status){
    if (status == CellStatus::Black || status == CellStatus::BlackQueen) {
        return Team::Black;
    }
    if (status == CellStatus::White || status == CellStatus::WhiteQueen) {
        return Team::White;
    }
    return Team::None;
}

static bool operator==(Team team, CellStatus cell) {
    return (team == Team::Black && (cell == CellStatus::Black || cell == CellStatus::BlackQueen))||
            (team == Team::White && (cell == CellStatus::White || cell == CellStatus::WhiteQueen));
}

static bool operator!=(Team team, CellStatus cell) {
    return !(team == cell);
}

static bool operator!=(CellStatus cell, Team team) {
    return !(team == cell);
}

static bool operator==(CellStatus cell, Team team) {
    return team == cell;
}

static Team Opponent(Team team) {
    assert(team != Team::None);
    if(team == Team::Black) {
        return Team::White;
    }
    return Team::Black;
}

static bool IsQueen(CellStatus cell) {
    if (cell == CellStatus::BlackQueen || cell == CellStatus::WhiteQueen) {
        return true;
    }
    assert(cell == CellStatus::Black || cell == CellStatus::White);
    return false;
}

struct GameState {
    std::array<CellStatus, BoardSize * BoardSize> State;
    const GameState* Parent = nullptr;
    Team CurrentTeam;


    GameState(Team team = Team::White) : CurrentTeam(team) {
        for (int i = 0; i < BoardSize; ++i) {
            for (int j = 0; j < BoardSize; ++j) {
                auto& pos = State[Index(i, j)];
                if (i < BoardSize / 2 - 1) {
                    if (j % 2 == i % 2) {
                        pos = CellStatus::Black;
                        continue;
                    }
                } else if (i > BoardSize / 2) {
                    if (j % 2 == i % 2) {
                        pos = CellStatus::White;
                        continue;
                    }
                }
                pos = CellStatus::None;
            }
        }
    }
    GameState(const GameState* parent) : State(parent->State), Parent(parent), CurrentTeam(Opponent(parent->CurrentTeam)) {
    }

    GameState(const GameState&) = default;
    GameState(GameState&& other) {
        std::swap(other.State, State);
        CurrentTeam = other.CurrentTeam;
    }

    static GameState CreateEmpty();

    bool Equal(const GameState& other) const {
        return State == other.State;
    }

    std::size_t Hash() const {
        std::size_t val = 16;
        for(auto& v : State) {
            unsigned int i = static_cast<unsigned int>(typename std::underlying_type<CellStatus>::type(v));
            val ^= i + 0x9e3779b9 + (val << 6) + (val >> 2);
        }
        return val;
    }

    static unsigned int Index(int i, int j) {
        return static_cast<unsigned int>(i * BoardSize + j);
    }

    bool IsTerminal() const {
        bool whitePresent = false;
        bool blackPresent = false;
        for(auto& cell : State) {
            if(TeamOfCell(cell) == Team::Black) {
                blackPresent = true;
            } else if (TeamOfCell(cell) == Team::White) {
                whitePresent = true;
            }
            if(blackPresent && whitePresent) {
                return false;
            }
        }
        return true;
    }

    CellStatus At(int i, int j) const {
        if(i < 0 || i >= BoardSize || j < 0 || j >= BoardSize) {
            return CellStatus::Forbidden;
        }
        return State[Index(i, j)];
    }

    CellStatus& Cell(int i, int j) {
        if(i < 0 || i >= BoardSize || j < 0 || j >= BoardSize) {
            assert(false);
        }
        return State[Index(i, j)];
    }


    std::vector<std::unique_ptr<GameState>> KillingMovesFor(Team team) const;
    std::vector<std::unique_ptr<GameState>> NonKillingMovesFor(Team team) const;

    std::vector<std::unique_ptr<GameState>> KillingMovesForQueen(int i, int j) const;
    std::vector<std::unique_ptr<GameState>> KillingMovesForRegular(int i, int j) const;
    std::vector<std::unique_ptr<GameState>> NonKillingMovesForQueen(int i, int j) const;
    std::vector<std::unique_ptr<GameState>> NonKillingMovesForRegular(int i, int j) const;

    std::vector<std::unique_ptr<GameState>> Expand() const {
        auto killer_moves = KillingMovesFor(CurrentTeam);
        if (!killer_moves.empty()) {
            return killer_moves;
        }
        return NonKillingMovesFor(CurrentTeam);
    }

    void Dump(std::ostream& stream) const;

    bool operator==(const GameState& other) {
        return other.State == State && other.CurrentTeam == CurrentTeam;
    }

private:
    static const auto forbidden = CellStatus::Forbidden;
};
