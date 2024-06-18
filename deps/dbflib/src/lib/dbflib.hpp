#include <vector>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <cstdint>

/*
 * Dynamically linked binary file library
 */
namespace dbflib {
    // do not change this value
    constexpr uint64_t DB_FILE_MAGIC = 0x0d0a46424424;
    // minimum support version 
    constexpr uint8_t DB_FILE_MIN_VERSION = 0x10;
    // current version
    constexpr uint8_t DB_FILE_CURR_VERSION = 0x11;

    static_assert(DB_FILE_MIN_VERSION <= DB_FILE_CURR_VERSION && "Minimum version should be lower or equal to the current version");

    enum DB_FILE_VERSION_FEATURE : uint8_t {
        LINKING = 0x10,
        FAST_LINKING = 0x11,
    };

    enum DB_FILE_BUILDER_OPTIONS : uint8_t {
        // 64 bits align all new blocks
        DBFBO_ALIGN = 1,
    };

    typedef uint32_t BlockId;
    typedef uint32_t BlockOffset;
    typedef uint32_t BlockSize;

    struct DB_FILE_LINK {
        uint32_t origin;
        uint32_t destination;
    };

    struct DB_FILE {
        uint8_t magic[sizeof(decltype(DB_FILE_MAGIC))]{};
        uint8_t version{};
        uint8_t flags{};
        uint16_t links_count{};
        uint32_t links_table_offset{};
        uint32_t start_offset{};
        uint32_t data_size{};
        uint32_t file_size{};
        uint32_t __pad{};
        void* last_link{};

        template<typename StartType = void>
        constexpr StartType* Start() {
            return reinterpret_cast<StartType*>(magic + start_offset);
        }

        /*
         * Validate the file
         * @param len file size, 0 for unknown
         */
        void Validate(size_t len = 0) const {
            if (len && len < offsetof(DB_FILE, file_size) + sizeof(sizeof(file_size))) {
                throw std::runtime_error("invalid file: file too small");
            }

            if (*reinterpret_cast<decltype(DB_FILE_MAGIC)*>(magic) != DB_FILE_MAGIC) {
                throw std::runtime_error("invalid file: bad magic");
            }

            if (version < DB_FILE_MIN_VERSION) {
                throw std::runtime_error("invalid file: version too low");
            }

            if (len && file_size > len) {
                throw std::runtime_error("invalid file: read file too small");
            }

            if (start_offset > file_size) {
                throw std::runtime_error("invalid file: start offset after file end");
            }
        }

        /*
         * Link the file
         * @param force force the linking
         * @return if the file was linked
         */
        bool Link(bool force = false) {
            if (!force && version >= DB_FILE_VERSION_FEATURE::FAST_LINKING) {
                // store the last pointer to avoid linking the file more than once at the same location
                if (last_link == (void*)this) {
                    return false;
                }
                last_link = (void*)this;
            }
            if (version >= DB_FILE_VERSION_FEATURE::LINKING) {
                DB_FILE_LINK* links = reinterpret_cast<DB_FILE_LINK*>(magic + links_table_offset);
                for (size_t i = 0; i < links_count; i++) {
                    DB_FILE_LINK& link = links[i];

                    if (link.origin > file_size) throw std::runtime_error("invalid file: link after end file");
                    if (link.destination > file_size) throw std::runtime_error("invalid file: link after end file");

                    *reinterpret_cast<void**>(magic + link.origin) = magic + link.destination;
                }
            }
            return true;
        }
    };

    class DBFileBuilder {
        bool linked{};
        uint8_t flags{};
        std::vector<uint8_t> data{};
        std::unordered_map<BlockId, BlockSize> blocks{};
        std::vector<DB_FILE_LINK> links{};

        DB_FILE* Header() {
            return reinterpret_cast<DB_FILE*>(data.data());
        }

        inline void AssertNotLinked() {
#ifdef DEBUG
            if (linked) {
                throw std::runtime_error("builder already linked!");
            }
#endif
            linked = false;
        }
    public:
        /*
         * @param flags builder options, described in DB_FILE_BUILDER_OPTIONS
         */
        DBFileBuilder(uint8_t flags = 0) : flags(flags) {
            data.resize(sizeof(DBFileBuilder));
            Header()->start_offset = (uint32_t)data.size();
        }

        DBFileBuilder(DBFileBuilder& o) = delete;
        DBFileBuilder(DBFileBuilder&& o) = delete;

        /*
         * Align the buffer.
         * @param AlignType align type
         */
        template<typename AlignType = uint64_t>
        void AlignBlock() {
            data.resize((data.size() + sizeof(AlignType) - 1) & ~(sizeof(AlignType) - 1));
        }

        /*
         * Create a block from a buffer.
         * @param buffer buffer, should contain at least len bytes
         * @param len length of the buffer
         * @return block id
         */
        BlockId CreateBlock(void* buffer, size_t len) {
            AssertNotLinked();

            if (flags & DB_FILE_BUILDER_OPTIONS::DBFBO_ALIGN) {
                AlignBlock();
            }

            size_t id = data.size();
            if (len) {
                if (id + len > INT32_MAX) {
                    throw std::runtime_error("file too big");
                }
                data.insert(data.end(), (uint8_t*)buffer, (uint8_t*)buffer + len);
                blocks[(BlockId)id] = (BlockSize)len;
            }
            return (BlockId)id;
        }

        /*
         * Create a block inside the file.
         * @param BlockType pointer type to return
         * @param len length of the buffer to create
         * @return block id and pointer, the pointer is valid until a new block is created
         */
        template<typename BlockType = void>
        std::pair<BlockId, BlockType*> CreateBlock(const size_t len = sizeof(BlockType)) {
            AssertNotLinked();

            if (flags & DB_FILE_BUILDER_OPTIONS::DBFBO_ALIGN) {
                AlignBlock();
            }

            size_t id = data.size();
            if (len) {
                if (id + len > INT32_MAX) {
                    throw std::runtime_error("file too big");
                }
                data.resize(id + len);
                blocks[(BlockId)id] = (BlockSize)len;
            }
            return std::make_pair((BlockId)id, reinterpret_cast<BlockType*>(&data[id]));
        }

        /*
         * Get a block inside the file.
         * @param BlockType pointer type to return
         * @param id block id
         * @return block pointer, the pointer is valid until a new block is created
         */
        template<typename BlockType = void>
        BlockType* GetBlock(BlockId id) {
            if (id > data.size()) {
                throw std::runtime_error("invalid block");
            }
            return reinterpret_cast<BlockType*>(&data[id]);
        }

        /*
         * Get a block size.
         * @param id block id
         * @return block size
         */
        BlockSize GetBlockSize(BlockId id) const {
            auto it = blocks.find(id);
            if (it == blocks.end()) {
                return 0;
            }
            return it->second;
        }

        /*
         * Create a link between 2 locations.
         * @param blockOrigin origin block id
         * @param origin origin offset
         * @param blockDestination destination block id
         * @param destination destination offset
         */
        void CreateLink(BlockId blockOrigin, BlockOffset origin, BlockId blockDestination, BlockOffset destination = 0) {
            AssertNotLinked();
            BlockSize ors = GetBlockSize(blockOrigin);
            BlockSize dss = GetBlockSize(blockDestination);

            if (origin + 8 > ors || destination > dss) {
                throw std::runtime_error("trying to create a link after the end of a block");
            }
            
            if (links.size() >= UINT16_MAX) {
                throw std::runtime_error("too many links");
            }

            links.emplace_back((uint32_t)(blockOrigin + origin), (uint32_t)(blockDestination + destination));
        }

        /*
         * Build the file and return the start
         * @return file
         */
        DB_FILE* Build() {
            if (linked) {
                return Header();
            }
            linked = true;
            size_t dataSize{ data.size() - Header()->start_offset };
            size_t linksOffset{ data.size() };
            if (!links.empty()) {
                // insert links
                size_t len = sizeof(links[0]) * links.size();
                if (linksOffset + len > INT32_MAX) {
                    throw std::runtime_error("file too big");
                }
                data.insert(data.end(), reinterpret_cast<uint8_t*>(links.data()), reinterpret_cast<uint8_t*>(links.data()) + len);
            }

            DB_FILE* header = Header();

            *reinterpret_cast<uint64_t*>(header->magic) = DB_FILE_MAGIC;
            header->version = DB_FILE_CURR_VERSION;
            header->links_table_offset = (uint32_t)linksOffset;
            header->links_count = (uint16_t)links.size();
            header->data_size = (uint32_t)dataSize;
            header->file_size = (uint32_t)data.size();

            return header;
        }

        /*
         * Build the file and write it into a path
         */
        void WriteToFile(const std::filesystem::path& path) {
            DB_FILE* f = Build();

            std::ofstream of{ path, std::ios::binary };

            if (!of) {
                throw std::runtime_error("can't open output file");
            }

            of.write((const char*)f, f->file_size);

            of.close();
        }
    };

    class DBFileReader {
        std::string readData{};
        DB_FILE* file;

        void ValidateAndLink(size_t len = 0) {
            file->Validate(len);
            file->Link();
        }
    public:

        /*
         * Create a reader from a file
         * @param path path
         */
        DBFileReader(const std::filesystem::path& path) {
            std::ifstream in{ path, std::ios::binary };
            if (!in) {
                throw std::runtime_error("can't open input file");
            }

            in.seekg(0, std::ios::end);
            size_t length = in.tellg();
            in.seekg(0, std::ios::beg);

            readData.resize(length);

            in.read(readData.data(), length);

            in.close();

            file = reinterpret_cast<DB_FILE*>(readData.data());
            ValidateAndLink(length);
        }

        /*
         * Create a reader from a buffer
         * @param buffer buffer
         * @param length buffer size, 0 for unknown
         */
        DBFileReader(void* buffer, size_t length = 0) : file((DB_FILE*)buffer) {
            ValidateAndLink(length);
        }

        DBFileReader(DBFileBuilder& o) = delete;
        DBFileReader(DBFileBuilder&& o) = delete;

        /*
         * Get file data
         */
        constexpr DB_FILE* GetFile() {
            return file;
        }

        /*
         * Get start data
         * @param StartType return type
         */
        template<typename StartType = void>
        constexpr StartType* GetStart() {
            return reinterpret_cast<StartType*>(file->magic + file->start_offset);
        }
    };
}