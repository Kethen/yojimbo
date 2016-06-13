/*
    Yojimbo Client/Server Network Library.
    
    Copyright © 2016, The Network Protocol Company, Inc.

    All rights reserved.
*/

#ifndef YOJIMBO_NETWORK_H
#define YOJIMBO_NETWORK_H

#include "yojimbo_config.h"

#include <stdint.h>
#include <assert.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

struct addrinfo;
struct sockaddr_in6;
struct sockaddr_storage;

namespace yojimbo
{
    enum AddressType
    {
        ADDRESS_NONE,
        ADDRESS_IPV4,
        ADDRESS_IPV6
    };

    class Address
    {
        AddressType m_type;

        union
        {
            uint32_t m_address_ipv4;
            uint16_t m_address_ipv6[8];
        };

        uint16_t m_port;

   public:

        Address();

        Address( uint8_t a, uint8_t b, uint8_t c, uint8_t d, uint16_t port = 0 );

        explicit Address( uint32_t address, int16_t port = 0 );

        explicit Address( uint16_t a, uint16_t b, uint16_t c, uint16_t d,
                          uint16_t e, uint16_t f, uint16_t g, uint16_t h,
                          uint16_t port = 0 );

        explicit Address( const uint16_t address[], uint16_t port = 0 );

        explicit Address( const sockaddr_storage * addr );

        explicit Address( addrinfo * p );

        explicit Address( const char * address );

        explicit Address( const char * address, uint16_t port );

        void Clear();

        uint32_t GetAddress4() const;

        const uint16_t * GetAddress6() const;

        void SetPort( uint16_t port );

        uint16_t GetPort() const;

        AddressType GetType() const;

        const char * ToString( char buffer[], int bufferSize ) const;

        bool IsValid() const;

        bool IsLinkLocal() const;

        bool IsSiteLocal() const;

        bool IsMulticast() const;

		bool IsLoopback() const;

        bool IsGlobalUnicast() const;

        bool operator ==( const Address & other ) const;

        bool operator !=( const Address & other ) const;

    protected:

        void Parse( const char * address );
    };

#if YOJIMBO_SOCKETS

    void GetNetworkAddresses( Address * addresses, int & numAddresses, int maxAddresses );

    Address GetFirstNetworkAddress_IPV4();

    Address GetFirstNetworkAddress_IPV6();

    enum SocketError
    {
        SOCKET_ERROR_NONE,
        SOCKET_ERROR_CREATE_FAILED,
        SOCKET_ERROR_SET_NON_BLOCKING_FAILED,
        SOCKET_ERROR_SOCKOPT_IPV6_ONLY_FAILED,
        SOCKET_ERROR_BIND_IPV4_FAILED,
        SOCKET_ERROR_BIND_IPV6_FAILED,
        SOCKET_ERROR_GET_SOCKNAME_IPV4_FAILED,
        SOCKET_ERROR_GET_SOCKNAME_IPV6_FAILED
    };

#if YOJIMBO_PLATFORM == YOJIMBO_PLATFORM_WINDOWS
	typedef uint64_t SocketHandle;
#else // #if YOJIMBO_PLATFORM == YOJIMBO_PLATFORM_WINDOWS
	typedef int SocketHandle;
#endif // #if YOJIMBO_PLATFORM == YOJIMBO_PLATFORM_WINDOWS
						   
    class Socket
    {
    public:

        Socket( const Address & address );

        ~Socket();

        bool IsError() const;

        int GetError() const;

        bool SendPacket( const Address & to, const void * packetData, size_t packetBytes );
    
        int ReceivePacket( Address & from, void * packetData, int maxPacketSize );

        const Address & GetAddress() const;

    private:

        int m_error;
        Address m_address;
        SocketHandle m_socket;
    };

#endif // #if YOJIMBO_SOCKETS

    bool InitializeNetwork();

    void ShutdownNetwork();

    bool IsNetworkInitialized();
}

#endif // #ifndef YOJIMBO_NETWORK_H
