#include <iostream>
#include <sys/socket.h>
#include <memory>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>
#include "display/Display.h"
#include "../libs/spotify-api-plusplus/src/SpotifyAPI.h"
#include <unistd.h>

#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "network/httplib.h"

static const std::string CLIENT_ID = "a3c2abf0b6c746a5828ddd71e5645d90";
static const std::string CLIENT_SECRET = "300bff4fe52a4388b62fffa8fefa7f48";
static const std::string SCOPES = "user-read-playback-state%20user-modify-playback-state%20user-read-currently-playing";
static const std::string SPOTIFY_URL = "https://accounts.spotify.com";
static const std::string REDIRECT_URI = "http://localhost:8085/callback";
static const std::string AUTH_URL = "http://localhost:8085/callback";
constexpr int LISTEN_PORT = 8085;

int main(int argc, char *argv[])
{
    std::string ip_arg;
    if (argc < 2)
    {
        ip_arg = "0.0.0.0";
        std::cout << "Warning: Maybe you forgot to provide an IP address to listen on?" << std::endl;
    }
    else
    {
        std::string ip_arg(argv[2]);
    }

    httplib::Server server;
    httplib::Client client(SPOTIFY_URL);

    Display display;

    server.Get("/auth", [](const httplib::Request &req, httplib::Response &res) {
        httplib::Request spotifyAuthRequest;
        std::stringstream queryParams;
        queryParams << "/authorize?"
                    << "client_id=" << CLIENT_ID 
                    << "&response_type=code&redirect_uri=" << REDIRECT_URI 
                    << "&scope=" << SCOPES
                    << "&show_dialog=true";

        res.set_redirect(SPOTIFY_URL + queryParams.str());
    });

    server.Get("/callback", [&client, &display](const httplib::Request &req, httplib::Response &res) {
        auto requestCode = req.get_param_value("code");
        auto state = req.get_param_value("state");

        std::stringstream queryParams;
        queryParams << "grant_type=authorization_code"
                    << "&code=" << requestCode
                    << "&redirect_uri=" << REDIRECT_URI;

        httplib::Headers headers = {
            { "Authorization",  "Basic " + httplib::detail::base64_encode(
                CLIENT_ID + ":" + CLIENT_SECRET) },
            { "Content-Type", "application/x-www-form-urlencoded"}
        };
        SpotifyAPI api;
        if (auto clientRes = client.Post("/api/token?" + queryParams.str(), headers))
        {
            res.set_redirect("https://catoftheday.com");
            nlohmann::json body = nlohmann::json::parse(clientRes->body);
            std::string accessToken = body["access_token"];
            api.setAuthToken(accessToken);
            while (true)
            {
                std::cout <<"looping" << std::endl;
                auto currentlyPlaying = api.GetMyCurrentPlayingTrack()->GetItem();
                std::cout << "loop middle" << std::endl;
                display.printTrack(currentlyPlaying);
                std::cout << "loop after" << std::endl;
                sleep(1);
            }
        }
        else 
        {
            auto err = clientRes.error();
            std::cout << httplib::to_string(err) << std::endl;
            exit(-1);
        }

    
    });

    server.listen(ip_arg, LISTEN_PORT);
}
