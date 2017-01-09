#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <deque>
#include <vector>
#include <sstream>

namespace editor {

	enum RETURN_CODE {
		OK,
		Failiure,
		Permissions,
		FileNotFound
	};

	class Buffer {
	public:
		//Methods
		Buffer() {};
		~Buffer() {};

		RETURN_CODE populateFromFile(std::string path);
		RETURN_CODE saveContentsToFile(std::string path);
		RETURN_CODE updateLine(const std::string text,  int lineNo);
		RETURN_CODE UpdateMultipleLines(int _beginLineNo);
		const std::string getWholeFile(bool lineNumbers);
		std::string name;
		//Members
	private:
		std::deque<std::string> lines;
		
		//unsigned int lineNumber;
	};

	class Editor {
	public:
		Editor();
		~Editor();
	private:

		std::string savePath;
		Buffer currentBuffer;
		std::vector<Buffer> buffers;
		std::string command;

		std::string promptForPath();
		int promptForInt();

		RETURN_CODE openBuffer();
		RETURN_CODE editLine();
		RETURN_CODE switchBuffer();
		RETURN_CODE killBuffer();
		RETURN_CODE editMultipleLines();

		std::string getBufferList();
		int chooseBuffer();
		void const  showHelp(); 
	};

}


