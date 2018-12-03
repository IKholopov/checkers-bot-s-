#ifndef FINAL
#define main _main
#define g_main main
#endif

#include <iostream>
#include <random>
#include <chrono>
#include <gtest/gtest.h>
#include <CheckerMCST.h>

TEST( GameState, InitDumpTest ) {
    GameState state;
    state.Dump(std::cout);
}

TEST( GameState, DISABLED_CustomDumpTest ) {
    GameState state = GameState::CreateEmpty();
    state.Cell(0, 0) = CellStatus::BlackQueen;
    state.Cell(1, 1) = CellStatus::White;
    state.Dump(std::cout);
}

TEST( GameState, DISABLED_KillerMovesRegular ) {
    {
        GameState state = GameState::CreateEmpty();
        state.Cell(2, 2) = CellStatus::Black;
        state.Cell(2, 4) = CellStatus::Black;
        state.Cell(3, 3) = CellStatus::White;
        state.Cell(4, 4) = CellStatus::White;
        state.Cell(4, 2) = CellStatus::Black;
        state.Dump(std::cout);
        std::cout << "-->" << std::endl;
        auto children = state.KillingMovesForRegular(3, 3);
        for(const auto& child : children) {
            child->Dump(std::cout);
        }
        std::cout << "------------------------------------" << std::endl;
    }
    {
        GameState state = GameState::CreateEmpty();
        state.Cell(1, 1) = CellStatus::Black;
        state.Cell(1, 3) = CellStatus::Black;
        state.Cell(1, 5) = CellStatus::Black;
        state.Cell(2, 2) = CellStatus::White;
        state.Cell(3, 3) = CellStatus::White;
        state.Cell(3, 1) = CellStatus::Black;
        state.Cell(3, 5) = CellStatus::Black;
        state.Cell(5, 5) = CellStatus::Black;
        state.Dump(std::cout);
        std::cout << "-->" << std::endl;
        auto children = state.KillingMovesForRegular(2, 2);
        for(const auto& child : children) {
            child->Dump(std::cout);
        }
        std::cout << "------------------------------------" << std::endl;
    }
    {
        GameState state = GameState::CreateEmpty();
        state.Cell(2, 2) = CellStatus::White;
        state.Cell(2, 4) = CellStatus::White;
        state.Cell(2, 6) = CellStatus::White;
        state.Cell(3, 3) = CellStatus::Black;
        state.Cell(4, 4) = CellStatus::Black;
        state.Cell(4, 2) = CellStatus::White;
        state.Cell(4, 6) = CellStatus::White;
        state.Cell(6, 6) = CellStatus::White;
        state.Dump(std::cout);
        std::cout << "-->" << std::endl;
        auto children = state.KillingMovesForRegular(3, 3);
        for(const auto& child : children) {
            child->Dump(std::cout);
        }
        std::cout << "------------------------------------" << std::endl;
    }
}

TEST( GameState, DISABLED_KillerMovesQueen ) {
    {
        GameState state = GameState::CreateEmpty();
        state.Cell(0, 0) = CellStatus::BlackQueen;
        state.Cell(3, 3) = CellStatus::White;
        state.Cell(4, 4) = CellStatus::White;
        state.Dump(std::cout);
        std::cout << "-->" << std::endl;
        auto children = state.KillingMovesForQueen(0, 0);
        for(const auto& child : children) {
            child->Dump(std::cout);
        }
        std::cout << "------------------------------------" << std::endl;
    }
    {
        GameState state = GameState::CreateEmpty();
        state.Cell(0, 0) = CellStatus::BlackQueen;
        state.Cell(3, 3) = CellStatus::White;
        state.Cell(2, 6) = CellStatus::White;
        state.Dump(std::cout);
        std::cout << "-->" << std::endl;
        auto children = state.KillingMovesForQueen(0, 0);
        for(const auto& child : children) {
            child->Dump(std::cout);
        }
        std::cout << "------------------------------------" << std::endl;
    }
    {
        GameState state = GameState::CreateEmpty();
        state.Cell(0, 0) = CellStatus::BlackQueen;
        state.Cell(3, 3) = CellStatus::White;
        state.Cell(3, 7) = CellStatus::White;
        state.Dump(std::cout);
        std::cout << "-->" << std::endl;
        auto children = state.KillingMovesForQueen(0, 0);
        for(const auto& child : children) {
            child->Dump(std::cout);
        }
        std::cout << "------------------------------------" << std::endl;
    }
}

TEST( GameState, DISABLED_Expand ) {
    {
        GameState state = GameState::CreateEmpty();
        state.Cell(2, 2) = CellStatus::Black;
        state.Cell(2, 4) = CellStatus::Black;
        state.Cell(4, 4) = CellStatus::White;
        state.Cell(4, 2) = CellStatus::Black;
        state.CurrentTeam = Team::White;
        state.Dump(std::cout);
        std::cout << "-->" << std::endl;
        auto children = state.Expand();
        for(const auto& child : children) {
            child->Dump(std::cout);
        }
        std::cout << "------------------------------------" << std::endl;
    }
    {
        GameState state = GameState::CreateEmpty();
        state.Cell(2, 2) = CellStatus::Black;
        state.Cell(2, 4) = CellStatus::Black;
        state.Cell(4, 4) = CellStatus::White;
        state.Cell(4, 2) = CellStatus::Black;
        state.Cell(3, 5) = CellStatus::Black;
        state.CurrentTeam = Team::White;
        state.Dump(std::cout);
        std::cout << "-->" << std::endl;
        auto children = state.Expand();
        for(const auto& child : children) {
            child->Dump(std::cout);
        }
        std::cout << "------------------------------------" << std::endl;
    }
    {
        GameState state = GameState::CreateEmpty();
        state.Cell(0, 2) = CellStatus::WhiteQueen;
        state.Cell(3, 7) = CellStatus::Black;
        state.Cell(6, 6) = CellStatus::Black;
        state.Cell(7, 3) = CellStatus::BlackQueen;
        state.CurrentTeam = Team::Black;
        state.Dump(std::cout);
        std::cout << "-->" << std::endl;
        auto children = state.Expand();
        ASSERT_FALSE(children.empty());
        for(const auto& child : children) {
            child->Dump(std::cout);
        }
        std::cout << "------------------------------------" << std::endl;
    }
}

TEST( GameState, RandomGame ) {
    std::mt19937 engine{56};
    std::cerr << "Random" << std::endl;
    int black_wins = 0;
    int white_wins = 0;
    for (int i = 0; i < 100; ++i) {
        RandomPlayer black_player{engine};
        RandomPlayer white_player{engine};

        auto state_ptr = std::make_unique<GameState>();
        int max_steps = 1000;
        int step = 0;
        IPlayer* current = &white_player;
        IPlayer* next= &black_player;
        for(;!state_ptr->IsTerminal() && !state_ptr->Expand().empty() && step < max_steps; ++step) {
            state_ptr = current->GetMove(state_ptr.get());
            next->OpponentMove(state_ptr.get());
            std::swap(current, next);
        }
        if (step < max_steps) {
            if (state_ptr->CurrentTeam == Team::White) {
                ++black_wins;
            } else {
                ++white_wins;
            }
        }
    }
    std::cerr << static_cast<float>(white_wins) / 100.0f << " ";
    std::cerr << static_cast<float>(black_wins) / 100.0f << std::endl;
}

TEST( MCST, SimpleGame ) {
    std::mt19937 engine{56};
    {
        std::cerr << "Simple" << std::endl;
        for (int steps = 4; steps <= 2048; steps *= 2 ) {
            int black_wins = 0;
            int white_wins = 0;
            for (int i = 0; i < 100; ++i) {
                MCSTPlayer<SimpleCheckerMCST<Team::Black, std::mt19937>> black_player{engine,
                            std::make_unique<GameState>(Team::White), steps};
                RandomPlayer white_player{engine};

                auto state_ptr = std::make_unique<GameState>();
                int max_steps = 1000;
                int step = 0;
                IPlayer* current = &white_player;
                IPlayer* next= &black_player;
                for(;!state_ptr->IsTerminal() && !state_ptr->Expand().empty() && step < max_steps; ++step) {
                    state_ptr = current->GetMove(state_ptr.get());
                    next->OpponentMove(state_ptr.get());
                    std::swap(current, next);
                }
                if (step < max_steps) {
                    if (state_ptr->CurrentTeam == Team::White) {
                        ++black_wins;
                    } else {
                        ++white_wins;
                    }
                }
            }
            std::cerr << steps << ": " << static_cast<float>(white_wins) / 100.0f << " ";
            std::cerr << static_cast<float>(black_wins) / 100.0f << std::endl;
        }
    }
}

TEST( MCST, WeightedGame ) {
    std::mt19937 engine{56};
    {
        std::cerr << "Weighted" << std::endl;
        for (int steps = 4; steps <= 2048; steps *= 2 ) {
            int black_wins = 0;
            int white_wins = 0;
            for (int i = 0; i < 100; ++i) {
                MCSTPlayer<WeightedCheckerMCST<Team::Black, std::mt19937>> black_player{engine,
                            std::make_unique<GameState>(Team::White), steps};
                RandomPlayer white_player{engine};

                auto state_ptr = std::make_unique<GameState>();
                int max_steps = 1000;
                int step = 0;
                IPlayer* current = &white_player;
                IPlayer* next= &black_player;
                for(;!state_ptr->IsTerminal() && !state_ptr->Expand().empty() && step < max_steps; ++step) {
                    state_ptr = current->GetMove(state_ptr.get());
                    next->OpponentMove(state_ptr.get());
                    std::swap(current, next);
                }
                if (step < max_steps) {
                    if (state_ptr->CurrentTeam == Team::White) {
                        ++black_wins;
                    } else {
                        ++white_wins;
                    }
                }
            }
            std::cerr << steps << ": " << static_cast<float>(white_wins) / 100.0f << " ";
            std::cerr << static_cast<float>(black_wins) / 100.0f << std::endl;
        }
    }
}

TEST( MCST, Versus ) {
    std::mt19937 engine{56};
    {
        std::cerr << "Versus" << std::endl;
        for (int steps = 4; steps <= 2048; steps *= 2 ) {
            int black_wins = 0;
            int white_wins = 0;
            for (int i = 0; i < 100; ++i) {
                MCSTPlayer<WeightedCheckerMCST<Team::Black, std::mt19937>> black_player{engine,
                            std::make_unique<GameState>(Team::White), steps};
                MCSTPlayer<SimpleCheckerMCST<Team::Black, std::mt19937>> white_player{engine,
                            std::make_unique<GameState>(Team::White), steps};

                auto state_ptr = std::make_unique<GameState>();
                int max_steps = 1000;
                int step = 0;
                IPlayer* current = &white_player;
                IPlayer* next= &black_player;
                for(;!state_ptr->IsTerminal() && !state_ptr->Expand().empty() && step < max_steps; ++step) {
                    state_ptr = current->GetMove(state_ptr.get());
                    next->OpponentMove(state_ptr.get());
                    std::swap(current, next);
                }
                if (step < max_steps) {
                    if (state_ptr->CurrentTeam == Team::White) {
                        ++black_wins;
                    } else {
                        ++white_wins;
                    }
                }
            }
            std::cerr << steps << ": " << static_cast<float>(white_wins) / 100.0f << " ";
            std::cerr << static_cast<float>(black_wins) / 100.0f << std::endl;
        }
    }
}

int g_main(int argc, char** argv) {

    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
