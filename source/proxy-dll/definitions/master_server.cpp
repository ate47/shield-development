#include <std_include.hpp>
#include "definitions/game.hpp"
#include "loader/component_loader.hpp"
#include "network.hpp"
//#include "blackops4.hpp"

#include "component/command.hpp"
#include <utilities/hook.hpp>
#include <curl/curl.h>
#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>
#include <regex>
#include <thread>
#include <chrono>
#include <fstream>
#include <filesystem>

namespace master_server {

    std::unordered_map<std::string, std::string> lobbies;

    size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
        ((std::string*)userp)->append((char*)contents, size * nmemb);
        return size * nmemb;
    }

    void save_json_response(const rapidjson::Document& doc) {
        rapidjson::Document jsonDoc;
        jsonDoc.SetObject();
        rapidjson::Document::AllocatorType& allocator = jsonDoc.GetAllocator();

        for (const auto& lobby : doc.GetArray()) {
            if (lobby.IsObject()) {
                uint64_t lobby_id = lobby["lobby_id"].GetUint64();
                std::string lobby_id_str = std::to_string(lobby_id);

                rapidjson::Value lobbyInfo(rapidjson::kObjectType);
                lobbyInfo.AddMember("Client IP", rapidjson::Value(lobby["client_ip"].GetString(), allocator), allocator);

                if (lobby.HasMember("host_doc") && lobby["host_doc"].IsString()) {
                    rapidjson::Document hostDoc;
                    hostDoc.Parse(lobby["host_doc"].GetString());

                    if (hostDoc.HasMember("attachment") && hostDoc["attachment"].IsObject()) {
                        const auto& attachment = hostDoc["attachment"];
                        if (attachment.HasMember("host_name") && attachment["host_name"].IsString()) {
                            lobbyInfo.AddMember("Host Name", rapidjson::Value(attachment["host_name"].GetString(), allocator), allocator);
                        }
                    }
                }

                if (lobby.HasMember("backend_doc") && lobby["backend_doc"].IsString()) {
                    rapidjson::Document backendDoc;
                    backendDoc.Parse(lobby["backend_doc"].GetString());

                    if (backendDoc.HasMember("ruleset_payload") && backendDoc["ruleset_payload"].IsObject()) {
                        const auto& ruleset = backendDoc["ruleset_payload"];
                        if (ruleset.HasMember("filter") && ruleset["filter"].IsObject()) {
                            const auto& filter = ruleset["filter"];
                            if (filter.HasMember("playlist_id")) {
                                lobbyInfo.AddMember("Playlist ID", rapidjson::Value(filter["playlist_id"].GetInt()), allocator);
                            }
                        }
                    }

                    if (backendDoc.HasMember("player_id_to_slots") && backendDoc["player_id_to_slots"].IsObject()) {
                        int player_count = backendDoc["player_id_to_slots"].MemberCount();
                        lobbyInfo.AddMember("Player Count", rapidjson::Value(player_count), allocator);
                    }
                }

                rapidjson::Value lobby_id_value(lobby_id_str.c_str(), allocator);
                jsonDoc.AddMember(lobby_id_value, lobbyInfo, allocator);
            }
        }

        rapidjson::StringBuffer buffer;
        rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
        jsonDoc.Accept(writer);

        std::ofstream ofs("project-bo4/lobbies/lobbies.json");
        ofs << buffer.GetString();
        ofs.close();
    }

    void fetch_lobbies() {
        CURL* curl;
        CURLcode res;
        std::string readBuffer;

        curl = curl_easy_init();
        if (curl) {
            curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:5000/lobbies"); //set real ip url here on release -- my dedi 
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

                            lobbies[std::to_string(lobby_id)] = client_ip;

                            if (lobby.HasMember("host_doc") && lobby["host_doc"].IsString()) {
                                logger::write(logger::LOG_TYPE_INFO, "Found host_doc in lobby");
                                rapidjson::Document host_doc;
                                host_doc.Parse(lobby["host_doc"].GetString());
                                if (host_doc.HasMember("attachment") && host_doc["attachment"].IsObject()) {
                                    const auto& attachment = host_doc["attachment"];
                                    if (attachment.HasMember("host_name") && attachment["host_name"].IsString()) {
                                        std::string host_name = attachment["host_name"].GetString();
                                        int player_count = 0;
                                        int playlist_id = 0;
                                        if (lobby.HasMember("backend_doc") && lobby["backend_doc"].IsString()) {
                                            logger::write(logger::LOG_TYPE_INFO, "Found backend_doc in lobby");
                                            rapidjson::Document backend_doc;
                                            backend_doc.Parse(lobby["backend_doc"].GetString());
                                            if (backend_doc.HasMember("player_id_to_slots") && backend_doc["player_id_to_slots"].IsObject()) {
                                                player_count = backend_doc["player_id_to_slots"].MemberCount();
                                            }
                                            if (backend_doc.HasMember("ruleset_payload") && backend_doc["ruleset_payload"].IsObject() &&
                                                backend_doc["ruleset_payload"].HasMember("filter") && backend_doc["ruleset_payload"]["filter"].IsObject() &&
                                                backend_doc["ruleset_payload"]["filter"].HasMember("playlist_id") && backend_doc["ruleset_payload"]["filter"]["playlist_id"].IsInt()) {
                                                playlist_id = backend_doc["ruleset_payload"]["filter"]["playlist_id"].GetInt();
                                            }
                                        }

                                        logger::write(logger::LOG_TYPE_INFO, "Lobby ID: %llu, Client IP: %s, Host Name: %s, Player Count: %d, Playlist ID: %d",
                                            lobby_id, client_ip.c_str(), host_name.c_str(), player_count, playlist_id);
                                    }
                                    else {
                                        logger::write(logger::LOG_TYPE_WARN, "Missing host_name in attachment");
                                    }
                                }
                                else {
                                    logger::write(logger::LOG_TYPE_WARN, "Missing attachment in host_doc");
                                }
                            }
                            else {
                                logger::write(logger::LOG_TYPE_WARN, "Missing host_doc in the lobby");
                                // Log the entire lobby object to debug the structure
                                rapidjson::StringBuffer buffer;
                                rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
                                lobby.Accept(writer);
                                logger::write(logger::LOG_TYPE_WARN, "Lobby structure: %s", buffer.GetString());
                            }
                        }
                    }
                }
                else {
                    logger::write(logger::LOG_TYPE_ERROR, "Invalid JSON response from master server");
                }

                save_json_response(doc);
            }

            curl_easy_cleanup(curl);
        }
    }

    void periodic_fetch() {
        while (true) {
            fetch_lobbies();
            std::this_thread::sleep_for(std::chrono::seconds(10)); // Fetch lobbies every 10 seconds
        }
    }

    class component final : public component_interface {
    public:
        void post_unpack() override {
            std::thread(periodic_fetch).detach();
        }
    };
}

REGISTER_COMPONENT(master_server::component)
