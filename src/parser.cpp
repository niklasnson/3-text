#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iterator>

#define program_name "[3^text]"

using namespace std;

class Options {
public:
  Options(int &argc, char **argv);
  ~Options();
  const string& get(const string &option);
  bool exists(const string &option);
private:
  vector<string> v;
};

Options::Options(int &argc, char **argv) {
  for (int i=1; i < argc; ++i) {
    v.push_back(string(argv[i]));
  }
}

Options::~Options() {
}

const string & Options::get(const string &option) {
  vector<string>::const_iterator it;
  it = find(v.begin(), v.end(), option);
  if (it != v.end() && ++it != v.end()) {
    return *it;
  }
  return "";
}

bool Options::exists(const string &option) {
  return find(v.begin(), v.end(), option)
                     != v.end();
}

class Parser {
public:
  Parser(const string &filename);
  ~Parser();

  bool   parse(bool downcase, bool upcase, bool tags);
  void   render();
  void   render_tags(string tags);
  void   to_downcase();
  void   to_upcase();
  string collect_tags();
  string strip(string pattern, string str);
  string time_to_read();
  int    count_words();
private:
  vector<string> v;
};

Parser::Parser(const string &filename) {
  fstream fs(filename);
  if (!fs) {
    cerr << "Error: file " << filename << " not found" << endl;
  }
  string line;
  string word;
  while (getline(fs, line))
  {
    v.push_back(line);
  }
}

Parser::~Parser() {
}

void Parser::to_downcase() {
  for_each(v.begin(), v.end(), [](string &s){ transform(s.begin(), s.end(), s.begin(), ::tolower);});
}

void Parser::to_upcase() {
  for_each(v.begin(), v.end(), [](string &s){ transform(s.begin(), s.end(), s.begin(), ::toupper);});
}

string Parser::collect_tags() {
  vector<string> tags;
  string str;
  for_each(v.begin(), v.end(), [&tags](string &word){
    istringstream iss(word);
    do
    {
      string str;
      iss >> str;
      if (str[0] == '#')
        tags.push_back(str);
    } while (iss);
  });

  auto first = begin(tags);
  auto last = end(tags);

  for_each(tags.begin(), tags.end(), [&first, &last, &str](string &tag) {
    if (first != last)
    {
      str.append(*first++);
      if(first != last)
      {
        str.append(", ");
      }
    }
  });

  return str;
}

string Parser::strip(string pattern, string str) {
  std::size_t found = str.find_first_of(pattern);
  while (found!=std::string::npos) {
    str.erase(found, pattern.size());
    found = str.find_first_of(pattern, found +1);
  }
  return str;
}

string Parser::time_to_read() {
  int words_count = count_words();
  if (words_count < 250)
  {
    return " { >1 min }";
  }
  else
  {
    int time = words_count / 250;
    string str = " { ";
    str.append(to_string(time));
    str.append( " min }");
    return str;
  }
}

int Parser::count_words() {
  int counter = 0;
  for_each(v.begin(), v.end(), [&counter](string &s) {
    string buffert;
    stringstream ss(s);
    while (ss >> buffert) {
      ++counter;
    }
  });
  return counter;
}

void Parser::render() {
  string str;
  for_each(v.begin(), v.end(), [&str](string &s) {
    if (s.length() == 0) {
      str.append("\n\n");
    } else {
      str.append(s.append(" "));
    }
  });
  str = strip("#", str);

  cout << "=={ content }===========================" << endl << endl;
  printf("%s \n", str.c_str());
  cout << endl;
}

void Parser::render_tags(string tags) {
  cout << "=={  tags  }============================" << endl << endl;
  cout << strip("#", tags) <<  time_to_read() << endl;
  cout << endl;
}

bool Parser::parse(bool downcase, bool upcase, bool tags) {
  if (downcase) {
    to_downcase();
  }
  if (upcase) {
    to_upcase();
  }

  if (tags) {
    render_tags(collect_tags());
  }
  render();
  return true;
}



int main(int argc, char **argv) {
  if (argc >1) {
    Options options(argc, argv);
    Parser parser(argv[argc -1]);

    bool downcase = (options.exists("-d") ? true : false);
    bool upcase   = (options.exists("-u") ? true : false);
    bool tags     = (options.exists("-t") ? true : false);
    parser.parse(downcase, upcase, tags);
    return 0;
  }
  else {
    return 1;
  }
}
