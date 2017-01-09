#include "Editor.h"

namespace editor {

	int str(std::string s) {

		std::stringstream s_s(s);
		int i;
		s_s >> i;

		if (!i) {
			return 0; //Stupid
		}

		return i;
	}

	Editor::Editor() {
		savePath = "";
		//Give currentBuffer a value
		Buffer _dontUseMe;
		currentBuffer = _dontUseMe;
		//Enters main editor loop
		std::string command;
		while (command != "Q") {
			getline(std::cin, command);
			//Yes, I know this is stupid.
			//--Begin open buffer
			if (command[0] == 'b')
				openBuffer();

			//--End open buffer
			//--Begin edit line
			else if (command[0] == 'e')
				editLine();

			//--End edit line
			//--Begin v iew file
			else if (command[0] == 'v')
				std::cout << currentBuffer.getWholeFile(true);
			//--End view file
			//--Begin save buffer
			else if (command[0] == 's') {
				std::string path = promptForPath();
				currentBuffer.saveContentsToFile(path);
			}
			//--End save buffer
			//--Begin switch buffer
			else if (command[0] == 'c')
				switchBuffer();

			else if (command[0] == 'x')
				killBuffer();

			else if (command[0] == 'h')
				showHelp();

			else if (command[0] == 'l')
				editMultipleLines();
			
			//--End switch buffer
				
		}

	}
	Editor::~Editor() {
	}

	RETURN_CODE Editor::setCurrentBuffer()
	{
		return RETURN_CODE();
	}

	std::string Editor::promptForPath()
	{
		std::cout << "/? ";
		std::string path;
		getline(std::cin, path);
		if (path.empty()) {
			path = savePath;
		}
		else
			savePath = path;
		return path;
	}

	int Editor::promptForInt()
	{
		std::cout << "#? ";
		std::string _input;
		getline(std::cin, _input);
		return str(_input); //TODO: FIX THIS TERRIBLE FUNCTION!
		
	}

	RETURN_CODE Editor::openBuffer()
	{
		Buffer b;
		std::string path = promptForPath();
		RETURN_CODE ret = b.populateFromFile(path);
		currentBuffer = b;
		buffers.push_back(b);
		return RETURN_CODE::OK;
	}

	RETURN_CODE Editor::editLine()
	{
		int _lineNo = promptForInt();
		std::cout << "\"\"? ";
		std::string _newLineVal;
		getline(std::cin, _newLineVal);
		currentBuffer.updateLine(_newLineVal, _lineNo);
		return RETURN_CODE::OK;
	}

	RETURN_CODE Editor::switchBuffer()
	{
		currentBuffer = buffers[chooseBuffer()];
		return RETURN_CODE::OK;
			
	}

	RETURN_CODE Editor::killBuffer()
	{
		buffers.erase(buffers.begin() + chooseBuffer());
		return RETURN_CODE();
	}

	RETURN_CODE Editor::editMultipleLines()
	{
		int startHere = promptForInt();
		currentBuffer.UpdateMultipleLines(startHere);
		return RETURN_CODE::OK;
	}

	RETURN_CODE Buffer::UpdateMultipleLines(int _beginLineNo)
	{
		
		std::string line = "xyz";
		int i = 0;
		while (!line.empty()) {
			getline(std::cin, line);
			std::deque<std::string>::iterator it = lines.begin() + i;
			lines.insert(it, line);
			i++;
		}
		return RETURN_CODE::OK;
	}

	std::string Editor::getBufferList()
	{
		unsigned int i = 0;
		std::string fancyBufferList;
		for (std::vector<Buffer>::iterator it = buffers.begin(); it < buffers.end(); it++, i++) {
			fancyBufferList =  fancyBufferList + (*it).name + " - " + std::to_string(i) + "\n";
		}

		return fancyBufferList;

	}

	int Editor::chooseBuffer()
	{
		std::cout << getBufferList();
		int bufferNo = promptForInt();
		if ((bufferNo < buffers.size()) && (bufferNo >= 0)) {
			return bufferNo;
		}
		return 0;
	}

	void const Editor::showHelp()
	{
		std::cout << "Welcome to a terrible, terrible line based editor that I wrote because I was bored. No help for you!" << std::endl;
	}

	/*std::ofstream Editor::ReadFile() {
		return std::ofstream();
	}*/


	RETURN_CODE Buffer::populateFromFile(std::string path) {
		std::deque<std::string> _lines;
		std::string _line;
		std::ifstream _f;
		_f.open(path);
		//Does file exist?
		if (!_f) {
			return RETURN_CODE::FileNotFound;
		}
		//Set buffer to file line by line
		while (std::getline(_f, _line )) {
			_lines.push_back(_line);
		}
		//Name buffer
		name = path;

		lines = _lines;

		return RETURN_CODE::OK;
	}

	RETURN_CODE Buffer::saveContentsToFile(std::string path) {
		std::remove(path.c_str());
		std::string _file = getWholeFile(false);
		
		std::ofstream _f;
		
		_f.open(path,std::ios::out);
		
		_f << _file;
		
		_f.close();

		return RETURN_CODE::OK;
	}

	RETURN_CODE Buffer::updateLine(const std::string text,  int lineNo)
	{
		if (lineNo < 0)
			lines.push_front(text);
		else if (lineNo > lines.size())
			lines.push_back(text);
		else
			lines[lineNo - 1] = text;
		
		return RETURN_CODE::OK;
	}

	const std::string Buffer::getWholeFile(bool lineNumbers)
	{
		std::string _file;
		int i = 0;
		for (const std::string& _line : lines) {
			++i;

			if (lineNumbers) {
				std::string is = std::to_string(i);
				if (is.length() == 1)
					_file.append(is + "  ");
				else if (is.length() == 2)
					_file.append(is + " ");

			}
			_file.append(_line + "\n");

		}
			
		return _file;
	}
}

