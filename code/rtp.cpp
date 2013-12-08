/*
   Here's a small IPv4 example: it asks for a portbase and a destination and 
   starts sending packets to that destination.
*/

#include "rtpsession.h"
#include "rtpudpv4transmitter.h"
#include "rtpipv4address.h"
#include "rtpsessionparams.h"
#include "rtperrors.h"
#ifndef WIN32
	#include <netinet/in.h>
	#include <arpa/inet.h>
#else
	#include <winsock2.h>
#endif // WIN32
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>

using namespace jrtplib;

RTPSession sess;

//
// This function checks if there was a RTP error. If so, it displays an error
// message and exists.
//
int RTP_init();
int RTP_send(unsigned char * sdat, int ndat);
int RTP_end();

void checkerror(int rtperr)
{
	if (rtperr < 0)
	{
		std::cout << "ERROR: " << RTPGetErrorString(rtperr) << std::endl;
		exit(-1);
	}
}

//
// The main routine
//

int RTP_init()
{
	char if_default;
#ifdef WIN32
	WSADATA dat;
	WSAStartup(MAKEWORD(2,2),&dat);
#endif // WIN32
	uint16_t portbase,destport;
	uint32_t destip;
	std::string ipstr;
	int status,i,num;
	printf("Use default IP and port? (192.168.1.100:4444)\n");
	if_default = 'y';//getchar();

	if (if_default == 'y')
	{
		portbase=4444;
		ipstr = "192.168.1.100";
		destport = 4444;
	}
	else
	{
		std::cout << "Enter local portbase:" << std::endl;
		std::cin >> portbase;
		std::cout << std::endl;
		std::cout << "Enter the destination IP address" << std::endl;
		std::cin >> ipstr;
		std::cout << std::endl;
		std::cout << "Enter the destination port" << std::endl;
		std::cin >> destport;
		std::cout << std::endl;
	}
	
	destip = inet_addr(ipstr.c_str());
	if (destip == INADDR_NONE)
	{
		std::cerr << "Bad IP address specified" << std::endl;
		return -1;
	}
	
	// The inet_addr function returns a value in network byte order, but
	// we need the IP address in host byte order, so we use a call to
	// ntohl
	destip = ntohl(destip);
	

	
	std::cout << std::endl;
	// Now, we'll create a RTP session, set the destination, send some
	// packets and poll for incoming data.
	
	RTPUDPv4TransmissionParams transparams;
	RTPSessionParams sessparams;
	
	// IMPORTANT: The local timestamp unit MUST be set, otherwise
	//            RTCP Sender Report info will be calculated wrong
	// In this case, we'll be sending 10 samples each second, so we'll
	// put the timestamp unit to (1.0/10.0)
	sessparams.SetOwnTimestampUnit(1.0/25.0);		
	
	sessparams.SetAcceptOwnPackets(true);
	transparams.SetPortbase(portbase);
	status = sess.Create(sessparams,&transparams);	
	sess.SetMaximumPacketSize(20480);
	sessparams.SetMaximumPacketSize(20480);
	printf("%d",sessparams.GetMaximumPacketSize());
	checkerror(status);
	
	RTPIPv4Address addr(destip,destport);
	
	status = sess.AddDestination(addr);
	checkerror(status);
	return 1;
}

int RTP_send(char * sdat, int ndat)
{
	int status,i,num;
	// send the packet
	status = sess.SendPacket((void *)sdat,ndat,96,false,1);
	checkerror(status);

#ifndef RTP_SUPPORT_THREAD
	printf("fuck\n");
		status = sess.Poll();
		checkerror(status);
#endif // RTP_SUPPORT_THREAD
	return 1;
}

int RTP_end()
{
	sess.BYEDestroy(RTPTime(10,0),0,0);

#ifdef WIN32
	WSACleanup();
#endif // WIN32
	return 1;
}

