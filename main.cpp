#include <iostream>
#include <GameState.h>

int main() {
    /*
    while (std::cin.peek() != EOF) {
        std::vector<unsigned char> values;
        for (int i = 0; i < 16; ++i) {
            unsigned int val;
            std::cin >> val;
            values.push_back(val);
        }
        while(std::cin.peek() != EOF && std::cin.peek() != ':') {
            std::cin.get();
        }
        if(std::cin.peek() == EOF) {
            return 0;
        }
        std::cout << static_cast<char>(std::cin.get()) << " ";
        while(std::cin.peek() != EOF && std::cin.peek() != ':') {
            std::cin.get();
        }
        if(std::cin.peek() == EOF) {
            return 0;
        }
        std::cin.get();
        int expected;
        std::cin >> expected;
        PuzzleState state{values};
        BestFirstBeamSearch<PuzzleState, PathScore, HeuristicScore> search(std::move(state), 4096 );
        search.Iterate();
        int result = search.Terminal()->GetPath().size();
        std::cout << expected << ":" << result << std::endl;
    }
    return 0;
    */
}
