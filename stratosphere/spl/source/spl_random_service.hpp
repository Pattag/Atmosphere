/*
 * Copyright (c) Atmosphère-NX
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once
#include <stratosphere.hpp>
#include "spl_secure_monitor_manager.hpp"

namespace ams::spl {

    class RandomService final {
        protected:
            SecureMonitorManager &m_manager;
        public:
            explicit RandomService(SecureMonitorManager *manager) : m_manager(*manager) { /* ... */ }
        public:
            /* Actual commands. */
            Result GenerateRandomBytes(const sf::OutBuffer &out) {
                return m_manager.GenerateRandomBytes(out.GetPointer(), out.GetSize());
            }
    };
    static_assert(spl::impl::IsIRandomInterface<RandomService>);

}
