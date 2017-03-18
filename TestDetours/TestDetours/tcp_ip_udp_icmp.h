#ifndef TCP_IP_UDP_ICMP_H
#define TCP_IP_UDP_ICMP_H

typedef struct tcpheader
{
	unsigned short int sport;	//source address port
	unsigned short int dport;	//destination address port
	unsigned int th_seq;		//sequence number
	unsigned int th_ack;		//acknowledge number
	unsigned char th_x2:4;		//header length
	unsigned char th_off:4;    //reserved
	unsigned char  th_flag;    //flags: URG ACK PSH RST SYN FIN
	unsigned short int th_win;	//window size
	unsigned short int th_sum;	//check sum
	unsigned short int th_urp;	//urgent pointer
} TCP_HDR;

struct ipheader
{
	unsigned char h_lenver;		//version & header length
	unsigned char ip_tos;		//tos
	unsigned short int ip_len;	//total length
	unsigned short int ip_id;	//id
	unsigned short int ip_off;	//offset
	unsigned char ip_ttl;		//time to live
	unsigned char ip_p;			//protocal
	unsigned short int ip_sum;	//check sum
	unsigned int ip_src;		//source address
	unsigned int ip_dst;		//destination address
};

typedef struct udphdr
{
	unsigned short sport;		//source port
	unsigned short dport;		//destination port
	unsigned short len;			//UDP length
	unsigned short cksum;		//check sum(include data)
} UDP_HDR;

typedef struct icmphdr    
{ 
	unsigned short sport;
	unsigned short dport;
	BYTE i_type;           
	BYTE i_code;           
	USHORT i_cksum;         
	USHORT i_id;           
	USHORT i_seq;          
	ULONG timestamp;      
} ICMP_HDR;

#endif