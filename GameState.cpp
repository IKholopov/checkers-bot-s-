#include <GameState.h>

GameState GameState::CreateEmpty()
{
    GameState state;
    for (int i = 0; i < BoardSize; ++i) {
        for (int j = 0; j < BoardSize; ++j) {
            state.Cell(i, j) = CellStatus::None;
        }
    }
    return state;
}

std::vector<std::unique_ptr<GameState> > GameState::KillingMovesFor(Team team) const {
    std::vector<std::unique_ptr<GameState>> children;
    for (int i = 0; i < BoardSize; ++i) {
        for (int j = 0; j < BoardSize; ++j) {
            if (State[Index(i, j)] != team) {
                continue;
            }
            if (IsQueen(State[Index(i, j)])) {
                auto moves = KillingMovesForQueen(i, j);
                std::move(moves.begin(), moves.end(), std::back_inserter(children));
            } else {
                auto moves = KillingMovesForRegular(i, j);
                std::move(moves.begin(), moves.end(), std::back_inserter(children));
            }
        }
    }
    return children;
}

std::vector<std::unique_ptr<GameState> > GameState::NonKillingMovesFor(Team team) const {
    std::vector<std::unique_ptr<GameState>> children;
    for (int i = 0; i < BoardSize; ++i) {
        for (int j = 0; j < BoardSize; ++j) {
            if (State[Index(i, j)] != team) {
                continue;
            }
            if (IsQueen(State[Index(i, j)])) {
                auto moves = NonKillingMovesForQueen(i, j);
                std::move(moves.begin(), moves.end(), std::back_inserter(children));
            } else {
                auto moves = NonKillingMovesForRegular(i, j);
                std::move(moves.begin(), moves.end(), std::back_inserter(children));
            }
        }
    }
    return children;
}

std::vector<std::unique_ptr<GameState>> GameState::KillingMovesForRegular(int i, int j) const {
    const Team my_team = TeamOfCell(At(i, j));
    assert(my_team != Team::None);
    assert(!IsQueen(State[Index(i, j)]));
    std::vector<std::unique_ptr<GameState>> children;
    for (int dy = -1; dy < 2; dy += 2) {
        for (int dx = -1; dx < 2; dx += 2) {
            if (TeamOfCell(At(i + dy, j + dx)) == Opponent(my_team) &&
                    At(i + dy * 2, j + dx * 2) == CellStatus::None)
            {
                std::unique_ptr<GameState> child = std::make_unique<GameState>(this);
                child->Cell(i, j) = CellStatus::None;
                child->Cell(i + dy, j + dx) = CellStatus::None;
                child->Cell(i + 2 * dy, j + 2 * dx) = my_team == Team::White ? CellStatus::White
                                                                             : CellStatus::Black;
                auto grand_children = child->KillingMovesForRegular(i + 2 * dy, j + 2 * dx);
                if (grand_children.empty()) {
                    if ((my_team == Team::White && i + 2 * dy == 0) ||
                        (my_team == Team::Black && i + 2 * dy == BoardSize - 1 ))
                    {
                        child->Cell(i + 2 * dy, j + 2 * dx) = my_team == Team::White ? CellStatus::WhiteQueen
                                                                                     : CellStatus::BlackQueen;
                    }
                    children.emplace_back(std::move(child));
                    continue;
                }
                std::move(grand_children.begin(), grand_children.end(), std::back_inserter(children));
            }
        }
    }
    for (auto& child : children) {
        child->CurrentTeam = Opponent(my_team);
    }
    return children;
}

std::vector<std::unique_ptr<GameState> > GameState::NonKillingMovesForQueen(int i, int j) const {
    Team my_team = TeamOfCell(State[Index(i, j)]);
    assert(my_team != Team::None);
    assert(IsQueen(State[Index(i, j)]));
    std::vector<std::unique_ptr<GameState>> children;
    for (int dy = -1; dy < 2; dy += 2) {
        for (int dx = -1; dx < 2; dx += 2) {
            for (int a = 1;
                 At(i + dy * a, j + dx * a) == CellStatus::None;
                 a += 1)
            {
                std::unique_ptr<GameState> child = std::make_unique<GameState>(this);
                child->Cell(i, j) = CellStatus::None;
                child->Cell(i + dy * a, j + dx * a) = my_team == Team::White ? CellStatus::WhiteQueen
                                                                             : CellStatus::BlackQueen;
                child->CurrentTeam = Opponent(my_team);
                children.emplace_back(std::move(child));
            }
        }
    }
    return children;
}

std::vector<std::unique_ptr<GameState> > GameState::NonKillingMovesForRegular(int i, int j) const {
    Team my_team = TeamOfCell(State[Index(i, j)]);
    assert(my_team != Team::None);
    assert(!IsQueen(State[Index(i, j)]));
    std::vector<std::unique_ptr<GameState>> children;
    const int dy = my_team == Team::White ? -1 : 1;
    for (int dx = -1; dx < 2; dx += 2) {
        if (At(i + dy, j + dx) == CellStatus::None) {
            std::unique_ptr<GameState> child = std::make_unique<GameState>(this);
            child->Cell(i, j) = CellStatus::None;
            child->Cell(i + dy, j + dx) = my_team == Team::White ? CellStatus::White
                                                                 : CellStatus::Black;
            child->CurrentTeam = Opponent(my_team);
            if ((my_team == Team::White && i + dy == 0) ||
                (my_team == Team::Black && i +  dy == BoardSize - 1 ))
            {
                child->Cell(i + dy, j + dx) = my_team == Team::White ? CellStatus::WhiteQueen
                                                                     : CellStatus::BlackQueen;
            }
            children.emplace_back(std::move(child));
        }
    }
    return children;
}

std::vector<std::unique_ptr<GameState>> GameState::KillingMovesForQueen(int i, int j) const {
    Team my_team = TeamOfCell(State[Index(i, j)]);
    assert(my_team != Team::None);
    assert(IsQueen(State[Index(i, j)]));
    std::vector<std::unique_ptr<GameState>> children;
    for (int dy = -1; dy < 2; dy += 2) {
        for (int dx = -1; dx < 2; dx += 2) {
            for (int a = 1;
                 At(i + dy * (a + 1), j + dx * (a + 1)) != CellStatus::Forbidden;
                 a += 1)
            {
                if (TeamOfCell(At(i + dy * a, j + dx * a)) == Opponent(my_team) &&
                    At(i + dy * (a + 1), j + dx * (a + 1)) == CellStatus::None)
                {
                    std::unique_ptr<GameState> child = std::make_unique<GameState>(this);
                    child->Cell(i, j) = CellStatus::None;
                    child->Cell(i + dy * a, j + dx * a) = CellStatus::None;
                    child->Cell(i + dy* (a + 1), j + dx * (a + 1)) = my_team == Team::White ? CellStatus::WhiteQueen
                                                                                 : CellStatus::BlackQueen;
                    auto grand_children = child->KillingMovesForQueen(i + dy * (a + 1), j + dx * (a + 1));
                    if (grand_children.empty()) {
                        children.emplace_back(std::move(child));
                    } else {
                        std::move(grand_children.begin(), grand_children.end(), std::back_inserter(children));
                    }
                }
                if (At(i + dy * a, j + dx * a) != CellStatus::None) {
                    break;
                }
            }
        }
    }
    for (auto& child : children) {
        child->CurrentTeam = Opponent(my_team);
    }
    return children;
}

void GameState::Dump(std::ostream& stream) const {
    char c = 'A';
    for (int i = 0; i < BoardSize; ++i) {
        for (int j = 0; j < BoardSize; ++j) {
            int background_color = ( (i + j) % 2 == 0 ) ? 46 : 47;
            switch (At(i, j)) {
            case CellStatus::None:
                stream << "\033[0;31;"+std::to_string(background_color) + "m   \033[0m";
                break;
            case CellStatus::White:
                stream << "\033[1;33;"+std::to_string(background_color) + "m # \033[0m";
                break;
            case CellStatus::WhiteQueen:
                stream << "\033[1;33;"+std::to_string(background_color) + "m * \033[0m";
                break;
            case CellStatus::Black:
                stream << "\033[1;31;"+std::to_string(background_color) + "m # \033[0m";
                break;
            case CellStatus::BlackQueen:
                stream << "\033[1;31;"+std::to_string(background_color) + "m * \033[0m";
                break;
            case CellStatus::Forbidden:
                assert(false);
            }
        }
        stream << BoardSize - i;
        stream << std::endl;
    }
    for (int j = 0; j < BoardSize; ++j) {
        stream << " " + std::string(1, c++) + " ";
    }
    stream << std::endl;
    stream << std::endl;
}
