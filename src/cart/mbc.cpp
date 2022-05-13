#include "core.hpp"

#include "mbc.hpp"

#include <sstream>

MBC::MBC(std::vector<u8>&& rom)
    : m_Rom(rom) {}

MBC::~MBC() = default;

auto MBC::getCartType(const std::vector<u8>& data) -> Cart::Type
{
    u8 cartType = data[CART_TYPE];

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

auto MBC::getCartTitle(const std::vector<u8>& data) -> const std::string
{
    // Since the title might or might not contain null bytes,
    // copy all the possible data over, then remove anything after
    // the first null byte
    
    auto it = data.begin() + CART_TITLE;
    std::string title(it, it + CART_TITLE_SIZE);
    title.erase(std::find(title.begin(), title.end(), '\0'), title.end());

    // If the version is not version 0, add "(Version n)" to the title

    u8 version = data[CART_VERSION_NUMBER];
    if(version)
    {
        std::stringstream ss;
        ss << title << " (Version " << static_cast<int>(version) << ")";
        title = ss.str();
    }

    return title;
}

auto MBC::load(const char* path) -> std::vector<u8>
{
    std::vector<u8> rom;
    
    std::ifstream data(path, std::ios::in | std::ios::binary);
    rom.assign((std::istreambuf_iterator<char>(data)), {});
    DEBUG("Rom Size: " << std::dec << rom.size() / 1024 << "KB!");

    return rom;
}
