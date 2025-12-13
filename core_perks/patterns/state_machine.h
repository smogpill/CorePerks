// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once
#include "core_perks/patterns/reference.h"

namespace cp
{
	using StateMessageID = uint32;
	using StateEventID = uint32;
	using StateID = uint32;

	class StateMessage
	{
	public:
		StateMessage() = default;
		StateMessage(StateMessageID id) : id_(id) {}
		template <class T>
		StateMessage(const T& typed_id) : StateMessage(static_cast<StateMessageID>(typed_id)) {}
		virtual ~StateMessage() = default;

		StateMessageID get_id() const { return id_; }

	private:
		StateMessageID id_ = 0;
	};

	class StateEvent
	{
	public:
		StateEvent(StateEventID id) : id_(id) {}
		template <class T>
		StateEvent(const T& typed_id) : StateEvent(static_cast<StateEventID>(typed_id)) {}
		virtual ~StateEvent() = default;

		StateEventID get_id() const { return id_; }

	private:
		StateEventID id_ = 0;
	};

	class StateMachine : public RefCounted
	{
	public:
		explicit StateMachine(StateID id);
		template <class T>
		explicit StateMachine(const T& typed_id) : StateMachine(static_cast<StateID>(typed_id)) {}
		virtual ~StateMachine();

		void add_state(StateMachine& state);
		void set_current_state(StateID id, const StateMessage& message = StateMessage());
		template <class T>
		void set_current_state(const T& typed_id, const StateMessage& message = StateMessage()) { set_current_state(static_cast<StateID>(typed_id), message); }
		template <class T>
		StateMachine* get_state(const T& typed_id) const { return get_state(static_cast<StateID>(typed_id)); }
		StateMachine* get_state(StateID id) const;
		StateID get_id() const { return id_; }
		template <class T>
		T get_typed_id() const { return static_cast<T>(id_); }
		void update();
		void handle_event(const StateEvent& event);
		StateMachine* get_parent() { return parent_; }

	protected:
		virtual void on_enter(StateMachine* from, const StateMessage& message) {}
		virtual void on_exit(StateMachine* to, const StateMessage& message) {}
		virtual void on_update() {}
		virtual void on_event(const StateEvent& event) {}

	private:
		StateMachine* parent_ = nullptr;
		StateMachine* current_state_ = nullptr;
		StateID id_ = 0;
		std::vector<RefPtr<StateMachine>> states_;
	};
}
