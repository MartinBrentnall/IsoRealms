/*
 * Copyright 2025 Martin Brentnall
 *
 * This file is part of IsoRealms.
 *
 * IsoRealms is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * IsoRealms is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with IsoRealms.  If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once

#include <iomanip>
#include <cstdlib>
#include <vector>
#include <arpa/inet.h>
#include <unistd.h>

#include <curl/curl.h>
#include <openssl/err.h>
#include <openssl/dh.h>
#include <openssl/ssl.h>
#include <openssl/conf.h>
#include <openssl/engine.h>
#include <openssl/opensslv.h>
#include <json-c/json.h>

#include "IsoRealms.h"

extern char *__progname;

namespace IsoRealms::Hue {
  class Hue;

  class HueManager final {
    public:
        
    /**********************\
     * Resource Interface *
    \**********************/
    HueManager(Hue& hue, IResourceData& data);
    void registerAssets(ResourceAssetRegistry& assets);
    void hintInUse(bool inUse);
    bool renderIcon();
    void getProperties(IPropertyMaker& owner, const Metadata& metadata);
    void removed();

    virtual ~HueManager();

    /*********************\
     * Module interfaces *
    \*********************/
    void updateRuntime(unsigned int milliseconds);
    void updateEditing(unsigned int milliseconds);
    void reset();

    
    private:
    inline static const std::string JSON_COLOUR = "colour";

    inline static const int HUE_DTLS_MAX_PAYLOAD_SIZE = 1350;
    inline static const int HUE_DTLS_STATE_INIT       = 10;
    inline static const int HUE_DTLS_STATE_CONNECTED  = 20;
    inline static const int HUE_DTLS_STATE_CLEANEDUP  = 30;

    inline static const int DTLS_PORT = 2100;
    inline static const int SSL_PORT  = 443;
    
    inline static const std::string HUE_ENTERTAINMENT_API_NEEDED = "1.22.0";

    inline static const int AREA_NAME_LEN       = 33;
    inline static const int MAX_LIGHTS_PER_AREA = 10;

    inline static const int HUE_APP_NAME_SIZE    = 21;
    inline static const int HUE_DEVICE_NAME_SIZE = 20;

    // Generic errors
    inline static const int HUE_ERR_UNAUTHORIZED        = 1;
    inline static const int HUE_ERR_INVALID_MSG         = 2;
    inline static const int HUE_ERR_NOT_AVAILABLE       = 3;
    inline static const int HUE_ERR_INCORRECT_METHOD    = 4;
    inline static const int HUE_ERR_MISSING_PARAMS      = 5;
    inline static const int HUE_ERR_PARAM_NOT_AVAILABLE = 6;
    inline static const int HUE_ERR_INVALID_VALUE       = 7;
    inline static const int HUE_ERR_NOT_MODIFIABLE      = 8;
    inline static const int HUE_ERR_TOO_MANY            = 11;
    inline static const int HUE_ERR_PORTAL_REQUIRED     = 12;
    inline static const int HUE_ERR_INTERNAL_ERROR      = 901;

    // Command specific errors
    inline static const int HUE_ERR_LINK_BUTTON_NOT_PUSHED          = 101;
    inline static const int HUE_ERR_DHCP_NOT_DISABLED               = 110;
    inline static const int HUE_ERR_INVALID_UPDATASTATE             = 111;
    inline static const int HUE_ERR_PARAM_NOT_MODIFIABLE            = 201;
    inline static const int HUE_ERR_COMMISSIONABLE_LIST_FULL        = 203;
    inline static const int HUE_ERR_GROUP_TABLE_FULL                = 301;
    inline static const int HUE_ERR_DELETE_NOT_PERMITTED            = 305;
    inline static const int HUE_ERR_ALREADY_USED                    = 306;
    inline static const int HUE_ERR_SCENE_BUFFER_FULL               = 402;
    inline static const int HUE_ERR_SCENE_LOCKED                    = 403;
    inline static const int HUE_ERR_GROUP_EMPTY                     = 404;
    inline static const int HUE_ERR_CANNOT_CREATE_SENSOR            = 501;
    inline static const int HUE_ERR_SENSOR_LIST_FULL                = 502;
    inline static const int HUE_ERR_COMMISSIONABLE_SENSOR_LIST_FULL = 503;
    inline static const int HUE_ERR_RULE_ENGINE_FULL                = 601;
    inline static const int HUE_ERR_CONDITION_ERROR                 = 607;
    inline static const int HUE_ERR_ACTION_ERROR                    = 608;
    inline static const int HUE_ERR_UNABLE_TO_ACTIVATE              = 609;
    inline static const int HUE_ERR_SCHEDULE_LIST_FULL              = 701;
    inline static const int HUE_ERR_INVALID_TIMEZONE                = 702;
    inline static const int HUE_ERR_CANNOT_SET_SCHED_TIME           = 703;
    inline static const int HUE_ERR_CANNOT_CREATE_SCHEDULE          = 704;
    inline static const int HUE_ERR_SCHEDULE_IN_PAST                = 705;
    inline static const int HUE_ERR_COMMAND_ERROR                   = 706;
    inline static const int HUE_ERR_MODEL_INVALID                   = 801;
    inline static const int HUE_ERR_FACTORY_NEW                     = 802;
    inline static const int HUE_ERR_INVALID_STATE                   = 803;

    inline static const int HUE_MSG_DEBUG = 3;
    inline static const int HUE_MSG_INFO  = 2;
    inline static const int HUE_MSG_ERR   = 1;
    inline static const int HUE_MSG_OFF   = 0;

    inline static const int DEBUG_LEVEL = HUE_MSG_ERR;
    
    class DTLS {
      private:
      SSL_CTX *cSSLContext;
      SSL *cSSL;
      BIO *cBIO;
      struct sockaddr_in cRemoteAddress;
      struct sockaddr_in cLocalAddress;
      int cPort;
      int cFD;
      std::string cUsername;
      std::string cPSK;
      int cState;
      void *cUserData;
      int cDebugLevel;
      
      public:
      int init(const std::string& username, const std::string& psk, int debugLevel);
      int connect(const std::string& address, int port);
      int sendData(void *buf, int length);
      void cleanup(); // FIXME:TripPlayer destructor?
      
      std::string getUsername() const;
      std::string getPSK() const;
      
      void debug(int level, std::string format);
    };

    class EntertainmentArea {
      public:
      uint16_t cID;
      std::string cName;
      uint16_t cLightIDs[10];
    };

    class WhiteListEntry {
      public:
      std::string cUsername;
      std::string cLastUseDate;
      std::string cCreatedDate;
      std::string cName;
    };

    class REST {
      private:
      enum class RequestType {
        GET,
        PUT,
        POST,
        DELETE
      };    
        
      int cDebugLevel;
      void* cUserData;
      std::string cAPIVersion;
      std::string cUsername;
      std::string cClientkey;
      std::string cAddress;
      int cPort;
      std::string cUploadData; /* Used for PUT/POST requests */
      std::string cReceivedData;
      CURL* cCurl;
      std::vector<std::unique_ptr<EntertainmentArea>> cEntertainmentAreas;
      std::vector<std::unique_ptr<WhiteListEntry>> cWhiteList;
      std::string cDeviceType;

      int configureCURL(RequestType requestType, const std::string& url);
      int parseErrorMessage(const std::string& message, int* outType);
      std::string extractJSONWhitelistFromConfig();
      std::string getValueFromJSONObj(json_object *jsonObj, const std::string& key);
      int parseUnauthConfigurationResponseJSON();
      int parseRegisterResponse();
      int addWhitelistEntry(const std::string& username, const std::string& data);
      int extractJSONEntriesFromWhitelist(std::string jsonWhiteList);
      void freeWhitelist();
      int parseEntertainmentGroupsJSON();
      int getConfig();
      int getWhitelist(std::vector<WhiteListEntry*>* outWhitelistEntries);
      std::string getDeviceType();
      int getUnauthConfig();
      
      public:  
      int init(const std::string& address, int port, const std::string& username, int debug_level);
      int activateStream(int group);
      int deleteUser(const std::string& username);
      int getEntertainmentAreas(std::vector<EntertainmentArea*>* outAreas);
      int validateAPIVersion();
      int registerUser(std::string* outUsername, std::string* outClientKey);
      void cleanup();
      
      void debug(int level, std::string message);
      void appendRecievedData(char* contents, size_t size);
      std::string getUploadData();
    };

    class Entertainment {
      private:
      struct MessageHeader {
        char cProtocolName[9];
        uint8_t cVersionMajor;
        uint8_t cVersionMinor;
        uint8_t cSequenceNumber; /* not used */
        uint8_t cReserved1[2];
        uint8_t cColourSpace;    /* 0x00 = RGB; 0x01 = XY Brightness */
        uint8_t cReserved2[1];
      } __attribute__((packed));

      struct MessageData {
        uint8_t cType;
        uint16_t cID;
        uint16_t cRed;
        uint16_t cGreen;
        uint16_t cBlue;
      } __attribute__((packed));

      int cLightCount;
      MessageHeader cHeader;
      MessageData* cData;
      int cBufferSize;
      void* cBuffer;

      public:
      Entertainment();
      int init(int lightCount);
      int setLightID(int index, uint16_t lightID);
      int setLight(int index, uint16_t red, uint16_t green, uint16_t blue);
      int getMessage(void **outBuffer, int *outBufferLength);
      void cleanup();
    };

    static int cval(char c);
    static int hex2bin(std::string str, unsigned char *out);
    static unsigned int psk_cb(SSL *ssl, const char* hint, char *identity, unsigned int max_identity_len, unsigned char *psk, unsigned int max_psk_len);

    static int curlTraceCallback(CURL* curl, curl_infotype type, char* data, size_t size, void* userp);
    static size_t curlReadCallback(void* ptr, size_t size, size_t nmemb, void* stream);
    static size_t curlWriteCallback(void* contents, size_t size, size_t nmemb, void* userp);
    
    int RESTInit();
    void RESTCleanup();
  
    class Bulb {
      public:
      Bulb(HueManager& parent, IResourceData& data, int id);
      Bulb(HueManager& parent, IResourceData& data, int id, JSONObject object);
      void save(JSONObject object);
      Colour& getColour();
      void sync();
      void getProperties(IPropertyMaker& owner, const Metadata& metadata, std::function<void()> removeFunction);

      private:
      HueManager& cParent;
      int cDefID;
      Colour cDefColour;
    };
    
    // External interfaces.
    IResourceData& cResourceData;

    std::string cDefBridgeAddress;
    std::string cDefBridgeUser;
    std::string cDefBridgePSK;
    std::vector<std::unique_ptr<Bulb>> cDefBulbs;
    
    DTLS cDTLS;
    Entertainment cEntertainment;
    REST cREST;
  };
}
