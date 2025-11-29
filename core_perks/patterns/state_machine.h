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
		StateMessage(StateMessageID id) : _id(id) {}
		template <class T>
		StateMessage(const T& typedId) : StateMessage(static_cast<StateMessageID>(typedId)) {}
		virtual ~StateMessage() = default;

		auto get_id() const { return _id; }

	private:
		StateMessageID _id = 0;
	};

	class StateEvent
	{
	public:
		StateEvent(StateEventID id) : _id(id) {}
		template <class T>
		StateEvent(const T& typed_id) : StateEvent(static_cast<StateEventID>(typed_id)) {}
		virtual ~StateEvent() = default;

		auto get_id() const { return _id; }

	private:
		StateEventID _id = 0;
	};

	class StateMachine : public RefCounted<StateMachine>
	{
	public:
		explicit StateMachine(StateID id);
		template <class T>
		explicit StateMachine(const T& typedId) : StateMachine(static_cast<StateID>(typedId)) {}
		virtual ~StateMachine();

		void add_state(StateMachine& state);
		void set_current_state(StateID id, const StateMessage& message = StateMessage());
		template <class T>
		void set_current_state(const T& typedId, const StateMessage& message = StateMessage()) { set_current_state(static_cast<StateID>(typedId), message); }
		template <class T>
		auto get_state(const T& typedId) const -> StateMachine* { return get_state(static_cast<StateID>(typedId)); }
		auto get_state(StateID id) const -> StateMachine*;
		auto get_id() const { return _id; }
		template <class T>
		auto get_typed_id() const { return static_cast<T>(_id); }
		void update();
		void handle_event(const StateEvent& event);
		auto get_parent() -> StateMachine* { return _parent; }

	protected:
		virtual void on_enter(StateMachine* from, const StateMessage& message) {}
		virtual void on_exit(StateMachine* to, const StateMessage& message) {}
		virtual void on_update() {}
		virtual void on_event(const StateEvent& event) {}

	private:
		StateMachine* _parent = nullptr;
		StateMachine* _current_state = nullptr;
		StateID _id = 0;
		std::vector<RefPtr<StateMachine>> _states;
	};

	class LambdaStateMachine : public StateMachine
	{
		using Base = StateMachine;
	public:
		using OnUpdateFunc = std::function<void()>;
		using OnEnterFunc = std::function<void(StateMachine*, const StateMessage&)>;
		using OnExitFunc = std::function<void(StateMachine*, const StateMessage&)>;
		using OnEventFunc = std::function<void(const StateEvent&)>;

		explicit LambdaStateMachine(StateID id) : Base(id) {}
		void set_on_update(OnUpdateFunc func) { _on_update = func; }
		void set_on_enter(OnEnterFunc func) { _on_enter = func; }
		void set_on_exit(OnExitFunc func) { _on_exit = func; }
		void set_on_event(OnEventFunc func) { _on_event = func; }

	protected:
		void on_enter(StateMachine* from, const StateMessage& message) override;
		void on_exit(StateMachine* to, const StateMessage& message) override;
		void on_update() override;
		void on_event(const StateEvent& event) override;

	private:
		OnUpdateFunc _on_update;
		OnEnterFunc _on_enter;
		OnExitFunc _on_exit;
		OnEventFunc _on_event;
	};
}
