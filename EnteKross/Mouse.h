#pragma once

#include <queue>

class Mouse
{
	friend class Window;
public:
	class Event
	{
	public:
		enum class Type
		{
			Invalid,
			Move,
			LPress,
			LRelease,
			RPress,
			RRelease,
			MPress,
			MRelease,
			WheelUp,
			WheelDown,
			Enter,
			Leave
		};
	public:
		Event() noexcept
		{
			type = Type::Invalid;
			x = 0;
			y = 0;
			lPressed = false;
			rPressed = false;
			mPressed = false;
		}
		Event(Type type, Mouse& mouse) noexcept
			:
			type(type)
		{
			x = mouse.x;
			y = mouse.y;
			lPressed = mouse.lPressed;
			rPressed = mouse.rPressed;
			mPressed = mouse.mPressed;
		}
	public:
		bool IsValid()			const noexcept { return type != Type::Invalid; }
		bool LeftIsPressed()	const noexcept { return type == Type::LPress; }
		bool RightIsPressed()	const noexcept { return type == Type::RPress; }
		bool MiddleIsPressed()	const noexcept { return type == Type::MPress; }
		Type GetType()					const noexcept { return type; }
		int GetX()						const noexcept { return x; }
		int GetY()						const noexcept { return y; }
		std::pair<int, int> GetPos()	const noexcept { return { x,y }; }
	private:
		Type type;
		int x;
		int y;
		bool lPressed;
		bool rPressed;
		bool mPressed;
	};
public:
	Mouse();
public:
	bool IsInsideWindow()			const noexcept;
	std::pair<int, int> GetPos()	const noexcept;
	int GetPosX()					const noexcept;
	int GetPosY()					const noexcept;
	bool LeftIsPressed()			const noexcept;
	bool RightIsPressed()			const noexcept;
	bool MiddleIsPressed()			const noexcept;

	Event Read() noexcept;
	void Clear() noexcept;
private:  // Accessed on the Window-Side
	void OnMouseMove(int x, int y)	noexcept;
	void OnLeftPress()				noexcept;
	void OnRightPress()				noexcept;
	void OnMiddlePress()			noexcept;
	void OnLeftRelease()			noexcept;
	void OnRightRelease()			noexcept;
	void OnMiddleRelease()			noexcept;
	void OnMouseEnter()				noexcept;
	void OnMouseExit()				noexcept;
	void OnWheelDown()				noexcept;
	void OnWheelUp()				noexcept;
	void OnWheelDelta(int delta)	noexcept;

	void TrimBuffer() noexcept;
private:
	static constexpr unsigned int bufferSize = 16u;
	std::queue<Event> buffer;
private:
	int x;
	int y;
	bool lPressed;
	bool rPressed;
	bool mPressed;
	bool isInsideWindow;
	int wheelDelta;
};