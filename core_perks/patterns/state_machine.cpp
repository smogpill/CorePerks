// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#include "precompiled.h"
#include "core_perks/patterns/state_machine.h"

namespace cp
{
	StateMachine::StateMachine(StateID id)
		: _id(id)
	{
	}

	StateMachine::~StateMachine()
	{
		if (_current_state)
			_current_state->on_exit(nullptr, StateMessage());
	}

	void StateMachine::add_state(StateMachine& state)
	{
		CP_ASSERT(get_state(state.get_id()) == nullptr);
		_states.push_back(RefPtr(&state));
		state._parent = this;
	}

	void StateMachine::update()
	{
		on_update();
		if (_current_state)
			_current_state->update();
	}

	void StateMachine::handle_event(const StateEvent& event)
	{
		on_event(event);
		if (_current_state)
			_current_state->on_event(event);
	}

	void StateMachine::set_current_state(StateID id, const StateMessage& message)
	{
		if (_current_state && _current_state->get_id() == id)
			return;

		StateMachine* previous_state = _current_state;
		StateMachine* next_state = nullptr;
		for (RefPtr<StateMachine>& state : _states)
		{
			if (state->_id == id)
			{
				next_state = state.get();
				break;
			}
		}

		if (_current_state)
			_current_state->on_exit(next_state, message);
		_current_state = next_state;
		if (_current_state)
			_current_state->on_enter(previous_state, message);
	}

	auto StateMachine::get_state(StateID id) const -> StateMachine*
	{
		auto it = std::ranges::find_if(_states, [id](const RefPtr<StateMachine>& state) { return state->_id == id; });
		return it != _states.end() ? it->get() : nullptr;
	}

	void LambdaStateMachine::on_update()
	{
		Base::on_update();
		if (_on_update)
			_on_update();
	}

	void LambdaStateMachine::on_enter(StateMachine* from, const StateMessage& message)
	{
		Base::on_enter(from, message);
		if (_on_enter)
			_on_enter(from, message);
	}

	void LambdaStateMachine::on_exit(StateMachine* to, const StateMessage& message)
	{
		if (_on_exit)
			_on_exit(to, message);
		Base::on_exit(to, message);
	}

	void LambdaStateMachine::on_event(const StateEvent& event)
	{
		Base::on_event(event);
		if (_on_event)
			_on_event(event);
	}
}
