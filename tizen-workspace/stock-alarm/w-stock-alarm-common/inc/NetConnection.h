/*
 * NetConnectionManager.h
 *
 *  Created on: Dec 15, 2016
 *      Author: gabin
 */

#ifndef _NET_CONNECTION_MANAGER_H_
#define _NET_CONNECTION_MANAGER_H_

#include "WDefine.h"
#include <net_connection.h>
#include <functional>

class WAPP_ASSIST_EXPORT NetConnection
{
public:
    enum class NetState : unsigned
    {
        DISCONNECTED = 0,
        WIFI,
        CELLULAR,
        ETHERNET,
        BT,
        UNKNOWN
    };

    enum class CellularState : unsigned
    {
        OUT_OF_SERVICE = 0,
        FLIGHT_MODE,
        ROAMING_OFF,
        CALL_ONLY_AVAILABLE,
        AVAILABLE,              // cellular available but not connected yet.
        CONNECTED,              // connected.
        UNKNOWN
    };

    enum class WifiState : unsigned
    {
        DEACTIVATED = 0,            // wifi is not activated.
        DISCONNECTED,           // wifi is disconnected.
        CONNECTED,
        UNKNOWN
    };

    enum class BtState : unsigned
    {
        DEACTIVATED = 0,
        DISCONNECTED,
        CONNECTED,
        UNKNOWN
    };

    enum class EthernetState : unsigned
    {
        DEACTIVATED = 0,
        DISCONNECTED,
        CONNECTED,
        UNKNWON
    };

    static NetConnection* getInstance();
    static void destroy();

    int initialize();
    int finalize();

    NetConnection::NetState getNetworkState() const;
    NetConnection::CellularState getCellularState() const;
    NetConnection::WifiState getWifiState() const;
    NetConnection::BtState getBtState() const;
    NetConnection::EthernetState getEthernetState() const;

    void setOnConnectionTypeChangedCb(const std::function<void(NetConnection::NetState type)>& connectionChangedCb);
    void setOnIpAddressChangedCb(const std::function<void(const char *ipv4_address, const char *ipv6_address)>& ipChangedCb);

private:
    NetConnection();
    virtual ~NetConnection();

    void _setConnectionCallback();
    void _unsetConnectionCallback();

private:
    static void _onConnectionTypeChanged(connection_type_e type, void* userData);
    static void _onIpAddressChanged(const char* ipv4_address, const char* ipv6_address, void* userData);

private:
    static NetConnection* _instance;
    connection_h _conHandler;

    std::function<void(NetConnection::NetState type)> _connectionChangedCb;
    std::function<void(const char *ipv4_address, const char *ipv6_address)> _ipChangedCb;

};


#endif /* _NET_CONNECTION_MANAGER_H_ */
