#include "core.hpp"

#include "mbc.hpp"

#include <filesystem>
#include <sstream>

auto MBC::getCartType(const std::vector<u8>& data) -> Cart::Type
{
    u8 cartType = data[CART_TYPE];

    #ifndef NDEBUG
        switch(static_cast<Cart::Type>(cartType))
        {
            case Cart::Type::ROM_ONLY:
            DEBUG("Loaded ROM Only.");
            break;
        case Cart::Type::MBC1:
        case Cart::Type::MBC1_RAM:
        case Cart::Type::MBC1_RAM_BATTERY:
            DEBUG("Loaded MBC1.");
            break;
        case Cart::Type::MBC3_TIMER_BATTERY:
        case Cart::Type::MBC3_TIMER_RAM_BATTERY_2:
        case Cart::Type::MBC3:
        case Cart::Type::MBC3_RAM_2:
        case Cart::Type::MBC3_RAM_BATTERY_2:
            DEBUG("Loaded MBC3.");
            break;
        case Cart::Type::MBC5:
        case Cart::Type::MBC5_RAM:
        case Cart::Type::MBC5_RAM_BATTERY:
        case Cart::Type::MBC5_RUMBLE:
        case Cart::Type::MBC5_RUMBLE_RAM:
        case Cart::Type::MBC5_RUMBLE_RAM_BATTERY:
            DEBUG("Loaded MBC5.");
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

    //Temporarily remove 'negative' characters while I don't have proper
    //cartridge header parsing yet
    title.erase(std::remove_if(title.begin(), title.end(), 
    [](auto const& c) -> bool { return c < 0; }), title.end());

    return title;
}

auto MBC::getCartRamSize(const std::vector<u8>& data) -> u32
{
    switch(data[CART_RAM_SIZE])
    {
        case 0x00:
            DEBUG("No RAM.");
            return 4  * RAM_BANK_SIZE;
        case 0x01:
            WARN("Unused RAM Size!");
            return 0;
        case 0x02:
            DEBUG("Ram Size: " << std::dec << 1  * RAM_BANK_SIZE / 1024 << "KB.");
            return 1  * RAM_BANK_SIZE;
        case 0x03:
            DEBUG("Ram Size: " << std::dec << 4  * RAM_BANK_SIZE / 1024 << "KB.");
            return 4  * RAM_BANK_SIZE;
        case 0x04:
            DEBUG("Ram Size: " << std::dec << 16 * RAM_BANK_SIZE / 1024 << "KB.");
            return 16 * RAM_BANK_SIZE;
        case 0x05:
            DEBUG("Ram Size: " << std::dec << 8  * RAM_BANK_SIZE / 1024 << "KB.");
            return 8  * RAM_BANK_SIZE;
        default:
            WARN("Unknown RAM Size!");
            return 0;
    }
}

auto MBC::loadRom(const std::string& path) -> std::vector<u8>
{
    std::vector<u8> rom;
    std::ifstream data(path, std::ios::in | std::ios::binary);
    rom.assign((std::istreambuf_iterator<char>(data)), {});
    DEBUG("Rom Size: " << std::dec << rom.size() / 1024 << "KB.");

    return rom;
}

auto MBC::loadRam(const std::string& path) -> std::vector<u8>
{
    std::vector<u8> ram{};

    if(std::filesystem::exists(path))
    {
        std::ifstream data(path, std::ios::in | std::ios::binary);
        ram.assign((std::istreambuf_iterator<char>(data)), {});
        DEBUG("Ram Size: " << std::dec << ram.size() / 1024 << "KB.");
    }

    return ram;
}

MBC::MBC(std::vector<u8>&& rom, std::vector<u8>&& ram)
    : m_Rom(rom)
{
    u32 ramSize = getCartRamSize(m_Rom);

    if(ram.empty())
    {
        m_Ram = std::vector<u8>(ramSize, 0);
    }
    else if(ram.size() != ramSize)
    {
        ERROR("Invalid RAM size. Data may be corrupted!");
    }
    else
    {
        DEBUG("Read RAM from disk.");
        m_Ram = std::move(ram);
    }
}

MBC::~MBC() = default;

auto MBC::getRam() -> const std::vector<u8>&
{
    return m_Ram;
}
