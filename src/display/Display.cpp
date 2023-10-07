#include "Display.h"
#include <cinttypes>
#include <string>
#include <iostream>
#include <unistd.h> 
#include <algorithm>
#ifdef BUILD_PI
// GPIO chip i2c address
constexpr uint8_t I2C_ADDR = 0x27;

// Control line PINs
constexpr uint8_t CTRL_EN = 2;
constexpr uint8_t CTRL_RW = 1;
constexpr uint8_t CTRL_RS = 0;

// Data line PINs
constexpr uint8_t DATA_4 = 4;
constexpr uint8_t DATA_5 = 5;
constexpr uint8_t DATA_6 = 6;
constexpr uint8_t DATA_7 = 7;

// Backlight PIN
constexpr uint8_t BACKLIGHT = 3;

// LCD display size (1x16, 2x16, 4x20)
constexpr uint8_t ROWS = 2;
constexpr uint8_t COLS = 16;

static const std::string DEVNAME = "/dev/i2c-1";
#endif 

Display::Display()
#ifdef BUILD_PI
  , display(LiquidCrystal_I2C(DEVNAME.c_str(), I2C_ADDR, CTRL_EN, CTRL_RW, CTRL_RS, DATA_4, DATA_5, DATA_6, DATA_7, BACKLIGHT, POSITIVE))
{
    display.begin(ROWS, COLS);
#else
{
    std::cout << "initializing display" << std::endl;
#endif
    //displayThread = std::thread(&Display::run, this);
}

void Display::clear()
{
#ifdef BUILD_PI
    display.clear();
#else
    std::cout << "clearing" << std::endl;
#endif
}

void Display::printBeginning(const std::string &value)
{
#ifdef BUILD_PI
    display.setCursor(0, 0);
    display.clear();
    display.print(value.c_str());
    display.scrollDisplayLeft();
#else 
    std::cout << value << std::endl;
#endif
}

void Display::printPosition(const std::string &message, int position)
{
#ifdef BUILD_PI
    display.setCursor(position)
#else
    std::cout << message << std::endl;
#endif
}

void Display::printTrack(Track track)
{
    std::string artistString = "";
    for (const auto &artist : track.GetArtists())
    {
        artistString += artist->GetName() + ", ";
    }
    artistString[artistString.length() - 1] = '\0';
#ifdef BUILD_PI
    display.clear();
    display.setCursor(0, 0);
    display.print(track.GetName().c_str());
    display.setCursor(0, 1);
    display.print(artistString.c_str());
#else
    std::cout << artistString << std::endl;
    std::cout << track.GetName() << std::endl;
#endif
}

void Display::run()
{
    printBeginning("Nothing playing");
    while (nullptr == currentTrack)
    {
        sleep(1);
    }

    std::string oldTrackId = currentTrack->GetId();
    int currentIdx;
    while (true)
    {
        if (nullptr == currentTrack)
        {
            printBeginning("Nothing playing");
        }
        if (oldTrackId == currentTrack->GetId())
        {
            continue;
        }
        sleep(1);
    }
}