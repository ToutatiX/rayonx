#include <configs/log_config.h>

namespace Logger
{
    typedef int LOG_LEVEL_td;
    typedef String LOG_NAME_td;

    const LOG_LEVEL_td LEVEL_GOOD_NEWS = 1; // Used for reporting good news, the highest priority of content
    const LOG_LEVEL_td LEVEL_ERROR = 1;     // Used for reporting errors, the highest priority of content
    const LOG_LEVEL_td LEVEL_STATS = 2;     // Used for statistics reporting. Almost as important as errors
    const LOG_LEVEL_td LEVEL_WARNING = 3;   // Used for warnings. Device should still be partly working (and making do).
    const LOG_LEVEL_td LEVEL_VERBOSE = 4;   // Used for dumping everything to print. Also check

    const LOG_NAME_td NAME_Wifi    = "ESP32 ";     

    void init() {
        Serial.begin(DEBUG_BAUDRATE);
    }      
    
    /**
     * @brief Log something to the StatusLogger system.
     *  If the (log) level is less than LOG_LEVEL_SAVE then we'll print both to Serial and to the cacher to be retrieved later (use sparingly)!
     *
     * @param level The log level (e.g. LEVEL_ERROR or LEVEL_VERBOSE)
     * @param device The device that is reporting the log (e.g. NAME_ACCEL)
     * @param msg The message that you want to log (e.g. "Successfully uploaded X")
     * @param print_to_serial An optional override to force to print to serial
     * @param stream  An optional override to change where this log is sent
     */
    void log(LOG_LEVEL_td level, String device, String msg, bool print_to_serial = false, Stream *stream = &Serial)
    {
        #ifdef DEBUG
            String str =  "["+String((int)(millis()/60000)) +"m"+ String((float)((int)(millis()/10) %6000)/100) +"s]";
            for (int i = str.length() ; i<=11; i++) { str += " ";}
            str += " : ";
            str += device;
            str += " : ";
            str += level;
            str += " : ";
            str += msg;

            if (level <= LOG_LEVEL_PRINT || print_to_serial == true) //
            {   
                stream->println(str);
            }
        #endif
    }
}