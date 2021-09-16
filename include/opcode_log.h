#pragma once

#ifdef LOG_ALL

    #define LOG_A_REG() LOG("A Register updated to: 0x" << std::setw(2) << std::setfill('0') << std::hex << static_cast<u16>(m_Registers.A) << ".")
    #define LOG_F_REG() LOG("F Register updated to: 0x" << std::setw(2) << std::setfill('0') << std::hex << static_cast<u16>(m_Registers.F) << ".")
    #define LOG_B_REG() LOG("B Register updated to: 0x" << std::setw(2) << std::setfill('0') << std::hex << static_cast<u16>(m_Registers.B) << ".")
    #define LOG_C_REG() LOG("C Register updated to: 0x" << std::setw(2) << std::setfill('0') << std::hex << static_cast<u16>(m_Registers.C) << ".")
    #define LOG_D_REG() LOG("D Register updated to: 0x" << std::setw(2) << std::setfill('0') << std::hex << static_cast<u16>(m_Registers.D) << ".")
    #define LOG_E_REG() LOG("E Register updated to: 0x" << std::setw(2) << std::setfill('0') << std::hex << static_cast<u16>(m_Registers.E) << ".")
    #define LOG_H_REG() LOG("H Register updated to: 0x" << std::setw(2) << std::setfill('0') << std::hex << static_cast<u16>(m_Registers.H) << ".")
    #define LOG_L_REG() LOG("F Register updated to: 0x" << std::setw(2) << std::setfill('0') << std::hex << static_cast<u16>(m_Registers.L) << ".")

    #define LOG_FLAGS() LOG("Flags updated to: "                                          \
                            << ((m_Registers.F & Flags::Register::Zero)      ? "Z" : "_") \
                            << ((m_Registers.F & Flags::Register::Negative)  ? "N" : "_") \
                            << ((m_Registers.F & Flags::Register::HalfCarry) ? "H" : "_") \
                            << ((m_Registers.F & Flags::Register::Carry)     ? "C" : "_") \
                            << ".")

    #define LOG_AF_REG() LOG("AF Register updated to: 0x" << std::setw(4) << std::setfill('0') << std::hex << static_cast<u16>(m_Registers.AF) << ".")
    #define LOG_BC_REG() LOG("BC Register updated to: 0x" << std::setw(4) << std::setfill('0') << std::hex << static_cast<u16>(m_Registers.BC) << ".")
    #define LOG_DE_REG() LOG("DE Register updated to: 0x" << std::setw(4) << std::setfill('0') << std::hex << static_cast<u16>(m_Registers.DE) << ".")
    #define LOG_HL_REG() LOG("HL Register updated to: 0x" << std::setw(4) << std::setfill('0') << std::hex << static_cast<u16>(m_Registers.HL) << ".")
    #define LOG_SP_REG() LOG("SP Register updated to: 0x" << std::setw(4) << std::setfill('0') << std::hex << static_cast<u16>(m_Registers.SP) << ".")

    #define LOG_WRITE(addr, val) LOG("Wrote 0x" << std::setw(2) << std::setfill('0') << std::hex << static_cast<u16>(val) \
                                                << " to address 0x" << std::setw(4) << addr << ".")

    #define LOG_READ(addr) LOG("Read 0x" << std::setw(2) << std::setfill('0') << std::hex << static_cast<u16>(m_MMU->read(addr)) \
                                        << " from 0x" << std::setw(4) << std::setfill('0') << std::hex << addr << ".")

    #define LOG_PUSH() LOG("Pushed 0x" << std::setw(2) << std::setfill('0') << std::hex     \
                                    << static_cast<u16>(m_MMU->read(m_Registers.SP    )) \
                                    << std::setw(2) << std::setfill('0') << std::hex     \
                                    << static_cast<u16>(m_MMU->read(m_Registers.SP + 1)) << " to the stack.")

    #define LOG_POP() LOG("Popped 0x" << std::setw(2) << std::setfill('0') << std::hex     \
                                    << static_cast<u16>(m_MMU->read(m_Registers.SP - 2)) \
                                    << std::setw(2) << std::setfill('0') << std::hex     \
                                    << static_cast<u16>(m_MMU->read(m_Registers.SP - 1)) << " from the stack.")

    #define LOG_JP(addr) LOG("Jumped to: 0x" << std::setw(4) << std::setfill('0') << std::hex << addr << ".");
    #define LOG_NJP() LOG("Did not jump.");

    #define LOG_RET() LOG("Returned to: 0x" << std::setw(4) << std::setfill('0') << std::hex << m_Registers.PC << ".");
    #define LOG_NRET() LOG("Did not return.");

    #define LOG_DI() LOG("Disabled Interrupts.")
    #define LOG_EI() LOG("Enabled Interrupts.")

#else

    #define LOG_A_REG() ((void)0)
    #define LOG_F_REG() ((void)0)
    #define LOG_B_REG() ((void)0)
    #define LOG_C_REG() ((void)0)
    #define LOG_D_REG() ((void)0)
    #define LOG_E_REG() ((void)0)
    #define LOG_H_REG() ((void)0)
    #define LOG_L_REG() ((void)0)

    #define LOG_FLAGS() ((void)0)

    #define LOG_AF_REG() ((void)0)
    #define LOG_BC_REG() ((void)0)
    #define LOG_DE_REG() ((void)0)
    #define LOG_HL_REG() ((void)0)
    #define LOG_SP_REG() ((void)0)

    #define LOG_WRITE(addr, val) ((void)0)
    #define LOG_READ(addr) ((void)0)

    #define LOG_PUSH() ((void)0)
    #define LOG_POP() ((void)0)

    #define LOG_JP(addr) ((void)0)
    #define LOG_NJP() ((void)0)

    #define LOG_RET() ((void)0)
    #define LOG_NRET() ((void)0)

    #define LOG_DI() ((void)0)
    #define LOG_EI() ((void)0)

#endif