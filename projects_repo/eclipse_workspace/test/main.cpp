/*
 * main.cpp
 *
 *  Created on: Oct 12, 2015
 *      Author: uzair
 */



#include <tlm>
#include <tlm_utils/simple_initiator_socket.h>
#include <tlm_utils/simple_target_socket.h>
#include <stdio.h>



class initiator : public sc_core::sc_module {
public:
	tlm_utils::simple_initiator_socket<initiator> *m_socket[2];

	initiator(sc_core::sc_module_name name);

private:

}

int sc_main(int argc, char *argv[]) {

}