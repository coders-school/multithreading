#pragma once
#include <string>

namespace libmail {

bool send_mail(const std::string& address, const std::string& message);

}