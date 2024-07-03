#include <std_include.hpp>
#include "definitions/game.hpp"
#include "loader/component_loader.hpp"

#include <utilities/hook.hpp>
#include <curl/curl.h>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

namespace master_server {

    size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
        ((std::string*)userp)->append((char*)contents, size * nmemb);
        return size * nmemb;
    }

    void fetch_lobbies() {
        CURL* curl;
        CURLcode res;
        std::string readBuffer;

        curl = curl_easy_init();
        if (curl) {
            curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:5000/lobbies");
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
            res = curl_easy_perform(curl);

            if (res != CURLE_OK) {
                logger::write(logger::LOG_TYPE_ERROR, "curl_easy_perform() failed: %s", curl_easy_strerror(res));
            }
            else {
                logger::write(logger::LOG_TYPE_INFO, "Received response from master server");

                rapidjson::Document doc;
                doc.Parse(readBuffer.c_str());

                if (doc.IsArray()) {
                    for (auto& lobby : doc.GetArray()) {
                        if (lobby.IsObject()) {
                            uint64_t lobby_id = lobby["lobby_id"].GetUint64();
                            std::string client_ip = lobby["client_ip"].GetString();
                            logger::write(logger::LOG_TYPE_INFO, "Lobby ID: %llu, Client IP: %s", lobby_id, client_ip.c_str());
                        }
                    }
                }
                else {
                    logger::write(logger::LOG_TYPE_ERROR, "Invalid JSON response from master server");
                }
            }

            curl_easy_cleanup(curl);
        }
    }

    class component final : public component_interface {
    public:
        void post_unpack() override {
            fetch_lobbies();
        }
    };
}

REGISTER_COMPONENT(master_server::component)