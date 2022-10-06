#pragma once

#include <queue>
#include <bitset>

class Keyboard
{
	friend class Window;
public:
	class Event
	{
	public:
		enum class Type
		{
			Invalid,
			Press,
			Release
		};
	public:
		Event() noexcept
		{
			type = Type::Invalid;
			keycode = 0;
		}
		Event(Type type, unsigned char keycode) noexcept
			:
			type(type),
			keycode(keycode)
		{}
	public:
		bool IsValid() const noexcept
		{
			return type != Type::Invalid;
		}
		bool IsPress() const noexcept
		{
			return type == Type::Press;
		}
		bool IsRelease() const noexcept
		{
			return type == Type::Release;
		}
		unsigned char GetKeycode() const noexcept
		{
			return keycode;
		}
	private:
		Type type;
		unsigned char keycode;
	};
public:
	Keyboard() = default;
	Keyboard(const Keyboard&) = delete;
	Keyboard& operator =(const Keyboard&) = delete;
public:
	void ClearBuffers() noexcept;
	// Key stuff
	bool KeyIsPressed(unsigned char keycode)  const noexcept;
	bool KeyIsEmpty()						  const noexcept;
	Event ReadKey()									noexcept;
	void ClearKeys()								noexcept;
	// Char stuff
	bool CharIsEmpty()						  const noexcept;
	unsigned char ReadChar()						noexcept;
	void ClearChars()								noexcept;
	// Autorepeat
	bool AutoRepeatEnabled()				  const noexcept;
	void EnableAutoRepeat()							noexcept;
	void DisableAutoRepeat()						noexcept;
private:
	template<typename T>
	void TrimBuffer(std::queue<T>& buffer)			noexcept;
	void OnKeyDown(unsigned char keycode)			noexcept;
	void OnKeyRelease(unsigned char keycode)		noexcept;
	void OnChar(unsigned char keycode)				noexcept;
private:
	static constexpr unsigned int bufferSize = 16u;
	static constexpr unsigned int nKeys = 256u;
	bool autorepeat = false;
	std::bitset<nKeys> keyStates;
	std::queue<Event> keyBuffer;
	std::queue<unsigned char> charBuffer;
};

template<typename T>
inline void Keyboard::TrimBuffer(std::queue<T>& buffer) noexcept
{
	while (buffer.size() > bufferSize)
	{
		buffer.pop();
	}
}