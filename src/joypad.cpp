#include "core.hpp"

#include "joypad.hpp"

void Joypad::press(Button button)
{
    switch(button)
    {
        case Button::Right:  right  = 0; TRACE("Pressed right");  break;
        case Button::Left:   left   = 0; TRACE("Pressed left");   break;
        case Button::Up:     up     = 0; TRACE("Pressed up");     break;
        case Button::Down:   down   = 0; TRACE("Pressed down");   break;

        case Button::A:      a      = 0; TRACE("Pressed a");      break;
        case Button::B:      b      = 0; TRACE("Pressed b");      break;
        case Button::Select: select = 0; TRACE("Pressed select"); break;
        case Button::Start:  start  = 0; TRACE("Pressed start");  break;

        default:                         WARN("Invalid Key Press");
    }
}

void Joypad::release(Button button)
{
    switch(button)
    {
        case Button::Right:  right  = 1; TRACE("Released right");  break;
        case Button::Left:   left   = 1; TRACE("Released left");   break;
        case Button::Up:     up     = 1; TRACE("Released up");     break;
        case Button::Down:   down   = 1; TRACE("Released down");   break;

        case Button::A:      a      = 1; TRACE("Released a");      break;
        case Button::B:      b      = 1; TRACE("Released b");      break;
        case Button::Select: select = 1; TRACE("Released select"); break;
        case Button::Start:  start  = 1; TRACE("Released start");  break;

        default:                         WARN("Invalid Key Press");
    }
}

void Joypad::setInput(u8 val)
{
    using bit_functions::get_bit;

    direction = !get_bit(val, P14_INPUT_BIT);
    action    = !get_bit(val, P15_INPUT_BIT);
}

auto Joypad::getInput() -> u8
{
    u8 input = 0;

    if(direction)
    {
        bit_functions::set_bit_to(input, P10_INPUT_BIT, right);
        bit_functions::set_bit_to(input, P11_INPUT_BIT, left);
        bit_functions::set_bit_to(input, P12_INPUT_BIT, up);
        bit_functions::set_bit_to(input, P13_INPUT_BIT, down);
    }

    if(action)
    {
        bit_functions::set_bit_to(input, P10_INPUT_BIT, a);
        bit_functions::set_bit_to(input, P11_INPUT_BIT, b);
        bit_functions::set_bit_to(input, P12_INPUT_BIT, select);
        bit_functions::set_bit_to(input, P13_INPUT_BIT, start);
    }

    bit_functions::set_bit_to(input, P14_INPUT_BIT, direction);
    bit_functions::set_bit_to(input, P15_INPUT_BIT, action);

    // Temp check to ignore SGB
    u8 checkSGB = input & 0x30;
    if(checkSGB == 0x10 || checkSGB == 0x20)
    {
        return input;
    }

    return 0xFF;
}

// TODO: Allow for rebinding

auto Joypad::getButton(SDL_Keycode keycode) -> Button
{
    switch(keycode)
    {
        case SDLK_RIGHT:     return Button::Right;
        case SDLK_LEFT:      return Button::Left;
        case SDLK_UP:        return Button::Up;
        case SDLK_DOWN:      return Button::Down;

        case SDLK_z:         return Button::A;
        case SDLK_x:         return Button::B;
        case SDLK_BACKSPACE: return Button::Select;
        case SDLK_RETURN:    return Button::Start;
    }

    return Button::None;
}
