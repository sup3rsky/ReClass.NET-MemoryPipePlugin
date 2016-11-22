#include "BinaryWriter.hpp"

BinaryWriter::BinaryWriter(Stream& _output)
	: output(_output),
	  buffer{ 0 }
{

}
//---------------------------------------------------------------------------
void BinaryWriter::Write(bool value)
{
	buffer[0] = (uint8_t)(value ? 1 : 0);
	output.Write(buffer, 0, 1);
}
//---------------------------------------------------------------------------
void BinaryWriter::Write(uint8_t value)
{
	output.WriteByte(value);
}
//---------------------------------------------------------------------------
void BinaryWriter::Write(const uint8_t* buffer, int offset, int length)
{
	output.Write(buffer, 0, length);
}
//---------------------------------------------------------------------------
void BinaryWriter::Write(short value)
{
	buffer[0] = (uint8_t)value;
	buffer[1] = (uint8_t)(value >> 8);
	output.Write(buffer, 0, 2);
}
//---------------------------------------------------------------------------
void BinaryWriter::Write(unsigned short value)
{
	buffer[0] = (uint8_t)value;
	buffer[1] = (uint8_t)(value >> 8);
	output.Write(buffer, 0, 2);
}
//---------------------------------------------------------------------------
void BinaryWriter::Write(int value)
{
	buffer[0] = (uint8_t)value;
	buffer[1] = (uint8_t)(value >> 8);
	buffer[2] = (uint8_t)(value >> 16);
	buffer[3] = (uint8_t)(value >> 24);
	output.Write(buffer, 0, 4);
}
//---------------------------------------------------------------------------
void BinaryWriter::Write(unsigned int value)
{
	buffer[0] = (uint8_t)value;
	buffer[1] = (uint8_t)(value >> 8);
	buffer[2] = (uint8_t)(value >> 16);
	buffer[3] = (uint8_t)(value >> 24);
	output.Write(buffer, 0, 4);
}
//---------------------------------------------------------------------------
void BinaryWriter::Write(long long value)
{
	buffer[0] = (uint8_t)value;
	buffer[1] = (uint8_t)(value >> 8);
	buffer[2] = (uint8_t)(value >> 16);
	buffer[3] = (uint8_t)(value >> 24);
	buffer[4] = (uint8_t)(value >> 32);
	buffer[5] = (uint8_t)(value >> 40);
	buffer[6] = (uint8_t)(value >> 48);
	buffer[7] = (uint8_t)(value >> 56);
	output.Write(buffer, 0, 8);
}
//---------------------------------------------------------------------------
void BinaryWriter::Write(unsigned long long value)
{
	buffer[0] = (uint8_t)value;
	buffer[1] = (uint8_t)(value >> 8);
	buffer[2] = (uint8_t)(value >> 16);
	buffer[3] = (uint8_t)(value >> 24);
	buffer[4] = (uint8_t)(value >> 32);
	buffer[5] = (uint8_t)(value >> 40);
	buffer[6] = (uint8_t)(value >> 48);
	buffer[7] = (uint8_t)(value >> 56);
	output.Write(buffer, 0, 8);
}
//---------------------------------------------------------------------------
void BinaryWriter::Write(const void* value)
{
#ifdef _WIN64
	Write(reinterpret_cast<unsigned long long>(value));
#else
	Write(reinterpret_cast<unsigned int>(value));
#endif
}
//---------------------------------------------------------------------------
void BinaryWriter::Write(float value)
{
	auto tmp = *(unsigned int*)&value;
	buffer[0] = (uint8_t)tmp;
	buffer[1] = (uint8_t)(tmp >> 8);
	buffer[2] = (uint8_t)(tmp >> 16);
	buffer[3] = (uint8_t)(tmp >> 24);
	output.Write(buffer, 0, 4);
}
//---------------------------------------------------------------------------
void BinaryWriter::Write(double value)
{
	auto tmp = *(unsigned long long*)&value;
	buffer[0] = (uint8_t)tmp;
	buffer[1] = (uint8_t)(tmp >> 8);
	buffer[2] = (uint8_t)(tmp >> 16);
	buffer[3] = (uint8_t)(tmp >> 24);
	buffer[4] = (uint8_t)(tmp >> 32);
	buffer[5] = (uint8_t)(tmp >> 40);
	buffer[6] = (uint8_t)(tmp >> 48);
	buffer[7] = (uint8_t)(tmp >> 56);
	output.Write(buffer, 0, 8);
}
//---------------------------------------------------------------------------
void BinaryWriter::Write(const std::wstring& value)
{
	int byteLength = (int)value.length() * sizeof(std::wstring::value_type);
	Write7BitEncodedInt(byteLength);

	output.Write(reinterpret_cast<const uint8_t*>(value.data()), 0, byteLength);
}
//---------------------------------------------------------------------------
void BinaryWriter::Write7BitEncodedInt(int value)
{
	// Write out an int32 7 bits at a time. The high bit of the byte,
	// when on, tells reader to continue reading more byte.
	auto v = (unsigned int)value;
	while (v >= 0x80)
	{
		Write((uint8_t)(v | 0x80));
		v >>= 7;
	}
	Write((uint8_t)v);
}
//---------------------------------------------------------------------------
