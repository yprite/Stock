/*
 * NetConnectionManager.cpp
 *
 *  Created on: Dec 15, 2016
 *      Author: gabin
 */

#include "NetConnection.h"
#include "WDebugBase.h"
#ifdef LOG_TAG
#undef LOG_TAG
#endif

#define LOG_TAG "NetConnection"

NetConnection* NetConnection::_instance = nullptr;

NetConnection* NetConnection::getInstance()
{
    if (!_instance)
        _instance = new NetConnection();

    return _instance;
}
void NetConnection::destroy()
{
    if (_instance)
        delete _instance;
    _instance = nullptr;
}

int NetConnection::initialize()
{
    int err = connection_create(&_conHandler);

    if (err != CONNECTION_ERROR_NONE)
    {
        WERROR("connection_create failed.(%d)", err);
        return -1;
    }
    _setConnectionCallback();

    return 0;
}
int NetConnection::finalize()
{
    if (_conHandler)
    {
        _unsetConnectionCallback();
        connection_destroy(_conHandler);
        _conHandler = nullptr;
    }
    return 0;
}

NetConnection::NetConnection()
{
    _conHandler = nullptr;
    _connectionChangedCb = nullptr;
    _ipChangedCb = nullptr;
}

NetConnection::~NetConnection()
{

}

void NetConnection::setOnConnectionTypeChangedCb(const std::function<void(NetConnection::NetState type)>& connectionChangedCb)
{
    _connectionChangedCb = connectionChangedCb;
}

void NetConnection::setOnIpAddressChangedCb(const std::function<void(const char *ipv4_address, const char *ipv6_address)>& ipChangedCb)
{
    _ipChangedCb = ipChangedCb;
}

NetConnection::NetState NetConnection::getNetworkState() const
{
    connection_type_e conType;
    int err = connection_get_type(_conHandler, &conType);
    NetConnection::NetState netState;

    if (err == CONNECTION_ERROR_NONE)
    {
        switch (conType)
        {
            case CONNECTION_TYPE_DISCONNECTED:
                netState = NetConnection::NetState::DISCONNECTED;
                break;
            case CONNECTION_TYPE_WIFI:
                netState = NetConnection::NetState::WIFI;
                break;
            case CONNECTION_TYPE_CELLULAR:
                netState = NetConnection::NetState::CELLULAR;
                break;
            case CONNECTION_TYPE_ETHERNET:
                netState = NetConnection::NetState::ETHERNET;
                break;
            case CONNECTION_TYPE_BT:
                netState = NetConnection::NetState::BT;
                break;
            default:
                netState = NetConnection::NetState::UNKNOWN;
                break;
        }
    }
    else
    {
        WERROR("connection_get_type failed.(%d)", err);
        netState = NetConnection::NetState::UNKNOWN;
    }

    return netState;
}

NetConnection::CellularState NetConnection::getCellularState() const
{
    connection_cellular_state_e state;
    int err = connection_get_cellular_state(_conHandler, &state);
    NetConnection::CellularState cellularState;

    if (err == CONNECTION_ERROR_NONE)
    {
        switch (state)
        {
            case CONNECTION_CELLULAR_STATE_OUT_OF_SERVICE:
                cellularState = NetConnection::CellularState::OUT_OF_SERVICE;
                break;
            case CONNECTION_CELLULAR_STATE_FLIGHT_MODE:
                cellularState = NetConnection::CellularState::FLIGHT_MODE;
                break;
            case CONNECTION_CELLULAR_STATE_ROAMING_OFF:
                cellularState = NetConnection::CellularState::ROAMING_OFF;
                break;
            case CONNECTION_CELLULAR_STATE_CALL_ONLY_AVAILABLE:
                cellularState = NetConnection::CellularState::CALL_ONLY_AVAILABLE;
                break;
            case CONNECTION_CELLULAR_STATE_AVAILABLE:
                cellularState = NetConnection::CellularState::AVAILABLE;
                break;
            case CONNECTION_CELLULAR_STATE_CONNECTED:
                cellularState = NetConnection::CellularState::CONNECTED;
                break;
            default:
                cellularState = NetConnection::CellularState::UNKNOWN;
                break;
        }
    }
    else
    {
        WERROR("connection_get_cellulatr_state failed.(%d)", err);
        cellularState = NetConnection::CellularState::UNKNOWN;
    }

    return cellularState;
}

NetConnection::WifiState NetConnection::getWifiState() const
{
    connection_wifi_state_e state;
    int err = connection_get_wifi_state(_conHandler, &state);
    NetConnection::WifiState wifiState;

    if (err == CONNECTION_ERROR_NONE)
    {
        switch (state)
        {
            case CONNECTION_WIFI_STATE_DEACTIVATED:
                wifiState = NetConnection::WifiState::DEACTIVATED;
                break;
            case CONNECTION_WIFI_STATE_DISCONNECTED:
                wifiState = NetConnection::WifiState::DISCONNECTED;
                break;
            case CONNECTION_WIFI_STATE_CONNECTED:
                wifiState = NetConnection::WifiState::CONNECTED;
                break;
            default:
                wifiState = NetConnection::WifiState::UNKNOWN;
                break;
        }
    }
    else
    {
        WERROR("connection_get_wifi_state failed.(%d)", err);
        wifiState = NetConnection::WifiState::UNKNOWN;
    }

    return wifiState;
}

NetConnection::BtState NetConnection::getBtState() const
{
    connection_bt_state_e state;
    int err = connection_get_bt_state(_conHandler, &state);
    NetConnection::BtState btState;

    if (err == CONNECTION_ERROR_NONE)
    {
        switch (state)
        {
            case CONNECTION_BT_STATE_DEACTIVATED:
                btState = NetConnection::BtState::DEACTIVATED;
                break;
            case CONNECTION_BT_STATE_DISCONNECTED:
                btState = NetConnection::BtState::DISCONNECTED;
                break;
            case CONNECTION_BT_STATE_CONNECTED:
                btState = NetConnection::BtState::CONNECTED;
                break;
            default:
                btState = NetConnection::BtState::UNKNOWN;
                break;
        }
    }
    else
    {
        WERROR("connection_get_bt_state failed.(%d)", err);
        btState = NetConnection::BtState::UNKNOWN;
    }

    return btState;
}

NetConnection::EthernetState NetConnection::getEthernetState() const
{
    connection_ethernet_state_e state;
    int err = connection_get_ethernet_state(_conHandler, &state);
    NetConnection::EthernetState ethernetState;

    if (err == CONNECTION_ERROR_NONE)
    {
        switch (state)
        {
            case CONNECTION_ETHERNET_STATE_DEACTIVATED:
                ethernetState = NetConnection::EthernetState::DEACTIVATED;
                break;
            case CONNECTION_ETHERNET_STATE_DISCONNECTED:
                ethernetState = NetConnection::EthernetState::DISCONNECTED;
                break;
            case CONNECTION_ETHERNET_STATE_CONNECTED:
                ethernetState = NetConnection::EthernetState::CONNECTED;
                break;
            default:
                ethernetState = NetConnection::EthernetState::UNKNWON;
                break;
        }
    }
    else
    {
        WERROR("connection_get_ethernet_state failed.(%d)", err);
        ethernetState = NetConnection::EthernetState::UNKNWON;
    }

    return ethernetState;
}

void NetConnection::_setConnectionCallback()
{
    connection_set_type_changed_cb(_conHandler, _onConnectionTypeChanged, this);
    connection_set_ip_address_changed_cb(_conHandler, _onIpAddressChanged, this);
}

void NetConnection::_unsetConnectionCallback()
{
    connection_unset_type_changed_cb(_conHandler);
    connection_unset_ip_address_changed_cb(_conHandler);
}

void NetConnection::_onConnectionTypeChanged(connection_type_e type, void* userData)
{
    auto self = static_cast<NetConnection *>(userData);
    NetConnection::NetState netState;

    if (self->_connectionChangedCb)
    {
        switch (type)
        {
            case CONNECTION_TYPE_DISCONNECTED:
                netState = NetConnection::NetState::DISCONNECTED;
                break;
            case CONNECTION_TYPE_WIFI:
                netState = NetConnection::NetState::WIFI;
                break;
            case CONNECTION_TYPE_CELLULAR:
                netState = NetConnection::NetState::CELLULAR;
                break;
            case CONNECTION_TYPE_ETHERNET:
                netState = NetConnection::NetState::ETHERNET;
                break;
            case CONNECTION_TYPE_BT:
                netState = NetConnection::NetState::BT;
                break;
            default:
                netState = NetConnection::NetState::UNKNOWN;
                break;
        }
        self->_connectionChangedCb(netState);
    }

}

void NetConnection::_onIpAddressChanged(const char* ipv4_address, const char* ipv6_address, void* userData)
{
    auto self = static_cast<NetConnection *>(userData);

    if (self->_ipChangedCb)
        self->_ipChangedCb(ipv4_address, ipv6_address);
}
