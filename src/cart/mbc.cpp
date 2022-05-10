#include "core.hpp"

#include "mbc.hpp"

auto MBC::getCartType(const char* path) -> Cart::Type
{
    std::ifstream rom(path, std::ios::in | std::ios::binary);
    rom.seekg(CART_TYPE);

    char cartType;
    rom.read(&cartType, 1);

    #ifndef NDEBUG
        switch(static_cast<Cart::Type>(cartType))
        {
            case Cart::Type::ROM_ONLY:
            DEBUG("Loaded ROM Only!");
            break;
        case Cart::Type::MBC1:
        case Cart::Type::MBC1_RAM:
        case Cart::Type::MBC1_RAM_BATTERY:
            DEBUG("Loaded MBC1!");
            break;
        case Cart::Type::MBC3_TIMER_BATTERY:
        case Cart::Type::MBC3_TIMER_RAM_BATTERY_2:
        case Cart::Type::MBC3:
        case Cart::Type::MBC3_RAM_2:
        case Cart::Type::MBC3_RAM_BATTERY_2:
            DEBUG("Loaded MBC3!");
            break;
        default:
            WARN("Unknown MBC Type: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(cartType) << ", falling back to ROM Only!");
        }
        
    #endif

    return static_cast<Cart::Type>(cartType);
}

auto MBC::getCartTitle(const char* path) -> const std::string
{
    std::ifstream rom(path, std::ios::in | std::ios::binary);
    rom.seekg(CART_TITLE);

    std::string title(CART_TITLE_SIZE, ' ');
    rom.read(&title[0], CART_TITLE_SIZE);

    return title;
}

void MBC::load(const char* path)
{
    std::ifstream rom(path, std::ios::in | std::ios::binary);
    m_Rom.assign((std::istreambuf_iterator<char>(rom)), {});
    DEBUG("Rom Size: " << std::dec << m_Rom.size() / 1024 << "KB!");
}
