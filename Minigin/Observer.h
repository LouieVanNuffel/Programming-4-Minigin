#pragma once

namespace dae
{
	struct Event;
	class GameObject;

	class Observer
	{
	public:
		virtual ~Observer() = default;
		virtual void Notify(const Event& event, const GameObject* gameObject) = 0;
	};
}
