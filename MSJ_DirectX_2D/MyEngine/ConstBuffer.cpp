#include "ConstBuffer.h"
#include "Device.h"

void ConstBuffer::Vertex_Set()
{
	Device::Get_Context()->VSSetConstantBuffers(m_Register, 1, &m_pConstBuffer);
}

void ConstBuffer::Pixel_Set()
{
	Device::Get_Context()->PSSetConstantBuffers(m_Register, 1, &m_pConstBuffer);
}