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
#include <mesosphere.hpp>

namespace ams::kern {

    Result KSessionRequest::SessionMappings::PushMap(KProcessAddress client, KProcessAddress server, size_t size, KMemoryState state, size_t index) {
        /* At most 15 buffers of each type (4-bit descriptor counts). */
        MESOSPHERE_ASSERT(index < ((1ul << 4) - 1) * 3);

        /* Get the mapping. */
        Mapping *mapping;
        if (index < NumStaticMappings) {
            mapping = std::addressof(m_static_mappings[index]);
        } else {
            /* Allocate a page for the extra mappings. */
            if (m_mappings == nullptr) {
                KPageBuffer *page_buffer = KPageBuffer::Allocate();
                R_UNLESS(page_buffer != nullptr, svc::ResultOutOfMemory());

                m_mappings = reinterpret_cast<Mapping *>(page_buffer);
            }

            mapping = std::addressof(m_mappings[index - NumStaticMappings]);
        }

        /* Set the mapping. */
        mapping->Set(client, server, size, state);

        return ResultSuccess();
    }

    Result KSessionRequest::SessionMappings::PushSend(KProcessAddress client, KProcessAddress server, size_t size, KMemoryState state) {
        MESOSPHERE_ASSERT(m_num_recv == 0);
        MESOSPHERE_ASSERT(m_num_exch == 0);
        return this->PushMap(client, server, size, state, m_num_send++);
    }

    Result KSessionRequest::SessionMappings::PushReceive(KProcessAddress client, KProcessAddress server, size_t size, KMemoryState state) {
        MESOSPHERE_ASSERT(m_num_exch == 0);
        return this->PushMap(client, server, size, state, m_num_send + m_num_recv++);
    }

    Result KSessionRequest::SessionMappings::PushExchange(KProcessAddress client, KProcessAddress server, size_t size, KMemoryState state) {
        return this->PushMap(client, server, size, state, m_num_send + m_num_recv + m_num_exch++);
    }

    void KSessionRequest::SessionMappings::Finalize() {
        if (m_mappings) {
            KPageBuffer::Free(reinterpret_cast<KPageBuffer *>(m_mappings));
            m_mappings = nullptr;
        }
    }

}
