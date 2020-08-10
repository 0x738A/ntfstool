#include "ntfs_explorer.h"

#include "ntfs_mft_record.h"
#include "ntfs_mft.h"
#include <Drive\volume.h>
#include <Drive\disk.h>

NTFSExplorer::NTFSExplorer(std::shared_ptr<Volume> volume)
{
	if (volume->disk_index() != DISK_INDEX_IMAGE)
	{
		_reader = std::make_shared<NTFSReader>(utils::strings::from_string(volume->name()));
	}
	else
	{
		_reader = std::make_shared<NTFSReader>(utils::strings::from_string(reinterpret_cast<Disk*>(volume->parent())->name()), volume->offset());
	}
	_MFT = std::make_shared<MFT>(_reader);
}

NTFSExplorer::~NTFSExplorer()
{
}

std::vector<std::wstring> NTFSExplorer::list(std::string directory)
{
	wprintf(L"Listing directory");

	std::vector<std::wstring> ret;

	std::shared_ptr<MFTRecord> rec = _MFT->record_from_path(directory);
	for (std::shared_ptr<IndexEntry> entry : rec->index())
	{
		ret.push_back(entry->name());
	}

	return ret;
}