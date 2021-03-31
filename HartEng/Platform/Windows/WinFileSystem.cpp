#include "HartEng/Core/FileSystem.h"

#include "HartEng/Asset/AssetManager.h"
#include "HartEng/Core/Log.h"

#include <Windows.h>
#include <filesystem>

namespace HE
{
	FileSystem::FileSystemChangedCallbackFn FileSystem::s_Callback;

	static bool s_Watching = true;
	static bool s_IgnoreNextChange = false;
	static HANDLE s_WatcherThread;

	void FileSystem::SetChangeCallback(const FileSystemChangedCallbackFn& callback)
	{
		s_Callback = callback;
	}

	bool FileSystem::CreateFolder(const std::string& filepath)
	{
		BOOL created = CreateDirectoryA(filepath.c_str(), NULL);
		if (!created)
		{
			DWORD error = GetLastError();

			if (error == ERROR_ALREADY_EXISTS)
				HE_CORE_ERROR("{0} already exists!", filepath);

			if (error == ERROR_PATH_NOT_FOUND)
				HE_CORE_ERROR("{0}: One or more directories don't exist.", filepath);

			return false;
		}

		return true;
	}

	bool FileSystem::Exists(const std::string& filepath)
	{
		DWORD attribs = GetFileAttributesA(filepath.c_str());

		if (attribs == INVALID_FILE_ATTRIBUTES)
			return false;

		return true;
	}

	std::string FileSystem::Rename(const std::string& filepath, const std::string& newName)
	{
		s_IgnoreNextChange = true;
		std::filesystem::path p = filepath;
		std::string newFilePath = p.parent_path().string() + "/" + newName + p.extension().string();
		MoveFileA(filepath.c_str(), newFilePath.c_str());
		s_IgnoreNextChange = false;
		return newFilePath;
	}

	void FileSystem::StartWatching()
	{
		DWORD threadId;
		s_WatcherThread = CreateThread(NULL, 0, Watch, 0, 0, &threadId);
		HE_CORE_ASSERT(s_WatcherThread != NULL, "FileSystem::StartWatching WatcherThread is NULL");
	}

	void FileSystem::StopWatching()
	{
		s_Watching = false;
		DWORD result = WaitForSingleObject(s_WatcherThread, 5000);
		if (result == WAIT_TIMEOUT)
			TerminateThread(s_WatcherThread, 0);
		CloseHandle(s_WatcherThread);
	}

	static std::string wchar_to_string(wchar_t* input)
	{
		std::wstring string_input(input);
		std::string converted(string_input.begin(), string_input.end());
		return converted;
	}

	unsigned long FileSystem::Watch(void* param)
	{
		LPCSTR	filepath = "assets";
		BYTE* buffer = new BYTE[10 * 1024]; // 1 MB
		OVERLAPPED overlapped = { 0 };
		HANDLE handle = NULL;
		DWORD bytesReturned = 0;

		handle = CreateFile(
			filepath,
			FILE_LIST_DIRECTORY,
			FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
			NULL,
			OPEN_EXISTING,
			FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED,
			NULL
		);

		ZeroMemory(&overlapped, sizeof(overlapped));

		if (handle == INVALID_HANDLE_VALUE)
			HE_CORE_ERROR("Unable to accquire directory handle: {0}", GetLastError());

		overlapped.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

		if (overlapped.hEvent == NULL)
		{
			HE_CORE_ERROR("CreateEvent failed!");
			return 0;
		}

		while (s_Watching)
		{
			DWORD status = ReadDirectoryChangesW(
				handle,
				buffer,
				10 * 1024 * sizeof(BYTE),
				TRUE,
				FILE_NOTIFY_CHANGE_CREATION | FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_DIR_NAME,
				&bytesReturned,
				&overlapped,
				NULL
			);

			if (!status)
				HE_CORE_ERROR(GetLastError());

			DWORD waitOperation = WaitForSingleObject(overlapped.hEvent, 5000);
			if (waitOperation != WAIT_OBJECT_0)
				continue;

			if (s_IgnoreNextChange)
				continue;

			std::string oldName;
			char fileName[MAX_PATH * 10] = "";

			FILE_NOTIFY_INFORMATION* current = (FILE_NOTIFY_INFORMATION*)buffer;
			for (;;)
			{
				ZeroMemory(fileName, sizeof(fileName));
				WideCharToMultiByte(CP_ACP, 0, current->FileName, current->FileNameLength / sizeof(WCHAR), fileName, sizeof(fileName), NULL, NULL);
				std::filesystem::path filepath = "assets/" + std::string(fileName);

				FileSystemChangedEvent e;
				e.FilePath = filepath.string();
				e.NewName = filepath.filename().string();
				e.OldName = filepath.filename().string();
				e.IsDirectory = std::filesystem::is_directory(filepath);

				switch (current->Action)
				{
				case FILE_ACTION_ADDED:
				{
					e.Action = FileSystemAction::Added;
					s_Callback(e);
					break;
				}
				case FILE_ACTION_REMOVED:
				{
					e.IsDirectory = AssetManager::IsDirectory(e.FilePath);
					e.Action = FileSystemAction::Delete;
					s_Callback(e);
					break;
				}
				case FILE_ACTION_MODIFIED:
				{
					e.Action = FileSystemAction::Modified;
					s_Callback(e);
					break;
				}
				case FILE_ACTION_RENAMED_OLD_NAME:
				{
					oldName = filepath.filename().string();
					break;
				}
				case FILE_ACTION_RENAMED_NEW_NAME:
				{
					e.OldName = oldName;
					e.Action = FileSystemAction::Rename;
					s_Callback(e);
					break;
				}
				}

				if (!current->NextEntryOffset)
					break;

				current += current->NextEntryOffset;
			}
		}

		return 0;
	}
}