import sys

sys.path.insert(0, "obj")

import duel_env

environment = duel_env.DuelEnvironment(888)

state = environment.get_state()

print("Episode time:", state.episode_time)
print("Player 1 position:", state.player1.position.x, state.player1.position.y,state.player1.position.z)