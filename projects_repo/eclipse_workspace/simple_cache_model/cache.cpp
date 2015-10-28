/*
 * cache.cpp
 *
 *  Created on: Oct 20, 2015
 *      Author: uzair
 */




#include "cache.hpp"
#include <math.h>
#include <algorithm>

cache::cache(sc_core::sc_module_name name, uint32_t total_cache_size, uint32_t cache_line_size, uint32_t num_of_ways, bool write_back, cache::eviction_policy evict_pol)
	:	sc_module(name),
		m_total_cache_size(total_cache_size),
		m_cache_line_size(cache_line_size),
		m_num_of_sets(total_cache_size/(cache_line_size*num_of_ways)),
		m_num_of_ways(num_of_ways),
		m_write_back(write_back),
		m_evict(evict_pol)
{
	m_cache_lines.resize(m_num_of_sets);
	for (uint32_t i=0; i<m_num_of_sets; i++) {
		m_cache_lines[i].resize(m_num_of_ways);
		for (uint32_t j=0; j<m_num_of_ways; j++) {
			m_cache_lines[i][j].valid = false;
		}
	}

	// ensuring that set bits <= mem_size_bits.....sort of a corner case but not expected to occur in real cache organizations
	// if this does occur though then cache would be inefficient as it will have slots for memory words which actually dont exist in memory......cache will be bigger than memory!
	assert(log2((double) m_num_of_sets) <= log2((double) MEM_SIZE));


	printf("CACHE_CONFIG \r\n");
	std::cout << name;
	printf("..total_cache_size:%dB", m_total_cache_size);
	printf("..cache_block_size:%dB", m_cache_line_size);
	printf("..num_of_sets:%d", m_num_of_sets);
	printf("..num_of_ways:%d", m_num_of_ways);
	printf("..write_back:%d", m_write_back);
	printf("..evict_policy:%d\r\n\r\n", m_evict);

}

void cache::update(tlm::tlm_generic_payload &payload, sc_core::sc_time &delay) {
	for (uint32_t i=0; i<m_num_of_ways; i++) {
		printf("valid:%d..", m_cache_lines[359][i].valid);
		printf("dirty:%d..", m_cache_lines[359][i].dirty);
		printf("tag:0x%08x..", m_cache_lines[359][i].tag);
		printf("lru:%d\r\n", m_cache_lines[359][i].evict_tag);
	}






	addr_t addr = (payload.get_address()/m_cache_line_size)*m_cache_line_size;			// rounded to cache-block address
	tlm::tlm_command cmd = payload.get_command();

	int way_free = -1;

	// finding out the cache set for this address for lookup
	uint32_t set = (addr >> (uint32_t) (log2((double) WORD_SIZE) + log2((double) m_cache_line_size/WORD_SIZE))) & ((1 << (uint32_t) log2((double)m_num_of_sets)) - 1);
	// as well as the tag
	addr_t tag = (addr >> (uint32_t) (log2((double) WORD_SIZE) + log2((double) m_cache_line_size/WORD_SIZE) + log2((double) m_num_of_sets)));

	delay += CACHE_LOOKUP_DELAY;
	// cache lookup for the found tag
	for (uint32_t i=0; i<m_num_of_ways; i++) {
		if (m_cache_lines[set][i].valid == true) {
			if (m_cache_lines[set][i].tag == tag) {
				printf("(%s)..", name());
				printf("cache hit for 0x%08x", payload.get_address());
				printf("..tag=0x%08x", tag);
				printf("...set=%d\r\n", set);

				if (cmd == tlm::TLM_WRITE_COMMAND) {	// write hit
					if (m_write_back) {
						m_cache_lines[set][i].dirty = true;
					}
					delay += WR_CACHE_DELAY;
				} else if (cmd == tlm::TLM_READ_COMMAND) {	// read hit
					delay += RD_CACHE_DELAY;
				}

				// for eviction policy management
				switch(m_evict) {
					case LRU:
						for (uint32_t j=0; j<m_num_of_ways; j++) {
							if (i!=j && m_cache_lines[set][j].valid==true && m_cache_lines[set][j].evict_tag<=m_cache_lines[set][i].evict_tag) {
								m_cache_lines[set][j].evict_tag++;
							}
							assert(m_cache_lines[set][j].evict_tag <= m_num_of_ways);
						}
						m_cache_lines[set][i].evict_tag = 1;
						break;
					case LFU:
						m_cache_lines[set][i].evict_tag++;
						break;
					case RAND:
						break;
					default:
						assert(0);
				}


				for (uint32_t i=0; i<m_num_of_ways; i++) {
					printf("valid:%d..", m_cache_lines[359][i].valid);
					printf("dirty:%d..", m_cache_lines[359][i].dirty);
					printf("tag:0x%08x..", m_cache_lines[359][i].tag);
					printf("lru:%d\r\n", m_cache_lines[359][i].evict_tag);
				}

				return;
			}
		} else {
			way_free = i;
		}
	}
	printf("(%s)..", name());
	printf("cache miss for 0x%08x", payload.get_address());
	printf("..tag=0x%08x", tag);
	printf("...set=%d\r\n", set);

	if (way_free == -1) {
		// do cache line replacement based on eviction policy
		uint64_t tmp = 0;
		switch(m_evict) {
			case LRU:
				for (uint32_t j=0; j<m_num_of_ways; j++) {
					if (m_cache_lines[set][j].evict_tag == m_num_of_ways) {
						way_free = j;
					}
				}
				break;
			case LFU:
				tmp = m_cache_lines[set][0].evict_tag;
				way_free = 0;
				for (uint32_t j=1; j<m_num_of_ways; j++) {
					if (tmp > m_cache_lines[set][j].evict_tag) {
						tmp = m_cache_lines[set][j].evict_tag;
						way_free = j;
					}
				}
				break;
			case RAND:
				way_free = rand()%4;
				break;
			default:
				assert(0);
		}

		// if the line to be evicted is dirty and cache policy is write-back then faking the cache line write to memory via timing modeling
		if (m_write_back && m_cache_lines[set][way_free].dirty==true) {
			delay += CACHE2MEM_LINE_DELAY;
		}
	}

	m_cache_lines[set][way_free].valid = true;
	m_cache_lines[set][way_free].dirty = false;
	m_cache_lines[set][way_free].tag = tag;
	if (m_evict == LRU) {
		for (uint32_t j=0; j<m_num_of_ways; j++) {
			if (way_free!=j && m_cache_lines[set][j].valid==true) {
				m_cache_lines[set][j].evict_tag++;
			}
			assert(m_cache_lines[set][j].evict_tag <= m_num_of_ways);
		}
		m_cache_lines[set][way_free].evict_tag = 1;
	} else {
		m_cache_lines[set][way_free].evict_tag = 0;
	}

	delay += MEM2CACHE_LINE_DELAY;
	if (cmd == tlm::TLM_WRITE_COMMAND) {					// write miss
		if (m_write_back) {
			m_cache_lines[set][way_free].dirty = true;
		}
		delay += WR_CACHE_DELAY;
	} else if (cmd == tlm::TLM_READ_COMMAND) {			// read miss
		delay += RD_CACHE_DELAY;
	}


	for (uint32_t i=0; i<m_num_of_ways; i++) {
		printf("valid:%d..", m_cache_lines[359][i].valid);
		printf("dirty:%d..", m_cache_lines[359][i].dirty);
		printf("tag:0x%08x..", m_cache_lines[359][i].tag);
		printf("lru:%d\r\n", m_cache_lines[359][i].evict_tag);
	}
}













