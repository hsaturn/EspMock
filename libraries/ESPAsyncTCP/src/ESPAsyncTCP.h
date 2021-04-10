/*
  EspMock for Asynchronous TCP library for Espressif MCUs

  Copyright (c) 2016 Hristo Gochkov. All rights reserved.
  This file is part of the esp8266 core for Arduino environment.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#pragma once

#include "IPAddress.h"
#include <functional>
#include <memory>

struct err_t {};
class AsyncClient;
class AsyncServer;
class ACErrorTracker;

#define ASYNC_WRITE_FLAG_COPY 0x01

struct tcp_pcb;
struct ip_addr;
#if ASYNC_TCP_SSL_ENABLED
struct SSL_;
typedef struct SSL_ SSL;
struct SSL_CTX_;
typedef struct SSL_CTX_ SSL_CTX;
#endif

typedef std::function<void(void*, AsyncClient*)> AcConnectHandler;
typedef std::function<void(void*, AsyncClient*, size_t len, uint32_t time)> AcAckHandler;
typedef std::function<void(void*, AsyncClient*, err_t error)> AcErrorHandler;
typedef std::function<void(void*, AsyncClient*, void *data, size_t len)> AcDataHandler;
typedef std::function<void(void*, AsyncClient*, struct pbuf *pb)> AcPacketHandler;
typedef std::function<void(void*, AsyncClient*, uint32_t time)> AcTimeoutHandler;
typedef std::function<void(void*, size_t event)> AsNotifyHandler;

enum error_events {
  EE_OK = 0,
  EE_ABORTED,       // Callback or foreground aborted connections
  EE_ERROR_CB,      // Stack initiated aborts via error Callbacks.
  EE_CONNECTED_CB,
  EE_RECV_CB,
  EE_ACCEPT_CB,
  EE_MAX
};

class AsyncClient {
  public:
    AsyncClient* prev;
    AsyncClient* next;

#if ASYNC_TCP_SSL_ENABLED
    AsyncClient(tcp_pcb* pcb = 0, SSL_CTX * ssl_ctx = NULL) {}
#else
    AsyncClient(tcp_pcb* pcb = 0) {}
#endif
    ~AsyncClient(){}

    AsyncClient & operator=(const AsyncClient &other) { return *this;}
    AsyncClient & operator+=(const AsyncClient &other) { return *this;}

    bool operator==(const AsyncClient &other){ return false; }

    bool operator!=(const AsyncClient &other) {
      return !(*this == other);
    }
#if ASYNC_TCP_SSL_ENABLED
    bool connect(IPAddress ip, uint16_t port, bool secure=false) { return false; }
    bool connect(const char* host, uint16_t port, bool secure=false) { return false; }
#else
    bool connect(IPAddress ip, uint16_t port) { return false; }
    bool connect(const char* host, uint16_t port) { return false; }
#endif
    void close(bool now = false) {}
    void stop() {}
    void abort() {}
    bool free() { return true; }

    bool canSend() { return true;} //ack is not pending
    size_t space() { return 100;}
    size_t add(const char* data, size_t size, uint8_t apiflags=0);//add for sending
    bool send();//send all data added with the method above
    size_t ack(size_t len); //ack data that you have not acked using the method below
    void ackLater(){} //will not ack the current packet. Call from onData
    bool isRecvPush(){ return false; }
#if DEBUG_ESP_ASYNC_TCP
    size_t getConnectionId(void) const { return _errorTracker->getConnectionId();}
#endif
#if ASYNC_TCP_SSL_ENABLED
    SSL *getSSL();
#endif

    size_t write(const char* data) { return strlen(data); }
    size_t write(const char* data, size_t size, uint8_t apiflags=0) { return size; } //only when canSend() == true

    uint8_t state() { return 0;}
    bool connecting() { return false; }
    bool connected() { return false; }
    bool disconnecting(){ return false; }
    bool disconnected() { return false; }
    bool freeable();//disconnected or disconnecting

    uint16_t getMss() { return 0; }
    uint32_t getRxTimeout() { return 0; }
    void setRxTimeout(uint32_t timeout) {}//no RX data timeout for the connection in seconds
    uint32_t getAckTimeout() { return 0;}
    void setAckTimeout(uint32_t timeout) {}//no ACK timeout for the last sent packet in milliseconds
    void setNoDelay(bool nodelay) {}
    bool getNoDelay() { return false;}

    uint32_t getRemoteAddress();
    uint16_t getRemotePort();
    uint32_t getLocalAddress();
    uint16_t getLocalPort();


    IPAddress remoteIP();
    uint16_t  remotePort();
    IPAddress localIP();
    uint16_t  localPort();

    void onConnect(AcConnectHandler cb, void* arg = 0){}     //on successful connect
    void onDisconnect(AcConnectHandler cb, void* arg = 0){}  //disconnected
    void onAck(AcAckHandler cb, void* arg = 0){}             //ack received
    void onError(AcErrorHandler cb, void* arg = 0){}         //unsuccessful connect or error
    void onData(AcDataHandler cb, void* arg = 0){}           //data received (called if onPacket is not used)
    void onPacket(AcPacketHandler cb, void* arg = 0){}       //data received
    void onTimeout(AcTimeoutHandler cb, void* arg = 0){}     //ack timeout
    void onPoll(AcConnectHandler cb, void* arg = 0){}        //every 125ms when connected

    void ackPacket(struct pbuf * pb){}
   const char * errorToString(err_t error){ return 0;}
    const char * stateToString() { return nullptr; }
};

#if ASYNC_TCP_SSL_ENABLED
typedef std::function<int(void* arg, const char *filename, uint8_t **buf)> AcSSlFileHandler;
struct pending_pcb;
#endif


class AsyncServer {
  public:

    AsyncServer(IPAddress addr, uint16_t port){}
    AsyncServer(uint16_t port){}
    ~AsyncServer(){}
    void onClient(AcConnectHandler cb, void* arg){}
#if ASYNC_TCP_SSL_ENABLED
    void onSslFileRequest(AcSSlFileHandler cb, void* arg){}
    void beginSecure(const char *cert, const char *private_key_file, const char *password){}
#endif
    void begin(){}
    void end(){}
    void setNoDelay(bool nodelay){}
    bool getNoDelay(){ return false;}
    uint8_t status() { return 0;}
};
