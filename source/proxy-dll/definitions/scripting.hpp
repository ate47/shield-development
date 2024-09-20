#pragma once

namespace game
{
	typedef uint32_t ScrVarIndex_t;
	typedef uint64_t ScrVarNameIndex_t;

	struct GSC_IMPORT_ITEM
	{
		uint32_t name;
		uint32_t name_space;
		uint16_t num_address;
		uint8_t param_count;
		uint8_t flags;
	};

	struct GSC_EXPORT_ITEM
	{
		uint32_t checksum;
		uint32_t address;
		uint32_t name;
		uint32_t name_space;
		uint32_t callback_event;
		uint8_t param_count;
		uint8_t flags;
	};

	struct GSC_OBJ
	{
		byte magic[8];
		int32_t crc;
		int32_t pad;
		uint64_t name;
		int32_t include_offset;
		uint16_t string_count;
		uint16_t exports_count;
		int32_t cseg_offset;
		int32_t string_offset;
		int16_t imports_count;
		uint16_t fixup_count;
		int32_t ukn2c;
		int32_t exports_offset;
		int32_t ukn34;
		int32_t imports_offset;
		uint16_t globalvar_count;
		int32_t fixup_offset;
		int32_t globalvar_offset;
		int32_t script_size;
		int32_t requires_implements_offset;
		int32_t ukn50;
		int32_t cseg_size;
		uint16_t include_count;
		byte ukn5a;
		byte requires_implements_count;

		inline GSC_EXPORT_ITEM* get_exports()
		{
			return reinterpret_cast<GSC_EXPORT_ITEM*>(magic + exports_offset);
		}

		inline GSC_IMPORT_ITEM* get_imports()
		{
			return reinterpret_cast<GSC_IMPORT_ITEM*>(magic + imports_offset);
		}

		inline uint64_t* get_includes()
		{
			return reinterpret_cast<uint64_t*>(magic + include_offset);
		}

		inline GSC_EXPORT_ITEM* get_exports_end()
		{
			return get_exports() + exports_count;
		}

		inline uint64_t* get_includes_end()
		{
			return get_includes() + include_count;
		}
	};


	enum scriptInstance_t : int32_t
	{
		SCRIPTINSTANCE_SERVER = 0x0,
		SCRIPTINSTANCE_CLIENT = 0x1,
		SCRIPTINSTANCE_MAX = 0x2,
	};

	typedef void (*BuiltinFunction)(scriptInstance_t);


	struct BO4_BuiltinFunctionDef
	{
		uint32_t canonId;
		uint32_t min_args;
		uint32_t max_args;
		BuiltinFunction actionFunc;
		uint32_t type;
	};

	struct __declspec(align(4)) BO4_scrVarGlobalVars_t
	{
		uint32_t name;
		ScrVarIndex_t id;
		bool persist;
	};


	enum ScrVarType_t : uint32_t {
		TYPE_UNDEFINED = 0x0,
		TYPE_POINTER = 0x1,
		TYPE_STRING = 0x2,
		TYPE_VECTOR = 0x3,
		TYPE_HASH = 0x4,
		TYPE_FLOAT = 0x5,
		TYPE_INTEGER = 0x6,
		TYPE_UINTPTR = 0x7,
		TYPE_ENTITY_OFFSET = 0x8,
		TYPE_CODEPOS = 0x9,
		TYPE_PRECODEPOS = 0xA,
		TYPE_API_FUNCTION = 0xB,
		TYPE_SCRIPT_FUNCTION = 0xC,
		TYPE_STACK = 0xD,
		TYPE_THREAD = 0xE,
		TYPE_NOTIFY_THREAD = 0xF,
		TYPE_TIME_THREAD = 0x10,
		TYPE_FRAME_THREAD = 0x11,
		TYPE_CHILD_THREAD = 0x12,
		TYPE_CLASS = 0x13,
		TYPE_SHARED_STRUCT = 0x14,
		TYPE_STRUCT = 0x15,
		TYPE_REMOVED_ENTITY = 0x16,
		TYPE_ENTITY = 0x17,
		TYPE_ARRAY = 0x18,
		TYPE_REMOVED_THREAD = 0x19,
		TYPE_FREE = 0x1a,
		TYPE_THREAD_LIST = 0x1b,
		TYPE_ENT_LIST = 0x1c,
		TYPE_COUNT
	};


	struct BO4_scrVarPub {
		const char* fieldBuffer;
		const char* error_message;
		byte* programBuffer;
		byte* endScriptBuffer;
		byte* programHunkUser; // HunkUser
		BO4_scrVarGlobalVars_t globalVars[16];
		ScrVarNameIndex_t entFieldNameIndex;
		ScrVarIndex_t freeEntList;
		ScrVarIndex_t tempVariable;
		uint32_t checksum;
		uint32_t entId;
		uint32_t varHighWatermark;
		uint32_t numScriptThreads;
		uint32_t numVarAllocations;
		int32_t varHighWatermarkId;
	};

	union ScrVarValueUnion_t
	{
		int64_t intValue;
		uintptr_t uintptrValue;
		float floatValue;
		int32_t stringValue;
		const float* vectorValue;
		byte* codePosValue;
		ScrVarIndex_t pointerValue;
	};

	struct ScrVarValue_t
	{
		ScrVarValueUnion_t u;
		ScrVarType_t type;
	};

	struct ScrVar_t_Info
	{
		uint32_t nameType : 3;
		uint32_t flags : 5;
		uint32_t refCount : 24;
	};

	struct ScrVar_t
	{
		ScrVarNameIndex_t nameIndex;
		ScrVar_t_Info _anon_0;
		ScrVarIndex_t nextSibling;
		ScrVarIndex_t prevSibling;
		ScrVarIndex_t parentId;
		ScrVarIndex_t nameSearchHashList;
		uint32_t pad0;
	};

	union ScrVarObjectInfo1_t
	{
		uint64_t object_o;
		unsigned int size;
		ScrVarIndex_t nextEntId;
		ScrVarIndex_t self;
		ScrVarIndex_t free;
	};

	union ScrVarObjectInfo2_t
	{
		uint32_t object_w;
		ScrVarIndex_t stackId;
	};

	struct function_stack_t
	{
		byte* pos;
		ScrVarValue_t* top;
		ScrVarValue_t* startTop;
		ScrVarIndex_t threadId;
		uint16_t localVarCount;
		uint16_t profileInfoCount;
	};

	struct function_frame_t
	{
		function_stack_t fs;
	};

	struct ScrVmContext_t
	{
		ScrVarIndex_t fieldValueId;
		ScrVarIndex_t objectId;
		byte* lastGoodPos;
		ScrVarValue_t* lastGoodTop;
	};

	typedef void (*VM_OP_FUNC)(scriptInstance_t, function_stack_t*, ScrVmContext_t*, bool*);


	struct BO4_scrVarGlob
	{
		ScrVarIndex_t* scriptNameSearchHashList;
		ScrVar_t* scriptVariables;
		ScrVarObjectInfo1_t* scriptVariablesObjectInfo1;
		ScrVarObjectInfo2_t* scriptVariablesObjectInfo2;
		ScrVarValue_t* scriptValues;
	};

	struct BO4_scrVmPub
	{
		void* unk0;
		void* unk8;
		void* executionQueueHeap; // HunkUser
		void* timeByValueQueue; // VmExecutionQueueData_t
		void* timeByThreadQueue[1024]; // VmExecutionQueue_t
		void* frameByValueQueue; // VmExecutionQueueData_t
		void* frameByThreadQueue[1024]; // VmExecutionQueue_t
		void* timeoutByValueList; // VmExecutionQueueData_t
		void* timeoutByThreadList[1024]; // VmExecutionQueue_t
		void* notifyByObjectQueue[1024]; // VmExecutionNotifyQueue_t
		void* notifyByThreadQueue[1024]; // VmExecutionNotifyQueue_t
		void* endonByObjectList[1024]; // VmExecutionNotifyQueue_t
		void* endonByThreadList[1024]; // VmExecutionNotifyQueue_t
		ScrVarIndex_t* localVars;
		ScrVarValue_t* maxstack;
		function_frame_t* function_frame;
		ScrVarValue_t* top;
		function_frame_t function_frame_start[64];
		ScrVarValue_t stack[2048];
		uint32_t time;
		uint32_t frame;
		int function_count;
		int callNesting;
		unsigned int inparamcount;
		bool showError;
		bool systemInitialized;
		bool vmInitialized;
		bool isShutdown;
	};

	struct objFileInfo_t
	{
		GSC_OBJ* activeVersion;
		int slot;
		int refCount;
		uint32_t groupId;
	};

	enum GSC_EXPORT_FLAGS : byte
	{
		GEF_LINKED = 0x01,
		GEF_AUTOEXEC = 0x02,
		GEF_PRIVATE = 0x04,
		GEF_CLASS_MEMBER = 0x08,
		GEF_CLASS_DESTRUCTOR = 0x10,
		GEF_VE = 0x20,
		GEF_EVENT = 0x40,
		GEF_CLASS_LINKED = 0x80,
		GEF_CLASS_VTABLE = 0x86
	};

	enum GSC_IMPORT_FLAGS : byte
	{
		GIF_FUNC_METHOD = 0x1,
		GIF_FUNCTION = 0x2,
		GIF_FUNCTION_THREAD = 0x3,
		GIF_FUNCTION_CHILDTHREAD = 0x4,
		GIF_METHOD = 0x5,
		GIF_METHOD_THREAD = 0x6,
		GIF_METHOD_CHILDTHREAD = 0x7,
		GIF_CALLTYPE_MASK = 0xF,
		GIF_DEV_CALL = 0x10,
		GIF_GET_CALL = 0x20,

		GIF_SHIELD_DEV_BLOCK_FUNC = 0x80,
	};

	namespace acts_debug
	{
		constexpr uint64_t MAGIC = 0x0d0a42444124;
		constexpr byte CURRENT_VERSION = 0x12;

		enum GSC_ACTS_DEBUG_FEATURES : byte
		{
			ADF_STRING = 0x10,
			ADF_DETOUR = 0x11,
			ADF_DEVBLOCK_BEGIN = 0x12,
			ADF_LAZYLINK = 0x12,
			ADF_CRC_LOC = 0x13,
			ADF_DEVSTRING = 0x14,
			ADF_LINES = 0x15,
			ADF_FILES = 0x15,
			ADF_FLAGS = 0x15,
		};

		enum GSC_ACTS_DEBUG_FLAGS : uint32_t
		{
			ADFG_OBFUSCATED = 1 << 0,
			ADFG_DEBUG = 1 << 1,
			ADFG_CLIENT = 1 << 2,
			ADFG_PLATFORM_SHIFT = 3,
			ADFG_PLATFORM_MASK = 0xF << ADFG_PLATFORM_SHIFT,
		};

		struct GSC_ACTS_DETOUR
		{
			uint64_t name_space;
			uint64_t name;
			uint64_t script;
			uint32_t location;
			uint32_t size;
		};

		struct GSC_ACTS_LAZYLINK
		{
			uint64_t name_space;
			uint64_t name;
			uint64_t script;
			uint32_t num_address;
		};

		struct GSC_ACTS_DEVSTRING
		{
			uint32_t string;
			uint32_t num_address;
		};

		struct GSC_ACTS_LINES
		{
			uint32_t start;
			uint32_t end;
			size_t lineNum;
		};

		struct GSC_ACTS_FILES
		{
			uint32_t filename;
			size_t lineStart;
			size_t lineEnd;
		};


		struct GSC_ACTS_DEBUG
		{
			byte magic[sizeof(MAGIC)];
			byte version;
			uint32_t flags;
			uint64_t actsVersion;
			uint32_t strings_offset{};
			uint32_t strings_count{};
			uint32_t detour_offset{};
			uint32_t detour_count{};
			uint32_t devblock_offset{};
			uint32_t devblock_count{};
			uint32_t lazylink_offset{};
			uint32_t lazylink_count{};
			uint32_t crc_offset{};
			uint32_t devstrings_offset{};
			uint32_t devstrings_count{};
			uint32_t lines_offset{};
			uint32_t lines_count{};
			uint32_t files_offset{};
			uint32_t files_count{};

			constexpr bool has_feature(GSC_ACTS_DEBUG_FEATURES feature) const
			{
				return version >= feature;
			}

			inline GSC_ACTS_DETOUR* get_detours(byte* start)
			{
				return reinterpret_cast<GSC_ACTS_DETOUR*>(start + detour_offset);
			}

			inline GSC_ACTS_DETOUR* get_detours_end(byte* start)
			{
				return get_detours(start) + detour_count;
			}

			inline uint32_t* get_devblocks(byte* start)
			{
				return reinterpret_cast<uint32_t*>(start + devblock_offset);
			}

			inline uint32_t* get_devblocks_end(byte* start)
			{
				return get_devblocks(start) + devblock_count;
			}

			inline uint32_t* get_strings(byte* start)
			{
				return reinterpret_cast<uint32_t*>(start + strings_offset);
			}

			inline uint32_t* get_strings_end(byte* start)
			{
				return get_strings(start) + strings_count;
			}
		};
	}
}