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
#include "HueManager.h"

namespace IsoRealms::Hue {
  const std::string HueManager::JSON_BRIDGE = "bridge";
  const std::string HueManager::JSON_BULBS  = "bulbs";
  const std::string HueManager::JSON_COLOUR = "colour";
  const std::string HueManager::JSON_ID     = "id";
  const std::string HueManager::JSON_PSK    = "psk";
  const std::string HueManager::JSON_USER   = "user";
  
  const int HueManager::DTLS_PORT = 2100;
  const int HueManager::SSL_PORT  = 443;
  
  const int HueManager::HUE_DTLS_MAX_PAYLOAD_SIZE = 1350;
  const int HueManager::HUE_DTLS_STATE_INIT       = 10;
  const int HueManager::HUE_DTLS_STATE_CONNECTED  = 20;
  const int HueManager::HUE_DTLS_STATE_CLEANEDUP  = 30;
  
  const std::string HueManager::HUE_ENTERTAINMENT_API_NEEDED = "1.22.0";

  const int HueManager::AREA_NAME_LEN        = 33;
  const int HueManager::MAX_LIGHTS_PER_AREA  = 10;

  const int HueManager::HUE_APP_NAME_SIZE    = 21;
  const int HueManager::HUE_DEVICE_NAME_SIZE = 20;

  const int HueManager::HUE_ERR_UNAUTHORIZED        = 1;
  const int HueManager::HUE_ERR_INVALID_MSG         = 2;
  const int HueManager::HUE_ERR_NOT_AVAILABLE       = 3;
  const int HueManager::HUE_ERR_INCORRECT_METHOD    = 4;
  const int HueManager::HUE_ERR_MISSING_PARAMS      = 5;
  const int HueManager::HUE_ERR_PARAM_NOT_AVAILABLE = 6;
  const int HueManager::HUE_ERR_INVALID_VALUE       = 7;
  const int HueManager::HUE_ERR_NOT_MODIFIABLE      = 8;
  //                                                  9?
  //                                                 10?
  const int HueManager::HUE_ERR_TOO_MANY          =  11;
  const int HueManager::HUE_ERR_PORTAL_REQUIRED   =  12;
  const int HueManager::HUE_ERR_INTERNAL_ERROR    = 901;

  const int HueManager::HUE_ERR_LINK_BUTTON_NOT_PUSHED          = 101;
  const int HueManager::HUE_ERR_DHCP_NOT_DISABLED               = 110;
  const int HueManager::HUE_ERR_INVALID_UPDATASTATE             = 111;
  const int HueManager::HUE_ERR_PARAM_NOT_MODIFIABLE            = 201;
  const int HueManager::HUE_ERR_COMMISSIONABLE_LIST_FULL        = 203;
  const int HueManager::HUE_ERR_GROUP_TABLE_FULL                = 301;
  const int HueManager::HUE_ERR_DELETE_NOT_PERMITTED            = 305;
  const int HueManager::HUE_ERR_ALREADY_USED                    = 306;
  const int HueManager::HUE_ERR_SCENE_BUFFER_FULL               = 402;
  const int HueManager::HUE_ERR_SCENE_LOCKED                    = 403;
  const int HueManager::HUE_ERR_GROUP_EMPTY                     = 404;
  const int HueManager::HUE_ERR_CANNOT_CREATE_SENSOR            = 501;
  const int HueManager::HUE_ERR_SENSOR_LIST_FULL                = 502;
  const int HueManager::HUE_ERR_COMMISSIONABLE_SENSOR_LIST_FULL = 503;
  const int HueManager::HUE_ERR_RULE_ENGINE_FULL                = 601;
  const int HueManager::HUE_ERR_CONDITION_ERROR                 = 607;
  const int HueManager::HUE_ERR_ACTION_ERROR                    = 608;
  const int HueManager::HUE_ERR_UNABLE_TO_ACTIVATE              = 609;
  const int HueManager::HUE_ERR_SCHEDULE_LIST_FULL              = 701;
  const int HueManager::HUE_ERR_INVALID_TIMEZONE                = 702;
  const int HueManager::HUE_ERR_CANNOT_SET_SCHED_TIME           = 703;
  const int HueManager::HUE_ERR_CANNOT_CREATE_SCHEDULE          = 704;
  const int HueManager::HUE_ERR_SCHEDULE_IN_PAST                = 705;
  const int HueManager::HUE_ERR_COMMAND_ERROR                   = 706;
  const int HueManager::HUE_ERR_MODEL_INVALID                   = 801;
  const int HueManager::HUE_ERR_FACTORY_NEW                     = 802;
  const int HueManager::HUE_ERR_INVALID_STATE                   = 803;
    
  const int HueManager::HUE_MSG_DEBUG = 3;
  const int HueManager::HUE_MSG_INFO  = 2;
  const int HueManager::HUE_MSG_ERR   = 1;
  const int HueManager::HUE_MSG_OFF   = 0;
  
  const int HueManager::DEBUG_LEVEL = HUE_MSG_ERR;
  
  HueManager::HueManager(Hue& hue, IResourceData& data) :
            cResourceData(data) {
  }
  
  HueManager::HueManager(Hue& hue, IResourceData& data, JSONObject object) :
            HueManager(hue, data) {
    cDefBridgeAddress = object.getString(JSON_BRIDGE);
    cDefBridgeUser    = object.getString(JSON_USER);
    cDefBridgePSK     = object.getString(JSON_PSK);

    for (JSONValue mBulbValue : object.getArray(JSON_BULBS)) {
      cDefBulbs.emplace_back(std::make_unique<Bulb>(*this, data, cDefBulbs.size(), mBulbValue.getObject()));
    }

    // RESTInit();
    // cREST.init(cDefBridgeAddress.c_str(), SSL_PORT, cDefBridgeUser.c_str(), DEBUG_LEVEL);

    // std::cout << "Getting entertainment areas" << std::endl;
    // std::vector<EntertainmentArea*> mEntertainmentAreas;
    // cREST.getEntertainmentAreas(&mEntertainmentAreas);
    // if (mEntertainmentAreas.size() <= 0) {
    //   cREST.cleanup();
    //   RESTCleanup();
    //   // FIXME:TripPlayer: throw
    //   throw ArgumentException("ERROR: HueManager::HueManager: No entertainement areas found.");
    // }

    // /* Count how many lights are in the entertainment area */
    // int mLightCount = 0;
    // for (int i = 0; i < MAX_LIGHTS_PER_AREA; ++i) {
    //   if (mEntertainmentAreas[0]->cLightIDs[i] == 0) {
    //     break;
    //   }
    //   mLightCount++;
    // }

    // if (mLightCount == 0) {
    //   cREST.cleanup();
    //   RESTCleanup();
    //   throw ArgumentException("ERROR: HueManager::HueManager: No lights found in entertainement area \"" + mEntertainmentAreas[0]->cName + "\".");
    // }

    // // Activate the entertainment area
    // std::cout << "Enabling entertainment area [" << mEntertainmentAreas[0]->cName << "]" << std::endl;
    // cREST.activateStream(mEntertainmentAreas[0]->cID);

    // /* Initialise hue entertainment */
    // cEntertainment.init(mLightCount);

    // /* Assign the light ID (as returned by the bridge) to each light to be controlled (0..n) */
    // for (int i = 0; i < mLightCount; i++) {
    //   cEntertainment.setLightID(i, mEntertainmentAreas[0]->cLightIDs[i]);
    // }

    // /* Connect to bridge using DTLS */
    // std::cout << "Making DTLS connection to bridge: " << cDefBridgeUser << "  " << cDefBridgePSK << std::endl;
    // cDTLS.init(cDefBridgeUser.c_str(), cDefBridgePSK.c_str(), DEBUG_LEVEL);
    // int retval = cDTLS.connect(cDefBridgeAddress.c_str(), DTLS_PORT);
    // if (retval) {
    //   cREST.cleanup();
    //   RESTCleanup();
    //   cDTLS.cleanup();
    //   cEntertainment.cleanup();
    //   throw ArgumentException("ERROR: HueManager::HueManager: Failed to make DTLS connection to bridge (returned: " + Utils::toString(retval) + ").");
    // }
    // printf("Running...\n");
  }

  void HueManager::registerAssets(ResourceAssetRegistry& assets) {
    // Nothing to do.
  }

  void HueManager::save(JSONObject object) const {
    object.addString(JSON_BRIDGE, cDefBridgeAddress);
    object.addString(JSON_PSK,    cDefBridgePSK);
    object.addString(JSON_USER,   cDefBridgeUser);
    JSONArray mBulbsArray = object.addArray(JSON_BULBS);
    for (const std::unique_ptr<Bulb>& mBulb : cDefBulbs) {
      JSONObject mBulbObject = mBulbsArray.addObject();
      mBulb->save(mBulbObject);
    }
  }

  void HueManager::hintInUse(bool inUse) {
    // Nothing to do.
  }
  
  bool HueManager::renderIcon() {
    return false;
  }
  
  void HueManager::getProperties(PropertyMaker& owner, const Metadata& metadata) {
    owner.createPropertyNativeString(metadata.getPropertyData("Address"), [this]() {return cDefBridgeAddress;}, [this](const std::string& value) {cDefBridgeAddress = value;});
    owner.createPropertyNativeString(metadata.getPropertyData("User"),    [this]() {return cDefBridgeUser;},    [this](const std::string& value) {cDefBridgeUser    = value;});
    owner.createPropertyNativeString(metadata.getPropertyData("PSK"),     [this]() {return cDefBridgePSK;},     [this](const std::string& value) {cDefBridgePSK     = value;});
    for (std::unique_ptr<Bulb>& mBulb : cDefBulbs) {
      owner.createPropertyAsset<Colour>(metadata.getPropertyData("Bulb"), mBulb->getColour(), [this, &mBulb]() {
        Utils::removeElementUnique(cDefBulbs, mBulb.get());
      });
    }
    owner.createPropertyAdd(metadata.getPropertyData("Bulb"), "Add...", [this, &owner, &metadata]() {
      cDefBulbs.emplace_back(std::make_unique<Bulb>(*this, cResourceData, cDefBulbs.size()));
      std::unique_ptr<Bulb>& mBulb  = cDefBulbs.back();
      return owner.createPropertyAsset<Colour>(metadata.getPropertyData("Bulb"), mBulb->getColour(), [this, &mBulb]() {
        Utils::removeElementUnique(cDefBulbs, mBulb.get());
      });
    });
  }

  void HueManager::removed() {
    // Nothing to do.
  }

  HueManager::~HueManager() {
    cREST.cleanup();
    RESTCleanup();
    cDTLS.cleanup();
    cEntertainment.cleanup();
  }

  void HueManager::updateRuntime(unsigned int milliseconds) {
    for (std::unique_ptr<Bulb>& mBulb : cDefBulbs) {
      mBulb->sync();
    }
  }

  void HueManager::updateEditing(unsigned int milliseconds) {
    for (std::unique_ptr<Bulb>& mBulb : cDefBulbs) {
      mBulb->sync();
    }
  }

  void HueManager::reset() {
    // FIXME:TripPlayer: Implement this
  }
  
  HueManager::Bulb::Bulb(HueManager& parent, IResourceData& data, int id) :
            cParent(parent),
            cDefID(id),
            cDefColour(data, 1.0f, 1.0f, 1.0f) {
  }
  
  HueManager::Bulb::Bulb(HueManager& parent, IResourceData& data, int id, JSONObject object) :
            Bulb(parent, data, id) {
    cDefColour.init(object, JSON_COLOUR);
  }
  
  void HueManager::Bulb::save(JSONObject object) {
    cDefColour.save(object, JSON_COLOUR);
  }

  Colour& HueManager::Bulb::getColour() {
    return cDefColour;
  }

  void HueManager::Bulb::sync() {
    // void *mMessageBuffer;
    // int mMessageBufferLength;
    // uint16_t mRed   = cDefColour->getRed()   * 65535.0f;
    // uint16_t mGreen = cDefColour->getGreen() * 65535.0f;
    // uint16_t mBlue  = cDefColour->getBlue()  * 65535.0f;
    // cParent.cEntertainment.setLight(cDefID, mRed, mGreen, mBlue);
    // cParent.cEntertainment.getMessage(&mMessageBuffer, &mMessageBufferLength);
    // if (cParent.cDTLS.sendData(mMessageBuffer, mMessageBufferLength)) {
    //   std::cout << "WARNING: HueManager::Bulb::sync: Failed to send colour data to hue system." << std::endl;
    // }
  }

  static int hue_dtls_ctx_index;

  void HueManager::DTLS::debug(int level, std::string message) {
    if (cDebugLevel >= level) {
      std::cout << "[dtls] " << message << std::endl;
    }
  }

  int HueManager::cval(char c) {
    if (c >= 'a') {
      return c - 'a' + 0x0a;
    }
    if (c >= 'A') {
      return c - 'A' + 0x0a;
    }
    return c - '0';
  }

  int HueManager::hex2bin(std::string str, unsigned char *out) {
    int i;
    for (i = 0; str[i] && str[i + 1]; i += 2) {
      if (!isxdigit(str[i]) && !isxdigit(str[i + 1])) {
        return -1;
      }
      out[i / 2] = (cval(str[i]) << 4) + cval(str[i + 1]);
    }
    return i / 2;
  }

  unsigned int HueManager::psk_cb(SSL *ssl, const char* hint, char *identity, unsigned int max_identity_len, unsigned char *psk, unsigned int max_psk_len) {
    DTLS* mDTLS = static_cast<DTLS*>(SSL_get_ex_data(ssl, hue_dtls_ctx_index));

    if (hint == nullptr) {
      mDTLS->debug(HUE_MSG_DEBUG, "nullptr received PSK identity hint, continuing anyway");
    } else {
      mDTLS->debug(HUE_MSG_DEBUG, "Received PSK identity hint '" + std::string(hint) + "'");
    }
    
    std::string mUsername = mDTLS->getUsername();
    std::string mPSK = mDTLS->getPSK();
    int ret = snprintf(identity, max_identity_len, "%s", mUsername.c_str());
    if (ret < 0 || (unsigned int) ret > max_identity_len) {
      mDTLS->debug(HUE_MSG_ERR, "Error, psk_identify too long");
      return 0;
    }

    if (mPSK.length() >= (max_psk_len * 2)) {
      mDTLS->debug(HUE_MSG_ERR,  "Error, psk_key too long");
      return 0;
    }

    /* convert the PSK key to binary */
    ret = hex2bin(mPSK, psk);
    if (ret <= 0) {
      mDTLS->debug(HUE_MSG_ERR, std::string("Error, Could not convert PSK key '") + mPSK + "' to binary key");
      return 0;
    }
    return ret;
  }

  int HueManager::DTLS::sendData(void *buf, int length) {
    if (cState != HUE_DTLS_STATE_CONNECTED) {
      debug(HUE_MSG_ERR, "dtls_send_data> hue_dtls_ctx wrong state (" + Utils::toString(cState) + " vs expected " + Utils::toString(HUE_DTLS_STATE_CONNECTED) + ")");
      return -1;
    }

    debug(HUE_MSG_DEBUG, "about to write " + Utils::toString(length) + " bytes");
    int len = SSL_write(cSSL, buf, length);
    
    switch (SSL_get_error(cSSL, len)) {
      case SSL_ERROR_NONE: {
        debug(HUE_MSG_DEBUG, "wrote " + Utils::toString(len) + " bytes");
        break;
      }
      case SSL_ERROR_WANT_WRITE: {
        /* Just try again later */
        break;
      }
      case SSL_ERROR_WANT_READ: {
        /* continue with reading */
        break;
      }
      case SSL_ERROR_SYSCALL: {
        debug(HUE_MSG_ERR, "Socket write error: ");
        debug(HUE_MSG_ERR, std::string(ERR_error_string(ERR_get_error(), static_cast<char*>(buf))) + " (" + Utils::toString(SSL_get_error(cSSL, len)) + ")");
        return -1;
      }
      case SSL_ERROR_SSL: {
        debug(HUE_MSG_ERR, "SSL write error: ");
        debug(HUE_MSG_ERR, std::string(ERR_error_string(ERR_get_error(), static_cast<char*>(buf))) + " (" + Utils::toString(SSL_get_error(cSSL, len)) + ")");
        return -1;
      }
      default: {
        debug(HUE_MSG_ERR, "Unexpected error while writing!");
        return -1;
      }
    }
    return 0;
  }

  int HueManager::DTLS::init(const std::string& username, const std::string& psk, int debugLevel) {
    static bool first_run = true;
    cDebugLevel = debugLevel;

    debug(HUE_MSG_INFO, std::string("hue_dtls_init() ") + (first_run == 1 ? "(first run)" : ""));

    if (first_run) {
      first_run = false;
      OpenSSL_add_ssl_algorithms();
      SSL_load_error_strings();
      hue_dtls_ctx_index = SSL_get_ex_new_index(0, const_cast<char*>("hue_dtls_ctx index"), nullptr, nullptr, nullptr);
    }
    cUsername = username;
    cPSK = psk;

    cFD = -1;
    cState = HUE_DTLS_STATE_INIT;
    return 0;
  }

  void HueManager::DTLS::cleanup() {
    debug(HUE_MSG_INFO, "dtls_cleanup() %s");

    if (cSSLContext) {
      SSL_CTX_free(cSSLContext);
      cSSLContext = nullptr;
    }

    if (cSSL) {
      SSL_shutdown(cSSL);
      SSL_free(cSSL);
      cSSL = nullptr;
    }

    if (cFD != -1) {
      close(cFD);
      cFD = -1;
    }
    cState = HUE_DTLS_STATE_CLEANEDUP;
  }

  int HueManager::DTLS::connect(const std::string& address, int port) {
    int retval;
    char err_buf[200];

    if (cState != HUE_DTLS_STATE_INIT) {
      debug(HUE_MSG_ERR, "hue_dtls_connect> hue_dtls_ctx wrong state (" + Utils::toString(cState) + " vs expected " + Utils::toString(HUE_DTLS_STATE_INIT) + ")");
      return -1;
    }
    debug(HUE_MSG_INFO, "Connecting to " + address + ":" + Utils::toString(port));

    memset((void *) &cRemoteAddress, 0, sizeof(struct sockaddr_in));
    memset((void *) &cLocalAddress, 0, sizeof(struct sockaddr_in));

    if (inet_pton(AF_INET, address.c_str(), &cRemoteAddress.sin_addr) == 1) {
      cRemoteAddress.sin_family = AF_INET;
      cRemoteAddress.sin_port = htons(port);
    } else {
      debug(HUE_MSG_ERR, "inet_pton failed");
    }

    cFD = socket(cRemoteAddress.sin_family, SOCK_DGRAM, 0);
    if (cFD < 0) {
      debug(HUE_MSG_ERR, "Failed to create socket");
      return -1;
    }
    cSSLContext = SSL_CTX_new(DTLS_client_method());
    SSL_CTX_set_min_proto_version(cSSLContext, DTLS1_2_VERSION);
    SSL_CTX_set_psk_client_callback(cSSLContext, psk_cb);

  #if OPENSSL_VERSION_NUMBER >= 0x10101000L
    SSL_CTX_set_ciphersuites(cSSLContext, "TLS_PSK_WITH_AES_128_GCM_SHA256");
  #endif

    cSSL = SSL_new(cSSLContext);

    /* Save a reference to the dtls ctx struct so callback can access it */
    SSL_set_ex_data(cSSL, hue_dtls_ctx_index, this);

    /* Create BIO, connect and set to already connected */
    cBIO = BIO_new_dgram(cFD, BIO_CLOSE);
    ::connect(cFD, (struct sockaddr *) &cRemoteAddress, sizeof(struct sockaddr_in));
    BIO_ctrl(cBIO, BIO_CTRL_DGRAM_SET_CONNECTED, 0, &cRemoteAddress);
    SSL_set_bio(cSSL, cBIO, cBIO);

    retval = SSL_connect(cSSL);
    if (retval <= 0) {
      switch (SSL_get_error(cSSL, retval)) {
        case SSL_ERROR_ZERO_RETURN:      debug(HUE_MSG_ERR, "SSL_connect failed with SSL_ERROR_ZERO_RETURN");      break;
        case SSL_ERROR_WANT_READ:        debug(HUE_MSG_ERR, "SSL_connect failed with SSL_ERROR_WANT_READ");        break;
        case SSL_ERROR_WANT_WRITE:       debug(HUE_MSG_ERR, "SSL_connect failed with SSL_ERROR_WANT_WRITE");       break;
        case SSL_ERROR_WANT_CONNECT:     debug(HUE_MSG_ERR, "SSL_connect failed with SSL_ERROR_WANT_CONNECT");     break;
        case SSL_ERROR_WANT_ACCEPT:      debug(HUE_MSG_ERR, "SSL_connect failed with SSL_ERROR_WANT_ACCEPT");      break;
        case SSL_ERROR_WANT_X509_LOOKUP: debug(HUE_MSG_ERR, "SSL_connect failed with SSL_ERROR_WANT_X509_LOOKUP"); break;
        case SSL_ERROR_SYSCALL:          debug(HUE_MSG_ERR, "SSL_connect failed with SSL_ERROR_SYSCALL");          break;
        case SSL_ERROR_SSL:              debug(HUE_MSG_ERR, "SSL_connect failed with SSL_ERROR_SSL");              break;
        default:                         debug(HUE_MSG_ERR, "SSL_connect failed with unknown error");              break;
      }
      debug(HUE_MSG_ERR, std::string(ERR_error_string(ERR_get_error(), err_buf)) + " (" + Utils::toString(SSL_get_error(cSSL, sizeof(err_buf))) + ")");
      debug(HUE_MSG_ERR, "errno = " + Utils::toString(errno) + " (" + strerror(errno) + ")");
      return -1;
    }
    debug(HUE_MSG_DEBUG, "Connected; Cipher: " + std::string(SSL_CIPHER_get_name(SSL_get_current_cipher(cSSL))));
    cState = HUE_DTLS_STATE_CONNECTED;
    return 0;
  }
  
  std::string HueManager::DTLS::getUsername() const {
    return cUsername;
  }
  
  std::string HueManager::DTLS::getPSK() const {
    return cPSK;
  }

  void HueManager::REST::debug(int level, std::string message) {
    if (cDebugLevel >= level) {
      std::cout << "[hue_rest] " << message << std::endl;
    }
  }

  int compareVersionString(const std::string& needed, const std::string& found) {
    unsigned version_major_needed = 0;
    unsigned version_minor_needed = 0;
    unsigned version_patch_needed = 0;

    unsigned version_major_found = 0;
    unsigned version_minor_found = 0;
    unsigned version_patch_found = 0;

    sscanf(needed.c_str(), "%u.%u.%u", &version_major_needed, &version_minor_needed, &version_patch_needed);
    sscanf(found.c_str(), "%u.%u.%u", &version_major_found, &version_minor_found, &version_patch_found);

    if (version_major_needed < version_major_found) {
      return 1;
    }
    if (version_major_needed > version_major_found) {
      return -1;
    }

    if (version_minor_needed < version_minor_found) {
      return 1;
    }
    if (version_minor_needed > version_minor_found) {
      return -1;
    }

    if (version_patch_needed < version_patch_found) {
      return 1;
    }
    if (version_patch_needed > version_patch_found) {
      return -1;
    }
    return 0;
  }

  int HueManager::RESTInit() {
    return curl_global_init(CURL_GLOBAL_DEFAULT);
  }

  void HueManager::RESTCleanup() {
    curl_global_cleanup();
  }

  void HueManager::REST::freeWhitelist() {
    cWhiteList.clear();
  }

  int HueManager::REST::init(const std::string& address, int port, const std::string& username, int debugLevel) {
    cDebugLevel = debugLevel;
    cUsername = username;
    cAddress = address;
    cPort = port;
    return 0;
  }

  void HueManager::REST::cleanup() {
    cEntertainmentAreas.clear();
    freeWhitelist();
  }

  size_t HueManager::curlReadCallback(void* ptr, size_t size, size_t nmemb, void* stream) {
    REST* mRest = static_cast<REST*>(stream);
    size_t max_size = size * nmemb;

    /* Ideally, we want to copy all of ctx->upload_data into ptr. But check there's space first */
    std::string mUploadData = mRest->getUploadData();
    size_t mBytesToCopy = std::min(max_size, mUploadData.length());
    memcpy(ptr, mUploadData.c_str(), mBytesToCopy);
    mRest->debug(HUE_MSG_INFO, " > " + std::string(static_cast<char*>(ptr)));
    return mBytesToCopy;
  }

  int HueManager::curlTraceCallback(CURL* curl, curl_infotype type, char* data, size_t size, void* userp) {
    REST* mRest = static_cast<REST*>(userp);
    if (type == CURLINFO_TEXT) {
      mRest->debug(HUE_MSG_DEBUG, "curl: " + std::string(data));
    }
    return 0;
  }

  size_t HueManager::curlWriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    REST *mRest = static_cast<REST*>(userp);
    size_t mRealSize = size * nmemb;
    mRest->debug(HUE_MSG_DEBUG, "curl_write_cb> recieved " + Utils::toString(static_cast<int>(mRealSize)) + " bytes");
    mRest->appendRecievedData(static_cast<char*>(contents), mRealSize);
    return nmemb;
  }

  int HueManager::REST::configureCURL(RequestType requestType, const std::string& url) {
    cCurl = curl_easy_init();
    cReceivedData = "";
    if (cCurl) {
      curl_easy_setopt(cCurl, CURLOPT_DEBUGFUNCTION, curlTraceCallback);
      curl_easy_setopt(cCurl, CURLOPT_DEBUGDATA, this);
      curl_easy_setopt(cCurl, CURLOPT_VERBOSE, 1L);
      curl_easy_setopt(cCurl, CURLOPT_WRITEFUNCTION, curlWriteCallback);
      curl_easy_setopt(cCurl, CURLOPT_WRITEDATA, this);
      curl_easy_setopt(cCurl, CURLOPT_SSL_VERIFYPEER, 0);
      curl_easy_setopt(cCurl, CURLOPT_SSL_VERIFYHOST, 0);
      curl_easy_setopt(cCurl, CURLOPT_READFUNCTION, curlReadCallback);
      curl_easy_setopt(cCurl, CURLOPT_TIMEOUT, 10L);

      if (requestType == RequestType::PUT) {
        curl_easy_setopt(cCurl, CURLOPT_UPLOAD, 1L);
        curl_easy_setopt(cCurl, CURLOPT_READDATA, this);

        /* provide the size of the upload, we specicially typecast the value
          to curl_off_t since we must be sure to use the correct data size */
        curl_easy_setopt(cCurl, CURLOPT_INFILESIZE_LARGE, static_cast<curl_off_t>(cUploadData.length()));
      } else if (requestType == RequestType::DELETE) {
        curl_easy_setopt(cCurl, CURLOPT_CUSTOMREQUEST, "DELETE");
      } else if (requestType == RequestType::POST) {
        curl_easy_setopt(cCurl, CURLOPT_POSTFIELDS, cUploadData.c_str());
      }
      curl_easy_setopt(cCurl, CURLOPT_URL, url.c_str());
      CURLcode mCURLCode = curl_easy_perform(cCurl);

      /* Check for errors */
      if (mCURLCode != CURLE_OK) {
        debug(HUE_MSG_ERR, "curl_easy_perform() failed: " + std::string(curl_easy_strerror(mCURLCode)));
      } else {
        debug(HUE_MSG_INFO, " < " + cReceivedData);
      }
      curl_easy_cleanup(cCurl);

      return (mCURLCode == CURLE_OK ? 0 : -1);
    } else {
      debug(HUE_MSG_ERR, "curl_easy_init() failed");
      return -1;
    }
  }

  int HueManager::REST::activateStream(int group) {
    cUploadData = "{\"stream\":{\"active\":true}}";
    std::string mURL = "https://" + cAddress + ":" + Utils::toString(cPort) + "/api/" + cUsername + "/groups/" + Utils::toString(group);
    debug(HUE_MSG_INFO, "URL = " + mURL);
    int mReturnValue = configureCURL(RequestType::PUT, mURL);
    cUploadData = "";
    return mReturnValue;
  }

  int HueManager::REST::parseErrorMessage(const std::string& message, int* outType) {
    debug(HUE_MSG_DEBUG, "parse_error_message> " + message);

    json_object* mJSONObject = json_tokener_parse(message.c_str());
    if (mJSONObject == nullptr) {
      debug(HUE_MSG_ERR, "Failed to parse JSON received: " + message);
      return -1;
    }
    *outType = 0;

    if (!json_object_is_type(mJSONObject, json_type_array)) {
      debug(HUE_MSG_DEBUG, "Not an error message (not an array)");
      json_object_put(mJSONObject);
      return 0;
    }

    if (json_object_array_length(mJSONObject) <= 0) {
      debug(HUE_MSG_DEBUG, "Not an error");
      json_object_put(mJSONObject);
      return 0;
    }

    json_object *mObj = json_object_array_get_idx(mJSONObject, 0);
    json_object *mObjParam = nullptr;
    json_object_object_get_ex(mObj, "error", &mObjParam);
    if (mObjParam == nullptr) {
      debug(HUE_MSG_DEBUG, "Not an error message");
      json_object_put(mJSONObject);
      return 0;
    }

    std::string mErrorMessage = json_object_get_string(mObjParam);

    json_object_put(mJSONObject);
    mJSONObject = json_tokener_parse(mErrorMessage.c_str());

    json_object_object_get_ex(mJSONObject, "type", &mObjParam);

    if (mObjParam == nullptr) {
      debug(HUE_MSG_ERR, "Failed to find type in error message");
      json_object_put(mJSONObject);
      return -1;
    }
    *outType = json_object_get_int(mObjParam);
    debug(HUE_MSG_DEBUG, "error type: " + Utils::toString(*outType));
    json_object_put(mJSONObject);
    return 1;
  }

  std::string HueManager::REST::extractJSONWhitelistFromConfig() {
    json_object *mJSONObject = json_tokener_parse(cReceivedData.c_str());
    if (mJSONObject == nullptr) {
      debug(HUE_MSG_ERR, "Failed to parse JSON received: " + cReceivedData);
      return "";
    }

    if (json_object_get_type(mJSONObject) != json_type_object) {
      debug(HUE_MSG_INFO, "Unexpected JSON received");
      json_object_put(mJSONObject);
      return "";
    }
    
    struct json_object_iterator mIterator = json_object_iter_begin(mJSONObject);
    struct json_object_iterator mEnd = json_object_iter_end(mJSONObject);
    while (!json_object_iter_equal(&mIterator, &mEnd)) {
      if (!strcmp("whitelist", json_object_iter_peek_name(&mIterator))) {
        const std::string mWhitelistJSON = json_object_get_string(json_object_iter_peek_value(&mIterator));
        json_object_put(mJSONObject);
        return mWhitelistJSON;
      }
      json_object_iter_next(&mIterator);
    }
    json_object_put(mJSONObject);
    return "";
  }

  std::string HueManager::REST::getValueFromJSONObj(json_object *jsonObj, const std::string& key) {
    json_object *mObjParam = nullptr;
    if (!json_object_object_get_ex(jsonObj, key.c_str(), &mObjParam)) {
      debug(HUE_MSG_ERR, "get_value_from_jobj> Failed to get [" + key + "] from JSON");
      return "";
    }

    const char* mParamValue = json_object_get_string(mObjParam);
    if (mParamValue == nullptr) {
      debug(HUE_MSG_ERR, "get_value_from_jobj> failed to get value for [" + key + "]");
      return "";
    }
    return mParamValue;
  }

  // {"name":"Hue Bridge","datastoreversion":"99","swversion":"1943185030","apiversion":"1.43.0","mac":"00:17:88:2d:30:81","bridgeid":"001788FFFE2D3081","factorynew":false,"replacesbridgeid":null,"modelid":"BSB002","starterkitid":""}
  int HueManager::REST::parseUnauthConfigurationResponseJSON() {
    debug(HUE_MSG_DEBUG, "parse_unauth_configuration_response> " + cReceivedData);

    json_object* mJSONObject = json_tokener_parse(cReceivedData.c_str());
    if (mJSONObject == nullptr) {
      debug(HUE_MSG_ERR, "parse_unauth_configuration_response> Failed to parse JSON received: " + cReceivedData);
      return -1;
    }
    cAPIVersion = getValueFromJSONObj(mJSONObject, "apiversion");
    json_object_put(mJSONObject);
    return 0;
  }

  // [{"success":{"username":"xM7Kno9zv7J8vIiM0rTjPU1NJsMjJpafXG4q8yqQ","clientkey":"B95676C8F5E21AEAD54E5D8A38844A21"}}]
  int HueManager::REST::parseRegisterResponse() {
    debug(HUE_MSG_DEBUG, "parse_register_response> " + cReceivedData);

    json_object *mJSONObject = json_tokener_parse(cReceivedData.c_str());
    if (mJSONObject == nullptr) {
      debug(HUE_MSG_ERR, "parse_register_response> Failed to parse JSON received: " + cReceivedData);
      return -1;
    }

    json_object *mObj = json_object_array_get_idx(mJSONObject, 0);
    // FIXME:TripPlayer obj type check
    json_object *mObjParam = nullptr;
    json_object_object_get_ex(mObj, "success", &mObjParam);
    if (mObjParam == nullptr) {
      debug(HUE_MSG_ERR, "parse_register_response> Not an success message");
      json_object_put(mJSONObject);
      return -1;
    }
    cUsername = getValueFromJSONObj(mObjParam, "username");
    cClientkey = getValueFromJSONObj(mObjParam, "clientkey");
    json_object_put(mJSONObject);
    return 0;
  }

  /* Populate entry_ptr with username, and last use date / created date / name extracted from data.
  * Note that memory is allocted for entry_ptr->last_use_date, ->created_date, etc. so must be free'd
  * when finished with (dealt with by free_whitelist())
  */
  int HueManager::REST::addWhitelistEntry(const std::string& username, const std::string& data) {
    json_object *mJSONObject = json_tokener_parse(data.c_str());
    if (mJSONObject == nullptr) {
      debug(HUE_MSG_ERR, "Failed to parse JSON: " + data);
      return -1;
    }
    cWhiteList.emplace_back(std::make_unique<WhiteListEntry>());
    cWhiteList.back()->cLastUseDate = getValueFromJSONObj(mJSONObject, "last use date");
    cWhiteList.back()->cCreatedDate = getValueFromJSONObj(mJSONObject, "create date");
    cWhiteList.back()->cName        = getValueFromJSONObj(mJSONObject, "name");
    cWhiteList.back()->cUsername    = username;
    json_object_put(mJSONObject);
    return 0;
  }

  /* Extract the app whitelist from the passed in whitelist extracted from a config api request,
  * and store in ctx->whitelist
  */
  int HueManager::REST::extractJSONEntriesFromWhitelist(std::string jsonWhiteList) {
    freeWhitelist();
    if (jsonWhiteList.empty()) {
      debug(HUE_MSG_INFO, "Empty whitelist!");
      return -1;
    }

    json_object *mJSONObject = json_tokener_parse(jsonWhiteList.c_str());
    if (mJSONObject == nullptr) {
      debug(HUE_MSG_ERR, "Failed to parse JSON whitelist: " + jsonWhiteList);
      return -1;
    }
    
    struct json_object_iterator mIterator = json_object_iter_begin(mJSONObject);
    struct json_object_iterator mEnd = json_object_iter_end(mJSONObject);
    while (!json_object_iter_equal(&mIterator, &mEnd)) {
      addWhitelistEntry(json_object_iter_peek_name(&mIterator), json_object_get_string(json_object_iter_peek_value(&mIterator)));
      json_object_iter_next(&mIterator);
    }
    json_object_put(mJSONObject);
    return 0;
  }

  /* Extract entertainement areas from "/groups" api request, and return in out_areas/out_areas_count.
  * Note: On success, memory is allocated for out_areas, this must be free'd by the caller.
  */
  int HueManager::REST::parseEntertainmentGroupsJSON() {
    json_object *mJSONObject = json_tokener_parse(cReceivedData.c_str());
    if (mJSONObject == nullptr) {
      debug(HUE_MSG_DEBUG, "Failed to parse JSON received: " + cReceivedData);
      return -1;
    }

    int mReturnValue = 0;
    int mErrorType;
    if ((mReturnValue = parseErrorMessage(cReceivedData, &mErrorType))) {
      if (mReturnValue < 0) {
        debug(HUE_MSG_ERR, "Failed to parse groups response");
      } else {
        if (mErrorType == HUE_ERR_UNAUTHORIZED) {
          debug(HUE_MSG_ERR, "Get groups failed: Unauthorized."); /* Link button on the bridge hasn't been pressed in the last 30s */
        } else {
          debug(HUE_MSG_ERR, "Get groups failed: Unexpected error type (" + Utils::toString(mErrorType) +") received from bridge");
        }
        mReturnValue = mErrorType;
      }
      json_object_put(mJSONObject);
      return mReturnValue;
    }

    if (json_object_get_type(mJSONObject) != json_type_object) {
      debug(HUE_MSG_DEBUG, "Unexpected JSON received");
      json_object_put(mJSONObject);
      return -1;
    }

    /* Add areas found with ligts to out_areas */
    struct json_object_iterator mIterator = json_object_iter_begin(mJSONObject);
    struct json_object_iterator mEnd = json_object_iter_end(mJSONObject);
    
    while (!json_object_iter_equal(&mIterator, &mEnd)) {
      struct json_object *mObj = json_object_iter_peek_value(&mIterator);
      json_object *mObjParam = nullptr;
      json_object_object_get_ex(mObj, "type", &mObjParam);

      if (!strcmp("Entertainment", json_object_get_string(mObjParam))) {
        cEntertainmentAreas.emplace_back(std::make_unique<EntertainmentArea>());
        json_object_object_get_ex(mObj, "name", &mObjParam);
        cEntertainmentAreas.back()->cName = json_object_get_string(mObjParam);
        cEntertainmentAreas.back()->cID = atoi(json_object_iter_peek_name(&mIterator));
        json_object_object_get_ex(mObj, "lights", &mObjParam);
        for (size_t i = 0; i < json_object_array_length(mObjParam) && i < MAX_LIGHTS_PER_AREA; ++i) {
          json_object *mLightObj = json_object_array_get_idx(mObjParam, i);
          cEntertainmentAreas.back()->cLightIDs[i] = json_object_get_int(mLightObj);
        }
      }
      json_object_iter_next(&mIterator);
    }
    json_object_put(mJSONObject);
    return 0;
  }

  int HueManager::REST::getEntertainmentAreas(std::vector<EntertainmentArea*>* outAreas) {
    std::string mURL = "https://" + cAddress + ":" + Utils::toString(cPort) + "/api/" + cUsername + "/groups";
    debug(HUE_MSG_INFO, "URL = " + mURL);
    int mReturnValue = configureCURL(RequestType::GET, mURL);

    cEntertainmentAreas.clear();

    if (mReturnValue) {
      debug(HUE_MSG_DEBUG, "GET request failed.");
      return -1;
    }

    if (parseEntertainmentGroupsJSON()) {
      debug(HUE_MSG_DEBUG, "Failed to get entertainment group");
      return -1;
    }

    for (std::unique_ptr<EntertainmentArea>& mEntertainmentArea : cEntertainmentAreas) {
      outAreas->push_back(mEntertainmentArea.get());
    }
    return mReturnValue;
  }

  /* Get the bridge config, and if successfull, populate whitelist. */
  int HueManager::REST::getConfig() {
    freeWhitelist();
    std::string mURL = "https://" + cAddress + ":" + Utils::toString(cPort) + "/api/" + cUsername + "/config";
    debug(HUE_MSG_INFO, "URL = " + mURL);
    int mReturnValue = configureCURL(RequestType::GET, mURL);
    if (mReturnValue) {
      debug(HUE_MSG_DEBUG, "GET request failed.");
      return -1;
    }
    std::string mWhitelistJSON = extractJSONWhitelistFromConfig();
    extractJSONEntriesFromWhitelist(mWhitelistJSON);
    return mReturnValue;
  }

  int HueManager::REST::getWhitelist(std::vector<WhiteListEntry*>* outWhitelistEntries) {
    if (getConfig()) {
      return -1;
    }

    for (std::unique_ptr<WhiteListEntry>& mWhiteListEntry : cWhiteList) {
      outWhitelistEntries->emplace_back(mWhiteListEntry.get());
    }
    return 0;
  }

  std::string HueManager::REST::getDeviceType() {
    char mDeviceName[HUE_DEVICE_NAME_SIZE];
    gethostname(mDeviceName, sizeof(mDeviceName));
    cDeviceType = "IsoRealms-" + std::string(__progname) + "#" + mDeviceName;
    if (cDeviceType.length() > 40) {
      cDeviceType = cDeviceType.substr(0, 40);
    }
    return cDeviceType;
  }

  int HueManager::REST::deleteUser(const std::string& username) {
    std::string mURL = "https://" + cAddress + ":" + Utils::toString(cPort) + "/api/" + cUsername + "/config/whitelist/" + username;
    debug(HUE_MSG_INFO, "URL = " + mURL);
    int mReturnValue = configureCURL(RequestType::DELETE, mURL);
    if (mReturnValue) {
      debug(HUE_MSG_ERR, "Delete request failed.");
      return -1;
    }
    return mReturnValue;
  }

  int HueManager::REST::getUnauthConfig() {
    std::string mURL = "https://" + cAddress + ":" + Utils::toString(cPort) + "/api/config";
    debug(HUE_MSG_INFO, "URL = " + mURL);
    int mReturnValue = configureCURL(RequestType::GET, mURL);
    cAPIVersion = "";
    if (mReturnValue) {
      debug(HUE_MSG_DEBUG, "GET request failed.");
      return -1;
    }
    if (parseUnauthConfigurationResponseJSON()) {
      debug(HUE_MSG_DEBUG, "Failed to get unauth config");
      return -1;
    }
    return mReturnValue;
  }

  int HueManager::REST::validateAPIVersion() {
    std::string mAPIVersionNeeded = HUE_ENTERTAINMENT_API_NEEDED;

    if (getUnauthConfig()) {
      debug(HUE_MSG_DEBUG, "Failed to compare version string");
      return -1;
    }

    int mReturnValue = compareVersionString(mAPIVersionNeeded, cAPIVersion);
    if (mReturnValue < 0) {
      debug(HUE_MSG_INFO, "api version found (" + cAPIVersion + ") is to low. must be >= 1.22.0");
      return -1;
    }
    debug(HUE_MSG_DEBUG, "api version found (" + cAPIVersion + ") is compatible");
    return 0;
  }

  int HueManager::REST::registerUser(std::string* outUsername, std::string* outClientKey) {
    *outUsername = "";
    *outClientKey = "";
    std::string mURL = "https://" + cAddress + ":" + Utils::toString(cPort) + "/api";
    debug(HUE_MSG_INFO, "URL = " + mURL);
    std::string mDeviceType = getDeviceType();
    cUploadData = "{\"devicetype\":\"" + mDeviceType + "\",\"generateclientkey\":true}";
    debug(HUE_MSG_INFO, "Body = " + cUploadData);
    int mReturnValue = configureCURL(RequestType::POST, mURL);
    cUploadData = "";
    if (mReturnValue) {
      debug(HUE_MSG_ERR, "Register failed.");
      return -1;
    }

    int mErrorType;
    if ((mReturnValue = parseErrorMessage(cReceivedData, &mErrorType))) {
      if (mReturnValue < 0) {
        debug(HUE_MSG_ERR, "Register failed");
      } else {
        if (mErrorType == HUE_ERR_LINK_BUTTON_NOT_PUSHED) {
          debug(HUE_MSG_ERR, "Register failed: Link button on the bridge not pressed within last 30 seconds");
        } else {
          debug(HUE_MSG_ERR, "Register failed: Unexpected error type (" + Utils::toString(mErrorType) + ") received from bridge");
        }
        mReturnValue = mErrorType;
      }
    } else {
      /* Not an error message, so assume it worked */
      if (parseRegisterResponse()) {
        return -2;
      } else {
        *outUsername = cUsername;
        *outClientKey = cClientkey;
        mReturnValue = 0;
      }
    }
    return mReturnValue;
  }

  void HueManager::REST::appendRecievedData(char* contents, size_t size) {
    cReceivedData.append(contents, size);
  }

  std::string HueManager::REST::getUploadData() {
    return cUploadData;
  }
  
  HueManager::Entertainment::Entertainment() :
    cLightCount(0),
    cData(nullptr),
    cBufferSize(0),
    cBuffer(nullptr) {
  }

  int HueManager::Entertainment::init(int lightCount) {
    cLightCount = lightCount;

    /* Allocate memory for light data */
    cData = static_cast<MessageData*>(calloc(lightCount, sizeof(MessageData)));
    if (!cData) {
      return -1;
    }
    memcpy(cHeader.cProtocolName, "HueStream", sizeof("HueStream") - 1);
    cHeader.cVersionMajor = 1;
    cHeader.cVersionMinor = 0;
    cHeader.cSequenceNumber = 0;
    cHeader.cReserved1[0] = 0;
    cHeader.cReserved1[1] = 0;
    cHeader.cColourSpace = 0; 
    cHeader.cReserved2[0] = 0;

    /* Allocate memory for full message */
    cBufferSize = sizeof(MessageHeader) + (cLightCount * sizeof(MessageData));
    cBuffer = malloc(cBufferSize);
    if (!cBuffer) {
      return -1;
    }
    memset(cBuffer, 0, cBufferSize);
    return 0;
  }

  int HueManager::Entertainment::setLightID(int index, uint16_t lightID) {
    if (index >= cLightCount) {
      return -1;
    }
    MessageData* mData = &cData[index];
    mData->cID = htons(lightID);
    return 0;
  }

  int HueManager::Entertainment::setLight(int index, uint16_t red, uint16_t green, uint16_t blue) {
    if (index >= cLightCount) {
      return -1;
    }
    MessageData* mData = &cData[index];
    mData->cRed = htons(red);
    mData->cGreen = htons(green);
    mData->cBlue = htons(blue);
    return 0;
  }

  int HueManager::Entertainment::getMessage(void **outBuffer, int *outBufferSize) {
    memcpy(cBuffer, &cHeader, sizeof(MessageHeader));
    memcpy((uint8_t*) cBuffer + sizeof(MessageHeader), cData, (cLightCount * sizeof(MessageData))); 
    
    *outBuffer = cBuffer;
    *outBufferSize = cBufferSize;
    return 0;
  }

  void HueManager::Entertainment::cleanup() {
    if (cBuffer) {
      free(cBuffer);
      cBuffer = nullptr;
    }

    if (cData) {
      free(cData);
      cData = nullptr;
    }
  }
}
