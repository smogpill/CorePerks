// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#include "precompiled.h"
#include "core_perks/patterns/state_machine.h"

namespace cp
{
	StateMachine::StateMachine(StateID id)
		: id_(id)
	{
	}

	StateMachine::~StateMachine()
	{
		if (current_state_)
			current_state_->on_exit(nullptr, StateMessage());
	}

	void StateMachine::add_state(StateMachine& state)
	{
		CP_ASSERT(get_state(state.get_id()) == nullptr);
		states_.push_back(RefPtr(&state));
		state.parent_ = this;
	}

	void StateMachine::update()
	{
		on_update();
		if (current_state_)
			current_state_->update();
	}

	void StateMachine::handle_event(const StateEvent& event)
	{
		on_event(event);
		if (current_state_)
			current_state_->on_event(event);
	}

	void StateMachine::set_current_state(StateID id, const StateMessage& message)
	{
		if (current_state_ && current_state_->get_id() == id)
			return;

		StateMachine* previous_state = current_state_;
		StateMachine* next_state = nullptr;
		for (RefPtr<StateMachine>& state : states_)
		{
			if (state->id_ == id)
			{
				next_state = state.get();
				break;
			}
		}

		if (current_state_)
			current_state_->on_exit(next_state, message);
		current_state_ = next_state;
		if (current_state_)
			current_state_->on_enter(previous_state, message);
	}

	auto StateMachine::get_state(StateID id) const -> StateMachine*
	{
		auto it = std::ranges::find_if(states_, [id](const RefPtr<StateMachine>& state) { return state->id_ == id; });
		return it != states_.end() ? it->get() : nullptr;
	}
}
