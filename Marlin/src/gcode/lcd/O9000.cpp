#include "../../inc/MarlinConfig.h"
#include "../gcode.h"
#include "../parser.h"
#include "../../lcd/dwin/e3v2/dwin.h"
#include "../../lcd/marlinui.h"
#include <map>
#include <functional>
#include <string> // Include for std::string
#include <string.h> // For strchr, strncpy, strlen

/**
 * M5000: Set Printing Details JOB from OctoPrint in LCD
 *
 * Params:
 *    - Filename
 *    - Print Time
 *    - Print Time Left
 *    - Total Layers
 *    - Current Layer
 *    - B64 thumbnail
 *    - Progress
 */
bool O9000_collecting = false;
char filename[50] = {0};
char print_time[50] = {0};
char ptime_left[50] = {0};
char total_layers[50] = {0};
char curr_layer[50] = {0};
char thumbnail[50] = {0};
char progress[10] = {0};
char param_value[50] = {0};
// bool serial_connection_active = false; // defined in serial.cpp


std::string getParsedValue(const std::string& str, int index) {
    if (str.empty()) return "";

    size_t start = 0;
    size_t end = 0;
    int i = 0;

    while ((end = str.find('|', start)) != std::string::npos) {
        if (i == index) {
            return str.substr(start, end - start);
        }
        start = end + 1;
        i++;
    }

    if (i == index)
    {
        return str.substr(start); // Handle last token
    }

    return ""; // Return empty string if index is out of bounds
}

void GcodeSuite::O9000() {
    if (!parser.string_arg || parser.string_arg[0] == '\0') {
        //SERIAL_ECHOLN("Empty Command Argument...");
        return;
    }

    char *my_string = parser.string_arg;
    //SERIAL_ECHOLNPAIR("Received: ", my_string);

    std::map<std::string, std::function<void(const char*)>> commandHandlers = {
        {"SC|", [&](const char*){
            // Received all params lets render
            //SERIAL_ECHOLN("Received all params, now render in LCD");
            TERN_(DWIN_CREALITY_LCD, DWIN_OctoPrintJob(filename, print_time, ptime_left, total_layers, curr_layer, thumbnail, progress));
        }},
        {"SFN|", [&](const char* arg){
            // Set FileName
            std::string filename_str = getParsedValue(arg, 1);
            strncpy(filename, filename_str.c_str(), sizeof(filename) - 1);
        }},
        {"SPT|", [&](const char* arg){
            // Set Print Time
            std::string print_time_str = getParsedValue(arg, 1);
            strncpy(print_time, print_time_str.c_str(), sizeof(print_time) - 1);
        }},
        {"SET|", [&](const char* arg){
            // Set Elapsed Time
            std::string ptime_left_str = getParsedValue(arg, 1);
            strncpy(ptime_left, ptime_left_str.c_str(), sizeof(ptime_left) - 1);
        }},
        {"STL|", [&](const char* arg){
            // Set Total Layers
            std::string total_layers_str = getParsedValue(arg, 1);
            strncpy(total_layers, total_layers_str.c_str(), sizeof(total_layers) - 1);
        }},
        {"SCL|", [&](const char* arg){
            // Set Current Layer
            std::string curr_layer_str = getParsedValue(arg, 1);
            strncpy(curr_layer, curr_layer_str.c_str(), sizeof(curr_layer) - 1);
        }},
        {"SPP|", [&](const char* arg){
            // Set Print Progress
            std::string progress_str = getParsedValue(arg, 1);
            strncpy(progress, progress_str.c_str(), sizeof(progress) - 1);
        }},
        {"UPP|", [&](const char* arg){
            // Update Print Progress
            std::string val_str = getParsedValue(arg, 1);
            TERN_(DWIN_CREALITY_LCD, DWIN_OctoUpdate_Progress(val_str.c_str()));
        }},
        {"UCL|", [&](const char* arg){
            // Update Current Layer
            std::string val_str = getParsedValue(arg, 1);
            TERN_(DWIN_CREALITY_LCD, DWIN_OctoUpdate_CLayer(val_str.c_str()));
        }},
        {"UET|", [&](const char* arg){
            // Update Elapsed Time
            std::string val_str = getParsedValue(arg, 1);
            TERN_(DWIN_CREALITY_LCD, DWIN_OctoUpdate_ETA(val_str.c_str()));
        }},
        {"UPT|", [&](const char* arg){
            // Update Print and Time
            std::string val_str_a = getParsedValue(arg, 1);
            std::string val_str_b = getParsedValue(arg, 2);
            TERN_(DWIN_CREALITY_LCD, DWIN_OctoUpdate_Progress(val_str_a.c_str()));
            TERN_(DWIN_CREALITY_LCD, DWIN_OctoUpdate_ETA(val_str_b.c_str()));
        }},
        {"PF|", [&](const char*){ TERN_(DWIN_CREALITY_LCD, DWIN_OctoJobFinish()); }},
        {"BU1|", [&](const char*){ TERN_(DWIN_CREALITY_LCD, DWIN_OctoShowGCodeImage()); }},
        {"OCON|", [&](const char*){ serial_connection_active = true; TERN_(DWIN_CREALITY_LCD, Goto_MainMenu()); }},
        {"OCOFF|", [&](const char*){ serial_connection_active = false; TERN_(DWIN_CREALITY_LCD, Goto_MainMenu()); }}
    };

    std::string command_key;
    const char* delimiterPos = strchr(my_string, '|');
    if (delimiterPos != nullptr) {
        command_key = std::string(my_string, delimiterPos - my_string + 1);
    } else {
        command_key = my_string;
    }

    auto it = commandHandlers.find(command_key);

    if (it != commandHandlers.end()) {
        it->second(my_string);
    } else {
        SERIAL_ECHOLN("Invalid Command Argument...");
    }

    // TODO THUMBNAIL RECBUFFER
}
