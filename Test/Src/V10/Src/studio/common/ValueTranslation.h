#pragma once

#include <string>
#include "../../include/wmdf_api.h"

class CValueTranslation
{
public:
  CValueTranslation(void);
  ~CValueTranslation(void);

public:
  // 将8 bit value 转译成string. 重点是转义无效值
  static std::string Translation8BitValue(int8_t value);

  // 将32 bit value 转译成string. 重点是转义无效值
  static std::string Translation32BitValue(int32_t value);

  // 将64 bit value 转译成string. 重点是转义无效值
  static std::string Translation64BitValue(int64_t value);

  // 将double value 转译成string. 重点是转义无效值
  static std::string TranslationDoubleValue(double value);

  // 将char* value 转译成string. 重点是转义无效值
  static std::string TranslationStringValue(std::string value);

private:
  static std::string m_not_available;
};