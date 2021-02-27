#include "HartEng/Core/UUID.h"

namespace HE
{
	UUID::UUID()
	{
		std::random_device rd;
		auto seed_data = std::array<int, std::mt19937::state_size> {};
		std::generate(std::begin(seed_data), std::end(seed_data), std::ref(rd));
		std::seed_seq seq(std::begin(seed_data), std::end(seed_data));
		std::mt19937 generator(seq);
		uuids::uuid_random_generator gen{ generator };

		m_UUID = gen();
	}

	UUID::UUID(const std::string& uuid)
	{
		m_UUID = uuids::uuid::from_string(uuid).value();
	}

	UUID::UUID(int32_t zero)
	{
	}

	void UUID::GenerateFromString(const std::string& key)
	{
		// This function will generate same uuid for one string every time
		uuids::uuid_name_generator gen(uuids::uuid::from_string("00000000-0000-0000-0000-000000000000").value());
		m_UUID = gen(key);
	}

	const uuids::uuid& UUID::GetUUID_c() const 
	{ 
		return m_UUID; 
	}

	uuids::uuid& UUID::GetUUID() 
	{ 
		return m_UUID; 
	}

	void UUID::Swap(UUID & other)
	{
		m_UUID.swap(other.GetUUID());
	}

	const std::string& UUID::ToString() const 
	{ 
		return uuids::to_string(m_UUID); 
	}

	bool UUID::IsNil() const
	{
		return m_UUID.is_nil();
	}
}