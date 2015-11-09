/*
 * main.cpp
 *
 *  Created on: Oct 20, 2015
 *      Author: uzair
 */




#include <tlm>
#include <tlm_utils/simple_initiator_socket.h>
#include <tlm_utils/simple_target_socket.h>
#include <stdio.h>
#include "cache.hpp"


#define MEM_SIZE 536870912



struct req_type {
	uint64_t addr;
	tlm::tlm_command cmd;
	unsigned char wr_data[4];
	unsigned char rd_data[4];
};

#define NUM_REQ 9

class req_generator : public sc_core::sc_module {
public:
	tlm_utils::simple_initiator_socket<req_generator> m_isocket;

	SC_HAS_PROCESS(req_generator);
	req_generator(sc_core::sc_module_name name)
		:	m_isocket("m_isocket")
	{
		SC_THREAD(main);
	}

	void main() {
		//unsigned char *mem_no_cache = new unsigned char[MEM_SIZE];
		req_type req_stimuli[NUM_REQ] = {
											{0x10345678, tlm::TLM_READ_COMMAND},
											{0x10345078, tlm::TLM_READ_COMMAND},
											{0x10345278, tlm::TLM_READ_COMMAND},
											{0x10345478, tlm::TLM_READ_COMMAND},
											{0x10345878, tlm::TLM_WRITE_COMMAND, {1,2,3,4}},
											{0x11345078, tlm::TLM_READ_COMMAND},
											{0x11345278, tlm::TLM_READ_COMMAND},
											{0x11345478, tlm::TLM_READ_COMMAND},
											{0x11345a78, tlm::TLM_READ_COMMAND}
								  	  	};

		while(1) {
			tlm::tlm_generic_payload trans;

			trans.set_data_length(4);
			for (int i=0; i<NUM_REQ; i++) {
				sc_core::sc_time delay = sc_core::SC_ZERO_TIME;

				trans.set_address(req_stimuli[i].addr);
				if (req_stimuli[i].cmd == tlm::TLM_WRITE_COMMAND) {
					trans.set_command(tlm::TLM_WRITE_COMMAND);
					trans.set_data_ptr((unsigned char *)&req_stimuli[i].wr_data);
				} else {
					trans.set_command(tlm::TLM_READ_COMMAND);
					trans.set_data_ptr((unsigned char *)&req_stimuli[i].rd_data);
				}

				m_isocket->b_transport(trans, delay);

				printf("delay:%s\r\n---------------\r\n", delay.to_string().c_str());

				wait(2, sc_core::SC_NS);
			}

			//assert(0);
			while(1) {
				wait(1, sc_core::SC_MS);
			}
		}
	}
};

#define WORD_SIZE 4

#define L1_TO_L2_CACHEBLOCK_DELAY sc_core::sc_time(20, sc_core::SC_NS)
#define L2_TO_L1_CACHEBLOCK_DELAY sc_core::sc_time(20, sc_core::SC_NS)

#define L2_TO_MEM_CACHEBLOCK_DELAY sc_core::sc_time(40, sc_core::SC_NS)
#define MEM_TO_L2_CACHEBLOCK_DELAY sc_core::sc_time(200, sc_core::SC_NS)

#define L1_LOOKUP_DELAY sc_core::sc_time(1, sc_core::SC_NS)
#define L1_READ_DELAY sc_core::sc_time(5, sc_core::SC_NS)
#define L1_WRITE_DELAY sc_core::sc_time(5, sc_core::SC_NS)

#define L2_LOOKUP_DELAY sc_core::sc_time(2, sc_core::SC_NS)
#define L2_READ_DELAY sc_core::sc_time(10, sc_core::SC_NS)
#define L2_WRITE_DELAY sc_core::sc_time(10, sc_core::SC_NS)

#define L1_TO_CPU_DELAY sc_core::sc_time(10, sc_core::SC_NS)
#define CPU_TO_L1_DELAY sc_core::sc_time(10, sc_core::SC_NS)


class target : public sc_core::sc_module {
public:
	tlm_utils::simple_target_socket<target> m_tsocket;

	target(sc_core::sc_module_name name, unsigned char *mem)
		:	m_tsocket("m_tsocket"),
			m_mem(mem),
			m_l1cache_i("m_l1cache_i", 1024, 16, 2),			// has to make write-through as it share a parent cache
			m_l1cache_d("m_l1cache_d", 1024, 16, 2),			// has to make write-thorugh as it share a parent cache
			m_l2cache("m_l2cache", 2048, 16, 4)
	{
		m_tsocket.register_b_transport(this, &target::b_transport);

		m_l2cache.set_parent(NULL);
		child_of_l2 = new std::vector< cache * >;
		child_of_l2->reserve(2);
		child_of_l2->push_back(&m_l1cache_i);
		child_of_l2->push_back(&m_l1cache_d);
		m_l2cache.set_children(child_of_l2);
		m_l2cache.set_delays((sc_core::sc_time)MEM_TO_L2_CACHEBLOCK_DELAY, (sc_core::sc_time)L2_TO_MEM_CACHEBLOCK_DELAY, (sc_core::sc_time) L2_LOOKUP_DELAY, (sc_core::sc_time) L2_WRITE_DELAY, (sc_core::sc_time) L2_READ_DELAY);

		m_l1cache_i.set_parent(&m_l2cache);
		m_l1cache_i.set_children(NULL);
		m_l1cache_i.set_delays((sc_core::sc_time)L2_TO_L1_CACHEBLOCK_DELAY, (sc_core::sc_time)L1_TO_L2_CACHEBLOCK_DELAY, (sc_core::sc_time) L1_LOOKUP_DELAY, (sc_core::sc_time) L1_WRITE_DELAY, (sc_core::sc_time) L1_READ_DELAY);

		m_l1cache_d.set_parent(&m_l2cache);
		m_l1cache_d.set_children(NULL);
		m_l1cache_d.set_delays((sc_core::sc_time)L2_TO_L1_CACHEBLOCK_DELAY, (sc_core::sc_time)L1_TO_L2_CACHEBLOCK_DELAY, (sc_core::sc_time) L1_LOOKUP_DELAY, (sc_core::sc_time) L1_WRITE_DELAY, (sc_core::sc_time) L1_READ_DELAY);
	}

	void b_transport(tlm::tlm_generic_payload &payload, sc_core::sc_time &delay) {
		uint64_t addr = (payload.get_address()/WORD_SIZE)*WORD_SIZE;
		tlm::tlm_command cmd = payload.get_command();
		uint32_t len = payload.get_data_length();
		unsigned char *ptr = payload.get_data_ptr();

		assert(addr>=0x00000000 && addr<0x20000000);

		/*static int count = 0;
		count++;*/
		if (payload.get_command() == tlm::TLM_WRITE_COMMAND) {
			delay += CPU_TO_L1_DELAY;
		}
		if (addr <= 0x11000000) {
			m_l1cache_d.update(payload, delay);
		} else {
			m_l1cache_i.update(payload, delay);
		}
		if (payload.get_command() == tlm::TLM_READ_COMMAND) {
			delay += L1_TO_CPU_DELAY;
		}

		if (cmd == tlm::TLM_WRITE_COMMAND) {
			std::memcpy(&m_mem[addr], ptr, (unsigned long) len);
		} else {
			std::memcpy(ptr, &m_mem[addr], (unsigned long) len);
		}
	}

	void print() {
		m_l1cache_d.print_cache_set(7);
		printf("----\r\n");
		m_l1cache_i.print_cache_set(7);
		printf("----\r\n");
		m_l2cache.print_cache_set(7);
		printf("----\r\n");
	}

private:
	unsigned char *m_mem;
	cache m_l1cache_i;
	cache m_l1cache_d;
	cache m_l2cache;
	std::vector< cache * > *child_of_l2;
};




int sc_main(int argc, char *argv[]) {
	unsigned char *mem = new unsigned char[MEM_SIZE];

	req_generator m_req("m_req");
	target m_target("m_target", mem);

	m_req.m_isocket(m_target.m_tsocket);

	sc_core::sc_start(1, sc_core::SC_MS);

	printf("END\r\n");
	m_target.print();

	return 0;
}
