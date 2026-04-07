#pragma once
/*
 *
0 = Oxygen sensor 1 disconnected 00000000000000000000000000000001
1 = Oxygen sensor 2 disconnected 00000000000000000000000000000010
2 = Oxygen sensor 3 disconnected 00000000000000000000000000000100
3 = Oxygen sensor 4 disconnected 00000000000000000000000000001000
4 = Reserved 00000000000000000000000000010000
5 = Reserved 00000000000000000000000000100000
6 = Reserved 00000000000000000000000001000000
7 = Reserved 00000000000000000000000010000000
8 = Concentrator 1 flow error 00000000000000000000000100000000
9 = Concentrator 2 flow error 00000000000000000000001000000000
10 = Concentrator 3 flow error 00000000000000000000010000000000
11 = Concentrator 4 flow error 00000000000000000000100000000000
12 = Reserved 00000000000000000001000000000000
13 = Reserved 00000000000000000010000000000000
14 = Reserved 00000000000000000100000000000000
15 = Reserved 00000000000000001000000000000000
16 = Main Compressor 1 Flow error 00000000000000010000000000000000
17 = Main Compressor 2 Flow error 00000000000000100000000000000000
18 = Reserved 00000000000001000000000000000000
19 = Reserved 00000000000010000000000000000000
20 = Concentrator Faulty 00000000000100000000000000000000
21 = Low & High Limit Count ERROR 00000000001000000000000000000000
22 = Low & High Limit Timeout ERROR 00000000010000000000000000000000
23 = Prestart Timeout ERROR 00000000100000000000000000000000
24 = Preproduce Timeout ERROR 00000001000000000000000000000000
25 = Reserved 00000010000000000000000000000000
26 = LCD Module Wires Broken 00000100000000000000000000000000
27 = Oxygen Temperature Overheat 00001000000000000000000000000000
28 = Reserved 00010000000000000000000000000000
29 = Tank Pressure Sensor Fault (<0.1V) 00100000000000000000000000000000
30 = Outputs overload protect 01000000000000000000000000000000
31 = Alarm mode 10000000000000000000000000000000
 *
 *
 */

namespace ON2Solutions {
  namespace locales::en {
    inline constexpr const char* info_device = "Device";
    inline constexpr const char* info_loader = "Loader";
    inline constexpr const char* info_fw = "FW";
    inline constexpr const char* info_fw_checksum = "FW Checksum";
    inline constexpr const char* info_module_name = "Module Name";
    inline constexpr const char* info_module_fw = "Module FW";
    inline constexpr const char* info_serial_number = "S/N";
    inline constexpr const char* info_ethernet_ip = "Ethernet IP";
    inline constexpr const char* info_wifi_ip = "WiFi IP";
    inline constexpr const char* info_lcd_fw = "LCD FW";
    inline constexpr const char* info_lcd_loader = "LCD Loader";
    inline constexpr const char* info_lcd_partition = "LCD Partition";
    inline constexpr const char* info_hour_run = "Hour Run";
    inline constexpr const char* info_restart = "Restart";
    inline constexpr const char* info_action = "Action";
    inline constexpr const char* timer = "Timer:";
    inline constexpr const char* restart = "Restart:";
    inline constexpr const char* sec = "sec";

    inline constexpr const char* header_title = "Welcome";
    inline constexpr const char* header_settings = "Settings";
    inline constexpr const char* header_information = "Info";
    inline constexpr const char* header_errors = "Errors";
    inline constexpr const char* button_ok = "OK";
    inline constexpr const char* button_cancel = "Cancel";
    inline constexpr const char* button_close = "Close";
    inline constexpr const char* button_back = "Back";
    inline constexpr const char* sign_in = "Sign In";
    inline constexpr const char* system_settings_header = "System Settings";
    inline constexpr const char* system_auth_header = "Security";
    inline constexpr const char* hour_run_reset = "Hour Run Reset";

    inline constexpr const char* login_email = "Email address";
    inline constexpr const char* login_password = "Password";

    inline constexpr const char* oxygen_level = "Oxygen Level";
    inline constexpr const char* oxygen_rate = "Oxygen Flow";
    inline constexpr const char* pressure_sensor_type = "Pressure Sensor Type";

    inline constexpr const char* oxygen_offset = "Oxygen offset";
    inline constexpr const char* flow_offset = "Flow offset";
    inline constexpr const char* tank_psi = "Tank (PSI)";
    inline constexpr const char* tank_pressure = "Tank Pressure";
    inline constexpr const char* booster_psi = "Booster Pressure";
    inline constexpr const char* temperature = "Temperature";
    inline constexpr const char* other = "Other";

    inline constexpr const char* oxygen_offset_setting = "Oxygen sensors offset";
    inline constexpr const char* oxygen_flow_setting = "Flow sensors offset";


    inline constexpr const char* bootloader_status_1 = "Bootloader is ready";
    inline constexpr const char* bootloader_status_2 = "Waiting firmware upload";
    inline constexpr const char* bootloader_status_3 = "Uploading Firmware";
    inline constexpr const char* bootloader_status_4 = "Update successfully completed";



    inline constexpr const char* status = "STAND BY";


    inline constexpr const char* info_bit_0_error  = "Oxygen sensor 1 disconnected";
    inline constexpr const char* info_bit_1_error  = "Oxygen sensor 2 disconnected";
    inline constexpr const char* info_bit_2_error  = "Oxygen sensor 3 disconnected";
    inline constexpr const char* info_bit_3_error  = "Oxygen sensor 4 disconnected";

    inline constexpr const char* info_bit_4_error  = "Reserved (4)";
    inline constexpr const char* info_bit_5_error  = "Reserved (5)";
    inline constexpr const char* info_bit_6_error  = "Reserved (6)";
    inline constexpr const char* info_bit_7_error  = "Reserved (7)";

    inline constexpr const char* info_bit_8_error  = "Concentrator 1 flow error";
    inline constexpr const char* info_bit_9_error  = "Concentrator 2 flow error";
    inline constexpr const char* info_bit_10_error = "Concentrator 3 flow error";
    inline constexpr const char* info_bit_11_error = "Concentrator 4 flow error";

    inline constexpr const char* info_bit_12_error = "Reserved (12)";
    inline constexpr const char* info_bit_13_error = "Reserved (13)";
    inline constexpr const char* info_bit_14_error = "Reserved (14)";
    inline constexpr const char* info_bit_15_error = "Reserved (15)";

    inline constexpr const char* info_bit_16_error = "Main Compressor 1 Flow error";
    inline constexpr const char* info_bit_17_error = "Main Compressor 2 Flow error";

    inline constexpr const char* info_bit_18_error = "Reserved (18)";
    inline constexpr const char* info_bit_19_error = "Reserved (19)";

    inline constexpr const char* info_bit_20_error = "Concentrator Faulty";

    inline constexpr const char* info_bit_21_error = "Low & High Limit Count ERROR";
    inline constexpr const char* info_bit_22_error = "Low & High Limit Timeout ERROR";

    inline constexpr const char* info_bit_23_error = "Prestart Timeout ERROR";
    inline constexpr const char* info_bit_24_error = "Preproduce Timeout ERROR";
    inline constexpr const char* info_bit_25_error = "Reserved (25)";
    inline constexpr const char* info_bit_26_error = "LCD Module Wires Broken";
    inline constexpr const char* info_bit_27_error = "Oxygen Temperature Overheat";
    inline constexpr const char* info_bit_28_error = "Reserved (28)";
    inline constexpr const char* info_bit_29_error = "Tank Pressure Sensor Fault (<0.1V)";
    inline constexpr const char* info_bit_30_error = "Outputs overload protect";
    inline constexpr const char* info_bit_31_error = "Alarm mode";


    inline constexpr const char* info_bits_error[32] = {
      locales::en::info_bit_0_error,
      locales::en::info_bit_1_error,
      locales::en::info_bit_2_error,
      locales::en::info_bit_3_error,
      locales::en::info_bit_4_error,
      locales::en::info_bit_5_error,
      locales::en::info_bit_6_error,
      locales::en::info_bit_7_error,
      locales::en::info_bit_8_error,
      locales::en::info_bit_9_error,
      locales::en::info_bit_10_error,
      locales::en::info_bit_11_error,
      locales::en::info_bit_12_error,
      locales::en::info_bit_13_error,
      locales::en::info_bit_14_error,
      locales::en::info_bit_15_error,
      locales::en::info_bit_16_error,
      locales::en::info_bit_17_error,
      locales::en::info_bit_18_error,
      locales::en::info_bit_19_error,
      locales::en::info_bit_20_error,
      locales::en::info_bit_21_error,
      locales::en::info_bit_22_error,
      locales::en::info_bit_23_error,
      locales::en::info_bit_24_error,
      locales::en::info_bit_25_error,
      locales::en::info_bit_26_error,
      locales::en::info_bit_27_error,
      locales::en::info_bit_28_error,
      locales::en::info_bit_29_error,
      locales::en::info_bit_30_error,
      locales::en::info_bit_31_error,
    };

  }
}