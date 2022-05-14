#include "core.hpp"

#include "mmu.hpp"

#include "gameboy.hpp"

#include <fstream>
#include <memory>

MMU::MMU(Gameboy& gb)
    : m_Gameboy(gb), m_Memory({}), m_BootRom({}), m_BootRomEnabled(false)
{
    DEBUG("Initializing MMU.");
}

void MMU::load(const std::string& path)
{
    if(m_Cart) return;

    std::vector<u8> rom = MBC::loadRom(path);
    std::vector<u8> ram = MBC::loadRam(path + ".sav");

    Cart::Type type   = MBC::getCartType(rom);
    std::string title = MBC::getCartTitle(rom);

    m_Gameboy.setTitle("Shatter Emulator: " + title);
    DEBUG("Loaded " << title << ".");

    switch(type)
    {
        case Cart::Type::ROM_ONLY:
            m_Cart = std::make_unique<RomOnly>(std::move(rom));
            break;
        case Cart::Type::MBC1:
        case Cart::Type::MBC1_RAM:
        case Cart::Type::MBC1_RAM_BATTERY:
            m_Cart = std::make_unique<MBC1>(std::move(rom), std::move(ram));
            break;
        case Cart::Type::MBC3_TIMER_BATTERY:
        case Cart::Type::MBC3_TIMER_RAM_BATTERY_2:
        case Cart::Type::MBC3:
        case Cart::Type::MBC3_RAM_2:
        case Cart::Type::MBC3_RAM_BATTERY_2:
            m_Cart = std::make_unique<MBC3>(std::move(rom), std::move(ram));
            break;
        default:
            m_Cart = std::make_unique<RomOnly>(std::move(rom));
    }
}

void MMU::loadBoot(const std::string& path)
{
    DEBUG("Loaded bootrom from " << path << ".");
    std::ifstream data(path, std::ios::in | std::ios::binary);
    data.read(reinterpret_cast<char*>(&m_BootRom[0]), BOOT_ROM_SIZE);
    m_BootRomEnabled = true;
}

void MMU::save(const std::string& path)
{
    auto& ram = m_Cart->getRam();

    if(ram.empty())
    {
        DEBUG("No ram to save.");
        return;
    }

    std::ofstream file(path + ".sav", std::ios::out | std::ios::binary);
    file.write(reinterpret_cast<const char*>(&ram[0]), ram.size());
    DEBUG("Saved data to: " << path << ".sav.");
}

auto MMU::read(u16 address) const -> u8
{
    if(address < ROM_END_ADDR)
    {
        if(address < BOOT_ROM_SIZE && m_BootRomEnabled)
        {
            return m_BootRom[address];
        }

        return m_Cart->read(address);
    }
    else if(address < VRAM_END_ADDR)
    {
        return m_Memory[address - ROM_SIZE];
    }
    else if(address < RAM_BANK_END_ADDR)
    {
        return m_Cart->read(address);
    }
    else if(address < INTERNAL_RAM_END_ADDR)
    {
        return m_Memory[address - ROM_SIZE];
    }
    else if(address < ECHO_RAM_END_ADDR)
    {
        return m_Memory[address - ROM_SIZE - INTERNAL_RAM_SIZE]; // Map back into RAM
    }
    else if(address < OAM_END_ADDR)
    {
        return m_Memory[address - ROM_SIZE];
    }
    else if(address < UNUSABLE_END_ADDR)
    {
        return UINT8_MAX;
    }
    else if(address < IO_END_ADDR)
    {
        switch(address)
        {
            case JOYPAD_REGISTER:
                return m_Gameboy.getInput();
            case TIMER_DIV_REGISTER:
                return m_Gameboy.getDIV();
            case BOOT_REGISTER:
                return m_BootRomEnabled ? 0 : 1;
            default:
                return m_Memory[address - ROM_SIZE];
        }
    }
    else
    {
        return m_Memory[address - ROM_SIZE];
    }
}

void MMU::write(u16 address, u8 val)
{
    if(address < ROM_END_ADDR)
    {
        m_Cart->write(address, val);
    }
    else if(address < VRAM_END_ADDR)
    {
        m_Memory[address - ROM_SIZE] = val;
    }
    else if(address < RAM_BANK_END_ADDR)
    {
        m_Cart->write(address, val);
    }
    else if(address < INTERNAL_RAM_END_ADDR)
    {
        m_Memory[address - ROM_SIZE] = val;
    }
    else if(address < ECHO_RAM_END_ADDR)
    {
        m_Memory[address - ROM_SIZE - INTERNAL_RAM_SIZE] = val; // Map back into RAM
    }
    else if(address < OAM_END_ADDR)
    {
        m_Memory[address - ROM_SIZE] = val;
    }
    else if(address < UNUSABLE_END_ADDR)
    {
        
    }
    else if(address < IO_END_ADDR)
    {
        switch(address)
        {
            case JOYPAD_REGISTER:
                m_Gameboy.setInput(val);
                break;
            case TIMER_DIV_REGISTER:
                m_Gameboy.resetDiv();
                break;
            case TIMER_TAC_REGISTER:
                m_Memory[address - ROM_SIZE] = val;
                switch(val & 0b11)
                {
                    case 0b00:
                        m_Gameboy.setTimerSpeed(TIMER_SPEED_00);
                        break;
                    case 0b01:
                        m_Gameboy.setTimerSpeed(TIMER_SPEED_01);
                        break;
                    case 0b10:
                        m_Gameboy.setTimerSpeed(TIMER_SPEED_10);
                        break;
                    case 0b11:
                        m_Gameboy.setTimerSpeed(TIMER_SPEED_11);
                        break;
                    default:
                        ASSERT(false, "Timer Speed Switch branched to invalid case!");
                }
                break;
            case DMA_TRANSFER_REGISTER:
                dmaTransfer(val);
                break;
            case BOOT_REGISTER:
                m_BootRomEnabled = (val == 0);
            default:
                m_Memory[address - ROM_SIZE] = val;
        }
    }
    else
    {
        m_Memory[address - ROM_SIZE] = val;
    }
}

auto MMU::isBootEnabled() const -> bool
{
    return m_BootRomEnabled;
}

void MMU::dmaTransfer(u8 val)
{
    u16 address = val * 0x100;
    for (u8 i = 0; i < DMA_TRANSFER_SIZE; ++i)
    {
        write(OAM_START_ADDR + i, read(address + i));
    }
}
