#include <iostream>

#include "duelenv.hpp"

int main()
{
    DuelEnvironment environment(888);

    float actionTime = 1.0f / 25.0f;
    int episodeCount = 10;

    for (int episode = 0; episode < episodeCount; episode++)
    {
        environment.Reset(888 + episode);
        float player1TotalReward = 0.0f;
        float player2TotalReward = 0.0f;
        int stepCount = 0;
        while (true)
        {
            DuelAction player1Action{};
            DuelAction player2Action{};

            DuelStepResult result = environment.Step(player1Action, player2Action, actionTime);
            player1TotalReward += result.player1Reward;
            player2TotalReward += result.player2Reward;
            stepCount += 1;
            if (result.terminated || result.truncated)
            {
                break;
            }
            
        }
        std::cout << "Episode" << episode << ", steps: " << stepCount << ", P1 reward: "  << player1TotalReward << ", P2 reward: " << player2TotalReward << "\n";

        return 0;
        
    }
    
}