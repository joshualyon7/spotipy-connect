#ifdef BUILD_PI
#include <liquidcrystal/LiquidCrystal_I2C.h>
#endif
#include <string>
#include <queue>
#include <mutex>
#include <thread>
#include "../../libs/spotify-api-plusplus/src/SpotifyAPI.h"

class Display
{
public:
    Display();
    void clear();
    void printBeginning(const std::string &value);
    void printPosition(const std::string &value, int position);
    void sendLine(const std::string &value);
    void setTrack(Track track);
    void printTrack(Track track);
private:
    std::thread displayThread;
    void run();
    std::unique_ptr<Track> currentTrack;
#ifdef BUILD_PI
    LiquidCrystal_I2C display;
#endif
};