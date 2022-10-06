#include "Keyboard.h"

void Keyboard::ClearBuffers() noexcept
{
	ClearKeys();
	ClearChars();
}

bool Keyboard::KeyIsPressed(unsigned char keycode) const noexcept
{
	return keyStates[keycode];
}

bool Keyboard::KeyIsEmpty() const noexcept
{
	return keyBuffer.empty();
}

Keyboard::Event Keyboard::ReadKey() noexcept
{
	if (!KeyIsEmpty())
	{
		Event e = keyBuffer.front();
		keyBuffer.pop();
		return e;
	}
	else
	{
		return {};
	}
}

void Keyboard::ClearKeys() noexcept
{
	keyBuffer = std::queue<Event>{};
}

bool Keyboard::CharIsEmpty() const noexcept
{
	return charBuffer.empty();
}

unsigned char Keyboard::ReadChar() noexcept
{
	if (!CharIsEmpty())
	{
		unsigned char c = charBuffer.front();
		charBuffer.pop();
		return c;
	}
	else
	{
		return 0u;
	}
}

void Keyboard::ClearChars() noexcept
{
	charBuffer = std::queue<unsigned char>{};
}

bool Keyboard::AutoRepeatEnabled() const noexcept
{
	return autorepeat;
}

void Keyboard::EnableAutoRepeat() noexcept
{
	autorepeat = true;
}

void Keyboard::DisableAutoRepeat() noexcept
{
	autorepeat = false;
}

void Keyboard::OnKeyDown(unsigned char keycode) noexcept
{
	keyStates[keycode] = true;
	keyBuffer.push({ Event::Type::Press, keycode });
	TrimBuffer(keyBuffer);
}

void Keyboard::OnKeyRelease(unsigned char keycode) noexcept
{
	keyStates[keycode] = false;
	keyBuffer.push({ Event::Type::Release, keycode });
	TrimBuffer(keyBuffer);
}

void Keyboard::OnChar(unsigned char keycode) noexcept
{
	charBuffer.push(keycode);
	TrimBuffer(charBuffer);
}