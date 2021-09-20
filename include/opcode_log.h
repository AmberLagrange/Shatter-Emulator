#pragma once

#define LOG_A_REG() OPCODE("A Register updated to: 0x" << std::setw(2) << std::setfill('0') << std::hex << static_cast<u16>(m_Registers.A) << ".")
#define LOG_F_REG() OPCODE("F Register updated to: 0x" << std::setw(2) << std::setfill('0') << std::hex << static_cast<u16>(m_Registers.F) << ".")
#define LOG_B_REG() OPCODE("B Register updated to: 0x" << std::setw(2) << std::setfill('0') << std::hex << static_cast<u16>(m_Registers.B) << ".")
#define LOG_C_REG() OPCODE("C Register updated to: 0x" << std::setw(2) << std::setfill('0') << std::hex << static_cast<u16>(m_Registers.C) << ".")
#define LOG_D_REG() OPCODE("D Register updated to: 0x" << std::setw(2) << std::setfill('0') << std::hex << static_cast<u16>(m_Registers.D) << ".")
#define LOG_E_REG() OPCODE("E Register updated to: 0x" << std::setw(2) << std::setfill('0') << std::hex << static_cast<u16>(m_Registers.E) << ".")
#define LOG_H_REG() OPCODE("H Register updated to: 0x" << std::setw(2) << std::setfill('0') << std::hex << static_cast<u16>(m_Registers.H) << ".")
#define LOG_L_REG() OPCODE("L Register updated to: 0x" << std::setw(2) << std::setfill('0') << std::hex << static_cast<u16>(m_Registers.L) << ".")

#define LOG_FLAGS() OPCODE("Flags updated to: " << ((m_Registers.F & Flags::Register::Zero)      ? "Z" : "_") \
                                                << ((m_Registers.F & Flags::Register::Negative)  ? "N" : "_") \
                                                << ((m_Registers.F & Flags::Register::HalfCarry) ? "H" : "_") \
                                                << ((m_Registers.F & Flags::Register::Carry)     ? "C" : "_") \
                                                << ".")

#define LOG_AF_REG() OPCODE("AF Register updated to: 0x" << std::setw(4) << std::setfill('0') << std::hex << static_cast<u16>(m_Registers.AF) << ".")
#define LOG_BC_REG() OPCODE("BC Register updated to: 0x" << std::setw(4) << std::setfill('0') << std::hex << static_cast<u16>(m_Registers.BC) << ".")
#define LOG_DE_REG() OPCODE("DE Register updated to: 0x" << std::setw(4) << std::setfill('0') << std::hex << static_cast<u16>(m_Registers.DE) << ".")
#define LOG_HL_REG() OPCODE("HL Register updated to: 0x" << std::setw(4) << std::setfill('0') << std::hex << static_cast<u16>(m_Registers.HL) << ".")
#define LOG_SP_REG() OPCODE("SP Register updated to: 0x" << std::setw(4) << std::setfill('0') << std::hex << static_cast<u16>(m_Registers.SP) << ".")

#define LOG_WRITE(addr) OPCODE("Wrote 0x" << std::setw(2) << std::setfill('0') << std::hex << static_cast<u16>(m_MMU->read(addr)) \
                                          << " to address 0x" << std::setw(4) << addr << ".")

#define LOG_READ(addr) OPCODE("Read 0x" << std::setw(2) << std::setfill('0') << std::hex << static_cast<u16>(m_MMU->read(addr)) \
                                        << " from 0x" << std::setw(4) << std::setfill('0') << std::hex << addr << ".")

#define LOG_PUSH() OPCODE("Pushed 0x" << std::setw(2) << std::setfill('0') << std::hex     \
                                      << static_cast<u16>(m_MMU->read(m_Registers.SP    )) \
                                      << std::setw(2) << std::setfill('0') << std::hex     \
                                      << static_cast<u16>(m_MMU->read(m_Registers.SP + 1)) << " to the stack.")

#define LOG_POP() OPCODE("Popped 0x" << std::setw(2) << std::setfill('0') << std::hex     \
                                     << static_cast<u16>(m_MMU->read(m_Registers.SP - 2)) \
                                     << std::setw(2) << std::setfill('0') << std::hex     \
                                     << static_cast<u16>(m_MMU->read(m_Registers.SP - 1)) << " from the stack.")

#define LOG_JP()  OPCODE("Jumped to: 0x" << std::setw(4) << std::setfill('0') << std::hex << m_Registers.PC << ".")
#define LOG_NJP() OPCODE("Did not jump.")

#define LOG_RET()  OPCODE("Returned to: 0x" << std::setw(4) << std::setfill('0') << std::hex << m_Registers.PC << ".")
#define LOG_NRET() OPCODE("Did not return.")

#define LOG_DI() OPCODE("Disabled Interrupts.")
#define LOG_EI() OPCODE("Enabled Interrupts.")