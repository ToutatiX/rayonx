
/* ***********************************************************************
 *  This is used to deal with the non volatile memory.
 *  The ESP is able to save the SSID and Password selected by the user.
*/

#include <Preferences.h>

namespace Memory {
	/**
	 * Private namespace
	*/
	namespace {
		Preferences preferences;
	}

  /**
   * @brief Update non-volatile memory 
   * @param ssid : the ssid String to be stored
   * @param pwd  : the password String to be stored
  */
  void update(String ssid, String pwd) {
    preferences.begin("credentials", false);
    preferences.putString("ssid", ssid); 
    preferences.putString("password", pwd); 
    Log::log(Log::LEVEL_VERBOSE, Log::ESP, "Save to memory: " + ssid + " | " + pwd);
    preferences.end();
  }

  /**
   * @brief Read non-volatile memory
   * @param ssid : the ssid variable to be updated
   * @param pwd  : the password variable to be updated
  */
  void read(String *ssid, String *pwd) {
    preferences.begin("credentials", false);
    *ssid = preferences.getString("ssid", ""); 
    *pwd = preferences.getString("password", "");
		Log::log(Log::LEVEL_VERBOSE, Log::ESP, "Read from memory: " + *ssid + " | " + *pwd);
    preferences.end();
  }

  /**
   * @brief Clear memory by setting ssid and password to ""
  */
  void clear() {
    preferences.begin("credentials", false);
    preferences.putString("ssid", ""); 
    preferences.putString("password", ""); 
    Log::log(Log::LEVEL_VERBOSE, Log::ESP, "Clear memory");
    preferences.end();
  }
}