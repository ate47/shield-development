#include <std_include.hpp>
#include "definitions/game.hpp"
#include "loader/component_loader.hpp"

#include <utilities/hook.hpp>

namespace experimental
{
	namespace
	{
		utilities::hook::detour UI_Interface_RegisterImage_Hook;
		bool UI_Interface_RegisterImage_Stub(void* luaVM, game::BO4_AssetRef_t* imageName, void* output_image)
		{
			if (imageName->hash == 0x2895D4A302658C0B) imageName->hash = 0x10cdcddd4b003f1e;
			if (imageName->hash == 0x2895D5A302658DBE) imageName->hash = 0x24947cc4d6ab64f7; // MultiMode
			if (imageName->hash == 0x2895D6A302658F71) imageName->hash = 0x732d821522573d32; // Seraph
			if (imageName->hash == 0x2895D7A302659124) imageName->hash = 0x01e4caa4f63b4140; // Battery
			if (imageName->hash == 0x2895D8A3026592D7) imageName->hash = 0x58b0b4f8a9f2a40b; // chaos crew

			return UI_Interface_RegisterImage_Hook.invoke<bool>(luaVM, imageName, output_image);
		}
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
#ifdef ONLINEDLL
#endif
		}
	};
}

REGISTER_COMPONENT(experimental::component)