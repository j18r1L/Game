#pragma once

#include "HartEng/vendor/stduuid/uuid.h"
#include "HartEng/Core/Log.h"

#include <string>

namespace HE
{
	class UUID
	{
	private:
		uuids::uuid m_UUID; // default uuid is nil

		uuids::uuid& GetUUID();
	public:
		UUID();
		UUID(const std::string& uuid);
		UUID(int32_t zero);

		virtual ~UUID() = default;

		void GenerateFromString(const std::string& key);

		const uuids::uuid& GetUUID_c() const;

		void Swap(UUID& other);

		const std::string& ToString() const;

		bool IsNil() const;

		operator std::string() { return uuids::to_string(m_UUID); }
		bool operator==(const UUID& other) const { return m_UUID == other.GetUUID_c(); }
		bool operator!=(UUID& other) const { return !(m_UUID == other.GetUUID()); }
	};
}

namespace std {

	template <>
	struct hash<HE::UUID>
	{
		std::size_t operator()(const HE::UUID& uuid) const
		{
			std::hash<std::string> hasher;
			return static_cast<std::size_t>(hasher(uuids::to_string(uuid.GetUUID_c())));
		}
	};
}