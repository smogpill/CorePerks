// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2026 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#include "pch.h"
#include "core_perks/extern/jolt/jolt_streams.h"

#ifdef CP_JOLT
namespace cp::jolt
{
	using SerializationMarker = uint32;
	static constexpr SerializationMarker serialization_marker = 7777;

	class StreamOutImpl final : public JPH::StreamOut
	{
	public:
		StreamOutImpl(cp::BinaryOutputStream& stream) : stream_(stream) {}
		void WriteBytes(const void* inData, size_t inNumBytes) override { stream_.write(inData, inNumBytes); }
		bool IsFailed() const override { return stream_.failed(); }
	private:
		BinaryOutputStream& stream_;
	};

	class StreamInImpl final : public JPH::StreamIn
	{
	public:
		StreamInImpl(cp::BinaryInputStream& stream) : stream_(stream) {}
		void ReadBytes(void* outData, size_t inNumBytes) override { stream_.read(outData, inNumBytes); }
		bool IsFailed() const override { return stream_.failed(); }
		bool IsEOF() const override { return stream_.eof(); }
	private:
		BinaryInputStream& stream_;
	};
}

namespace cp
{ 
	BinaryInputStream& operator>>(BinaryInputStream& stream, JPH::Ref<JPH::Shape>& shape)
	{
		bool exists = false;
		stream >> exists;
		if (!exists)
			return stream;
		jolt::StreamInImpl jolt_stream(stream);
		JPH::Shape::IDToShapeMap shape_map;
		JPH::Shape::IDToMaterialMap material_map;
		JPH::Shape::ShapeResult result = JPH::Shape::sRestoreWithChildren(jolt_stream, shape_map, material_map);
		if (result.IsValid())
		{
			shape = result.Get();
			jolt::SerializationMarker marker;
			stream >> marker;
			if (marker != jolt::serialization_marker)
			{
				CP_ERROR("Failed to deserialize Jolt's shape: wrong marker value");
				stream.set_failed();
			}
		}
		else
		{
			CP_ERROR("Failed to deserialize Jolt's shape: {}", result.GetError().data());
			stream.set_failed();
		}
		return stream;
	}

	BinaryOutputStream& operator<<(BinaryOutputStream& stream, const JPH::Ref<JPH::Shape>& shape)
	{
		stream << bool(shape != nullptr);
		if (!shape)
			return stream;
		jolt::StreamOutImpl jolt_stream(stream);
		JPH::Shape::ShapeToIDMap shape_map;
		JPH::Shape::MaterialToIDMap material_map;
		shape->SaveWithChildren(jolt_stream, shape_map, material_map);
		// Adding a dummy/marker value because Jolt considers stream's eof as a failure, which is wrong.
		stream << jolt::serialization_marker;
		return stream;
	}
}
#endif
