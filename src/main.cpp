#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iterator>
#include <algorithm>
#include <ctype.h>

#define program_name "[3^text]"
#define debug false

using namespace std;

class Text {
public:
  Text(int &argc, char **argv);
  ~Text();
  const string& get_command(const string &option);
  bool cmd_options_exists(const string &option);
  bool load_file(const string &filename);
  void downcase();
  void upcase();
  void tags();
  void show();
  void strip(string pattern);
private:
  vector<string> options;
  vector<string> content;
};

Text::Text(int &argc, char **argv){
  for (int i=1; i < argc; ++i)
  {
    options.push_back(string(argv[i]));
  }
}
Text::~Text(){}

const string& Text::get_command(const string &option) {
  vector<string>::const_iterator it;
  it = find(options.begin(), options.end(), option);
  if (it != options.end() && ++it != options.end()) {
    return *it;
  }
  return "";
}

bool Text::cmd_options_exists(const string &option) {
  return find(options.begin(), options.end(), option)
                     != options.end();
}

bool Text::load_file(const string &filename) {
  fstream fs(filename);
  if (!fs) {
    cerr << "error: file " << filename << " not found." << endl;
    return false;
  }
  string line;
  while (std::getline(fs, line))
    content.push_back(line);
  if (debug)
    show();
  return true;
}

void Text::downcase()
{
  for_each(content.begin(), content.end(), [](string &s){ transform(s.begin(), s.end(), s.begin(), ::tolower);});
}

void Text::upcase()
{
  for_each(content.begin(), content.end(), [](string &s){ transform(s.begin(), s.end(), s.begin(), ::toupper);});
}

void Text::tags()
{
  vector<string> tags;
  string word;
  for_each(content.begin(), content.end(), [&tags, &word](string &line){
    istringstream iss(line);
    do
    {
      string word;
      iss >> word;
      if (word[0] == '#')
        tags.push_back(word);
    } while (iss);
  });

  auto first = begin(tags);
  auto last = end(tags);
  for_each(tags.begin(), tags.end(), [&first, &last](string &tag){
    if (first != last)
    {
      cout << *first++;
      if (first != last) {
        cout << ", ";
      }
    }
  });
  cout << endl << endl;
}

void Text::strip(string pattern)
{
  for_each(content.begin(), content.end(), [&pattern](string &str) {
    std::size_t found = str.find_first_of(pattern);
    while (found!=std::string::npos) {
      str.erase(found, pattern.size());
      found=str.find_first_of(pattern,found+1);
    }
  });
}

void Text::show() {
  string output;
  for_each(content.begin(), content.end(), [&output](string &s) {
    if (s.length() == 0)
      output.append("\n\n");
    else
      output.append(s.append(" "));
  });
  printf("%s \n", output.c_str());
}

void error(int error_code) {
  printf("%s error: ", program_name);
  switch (error_code) {
    case 0:
      printf("\
no input file was specified. \n\
");
  }
}

void help() {
  printf("\n\
arguments: \n\
\ttext [options] filename.ext \n\n\
options: \n\
\t-d downcase text \n\
\t-u upcase text \n\
\t-t find and extract tags in text \n\
");
}

int main(int argc, char **argv){
  if (argc >1) {
    Text text(argc, argv);
    if (!text.load_file(argv[argc -1])) {
      return 1;
    }
    if (text.cmd_options_exists("-d")) {
      text.downcase();
    }
    if (text.cmd_options_exists("-u")) {
      text.upcase();
    }
    if (text.cmd_options_exists("-t")) {
      cout << "--{ tags }--" << endl << endl;
      text.tags();
      text.strip("#");
    }
    cout << "--{ body }--" << endl << endl;
    text.show();
  } else {
      error(0);
      help();
      return 1;
  }
  return 0;
}
