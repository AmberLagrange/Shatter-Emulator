#pragma once

#define LOG_A_REG() Logger(LogLevel::Opcode) << "A Register updated to: 0x" << std::setw(2) << std::setfill('0') << std::hex << static_cast<u16>(m_Registers.A) << "."
#define LOG_F_REG() Logger(LogLevel::Opcode) << "F Register updated to: 0x" << std::setw(2) << std::setfill('0') << std::hex << static_cast<u16>(m_Registers.F) << "."
#define LOG_B_REG() Logger(LogLevel::Opcode) << "B Register updated to: 0x" << std::setw(2) << std::setfill('0') << std::hex << static_cast<u16>(m_Registers.B) << "."
#define LOG_C_REG() Logger(LogLevel::Opcode) << "C Register updated to: 0x" << std::setw(2) << std::setfill('0') << std::hex << static_cast<u16>(m_Registers.C) << "."
#define LOG_D_REG() Logger(LogLevel::Opcode) << "D Register updated to: 0x" << std::setw(2) << std::setfill('0') << std::hex << static_cast<u16>(m_Registers.D) << "."
#define LOG_E_REG() Logger(LogLevel::Opcode) << "E Register updated to: 0x" << std::setw(2) << std::setfill('0') << std::hex << static_cast<u16>(m_Registers.E) << "."
#define LOG_H_REG() Logger(LogLevel::Opcode) << "H Register updated to: 0x" << std::setw(2) << std::setfill('0') << std::hex << static_cast<u16>(m_Registers.H) << "."
#define LOG_L_REG() Logger(LogLevel::Opcode) << "F Register updated to: 0x" << std::setw(2) << std::setfill('0') << std::hex << static_cast<u16>(m_Registers.L) << "."

#define LOG_FLAGS() Logger(LogLevel::Opcode) << "Flags updated to: "                                          \
                                             << ((m_Registers.F & Flags::Register::Zero)      ? "Z" : "_") \
                                             << ((m_Registers.F & Flags::Register::Negative)  ? "N" : "_") \
                                             << ((m_Registers.F & Flags::Register::HalfCarry) ? "H" : "_") \
                                             << ((m_Registers.F & Flags::Register::Carry)     ? "C" : "_") \
                                             << "."

#define LOG_AF_REG() Logger(LogLevel::Opcode) << "AF Register updated to: 0x" << std::setw(4) << std::setfill('0') << std::hex << static_cast<u16>(m_Registers.AF) << "."
#define LOG_BC_REG() Logger(LogLevel::Opcode) << "BC Register updated to: 0x" << std::setw(4) << std::setfill('0') << std::hex << static_cast<u16>(m_Registers.BC) << "."
#define LOG_DE_REG() Logger(LogLevel::Opcode) << "DE Register updated to: 0x" << std::setw(4) << std::setfill('0') << std::hex << static_cast<u16>(m_Registers.DE) << "."
#define LOG_HL_REG() Logger(LogLevel::Opcode) << "HL Register updated to: 0x" << std::setw(4) << std::setfill('0') << std::hex << static_cast<u16>(m_Registers.HL) << "."
#define LOG_SP_REG() Logger(LogLevel::Opcode) << "SP Register updated to: 0x" << std::setw(4) << std::setfill('0') << std::hex << static_cast<u16>(m_Registers.SP) << "."

#define LOG_WRITE(addr) Logger(LogLevel::Opcode) << "Wrote 0x" << std::setw(2) << std::setfill('0') << std::hex << m_MMU->read(addr) \
                                                 << " to address 0x" << std::setw(4) << addr << "."

#define LOG_READ(addr) Logger(LogLevel::Opcode) << "Read 0x" << std::setw(2) << std::setfill('0') << std::hex << static_cast<u16>(m_MMU->read(addr)) \
                                                << " from 0x" << std::setw(4) << std::setfill('0') << std::hex << addr << "."

#define LOG_PUSH() Logger(LogLevel::Opcode) << "Pushed 0x" << std::setw(2) << std::setfill('0') << std::hex     \
                                            << static_cast<u16>(m_MMU->read(m_Registers.SP    )) \
                                            << std::setw(2) << std::setfill('0') << std::hex     \
                                            << static_cast<u16>(m_MMU->read(m_Registers.SP + 1)) << " to the stack."

#define LOG_POP() Logger(LogLevel::Opcode) << "Popped 0x" << std::setw(2) << std::setfill('0') << std::hex     \
                                           << static_cast<u16>(m_MMU->read(m_Registers.SP - 2)) \
                                           << std::setw(2) << std::setfill('0') << std::hex     \
                                           << static_cast<u16>(m_MMU->read(m_Registers.SP - 1)) << " from the stack."

#define LOG_JP()  Logger(LogLevel::Opcode) << "Jumped to: 0x" << std::setw(4) << std::setfill('0') << std::hex << m_Registers.PC << "."
#define LOG_NJP() Logger(LogLevel::Opcode) << "Did not jump."

#define LOG_RET()  Logger(LogLevel::Opcode) << "Returned to: 0x" << std::setw(4) << std::setfill('0') << std::hex << m_Registers.PC << "."
#define LOG_NRET() Logger(LogLevel::Opcode) << "Did not return."

#define LOG_DI() Logger(LogLevel::Opcode) << "Disabled Interrupts."
#define LOG_EI() Logger(LogLevel::Opcode) << "Enabled Interrupts."