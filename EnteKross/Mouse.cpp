#include "Mouse.h"

Mouse::Mouse()
{
    x = 0;
    y = 0;
    lPressed = false;
    rPressed = false;
    mPressed = false;
    isInsideWindow = false;
    wheelDelta = 0;
}

bool Mouse::IsInsideWindow() const noexcept
{
    return isInsideWindow;
}

std::pair<int, int> Mouse::GetPos() const noexcept
{
    return { x,y };
}

int Mouse::GetPosX() const noexcept
{
    return x;
}

int Mouse::GetPosY() const noexcept
{
    return y;
}

bool Mouse::LeftIsPressed() const noexcept
{
    return lPressed;
}

bool Mouse::RightIsPressed() const noexcept
{
    return rPressed;
}

bool Mouse::MiddleIsPressed() const noexcept
{
    return mPressed;
}

Mouse::Event Mouse::Read() noexcept
{
    if (!buffer.empty())
    {
        Event e = buffer.front();
        buffer.pop();
        return e;
    }

    return {};
}

void Mouse::Clear() noexcept
{
    buffer = std::queue<Event>{};
}

void Mouse::OnMouseMove(int x, int y) noexcept
{
    this->x = x;
    this->y = y;
    buffer.push({ Event::Type::Move, *this });
    TrimBuffer();
}

void Mouse::OnLeftPress() noexcept
{
    lPressed = true;
    buffer.push({ Event::Type::LPress, *this });
    TrimBuffer();
}

void Mouse::OnRightPress() noexcept
{
    rPressed = true;
    buffer.push({ Event::Type::RPress, *this });
    TrimBuffer();
}

void Mouse::OnMiddlePress() noexcept
{
    mPressed = true;
    buffer.push({ Event::Type::MPress, *this });
    TrimBuffer();
}

void Mouse::OnLeftRelease() noexcept
{
    lPressed = false;
    buffer.push({ Event::Type::LRelease, *this });
    TrimBuffer();
}

void Mouse::OnRightRelease() noexcept
{
    rPressed = false;
    buffer.push({ Event::Type::RRelease, *this });
    TrimBuffer();
}

void Mouse::OnMiddleRelease() noexcept
{
    mPressed = false;
    buffer.push({ Event::Type::MRelease, *this });
    TrimBuffer();
}

void Mouse::OnMouseEnter() noexcept
{
    isInsideWindow = true;
    buffer.push({ Event::Type::Enter, *this });
    TrimBuffer();
}

void Mouse::OnMouseExit() noexcept
{
    isInsideWindow = false;
    buffer.push({ Event::Type::Leave, *this });
    TrimBuffer();
}

void Mouse::TrimBuffer() noexcept
{
    while (buffer.size() > bufferSize)
    {
        buffer.pop();
    }
}