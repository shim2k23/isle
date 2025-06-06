#ifndef MXDSFILE_H
#define MXDSFILE_H

#include "mxdssource.h"
#include "mxio.h"
#include "mxstring.h"
#include "mxtypes.h"

#include <windows.h>

// VTABLE: LEGO1 0x100dc890
// VTABLE: BETA10 0x101c2418
// SIZE 0x7c
class MxDSFile : public MxDSSource {
public:
	MxDSFile(const char* p_filename, MxULong p_skipReadingChunks);

#ifdef ISLE_APP
	~MxDSFile() override { Close(); }
#else
	// We have to explicitly use dllexport, otherwise this function cannot be exported,
	// since it is inlined everywhere in LEGO1.DLL
	// FUNCTION: LEGO1 0x100bfed0
	// FUNCTION: BETA10 0x10148ac0
	__declspec(dllexport) ~MxDSFile() override { Close(); }
#endif

	// FUNCTION: LEGO1 0x100c0120
	// FUNCTION: BETA10 0x10148dc0
	const char* ClassName() const override // vtable+0x0c
	{
		// STRING: LEGO1 0x10102594
		return "MxDSFile";
	}

	// FUNCTION: LEGO1 0x100c0130
	// FUNCTION: BETA10 0x10148de0
	MxBool IsA(const char* p_name) const override // vtable+0x10
	{
		return !strcmp(p_name, MxDSFile::ClassName()) || MxDSSource::IsA(p_name);
	}

	MxResult Open(MxULong) override;                 // vtable+0x14
	MxResult Close() override;                       // vtable+0x18
	MxResult Read(unsigned char*, MxULong) override; // vtable+0x20
	MxResult Seek(MxLong, MxS32) override;           // vtable+0x24
	MxULong GetBufferSize() override;                // vtable+0x28
	MxULong GetStreamBuffersNum() override;          // vtable+0x2c

	// FUNCTION: BETA10 0x1015e110
	void SetFileName(const char* p_filename) { m_filename = p_filename; }

	MxS32 CalcFileSize() { return GetFileSize(m_io.m_info.hmmio, NULL); }

	// SYNTHETIC: LEGO1 0x100c01e0
	// SYNTHETIC: BETA10 0x10148e40
	// MxDSFile::`scalar deleting destructor'

	// SIZE 0x0c
	struct ChunkHeader {
		// FUNCTION: BETA10 0x1015e040
		ChunkHeader() : m_majorVersion(0), m_minorVersion(0), m_bufferSize(0), m_streamBuffersNum(0) {}

		MxS16 m_majorVersion;     // 0x00
		MxS16 m_minorVersion;     // 0x02
		MxULong m_bufferSize;     // 0x04
		MxS16 m_streamBuffersNum; // 0x08
		MxS16 m_reserved;         // 0x0a
	};

private:
	MxResult ReadChunks();

	MxString m_filename;  // 0x14
	MXIOINFO m_io;        // 0x24
	ChunkHeader m_header; // 0x6c

	// If false, read chunks immediately on open, otherwise
	// skip reading chunks until ReadChunks is explicitly called.
	MxULong m_skipReadingChunks; // 0x78
};

#endif // MXDSFILE_H
