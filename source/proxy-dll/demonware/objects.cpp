#include <std_include.hpp>
#include <utils/io.hpp>
#include <utils/string.hpp>
#include <utils/cryptography.hpp>
#include <component/platform.hpp>
#include "tougprotocol.hpp"

#include "objects.hpp"

#include "resource.hpp"
#include <utils/nt.hpp>

namespace demonware
{
	std::string HexStringToBinaryString(const std::string& hex_str)
	{
		std::string data{};

		for (unsigned int i = 0; i < hex_str.length(); i += 2) {
			std::string byteString = hex_str.substr(i, 2);
			char byte = (char)strtol(byteString.c_str(), NULL, 16);
			data.push_back(byte);
		}

		return data;
	}

	std::string generate_publisher_objects_list_json(const std::string& category)
	{
		rapidjson::StringBuffer json_buffer{};
		rapidjson::PrettyWriter<rapidjson::StringBuffer> json_writer(json_buffer);

		json_writer.StartObject();

		json_writer.Key("next");
		json_writer.String("TODO");

		json_writer.Key("nextPageToken");
		json_writer.Null();

		json_writer.Key("objects");
		json_writer.StartArray();

		const auto objects_list_csv = utils::nt::load_resource(DW_PUBLISHER_OBJECTS_LIST);
		std::vector<std::string> objects = utils::string::split(objects_list_csv, "\r\n"); // WTF!?

		for (std::string object : objects)
		{
			std::string checksum_buffer = HexStringToBinaryString(utils::string::split(object, ',')[2]);

			json_writer.StartObject();

			json_writer.Key("owner");
			json_writer.String("treyarch");

			json_writer.Key("expiresOn");
			json_writer.Uint(0);

			json_writer.Key("name");
			json_writer.String(utils::string::split(object, ',')[0]);

			json_writer.Key("checksum");
			json_writer.String(utils::cryptography::base64::encode(checksum_buffer));

			json_writer.Key("acl");
			json_writer.String("public");

			json_writer.Key("objectID");
			json_writer.Uint(0);
			json_writer.Key("contentID");
			json_writer.Null();
			json_writer.Key("objectVersion");
			json_writer.String("");
			json_writer.Key("contentVersion");
			json_writer.Null();

			json_writer.Key("contentLength");
			json_writer.Uint64(std::stol(utils::string::split(object, ',')[1]));

			json_writer.Key("context");
			json_writer.String("t8-bnet");

			json_writer.Key("category");
			json_writer.Null();

			json_writer.Key("created");
			json_writer.Uint64(static_cast<int64_t>(time(nullptr)));

			json_writer.Key("modified");
			json_writer.Uint64(static_cast<int64_t>(time(nullptr)));

			json_writer.Key("extraData");
			json_writer.Null();
			json_writer.Key("extraDataSize");
			json_writer.Null();
			json_writer.Key("summaryContentLength");
			json_writer.Null();
			json_writer.Key("summaryChecksum");
			json_writer.Null();
			json_writer.Key("hasSummary");
			json_writer.Bool(false);

			json_writer.EndObject();
		}

		json_writer.EndArray();

		json_writer.EndObject();

		return json_buffer.GetString();
	}

	std::string get_user_file_path(const std::string& file)
	{
		return platform::get_userdata_directory() + "/" + file;
	}

	std::string get_user_file_checksum(std::string file_path)
	{
		std::string file_data;
		if (!utils::io::read_file(file_path, &file_data)) return "";

		return std::to_string(utils::cryptography::xxh32::compute(file_data));
	}

	std::string get_user_file_content(std::string file_path)
	{
		std::string file_data;
		if (!utils::io::read_file(file_path, &file_data)) return "";

		return utils::cryptography::base64::encode(file_data);
	}

	std::string deliver_user_objects_vectorized_json(std::vector<objectMetadata> requested_items)
	{
		rapidjson::StringBuffer json_buffer{};
		rapidjson::PrettyWriter<rapidjson::StringBuffer> json_writer(json_buffer);

		json_writer.StartObject();

		json_writer.Key("objects");
		json_writer.StartArray();

		for (size_t i = 0; i < requested_items.size(); i++)
		{
			if (requested_items[i].contentLength == 0 || requested_items[i].contentURL.empty())
			{
				continue;
			}

			//std::string file_path = get_user_file_path(requested_items[i].owner, requested_items[i].name);

			json_writer.StartObject();

			json_writer.Key("metadata");
			json_writer.StartObject();

			json_writer.Key("owner");
			json_writer.String(requested_items[i].owner.data());

			json_writer.Key("expiresOn");
			json_writer.Uint(0);

			json_writer.Key("name");
			json_writer.String(requested_items[i].name.data());

			json_writer.Key("checksum");
			json_writer.String(requested_items[i].checksum.data());

			json_writer.Key("acl");
			json_writer.String("public");

			json_writer.Key("objectID");
			json_writer.Uint(0);
			json_writer.Key("contentID");
			json_writer.Null();
			json_writer.Key("objectVersion");
			json_writer.String("");
			json_writer.Key("contentVersion");
			json_writer.Null();

			json_writer.Key("contentLength");
			json_writer.Uint64(requested_items[i].contentLength);

			json_writer.Key("context");
			json_writer.String("t8-bnet");

			json_writer.Key("category");
			json_writer.Null();

			json_writer.Key("created");
			json_writer.Uint64(requested_items[i].created);

			json_writer.Key("modified");
			json_writer.Uint64(requested_items[i].modified);

			json_writer.Key("extraData");
			json_writer.Null();
			json_writer.Key("extraDataSize");
			json_writer.Null();
			json_writer.Key("summaryContentLength");
			json_writer.Null();
			json_writer.Key("summaryChecksum");
			json_writer.Null();
			json_writer.Key("hasSummary");
			json_writer.Bool(false);

			json_writer.EndObject();

			json_writer.Key("content");
			json_writer.String(requested_items[i].contentURL.data());

			json_writer.Key("requestIndex");
			json_writer.Uint64(i);

			json_writer.EndObject();
		}

		json_writer.EndArray();

		json_writer.Key("errors");
		json_writer.StartArray();
		for (size_t i = 0; i < requested_items.size(); i++)
		{
			if (requested_items[i].contentLength == 0 || requested_items[i].contentURL.empty())
			{
				json_writer.StartObject();

				json_writer.Key("requestIndex");
				json_writer.Uint64(i);
				json_writer.Key("owner");
				json_writer.String(requested_items[i].owner.data());
				json_writer.Key("name");
				json_writer.String(requested_items[i].name.data());
				json_writer.Key("error");
				json_writer.String("Error:ClientError:NotFound");

				json_writer.EndObject();
			}
		}
		json_writer.EndArray();

		json_writer.EndObject();

		return json_buffer.GetString();
	}

	std::string deliver_user_objects_vectorized_json(std::vector<objectID> requested_items)
	{
		std::vector<objectMetadata> files_metadata_list;

		for (objectID file : requested_items)
		{
			std::string file_path = get_user_file_path(file.name);
			int64_t timestamp = static_cast<int64_t>(time(nullptr));
			files_metadata_list.push_back({ file.owner, file.name ,get_user_file_checksum(file_path),  static_cast<int64_t>(utils::io::file_size(file_path)), timestamp, timestamp, get_user_file_content(file_path) });
		}

		return deliver_user_objects_vectorized_json(files_metadata_list);
	}

	std::string generate_user_objects_list_json()
	{
		rapidjson::StringBuffer json_buffer{};
		rapidjson::PrettyWriter<rapidjson::StringBuffer> json_writer(json_buffer);

		json_writer.StartObject();

		json_writer.Key("nextPageToken");
		json_writer.Null();

		json_writer.Key("next");
		json_writer.String("TODO");

		json_writer.Key("objects");
		json_writer.StartArray();

		std::string userdata_directory = platform::get_userdata_directory();

		if (utils::io::directory_exists(userdata_directory))
		{
			std::vector<std::string> user_objects = utils::io::list_files(userdata_directory);

			for (std::string object : user_objects)
			{
				json_writer.StartObject();

				json_writer.Key("metadata");
				json_writer.StartObject();

				json_writer.Key("owner");
				json_writer.String(std::format("bnet-{}", platform::bnet_get_userid()));

				json_writer.Key("expiresOn");
				json_writer.Uint(0);

				json_writer.Key("name");
				json_writer.String(utils::io::file_name(object));

				json_writer.Key("checksum");
				json_writer.String(get_user_file_checksum(object));

				json_writer.Key("acl");
				json_writer.String("public");

				json_writer.Key("objectID");
				json_writer.Uint(0);
				json_writer.Key("contentID");
				json_writer.Null();
				json_writer.Key("objectVersion");
				json_writer.String("");
				json_writer.Key("contentVersion");
				json_writer.Null();

				json_writer.Key("contentLength");
				json_writer.Uint64(utils::io::file_size(object));

				json_writer.Key("context");
				json_writer.String("t8-bnet");

				json_writer.Key("category");
				json_writer.Null();

				json_writer.Key("created");
				json_writer.Uint64(static_cast<int64_t>(time(nullptr)));

				json_writer.Key("modified");
				json_writer.Uint64(static_cast<int64_t>(time(nullptr)));

				json_writer.Key("extraData");
				json_writer.Null();
				json_writer.Key("extraDataSize");
				json_writer.Null();
				json_writer.Key("summaryContentLength");
				json_writer.Null();
				json_writer.Key("summaryChecksum");
				json_writer.Null();
				json_writer.Key("hasSummary");
				json_writer.Bool(false);

				json_writer.EndObject();

				json_writer.Key("tags");
				json_writer.StartArray();
				json_writer.EndArray();

				json_writer.Key("statistics");
				json_writer.StartArray();
				json_writer.EndArray();

				json_writer.EndObject();
			}
		}
		json_writer.EndArray();

		json_writer.EndObject();

		return json_buffer.GetString();
	}

	std::string generate_user_objects_count_json()
	{
		std::string userdata_directory = platform::get_userdata_directory();

		int files_count = 0;
		if (utils::io::directory_exists(userdata_directory))
		{
			files_count = static_cast<int32_t>(utils::io::list_files(userdata_directory).size());
		}


		rapidjson::StringBuffer json_buffer{};
		rapidjson::PrettyWriter<rapidjson::StringBuffer> json_writer(json_buffer);

		json_writer.StartObject();

		json_writer.Key("total");
		json_writer.Uint(files_count);

		json_writer.Key("noCategory");
		json_writer.Uint(files_count);

		json_writer.Key("categories");
		json_writer.StartObject();
		json_writer.EndObject();

		json_writer.EndObject();

		return json_buffer.GetString();
	}

	std::string construct_vectorized_upload_list_json(std::vector<objectMetadata> uploaded_files)
	{
		rapidjson::StringBuffer json_buffer{};
		rapidjson::PrettyWriter<rapidjson::StringBuffer> json_writer(json_buffer);

		json_writer.StartObject();

		json_writer.Key("objects");
		json_writer.StartArray();

		for (size_t i = 0; i < uploaded_files.size(); i++)
		{
			//std::string file_path = get_user_file_path(requested_items[i].owner, requested_items[i].name);

			json_writer.StartObject();

			json_writer.Key("metadata");
			json_writer.StartObject();

			json_writer.Key("owner");
			json_writer.String(uploaded_files[i].owner.data());

			json_writer.Key("expiresOn");
			json_writer.Uint(0);

			json_writer.Key("name");
			json_writer.String(uploaded_files[i].name.data());

			json_writer.Key("checksum");
			json_writer.String(uploaded_files[i].checksum.data());

			json_writer.Key("acl");
			json_writer.String("public");

			json_writer.Key("objectID");
			json_writer.Uint(0);
			json_writer.Key("contentID");
			json_writer.Null();
			json_writer.Key("objectVersion");
			json_writer.String("");
			json_writer.Key("contentVersion");
			json_writer.Null();

			json_writer.Key("contentLength");
			json_writer.Uint64(uploaded_files[i].contentLength);

			json_writer.Key("context");
			json_writer.String("t8-bnet");

			json_writer.Key("category");
			json_writer.Null();

			json_writer.Key("created");
			json_writer.Uint64(uploaded_files[i].created);

			json_writer.Key("modified");
			json_writer.Uint64(uploaded_files[i].modified);

			json_writer.Key("extraData");
			json_writer.Null();
			json_writer.Key("extraDataSize");
			json_writer.Null();
			json_writer.Key("summaryContentLength");
			json_writer.Null();
			json_writer.Key("summaryChecksum");
			json_writer.Null();
			json_writer.Key("hasSummary");
			json_writer.Bool(false);

			json_writer.EndObject();

			json_writer.Key("requestIndex");
			json_writer.Uint64(i);

			json_writer.EndObject();
		}

		json_writer.EndArray();

		json_writer.Key("errors");
		json_writer.StartArray();
		json_writer.EndArray();

		json_writer.Key("validationTokens");
		json_writer.StartArray();
		json_writer.EndArray();

		json_writer.EndObject();

		return json_buffer.GetString();
	}

	std::string construct_vectorized_upload_list_json(std::vector<std::string> uploaded_files)
	{
		std::vector<objectMetadata> files_metadata_list;

		for (std::string file : uploaded_files)
		{
			std::string file_path = get_user_file_path(file);
			int64_t timestamp = static_cast<int64_t>(time(nullptr));
			files_metadata_list.push_back({ std::format("bnet-{}", platform::bnet_get_userid()), file ,get_user_file_checksum(file_path),  static_cast<int64_t>(utils::io::file_size(file_path)), timestamp, timestamp, get_user_file_content(file_path) });
		}

		return construct_vectorized_upload_list_json(files_metadata_list);
	}

	std::string serialize_objectstore_structed_buffer(std::string payload)
	{
		bdProtobufHelper header_1st;
		header_1st.writeString(1, "Content-Length", 16);
		header_1st.writeString(2, utils::string::va("%u", payload.length()), 8);

		bdProtobufHelper header_2nd;
		header_2nd.writeString(1, "Authorization", 16);
		header_2nd.writeString(2, "Bearer project-bo4", 2048);


		bdProtobufHelper buffer;
		buffer.writeString(1, header_1st.buffer.data(), static_cast<uint32_t>(header_1st.buffer.length()));
		buffer.writeString(1, header_2nd.buffer.data(), static_cast<uint32_t>(header_2nd.buffer.length()));
		buffer.writeUInt64(2, 200); // Status Code; Anything NON-2XX is Treated as Error
		buffer.writeString(3, payload.data(), static_cast<uint32_t>(payload.length()));

		return buffer.buffer;
	}
}
