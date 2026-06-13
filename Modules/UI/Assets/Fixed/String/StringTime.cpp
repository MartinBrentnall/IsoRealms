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
#include "StringTime.h"

namespace IsoRealms::UI {
  StringTime::StringTime(const Metadata& /*metadata*/, IResourceData& owner) :
            cDefValue(owner),
            format(DEFAULT_FORMAT) {
    parseFormatString();
  }

  StringTime::StringTime(const Metadata& metadata, IResourceData& owner, JSONObject object) :
            StringTime(metadata, owner) {
    cDefValue.set(object, JSON_VALUE);
    format = object.getString(JSON_FORMAT, DEFAULT_FORMAT);
    parseFormatString();
  }

  void StringTime::parseFormatString() {
    cFormatElements.clear();
    size_t mPosition = 0;
    while (mPosition < format.size()) {
      size_t mOpenPosition = format.find('{', mPosition);
      if (mOpenPosition == std::string::npos) {
        cFormatElements.emplace_back(format.substr(mPosition));
        break;
      }
      if (mOpenPosition > mPosition) {
        cFormatElements.emplace_back(format.substr(mPosition, mOpenPosition - mPosition));
      }
      size_t mClosePosition = format.find('}', mOpenPosition);

      if (mClosePosition == std::string::npos) {

        // Malformed - treat the rest as literal
        cFormatElements.emplace_back(format.substr(mOpenPosition));
        break;
      }

      std::string mTokenString = format.substr(mOpenPosition + 1, mClosePosition - mOpenPosition - 1);

      // Parse unit and optional width: e.g. "ms:02"
      std::string mUnit = mTokenString;
      int mWidth = 0;
      bool mZeroPad = false;

      size_t mColonPosition = mTokenString.find(':');
      if (mColonPosition != std::string::npos) {
        mUnit = mTokenString.substr(0, mColonPosition);
        std::string widthStr = mTokenString.substr(mColonPosition + 1);
        if (!widthStr.empty() && widthStr[0] == '0') {
          mZeroPad = true;
        }
        size_t i = 0;
        while (i < widthStr.size() && std::isdigit(widthStr[i])) {
          ++i;
        }
        if (i > 0) {
          mWidth = std::stoi(widthStr.substr(0, i));
        }
      }
      cFormatElements.emplace_back(mUnit, mWidth, mZeroPad);

      mPosition = mClosePosition + 1;
    }
  }

  std::string StringTime::getValue() const {
    std::chrono::milliseconds mDuration(cDefValue->getValue());

    std::chrono::days mDays = std::chrono::duration_cast<std::chrono::days>(mDuration);
    mDuration -= mDays;

    std::chrono::hours mHours = std::chrono::duration_cast<std::chrono::hours>(mDuration);
    mDuration -= mHours;

    std::chrono::minutes mMinutes = std::chrono::duration_cast<std::chrono::minutes>(mDuration);
    mDuration -= mMinutes;

    std::chrono::seconds mSeconds = std::chrono::duration_cast<std::chrono::seconds>(mDuration);
    mDuration -= mSeconds;

    int mValDays = static_cast<int>(mDays.count());
    int mValHours = static_cast<int>(mHours.count());
    int mValMinutes = static_cast<int>(mMinutes.count());
    int mValSeconds = static_cast<int>(mSeconds.count());
    int mValMilliseconds = static_cast<int>(mDuration.count()); // 0..999

    std::ostringstream mOutput;

    for (size_t i = 0; i < cFormatElements.size(); ++i) {
      if (!cFormatElements[i].isToken) {
        mOutput << cFormatElements[i].literal;
      } else {
        std::string mUnit = cFormatElements[i].unit;
        int mWidth = cFormatElements[i].width;
        bool mZeroPad = cFormatElements[i].zeroPad;

        if (mUnit == "d") {
          if (mZeroPad && mWidth > 0) {
            mOutput << std::setw(mWidth) << std::setfill('0') << mValDays;
          } else {
            mOutput << mValDays;
          }
        } else if (mUnit == "h") {
          if (mZeroPad && mWidth > 0) {
            mOutput << std::setw(mWidth) << std::setfill('0') << mValHours;
          } else {
            mOutput << mValHours;
          }
        } else if (mUnit == "m") {
          if (mZeroPad && mWidth > 0) {
            mOutput << std::setw(mWidth) << std::setfill('0') << mValMinutes;
          } else {
            mOutput << mValMinutes;
          }
        } else if (mUnit == "s") {
          if (mZeroPad && mWidth > 0) {
            mOutput << std::setw(mWidth) << std::setfill('0') << mValSeconds;
          } else {
            mOutput << mValSeconds;
          }
        } else if (mUnit == "ms") {
          if (mWidth > 0) {
            int mTruncatedMilliseconds = mValMilliseconds / static_cast<int>(std::pow(10, 3 - mWidth));
            mOutput << std::setw(mWidth) << std::setfill('0') << mTruncatedMilliseconds;
          } else {
            mOutput << mValMilliseconds;
          }
        }
      }
    }
    return mOutput.str();
  }

  bool StringTime::renderAssetIcon() const {
    return false;
  }

  void StringTime::saveAsset(JSONObject object) const {
    cDefValue.save(object, JSON_VALUE);
    object.addString(JSON_FORMAT, format, DEFAULT_FORMAT);
  }

  void StringTime::getAssetProperties(IPropertyMaker& owner) {
    owner.createPropertyTreeSelector(JSON_VALUE,  cDefValue);
    owner.createPropertyNativeString(JSON_FORMAT, [this]() {return format;}, [this](const std::string& value) {format = value; parseFormatString();}, DEFAULT_FORMAT);
  }

  bool StringTime::isDefaultConfiguration() const {
    return cDefValue.isDefaultConfigured() && format == DEFAULT_FORMAT;
  }

  std::string StringTime::getConversionPath() const {
    return "";
  }

  bool StringTime::isConfigurable() const {
    return false;
  }
}
