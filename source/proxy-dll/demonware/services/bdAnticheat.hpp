#pragma once

namespace demonware
{
	class bdAnticheat final : public service
	{
	public:
		bdAnticheat();

	private:
		void answerChallenges(service_server* server, byte_buffer* buffer) const;
		void reportConsoleID(service_server* server, byte_buffer* buffer) const;
		void reportConsoleDetails(service_server* server, byte_buffer* buffer) const;
		void answerTOTPChallenge(service_server* server, byte_buffer* buffer) const;
		void reportExtendedAuthInfo(service_server* server, byte_buffer* buffer) const;
	};
}
