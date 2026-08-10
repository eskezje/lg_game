#include <pybind11/pybind11.h>

#include "duelenv.hpp"

namespace py = pybind11;

PYBIND11_MODULE(duel_env, module)
{
    module.doc() = "Python bindings for DuelEnvironment";

    auto playerInputBinding = py::class_<PlayerInput>(module, "PlayerInput");
    playerInputBinding.def(py::init<>());
    playerInputBinding.def_readwrite("forward", &PlayerInput::forward);
    playerInputBinding.def_readwrite("sideward", &PlayerInput::sideward);

    auto vector2Binding = py::class_<Vector2>(module, "Vector2");
    vector2Binding.def(py::init<>());
    vector2Binding.def_readwrite("x", &Vector2::x);
    vector2Binding.def_readwrite("y", &Vector2::y);

    auto vector3Binding = py::class_<Vector3>(module, "Vector3");
    vector3Binding.def(py::init<>());
    vector3Binding.def_readwrite("x", &Vector3::x);
    vector3Binding.def_readwrite("y", &Vector3::y);
    vector3Binding.def_readwrite("z", &Vector3::z);

    auto playerStateBinding = py::class_<PlayerState>(module, "PlayerState");
    playerStateBinding.def(py::init<>());
    playerStateBinding.def_readwrite("position", &PlayerState::position);
    playerStateBinding.def_readwrite("velocity", &PlayerState::velocity);
    playerStateBinding.def_readwrite("direction", &PlayerState::direction);
    playerStateBinding.def_readwrite("health", &PlayerState::health);
    playerStateBinding.def_readwrite("max_health", &PlayerState::maxHealth);
    playerStateBinding.def_readwrite("gun_cooldown", &PlayerState::gunCooldown);

    auto duelActionBinding = py::class_<DuelAction>(module, "DuelAction");
    duelActionBinding.def(py::init<>());
    duelActionBinding.def_readwrite("input", &DuelAction::input);
    duelActionBinding.def_readwrite("mouse_delta", &DuelAction::mouseDelta);
    duelActionBinding.def_readwrite("fire", &DuelAction::fire);
    
    auto duelStateBinding = py::class_<DuelState>(module, "DuelState");
    duelStateBinding.def(py::init<>());
    duelStateBinding.def_readwrite("player1", &DuelState::player1);
    duelStateBinding.def_readwrite("player2", &DuelState::player2);
    duelStateBinding.def_readwrite("episode_time", &DuelState::episodeTime);
    duelStateBinding.def_readwrite("episode_duration", &DuelState::episodeDuration);
    duelStateBinding.def_readwrite("arena_half_size", &DuelState::arenaHalfSize);

    auto duelStepResultBinding = py::class_<DuelStepResult>(module, "DuelStepResult");
    duelStepResultBinding.def(py::init<>());
    duelStepResultBinding.def_readwrite("player1_reward", &DuelStepResult::player1Reward);
    duelStepResultBinding.def_readwrite("player2_reward", &DuelStepResult::player2Reward);
    duelStepResultBinding.def_readwrite("terminated", &DuelStepResult::terminated);
    duelStepResultBinding.def_readwrite("truncated", &DuelStepResult::truncated);

    auto duelEnvironmentBinding = py::class_<DuelEnvironment>(module, "DuelEnvironment");
    duelEnvironmentBinding.def(py::init<unsigned int>());
    duelEnvironmentBinding.def("get_state", &DuelEnvironment::GetState);

    auto seededReset = py::overload_cast<unsigned int>(&DuelEnvironment::Reset);
    duelEnvironmentBinding.def("reset", seededReset);
    duelEnvironmentBinding.def("step", &DuelEnvironment::Step);
}
